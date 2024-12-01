#include <stdlib.h>
#include <termios.h>
#include <errno.h>
#include <unistd.h>
#include<stdio.h>
#include<ctype.h>

struct termios orig_termios;

void die(const char *s) {
  perror(s);
  exit(1);
}

void disableRawMode() {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
    die("tcsetattr");
}
void enableRawMode() {
  tcgetattr(STDIN_FILENO, &orig_termios);
  atexit(disableRawMode);
  struct termios raw = orig_termios;
  raw.c_iflag &= ~(IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN | ICRNL);
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;

  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main() {
  enableRawMode();
  char c;
  while (1){
  	char c ='\0';
  	read(STDIN_FILENO,&c,1);
  	if(iscntrl(c)){
  		printf("%d\r\n",c);
  	}
  	else{
  		printf("%d ('%c')\r\n",c,c);
  	}
  	if (c=='q')break;
  }
  return 0;
}