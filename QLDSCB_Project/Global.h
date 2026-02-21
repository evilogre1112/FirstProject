
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
#include <conio.h>

using namespace std;

const int slMB_max = 300;
const int soHieuMB_max = 15;
const int loaiMB_max = 40;
const int maCB_max= 15;

// --- cấu trúc máy bay ---
struct MB {
    char soHieuMB[soHieuMB_max];
    char loaiMB[loaiMB_max];
    int socho;
    MB();
};

struct listMB {
    int slMB;
    MB* list[slMB_max];
    listMB();
};

struct MB_Stat {
        char soHieuMB[soHieuMB_max];  // Lấy từ cấu trúc Máy bay
        int SLB;                      // Biến đếm số lần xuất hiện trong DSCB
    };

// --- cấu trúc chuyến bay ---
struct DateTime{
    short hh, mm, dd, mt, yy;
    DateTime();
    void kiemtraDate();
};

struct CB{
    char maCB[maCB_max];              // mã chuyến bay
    DateTime ngayKH;                  // ngày khởi hành
    char* sbDich;                     // sân bay đích
    int trangThai;                    // 0 hủy chuyến, 1 còn vé, 2 hết vé, 3 hoàn tất
    char soHieuMB[soHieuMB_max];      // số hiệu máy bay
    int socho;
    char **DSV;                       // mảng con trỏ chứa cmnd chảu khách hàng
    CB* next;                         // chứa địa chỉ contror tiếp theo
    CB(int sc);
    ~CB();
};

struct listCB{
    int slCB;                         // số lượng chuyến bay
    CB* CBdau;                        // chuyến bay dầu tiên
    listCB();
};

// --- cấu trúc hành khách ---
struct HK{
    char* ho;                         // họ và tên đệm chứa 40 kí tự
    char* ten;                        // ten cho 15 ki tu
    char* cmnd;                         // cmnd có 13 kí tự
    char* ho_ten_full;
    HK *trai, *phai;                  
    HK();                             
    void ho_ten();                 // kt chuỗi có số ko cho so bat nhap lai
    void nhap_cmnd();              
};  

struct listHK{
    HK *goc ;
    int slHK;
    listHK();
};
#endif
