#pragma once

#include "appdata.hpp"
#include "string"
#include <sys/ioctl.h>

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
  TreeItem(AppDataNS::Dir *);
  TreeItem(AppDataNS::Dir *, TreeItem *);
  TreeItem(AppDataNS::Request *, TreeItem *);
  ~TreeItem() {
    for (auto *child : children) {
      delete child;
    }
  }
  WorkTreeItemType type;
  TreeItem *parent;
  AppDataNS::Request *reqptr;
  AppDataNS::Dir *dirptr;
  std::vector<TreeItem*> children;
  void addChild(std::string);
};

void home();
void ui();
void displayTree();
void goToChild(TreeItem *);
void backToParent();
void handleKeyStroke(const char &);
int read_key();

// input box
const int inputBoxRow = 4, inputBoxCol = 80, inputBoxWidth = 50;
void drawInputBox();
void clearInputBox();
void createNewItem();

inline size_t treeItemWidth = 29;
inline size_t active = 0;
inline struct winsize w;
inline TreeItem *worktree = nullptr;
} // namespace UI
