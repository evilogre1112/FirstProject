
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
const int soHieuMB_max = 16;
const int loaiMB_max = 41;
const int maCB_max= 16;

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
    int hh, mm, dd, mt, yy;
    DateTime();
    int get_hh();
    int get_mm();
    int get_dd();
    int get_mt();
    int get_yy();
    bool set_hh(int h);
    bool set_mm(int m);
    bool set_dd(int d);
    bool set_mt(int t);
    bool set_yy(int y);
};

struct CB{
    char maCB[maCB_max];              // mã chuyến bay
    DateTime ngayKH;                  // ngày khởi hành
    char* sbDich;                     // sân bay đích
    int trangThai;                    // 0 hủy chuyến, 1 còn vé, 2 hết vé, 3 hoàn tất
    char soHieuMB[soHieuMB_max];      // số hiệu máy bay
    int socho;                        // số chỗ trên máy bay
    char **DSV;                       // mảng con trỏ chứa cmnd chảu khách hàng
    CB* next;                         // chứa địa chỉ contro tiếp theo
    CB();                             // ko tham số
    CB(int sc);
    ~CB();
};

struct listCB{
    int slCB;                         // số lượng chuyến bay
    CB* head;                        // chuyến bay dầu tiên
    listCB();
};

// --- cấu trúc hành khách cây nhị phân ---
struct HK{
    char* ho;                           // họ và tên đệm chứa 40 kí tự
    char* ten;                          // ten cho 15 ki tu
    char* cmnd;                         // cmnd có 13 kí tự
    bool gioi_tinh;                     // true = gái   ;false = trai
    HK *left, *right;                  
    HK();
    ~HK();                             
    bool set_ho(char *new_ho);          // true thì đã cập nhật tên họ mới 
    bool set_ten(char *new_ten);
    char* get_ho();                   // trả về tên đệm vầ họ
    bool set_cmnd();                  // nhâp cmnd
    char* get_cmnd();                 // trả về cmnd
    char* get_ten();
};  

struct listHK{
    HK *goc ;
    int slHK;
    listHK();
};

void ClearScreen();
#endif
