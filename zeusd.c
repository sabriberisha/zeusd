#include "zeusd.h"

/* Other global stuff */
char buffer[BUFSIZE];
char b64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

void decodeblock(unsigned char in[], char *clrstr) {
  unsigned char out[4];
  out[0] = in[0] << 2 | in[1] >> 4;
  out[1] = in[1] << 4 | in[2] >> 2;
  out[2] = in[2] << 6 | in[3] >> 0;
  out[3] = '\0';
  strncat(clrstr, (char *)out, sizeof(out));
}

void b64_decode(char *b64src, char *clrdst) {
  int c, phase, i;
  unsigned char in[4];
  char *p;

  clrdst[0] = '\0';
  phase = 0; i=0;
  while(b64src[i]) {
    c = (int) b64src[i];
    if(c == '=') {
      decodeblock(in, clrdst);
      break;
    }
    p = strchr(b64, c);
    if(p) {
      in[phase] = p - b64;
      phase = (phase + 1) % 4;
      if(phase == 0) {
        decodeblock(in, clrdst);
        in[0]=in[1]=in[2]=in[3]=0;
      }
    }
    i++;
  }
}


/* finds the string AFTER : */
char *finddelim(char *ptr)
{
  while (*ptr != ':' && *ptr)
    ptr++;
  ptr[0]='\0';
  ptr++;
  return (ptr);
}

int main(int argc, char *argv[])
{
  int ret;
  fd_set rfd;
  struct timeval to = {10, 0};
  int sock=6;
  char tmpbuf[BUFSIZE];         /* temporary buffer */
  int numbytes = 0;                 /* short memory */
  int tmpcntr;                  /* temp int (ie counter/filedescriptor */
  int offset;                   /* offset in parsing */
  char *arg[5];
  char *bptr;                   /* temporary pointer needed for parsing */
  char *ver;
  char ip[BUFSIZE], *bip;
  char domain[BUFSIZE], *bdomain;
  char url[BUFSIZE], *burl;
  char serial[BUFSIZE], *bserial;

  extern char *envp[];


  bzero(tmpbuf, BUFSIZE);

again:
  FD_ZERO(&rfd);
  FD_SET(0, &rfd);
  ret = select(1 , &rfd, NULL, NULL, &to);
  if (ret < 0)
  {
    if (errno == EINTR)
      goto again;
  }
  if (ret == 0) {
    fprintf(stdout,"timeout\n");
    exit(0);
  }
  if (ret == -1) {
    fprintf(stdout,"socket error\n");
    exit(0);
  }

  numbytes = read(0, &tmpbuf, BUFSIZE);
  if (numbytes == 0)		/* EOF */
  {
    printf("PASS: FAILED TO READ\n");
    exit(0);
  }
  if (numbytes == -1)
    if (errno != EINTR)
    {
	printf("PASS: INTERRUPTED\n");
	exit(0);
    }

  bptr = tmpbuf;		/* use bptr as pointer to tamper with */

  ver = bptr;
  bserial = finddelim(ver);
  bip = finddelim(bserial);
  bdomain = finddelim(bip);
  burl = finddelim(bdomain);
 
  b64_decode(bserial,serial);
  b64_decode(bip,ip);
  b64_decode(bdomain,domain);
  b64_decode(burl, url);

  arg[0] = "check";
  arg[1] = (char *)serial;
  arg[2] = (char *)ip;
  arg[3] = (char *)domain;
  arg[4] = (char *)url;
  arg[5] = '\0';

  execve("/home/sabri/codebase/zeusd/zeus.php", (char **)&arg, NULL);
  exit(0);
}
