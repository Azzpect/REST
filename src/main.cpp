#include "termios.h"
#include "ui.hpp"
#include "unistd.h"

struct TermRaw {
  termios original;

  TermRaw() {
    tcgetattr(STDIN_FILENO, &original); // save original settings
    termios raw = original;
    raw.c_lflag &= ~(ECHO | ICANON); // disable echo + canonical mode
    raw.c_cc[VMIN] = 1;              // read 1 byte at a time
    raw.c_cc[VTIME] = 0;             // no timeout
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
  }

  ~TermRaw() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &original); // restore on exit
  }
};

int main() {
  TermRaw raw;

  UI::home(); 

  UI::ui();
  return 0;
}
