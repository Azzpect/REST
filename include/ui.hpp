#pragma once

#include "string"
#include "vector"
#include <vector>

#define TL "┌" // top-left
#define TR "┐" // top-right
#define BL "└" // bottom-right
#define BR "┘" // bottom-right
#define H "─"  // horizontal
#define V "│"  // vertical

namespace UI {

const std::string icon[] = {
    "██████╗    ███████╗    ███████╗   ████████╗", 
    "██╔══██╗   ██╔════╝    ██╔════╝   ╚══██╔══╝",
    "██████╔╝   █████╗      ███████╗      ██║",    
    "██╔══██╗   ██╔══╝      ╚════██║      ██║",
    "██║  ██║   ███████╗    ███████║      ██║",    
    "╚═╝  ╚═╝   ╚══════╝    ╚══════╝      ╚═╝",
};

const std::string options[] = {
"🗁  Workplace                                       e", 
"X  Exit                                            q"
};

class UIData {
  int active;
  std::vector<std::string> items;

  public:
  UIData() { 
    active = 0; 
    items = {"hello world", "bye world", "new world", "old world"};
  }

  int get_activeItem() { return active; }
  void inc_active() { active++; }
  void dec_active() { active--; }
  std::vector<std::string> getItems() { return items; }
  void pushItem(const std::string& item) { items.push_back(item); }
};

void home();
void ui();
void display(const std::string &, const bool &);
void handleKeyStroke(const char &);
int read_key();
void drawInputBox();
void createNewItem();

inline UIData ui_data;

} // namespace UI
