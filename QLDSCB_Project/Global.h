
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
const int ho_max=55;
const int ten_max=14;
const char* path_file_CB="Data/DSCB.txt";
const char* path_file_MB="Data/DSMB.txt";
const char* path_file_HK="Data/DSHK.txt";

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
    char **DSV;                       // mảng cấp phát động chứa cmnd chảu khách hàng
    CB* next;                         // chứa địa chỉ contro tiếp theo
    CB();                             // ko tham số
    CB(int sc);
    bool set_maCB(char ma[maCB_max]);
    bool set_ngayHK(int h, int m, int y, int mmt, int d);   //nhập vào theo giờ, phút, năm, tháng, ngày.
    bool set_sbDich(char *sbd);
    bool set_trangThai(int i);
    bool set_soHieuMB(char *shmb);
    bool set_socho(int c);

    ~CB();
};

struct listCB{
    int slCB;                         // số lượng chuyến bay
    CB* head;                        // chuyến bay dầu tiên
    listCB();
};

// --- cấu trúc hành khách cây nhị phân ---
struct HK{
    char *ho;                           // họ và tên đệm chứa 40 kí tự
    char *ten;                          // ten cho 15 ki tu
    char *cmnd;                         // cmnd có 13 kí tự
    bool phai;                     // true = gái   ;false = trai
    HK *left, *right;                  
    HK();
    ~HK();                             
    bool set_ho(char *new_ho);          // true thì đã cập nhật tên họ mới 
    bool set_ten(char *new_ten);
    char* get_ho();                   // trả về tên đệm vầ họ
    bool set_cmnd(char *new_cmnd);                  // nhâp cmnd
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

/**
 * @brief đọc file chuyến bay
 * @param dsMB danh sách chuyến bay
 * @param path_file_MB đường dẫn file chuyến bay
 * @return true nếu lấy thành công
 */

bool set_CB(listCB &dsCB, const char *path_file_CB);
/**
 * @brief đọc file máy bay
 * @param dsMB danh sách máy bay
 * @param path_file_MB đường dẫn file máy bay
 * @return true nếu lấy thành công
 */
bool set_MB(listMB &dsMB, const char *path_file_MB);
/*
    1.  ho 
    2.  ten
    3.   cmnd
*/
bool set_HK(listHK &dsHK, const char *path_file_HK);


#endif
