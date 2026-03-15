//Hạn chế sử dụng bits/stdc++.h, tốn bộ nhớ
#include <iostream>
#include <iomanip>
#include "Global.h"
#include "Logic.h"
#include "UI.h"

using namespace std ;

listMB dsMB ;
listCB dsCB ;
listHK dsHK ;

int main(){
    TotalWidth = GetTerminalWidth();
    Get_Data_MB(dsMB,path_file_MB);
    Get_Data_CB(dsCB,path_file_CB);
    Get_Data_HK(dsHK,path_file_HK);
    Sort_CB(dsCB);
    int slMB = dsMB.slMB ;
    Sort_MB(dsMB,0,slMB-1);
    MainScreen();
    cin.ignore();
    return 0;
}
