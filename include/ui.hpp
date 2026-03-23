#pragma once

#include "appdata.hpp"
#include "string"

#define TL "┌" // top-left
#define TR "┐" // top-right
#define BL "└" // bottom-right
#define BR "┘" // bottom-right
#define H "─"  // horizontal
#define VR "│"  // vertical
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

void home();
void ui();
void displayDir(const AppDataNS::Dir&);
void handleKeyStroke(const char &);
int read_key();
void drawInputBox();
void createNewItem();

inline int active = 0;

} // namespace UI
