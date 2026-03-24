#pragma once

#include "appdata.hpp"
#include "string"

#define TL "┌" // top-left
#define TR "┐" // top-right
#define BL "└" // bottom-right
#define BR "┘" // bottom-right
#define H "─"  // horizontal
#define VR "│" // vertical
#define CLOSED_DIR "🖿"
#define OPENED_DIR "🗁 "

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
    "🖿  Workplace                                       e",
    "X  Exit                                            q"};

enum WorkTreeItemType { DIR, REQ };

class TreeItem {
public:
  TreeItem(const AppDataNS::Dir *);
  TreeItem(const AppDataNS::Request *);
  std::string name;
  WorkTreeItemType type;
  const AppDataNS::Request *reqptr;
  const AppDataNS::Dir *dirptr;
};
class WorkTree {
public:
  WorkTree(const AppDataNS::Dir &);
  std::string name;
  AppDataNS::Dir parent;
  std::vector<TreeItem> treeItems;
};

void home();
void ui();
void displayTree(const WorkTree &);
void handleKeyStroke(const char &);
int read_key();
void drawInputBox();
void createNewItem();

inline size_t active = 0;
} // namespace UI
