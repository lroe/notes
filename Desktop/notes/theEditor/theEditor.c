
// includes
#include <stdlib.h>
#include <termios.h>
#include <errno.h>
#include <unistd.h>
#include<stdio.h>
#include<ctype.h>
#include <sys/ioctl.h>
#define CTRL_KEY(k)((k) & 0x1f)

//data
struct editorConfig{
  struct termios orig_termios;
  int screenrows;
  int screencols;
};
struct editorConfig E;

//terminal
void die(const char *s) {
    write(STDOUT_FILENO,"\x1b[2J",4);
  write(STDOUT_FILENO, "\x1b[H",3);
  perror(s);
  exit(1);
}

void disableRawMode() {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.orig_termios) == -1)
    die("tcsetattr");
}
void enableRawMode() {
  if (tcgetattr(STDIN_FILENO, &E.orig_termios) == -1) die("tcgetattr");
  atexit(disableRawMode);
  struct termios raw = E.orig_termios;
  raw.c_iflag &= ~(IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN | ICRNL);
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;

  
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}
//output


void editorDrawRows(){
  int y;
  for(y=0;y<E.screenrows;y++){
    write(STDOUT_FILENO,"~\r\n",3);

  }
}

void editorRefreshScreen(){
  write(STDOUT_FILENO,"\x1b[2J",4);
  write(STDOUT_FILENO, "\x1b[H",3);
  editorDrawRows();
  write(STDOUT_FILENO,"\x1b[H",3);
}

//input
char editorReadKey(){
  int nread;
  char c;
  while((nread=read(STDIN_FILENO,&c,1))!=1){
    if(nread==-1 && errno!= EAGAIN)die("read");
  }
  return c;
}
int getCursorPosition(int *rows, int *cols) {
  if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4) return -1;
  printf("\r\n");
  char c;
  while (read(STDIN_FILENO, &c, 1) == 1) {
    if (iscntrl(c)) {
      printf("%d\r\n", c);
    } else {
      printf("%d ('%c')\r\n", c, c);
    }
  }
  editorReadKey();
  return -1;
}


int getWindowSize(int *rows, int *cols) {
  struct winsize ws;
    if (1 || ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
    if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12) 
      return getCursorPosition(rows, cols);
  } else {
    *cols = ws.ws_col;
    *rows = ws.ws_row;
    return 0;
  }
}


void editorProcessKeyPress(){
  char c = editorReadKey();
  switch(c){
  case CTRL_KEY('q'):
    write(STDOUT_FILENO,"\x1b[2J",4);
   write(STDOUT_FILENO, "\x1b[H",3);
    exit(0);
    break;
  }
}
// init 
void initEditor() {
  if (getWindowSize(&E.screenrows, &E.screencols) == -1) die("getWindowSize");
}
int main() {
  enableRawMode();
   initEditor();
  char c;
  while (1){
    editorRefreshScreen();
  	editorProcessKeyPress();
  }
  return 0;
}