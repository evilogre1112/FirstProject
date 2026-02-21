
/*
    Global chứa các hàm quy định từ trước, tất cả
    các hàm sau này đều phải được khai báo trong
    Global và dùng đúng cấu trúc.
*/

#ifndef GLOBAL_H
#define GLOBAL_H

#include <iostream>
#include <string>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <fstream>

using namespace std;

const int slMB_max = 300;
const int soHieuMB_max = 15;
const int loaiMB_max = 40;

struct MB {
    char soHieuMB[soHieuMB_max];
    char loaiMB[loaiMB_max];
    int socho;
    MB() {
        strcpy(soHieuMB,"");
        strcpy(loaiMB,"");
        socho = 0;
    }
};

struct listMB {
    int slMB;
    MB* list[slMB_max];
    listMB() {
        slMB = 0;
        for (int i = 0; i < slMB_max; i++)
            list[i] = NULL;
    }
};


#endif
