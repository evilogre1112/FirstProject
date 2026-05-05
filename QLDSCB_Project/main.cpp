//Hạn chế sử dụng bits/stdc++.h, tốn bộ nhớ
#ifdef _WIN32
#include <windows.h> 
#endif
#include <iostream>
#include <iomanip>
#include "Global.h"
#include "Logic.h"
#include "UI.h"
#include "Extentions.h"

using namespace std ;



int main(){
    listMB dsMB ;
    listCB dsCB ;
    listHK dsHK ;
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
    #endif
    TotalWidth = GetTerminalWidth();
    Get_Data_MB(dsMB,path_file_MB);
    Get_Data_HK(dsHK,path_file_HK);
    Get_Data_CB(dsCB,dsMB,dsHK, path_file_CB);
    Sort_CB(dsCB);
    int slMB = dsMB.slMB ;
    Sort_MB(dsMB,0,slMB-1);
    TruyenThamSo(dsMB,dsCB,dsHK);
    MainScreen();
    cin.ignore();
    return 0;
}
