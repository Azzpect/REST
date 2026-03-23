#pragma once

#include "nlohmann/json.hpp"
#include "string"
#include "vector"
#include <cstdlib>
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
  nlohmann::json appdata;
  std::string appdata_dir = std::string(std::getenv("HOME")) + "/.local/share/rest";
  std::string appdata_file = std::string(std::getenv("HOME")) + "/.local/share/rest/appdata.json";

  public:
  UIData();
  int get_activeItem() { return active; }
  void inc_active() { active++; }
  void dec_active() { active--; }
  nlohmann::json getAppdata() { return appdata; }
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
