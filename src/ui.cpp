#include "ui.hpp"
#include "appdata.hpp"
#include <boost/algorithm/string/trim.hpp>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>

bool running = true;

AppDataNS::AppData appdata;

void UI::home() {
  std::cout << "\x1b[3J\x1b[2J\x1b[H\x1b[?25l";

  ioctl(STDOUT_FILENO, TIOCGWINSZ, &UI::w);
  int mid = UI::w.ws_col / 2;

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
  std::cout << "\x1b[3J\x1b[2J\x1b[H\x1b[?25l";
  UI::worktree = new UI::TreeItem(&appdata.appdata);
  for (int i = 0; i < UI::w.ws_row; i++) {
    std::cout << "\x1b[30G" << VR << std::endl;
  }
  while (running) {

    if (active >= worktree->children.size()) {
      active = 0;
    }

    UI::displayTree();

    int key = UI::read_key();
    handleKeyStroke(key);
  }
}

void UI::displayTree() {
  std::cout << "\x1b[H" << "  🗁  " << UI::worktree->dirptr->name << "/"
            << std::endl;
  size_t i = 0;

  if (UI::worktree->children.size() == 0) {
    std::string pad(3, ' ');
    std::cout << "\x1b[" << i + 2 << ";0H" << pad << "No records found!!!"
              << pad << std::endl;
    i++;
  } else {
    for (; i < UI::worktree->children.size(); i++) {
      std::cout << "\x1b[" << i + 2 << ";0H";
      if (UI::worktree->children[i]->type == DIR) {
        std::string pad(6, ' ');
        std::string line = pad + std::string(CLOSED_DIR) + "  " +
                           UI::worktree->children[i]->dirptr->name;
        std::string line_with_pad =
            line + std::string(treeItemWidth - line.size() + 3, ' ');

        if (active == i) {
          std::cout << "\x1b[41m" << line_with_pad << "\x1b[m" << std::endl;
        } else {
          std::cout << line_with_pad << std::endl;
        }
      } else {
        std::string pad(6 - UI::worktree->children[i]->reqptr->method.size(), ' ');
        std::string line = pad + UI::worktree->children[i]->reqptr->method +
                           "   " + UI::worktree->children[i]->reqptr->name;
        std::string line_with_pad =
            line + std::string(treeItemWidth - line.size(), ' ');
        if (active == i) {

          std::cout << "\x1b[41m" << line_with_pad << "\x1b[m" << std::endl;
        } else {
          std::cout << line_with_pad << std::endl;
        }
      }
    }
  }
  for (; i < UI::w.ws_row; i++) {
    std::cout << "\x1b[" << i + 2 << ";0H" << std::string(treeItemWidth, ' ');
  }
  std::cout.flush();
}

void UI::drawInputBox() {
  int row = UI::inputBoxRow, col = UI::inputBoxCol, width = UI::inputBoxWidth;
  std::string message = "Create New Item:";

  // position cursor and show it
  std::cout << "\x1b[" << row << ";" << col << "H";
  std::cout << TL;
  for (int i = 0; i < width - 2; i++) {
    if (i > 2 && size_t(i) < message.size() + 3)
      std::cout << message.at(i - 3);
    else
      std::cout << H;
  }
  std::cout << TR;

  std::cout << "\x1b[" << row + 1 << ";" << col << "H";
  std::cout << VR;
  for (int i = 0; i < width - 2; i++)
    std::cout << " ";
  std::cout << VR;

  std::cout << "\x1b[" << row + 2 << ";" << col << "H";
  std::cout << BL;
  for (int i = 0; i < width - 2; i++)
    std::cout << H;
  std::cout << BR;

  std::cout << "\x1b[" << row + 1 << ";" << col + 2 << "H";
  std::cout << "\x1b[?25h";
  std::cout.flush();
}

void UI::clearInputBox() {
  for (int i = UI::inputBoxRow; i - UI::inputBoxRow < 3; i++) {
    std::cout << "\x1b[" << i << ";" << UI::inputBoxCol << "H"
              << std::string(UI::inputBoxWidth, ' ');
  }
  std::cout.flush();
}

void UI::goToChild(TreeItem *newTree) {
  if (newTree->type == REQ) {
    return;
  }
  UI::worktree = newTree;
  active = 0;
}

void UI::backToParent() {
  if (UI::worktree->parent == nullptr)
    return;
  UI::worktree = UI::worktree->parent;
  active = 0;
}

void UI::createNewItem() {
  std::string entry;

  UI::drawInputBox();

  while (true) {
    int key = read_key();

    if (key == 27)
      break;

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

  UI::worktree->addChild(entry);

  // hide cursor again after input
  std::cout << "\x1b[?25l";
  UI::clearInputBox();
  std::cout.flush();
}

UI::TreeItem::TreeItem(AppDataNS::Dir *dir) {
  if (dir == nullptr) {
    throw std::runtime_error("couldn't create worktree");
  }
  this->type = DIR;
  this->reqptr = nullptr;
  this->dirptr = dir;
  this->parent = nullptr;
  this->children = {};
  for (auto &d : dir->dirs) {
    this->children.push_back(new UI::TreeItem(&d, this));
  }
  for (auto &r : dir->requests) {
    this->children.push_back(new UI::TreeItem(&r, this));
  }
}

UI::TreeItem::TreeItem(AppDataNS::Dir *dir, UI::TreeItem *parent) {
  if (dir == nullptr) {
    throw std::runtime_error("couldn't create worktree");
  }
  this->type = DIR;
  this->reqptr = nullptr;
  this->dirptr = dir;
  this->parent = parent;
  this->children = {};
  for (auto &d : dir->dirs) {
    this->children.push_back(new UI::TreeItem(&d, this));
  }
  for (auto &r : dir->requests) {
    this->children.push_back(new UI::TreeItem(&r, this));
  }
}

UI::TreeItem::TreeItem(AppDataNS::Request *req, UI::TreeItem *parent) {
  if (req == nullptr) {
    throw std::runtime_error("couldn't create worktree");
  }
  this->type = REQ;
  this->reqptr = req;
  this->dirptr = nullptr;
  this->parent = parent;
}

void UI::TreeItem::addChild(std::string name) {
  boost::algorithm::trim(name);
  if (name == "") return;
  std::regex r("(.*)(/)");
  if (std::regex_match(name, r)) {
    UI::worktree->dirptr->dirs.push_back(AppDataNS::Dir(name));
    AppDataNS::Dir* newDir = &UI::worktree->dirptr->dirs.back();
    UI::worktree->children.push_back(new UI::TreeItem(newDir, worktree));
  } else {
    UI::worktree->dirptr->requests.push_back(AppDataNS::Request(name));
    AppDataNS::Request* newReq = &UI::worktree->dirptr->requests.back();
    UI::worktree->children.push_back(new UI::TreeItem(newReq, worktree));
  }
}
