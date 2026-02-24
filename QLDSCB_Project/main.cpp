//Hạn chế sử dụng bits/stdc++.h, tốn bộ nhớ
#include <iostream>
#include "Global.h"
#include "Logic.h"
#include "UI.h"


#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <conio.h>
#else
    #include <sys/ioctl.h>
    #include <unistd.h>
    #include <termios.h>
    #include <unistd.h>
    int _getch() {
        struct termios oldt, newt;
        int ch;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return ch;
    }
#endif

using namespace std ;

int main(){
    TotalWidth = GetTerminalWidth();
    MainScreen();
    return 0;
}
