//Hạn chế sử dụng bits/stdc++.h, tốn bộ nhớ
#include <iostream>
#include <iomanip>
#include "Global.h"
#include "Logic.h"
#include "UI.h"

using namespace std ;


int main(){
    TotalWidth = GetTerminalWidth();
    listMB dsMB = Get_Data_MB(path_file_MB);
    listCB dsCB = Get_Data_CB(path_file_CB); 
    listHK dsHK = Get_Data_HK(path_file_HK);
    
    return 0;
}
