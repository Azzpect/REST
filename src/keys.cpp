#include "ui.hpp"
#include "unistd.h"

int UI::read_key() {
  char c;
  read(STDIN_FILENO, &c, 1);
  return c;
}

void UI::handleKeyStroke(const char &key) {
  switch (key) {
  case 'j':
    UI::active++;
    break;
  case 'k':
    UI::active--;
    break;
  case 'q':
    exit(0);
    break;
  case 'a':
    UI::createNewItem();
    break;
  case 'l':
    if (UI::worktree->children.size() == 0) break;
    UI::goToChild(UI::worktree->children[active]);
    break;
  case 'h':
    UI::backToParent();
    break;
  }
}
