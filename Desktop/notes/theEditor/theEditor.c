
// includes
#include <stdlib.h>
#include <termios.h>
#include <errno.h>
#include <unistd.h>
#include<stdio.h>
#include<ctype.h>

#define CTL_KEY(k)((k) & 0x1f)

//data
struct termios orig_termios;

//terminal
void die(const char *s) {
  perror(s);
  exit(1);
}

void disableRawMode() {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
    die("tcsetattr");
}
void enableRawMode() {
  if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
  atexit(disableRawMode);
  struct termios raw = orig_termios;
  raw.c_iflag &= ~(IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN | ICRNL);
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;

  
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}

char editorReadKey(){
  int nread;
  char c;
  while((nread=read(STDIN_FILENO,&c,1))!=1){
    if(nread==-1 && errno!= EAGAIN)die("read");
  }
  return c;
}

void editorProcessKeyPress(){
  char c = editorReadKey();
  switch(c){
  case CTRL_KEY('q'):
    exit(0);
    break;
  }
}
// init 
int main() {
  enableRawMode();
  char c;
  while (1){
  	editorProcessKeyPress();
  }
  return 0;
}