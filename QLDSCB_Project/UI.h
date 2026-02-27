#ifndef UI_H
#define UI_H

#include <iostream>
#include <string>
#include "Global.h"
#include "Logic.h"
using namespace std;
extern int TotalWidth;

#define boxWidth 60

#define RESET       "\033[0m"
#define YELLOW      "\033[33m"  
#define GREEN       "\033[32m"
#define GRAY        "\033[90m"
#define WHITE       "\033[37m" 
#define RED         "\033[31m"
#define MAGENTA     "\033[35m"
#define CYAN        "\033[36m"
#define BLUE        "\033[34m"
#define BOLD        "\033[1m"

// Các màu Bold (In đậm/Sáng hơn)
#define BOLDRED     "\033[1;31m"
#define BOLDGREEN   "\033[1;32m"
#define BOLDYELLOW  "\033[1;33m"
#define BOLDBLUE    "\033[1;34m"
#define BOLDMAGENTA "\033[1;35m"
#define BOLDCYAN    "\033[1;36m"
#define BOLDWHITE   "\033[1;37m"
#define BOLDUNDERLINE "\033[1;4m"

#define BG_YELLOW        "\033[43m"
#define BG_BRIGHT_YELLOW "\033[103m"
#define TEXT_BLACK       "\033[30m"

enum NavKey {
    NAV_UP,
    NAV_DOWN,
    NAV_ENTER,
    NAV_ESC,
    NAV_UNKNOWN
};

// ╔ ═ ╗ ║ ╠ ╚ ╝ ▶ ■
int GetTerminalWidth();
void ClearScreen();
int visualLength(const string& s);
void PrintBox(string text,bool ABOVE , bool UNDER );
void SubMenu(string title, string options[], int numOptions);
void MainScreen();
NavKey GetNavKey();

#endif