#include "unistd.h"
#include "ui.hpp"

int UI::read_key() {
  char c;
  read(STDIN_FILENO, &c, 1);
  return c;
}

void UI::handleKeyStroke(const char &key) {
  switch (key) {
  case 'j':
    UI::ui_data.inc_active();
    break;
  case 'k':
    UI::ui_data.dec_active();
    break;
  case 'q':
    exit(0);
    break;
  case 'a':
    UI::createNewItem();
    break;
  }
}
