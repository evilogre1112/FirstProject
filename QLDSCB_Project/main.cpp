//Hạn chế sử dụng bits/stdc++.h, tốn bộ nhớ
#include <iostream>
#include <iomanip>
#include "Global.h"
#include "Logic.h"
#include "UI.h"

using namespace std ;


int main(){
    TotalWidth = GetTerminalWidth();
    listMB dsMB ;
    Get_Data_MB(dsMB,path_file_MB);
    listCB dsCB ;
    Get_Data_CB(dsCB,path_file_CB);
    listHK dsHK ;
    Get_Data_HK(dsHK,path_file_HK);
    MainScreen();
    cin.ignore();
    return 0;
}
