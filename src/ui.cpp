#include "ui.hpp"
#include "appdata.hpp"
#include <iostream>
#include <stdexcept>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>

bool running = true;

AppDataNS::AppData appdata;

void UI::home() {
  std::cout << "\x1b[3J\x1b[2J\x1b[H\x1b[?25l";

  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  int mid = w.ws_col / 2;

  std::cout << "\x1b[10;H";
  for (std::string line : UI::icon) {
    std::cout << "\x1b[" << mid - 18 << "G";
    std::cout << line << std::endl;
  }
  std::cout << std::endl << std::endl << std::endl;
  for (std::string opt : UI::options) {
    std::cout << "\x1b[" << mid - 25 << "G";
    std::cout << opt << std::endl << std::endl << std::endl;
  }
  while (true) {
    int key = UI::read_key();
    if (key == 'e' || key == 'E')
      break;
    else if (key == 'q' || key == 'q')
      exit(0);
  }
}
void UI::ui() {
  UI::WorkTree tree(appdata.appdata);
  while (running) {
    std::cout << "\x1b[3J\x1b[2J\x1b[H\x1b[?25l";
    std::cout << "  🗁  " << tree.name << "/" << std::endl;

    if (active >= tree.treeItems.size()) {
      active = 0;
    }


    UI::displayTree(tree);

    int key = UI::read_key();
    handleKeyStroke(key);
  }
}

void UI::displayTree(const UI::WorkTree &tree) {
  if (tree.treeItems.size() == 0) {
    std::string pad(5, ' ');
    std::cout << pad << "No records found!!!" << pad << std::endl;
    return;
  }
  for (size_t i = 0; i < tree.treeItems.size(); i++) {
    if (tree.treeItems[i].type == DIR) {
      std::string pad(5, ' ');
      if (active == i) {
        std::cout << "\x1b[41m" << pad << CLOSED_DIR << "  "
                  << tree.treeItems[i].name << pad << "\x1b[m" << std::endl;
      } else {
        std::cout << pad << CLOSED_DIR << "  " << tree.treeItems[i].name << pad
                  << std::endl;
      }
    } else {
      std::string pad(5 - tree.treeItems[i].reqptr->method.size(), ' ');
      if (active == i) {
        std::cout << "\x1b[41m" << pad << tree.treeItems[i].reqptr->method
                  << "   " << tree.treeItems[i].name << pad << "\x1b[m"
                  << std::endl;
      } else {
        std::cout << pad << tree.treeItems[i].reqptr->method << "   "
                  << tree.treeItems[i].name << pad << std::endl;
      }
    }
  }
}

void UI::drawInputBox() {
  int row = 5, col = 80, width = 50;
  std::string message = "Create New Item:";

  // position cursor and show it
  std::cout << "\x1b[" << row - 1 << ";" << col << "H";
  std::cout << TL;
  for (int i = 0; i < width; i++) {
    if (i > 2 && size_t(i) < message.size() + 3)
      std::cout << message.at(i - 3);
    else
      std::cout << H;
  }
  std::cout << TR;

  std::cout << "\x1b[" << row << ";" << col << "H";
  std::cout << VR;
  for (int i = 0; i < width; i++)
    std::cout << " ";
  std::cout << VR;

  std::cout << "\x1b[" << row + 1 << ";" << col << "H";
  std::cout << BL;
  for (int i = 0; i < width; i++)
    std::cout << H;
  std::cout << BR;

  std::cout << "\x1b[" << row << ";" << col + 2 << "H";
  std::cout << "\x1b[?25h";
  std::cout.flush();
}

void UI::createNewItem() {
  std::string entry;

  UI::drawInputBox();

  while (true) {
    int key = read_key();

    if (key == 27)
      return;

    if (key == '\r' || key == '\n') {
      break;

    } else if (key == 127) {
      if (!entry.empty()) {
        entry.pop_back();
        // move left, erase the character
        std::cout << "\x1b[D \x1b[D";
        std::cout.flush();
      }

    } else if (key >= 32 && key <= 126) { // printable ASCII only
      entry += (char)key;
      std::cout << (char)key;
      std::cout.flush();
    }
  }

  // hide cursor again after input
  std::cout << "\x1b[?25l";
  std::cout.flush();
}

UI::TreeItem::TreeItem(const AppDataNS::Dir *dir) {
  if (dir == nullptr) {
    throw std::runtime_error("couldn't create worktree");
  }
  this->name = dir->name;
  this->type = DIR;
  this->reqptr = nullptr;
  this->dirptr = dir;
}

UI::TreeItem::TreeItem(const AppDataNS::Request *req) {
  if (req == nullptr) {
    throw std::runtime_error("couldn't create worktree");
  }
  this->name = req->name;
  this->type = REQ;
  this->reqptr = req;
  this->dirptr = nullptr;
}

UI::WorkTree::WorkTree(const AppDataNS::Dir &dir) {
  this->name = dir.name;

  for (const AppDataNS::Dir &dir : dir.dirs) {
    this->treeItems.push_back(UI::TreeItem(&dir));
  }
  for (const AppDataNS::Request &req : dir.requests) {
    this->treeItems.push_back(UI::TreeItem(&req));
  }
}
