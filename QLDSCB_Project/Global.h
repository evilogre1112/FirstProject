
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
const int cmnd_max=13;

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
    short hh, mm, dd, mt;
    int yy;
    DateTime();
    short get_hh();
    short get_mm();
    short get_dd();
    short get_mt();
    int get_yy();
    bool set_hh(short h);           
    bool set_mm(short m);
    bool set_dd(short d);
    bool set_mt(short t);
    bool set_yy(int y);
};

struct CB{
    char maCB[maCB_max];              // mã chuyến bay
    DateTime ngayKH;                  // ngày khởi hành
    char* sbDich;                     // sân bay đích
    int trangThai;                    // 0 hủy chuyến, 1 còn vé, 2 hết vé, 3 hoàn tất
    char soHieuMB[soHieuMB_max];      // số hiệu máy bay
    int socho;                        // số chỗ trên máy bay
    char **DSV;                       // mảng cáp phats động chứa cmnd chảu khách hàng
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
    bool phai;                     // true = gái   ;false = trai
    HK *left, *right;                  
    HK();
    ~HK();                             
    bool set_ho(char *new_ho);          // true thì đã cập nhật tên họ mới 
    bool set_ten(char *new_ten);
    char* get_ho();                   // trả về tên đệm vầ họ
    bool set_cmnd();                  // nhâp cmnd
    char* get_cmnd();                 // trả về cmnd
    char* get_ten();
    bool set_phai(bool gt);
    bool get_phai();
};  

struct listHK{
    HK *goc ;
    int slHK;
    listHK();

};

// hàm nhập dữ liệu từ file

/*
    1.  số lượng chuyến bay
    2.  mã chuyến bay 
    3.  ngày khưởi hành
    4.  sân bay đích
    5.  trạng thái 
    6.  số hiệu máy bay
    7.  số chỗ
    8.  mảng danh sách cmnd
    9.  con trỏ next
*/
bool set_CB(listCB &dsCB, const char *cb);

/*
    1. số lượng máy bay
    2. số hiệu máy bay
    3. loại máy bay
    4. số chỗ
*/
bool set_MB(listMB &dsMB, const char *mb);
/*
    1.  ho 
    2.  ten
    3.   cmnd
*/
bool set_HK(listCB &dsHK, const char *hk);

//--- 1 lần đọc 3 file ---
bool set_file(listMB &dsMB, listCB &dsCB, listHK & dsHK, const char *mb, const char *cb, const char *hk );
#endif
