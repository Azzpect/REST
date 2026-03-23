#include "ui.hpp"
#include "appdata.hpp"
#include "nlohmann/json.hpp"
#include <iostream>
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
  while (running) {
    AppDataNS::Dir data = appdata.appdata;
    std::cout << "\x1b[3J\x1b[2J\x1b[H\x1b[?25l";
    std::cout << "  🗁  " << data.name << "/" << std::endl;
    UI::displayDir(data);

    int key = UI::read_key();
    handleKeyStroke(key);
  }
}

void UI::displayDir(const AppDataNS::Dir &dir) {
  if (dir.dirs.size() + dir.requests.size() == 0) {
    std::string pad(5, ' ');
    std::cout << pad << "No records found!!!" << pad << std::endl;
    return;
  }
  for (auto d : dir.dirs) {
    std::string pad(5, ' ');
    std::cout << pad << CLOSED_DIR << "  " << d.name << pad << std::endl;
  }
  for (auto r : dir.requests) {
    std::string pad(5 - r.method.size(), ' ');
    std::cout << pad << r.method << "   " << r.name << pad << std::endl;
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
