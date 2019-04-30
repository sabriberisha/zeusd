#!/usr/local/bin/php
<?php

/* 

Return values:

F (0): Fail (internal error)
P (1): Pass
A (2): Access Denied
N (4): No ACLs found
R (5): Registration error (wifinanny not found)
U (6): User's device not known

*/


if((count($argv)) < 5)
 exit(0);

$_serial	= trim($argv[1]);
$_ip 		= trim($argv[2]);
$_domain	= trim($argv[3]);
$_url		= trim($argv[4]);

system("echo $_serial $_ip $_domain $_url >> /tmp/log.txt");
$w = "www.wifinanny.com";

/* pass everything to wifinanny.com */
if (strcasecmp($_domain,$w) == 0)
  die("PASS\n");

$link = @mysql_pconnect("db.cluecentral.net", "wifinanny", "PASSWORD")
   or die("FAIL: E1001 (internal database error)\n");
mysql_select_db("wifinanny")
   or die("FAIL: E1002 (internal database error)\n");


function find_account_by_serial($serial)
{
 global $link;
 
 /* get device ID */
 $result = mysql_query("select account from devices where serial=\"$serial\"")
  or die("FAIL: E1004 (internal database error)\n");
 $line = mysql_fetch_object($result);
 if(!$line)
  die("REGISTRATION ERROR\n");
 return($line->account);
}


function find_user_by_ip($account, $ip)
{
 global $link;
 
 /* get device ID */
 $result = mysql_query("select user from ip_to_user where account=$account and ip=\"$ip\"")
  or die("FAIL: E1006 (internal database error)\n");
 $line = mysql_fetch_object($result);
 if(!$line)
  return(-1);
 else
  return($line->user);
}

function check_filter($acl, $domain, $url)
{


 //$parse = parse_url($domain);
 //print_r($parse);
 //$domain = preg_replace('#^www\.(.+\.)#i', '$1', $parse['host']) . $parse['path'];

 $domain = "." . preg_replace('#^w{3}\.#', '$1', $domain);

 global $_link;
 $acls = mysql_query("select * from acl where id=$acl order by seq")
  or die("FAIL: E1011 (database error)\n");
 while ($filter = mysql_fetch_object($acls)) {
  $filterid = $filter->id;
  //echo "ACL: $filterid domain: $domain\n";
  $filters = mysql_query("select * from filters where domain=\"$domain\" and filterid=\"$filterid\"")
    or die("FAIL: E1011 (database error)\n");
  if($rule = mysql_fetch_object($filters)) {
    $seq = $rule->seq;
    $action = $rule->action;
    return($action);
  }
 }
 return(1);
}


function main()
{
 global $_serial, $_ip, $_domain, $_url, $_link;

 $account = find_account_by_serial($_serial);
 $user = find_user_by_ip($account, $_ip);

 /* if no matching user was found, log the entry and redirect */
 if ($user == -1) {
  mysql_query("insert into access_log values(0,$account,$user,\"$_url\",0,UNIX_TIMESTAMP(),0)")
    or die("FAIL: E1007 (database error)\n");
  die("UNKNOWN USER\n");
 }

 /* search ACLs
  * An ACL containts a list of filters. If no ACLs are found,
  * consider it a pass.
  */
 $acls = mysql_query("select * from acl where user=$user")
  or die("FAIL: E1008\n");
 while ($acl = mysql_fetch_object($acls)) {
  $aclid = $acl->id;
  if (check_filter($aclid, $_domain, $_url)) {
   mysql_query("insert into access_log values(0,$account,$user,\"$_url\",1,UNIX_TIMESTAMP(),0)")
    or die("FAIL: E1003\n");
   echo "PASS\n";
   exit;
  } else {
   mysql_query("insert into access_log values(0,$account,$user,\"$_url\",0,UNIX_TIMESTAMP(),0)")
    or die("FAIL: E1009\n");
   echo "ACCESS DENIED\n";
   exit;
  }
 }

 echo "PASS\n";
 exit(0);


 /* no ACLs were found or no matches, fail the client */
 mysql_query("insert into access_log values(0,$account,$user,\"$_url\",0,UNIX_TIMESTAMP(),0)")
  or die("FAIL: E1010\n");
 echo "NO ACLS\n";

 exit(0);
}



main();
?>
