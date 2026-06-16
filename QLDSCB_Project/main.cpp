//Hạn chế sử dụng bits/stdc++.h, tốn bộ nhớ
#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#include <windows.h>
#else
#include <clocale>
#endif
#include <iostream>
#include <iomanip>
#include "Global.h"
#include "Logic.h"
#include "UI.h"
#include "Extentions.h"

using namespace std ;

int main(){
    listMB dsMB;
    listCB dsCB ;
    listHK dsHK ;
    listSBD dsSBD;
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);

        CONSOLE_FONT_INFOEX cfi = {};
        cfi.cbSize = sizeof(cfi);
        GetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);

        wcscpy_s(cfi.FaceName, L"Cascadia Code");  // ← đổi tên font ở đây
        cfi.dwFontSize.Y = 16;
        SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
    #else
        setlocale(LC_ALL, "en_US.UTF-8");
    #endif
    TotalWidth = GetTerminalWidth();
    TotalHeight = GetTerminalHeight();
    Get_Data_MB(dsMB,path_file_MB);
    Get_Data_SBD(dsSBD, path_file_SBD);
    Get_Data_HK(dsHK,path_file_HK);
    Get_Data_CB(dsCB,dsMB,dsHK, path_file_CB);
    
    Sort_CB(dsCB);
    int slMB = dsMB.slMB ;
    Sort_MB(dsMB,0,slMB-1);
    TruyenThamSo(dsMB,dsCB,dsHK,dsSBD);
    Set_Data_CB(dsCB, path_file_CB);
    Set_Data_HK(dsHK, path_file_HK);
    Set_Data_MB(dsMB, path_file_MB);
    MainScreen();
    cin.ignore();
    return 0;
}
