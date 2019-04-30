#!/usr/local/bin/php
<?php

$src = "/.*\.a.*c.*\.com/";
$dst = "www.bcd.com";

if(preg_match($src, $dst))
 echo "Yes\n";
else
 echo "no\n";

?>
