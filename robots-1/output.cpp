
#include <curses.h>
#include <string>
#include "output.h"

#define X(s,x,y) (x)*2 + 4
#define Y(s,x,y) ((s).h - (y)) + 1

#define GRAY_COLOR (A_NORMAL | COLOR_PAIR(1))

void print_hint(std::string s, char c) {
  bool needs_highlight = true;
  attrset(A_NORMAL | COLOR_PAIR(1));
  addch('[');
  for(unsigned int i = 0; i<s.size(); ++i) {
    if (needs_highlight && s[i] == c) {
      attrset(A_BOLD | COLOR_PAIR(1));
      addch(c);
      attrset(A_NORMAL | COLOR_PAIR(1));
      needs_highlight = false;
    }
    else
      addch(s[i]);
  }
  addch(']');
}

void output(state &s, std::vector<std::string> &logbufdata) {
  
  // draw the map
  for(int i = 0; i<s.w; ++i) {
    for(int j = 0; j<s.h; ++j) {
      move(Y(s,i,j), X(s,i,j));
      
      if (i == s.x && j == s.y) {
        attrset(A_BOLD | COLOR_PAIR(4));
        addch('R');
      }
      else {
        switch(s.map[i][j]) {
          case EMPTY: 
            attrset(GRAY_COLOR);
            addch('.');
            break;
          case DIAMOND: 
            attrset(A_BOLD | COLOR_PAIR(8));
            addch('@');
            break;
        }
      }
      addch(' ');
    }
  }

  // Draw the interface
 
  int yy = Y(s,0,-2);
  int xx = X(s,0,-2);

  attrset(A_NORMAL | COLOR_PAIR(1));
  move(yy, xx);
  printw("Score: %i / %i", s.score, s.initial_diamonds);
  move(yy, xx + 18);
  printw("Time: %i", s.time);
  
  if (s.score == s.initial_diamonds) {
    move(yy, xx + 36);
    printw("SUCCESS");
  }

  move(yy+2, xx);
  print_hint("Quit", 'Q');
  
  move(yy+2, xx+10);
  if (s.play)
    print_hint("Pause", 'P');
  else {
    print_hint("Play", 'P');
    addch(' ');
  }

  move(yy+2, xx+20);
  print_hint("Step", 'S') ;
  
  move(yy+2, xx+30);
  print_hint("Fast-forward", 'F') ;
 
  /* Log */
  attrset(GRAY_COLOR);
  move(yy+4, xx);
  for (unsigned int i = 0; i < 80; i++) {
    addch('-');
  }
  move(yy+4+9, xx);
  for (unsigned int i = 0; i < 80; i++) {
    addch('-');
  }
  move(yy+4, xx+62);
  addch('|');
  printw(" Robot's log ");
  addch('|');

  for (unsigned int i = 0; i < logbufdata.size(); ++i) {
    move(yy+5 + i, xx);
    clrtoeol();
  }

  attrset(A_NORMAL | COLOR_PAIR(1));
  for (unsigned int i = 0; i < logbufdata.size(); ++i) {
    move(yy+5 + i, xx);
    printw("%s", logbufdata[i].c_str());
  }

  refresh();
}
