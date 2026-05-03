
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
#include <algorithm> 
#include <iomanip>   

using namespace std;

const int slMB_max = 300;
const int slHK_max = 100000;
const int soHieuMB_max = 40;
const int loaiMB_max = 40;
const int maCB_max= 16;
const int cmnd_max = 13;    
const int ho_max=50;
const int ten_max=14;
const int sbDich_max=50;
const int socho_max=600;
static const char* path_file_CB = "Data/DSCB.txt";
static const char* path_file_MB = "Data/DSMB.txt";
static const char* path_file_HK = "Data/DSHK.txt";


//---- cấu trúc stack cho mọi data dùng danh sách liên kết ----//
template <typename T>
struct Stack {
    T data;
    Stack* next;
    Stack* top;
    Stack(){
        top = NULL;
    }
    Stack(T val){
        data = val;
        next = NULL;
        top = NULL;
    }
    bool isEmpty() {
        return top == NULL;
    }
    void push(T val){
        Stack* new1 =new Stack(val);
        new1->next = top;
        top = new1;
    }
    T pop(){
        Stack*tmp =top;
        top = top->next;
        T val = tmp->data;
        delete tmp;
        return val;
    }
};


//---- Cáu trúc thời gian -----

struct DateTime{
    int hh, mm, dd, mt, yy;
    DateTime();
    int get_hh()const;
    int get_mm()const;
    int get_dd()const;
    int get_mt()const;
    int get_yy()const;
    bool set_hh(int h);
    bool set_mm(int m);
    bool set_dd(int d);
    bool set_mt(int t);
    bool set_yy(int y);
    void time_now();
};


// --- cấu trúc chuyến bay ---

struct CB{
    char maCB[maCB_max];              // mã chuyến bay
    DateTime ngayKH;                  // ngày khởi hành
    char* sbDich;                     // sân bay đích
    int trangThai;                    // 0 hủy chuyến, 1 còn vé, 2 hết vé, 3 hoàn tất
    char soHieuMB[soHieuMB_max];      // số hiệu máy bay
    int socho;                        // số chỗ trên máy bay
    int sove;                         // số vé đã đặt trên chuyến bay đó
    char **DSV;                       // mảng cấp phát động chứa cmnd chảu khách hàng 3 kí tự cuối chứa vị trí vé
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
    void Clear();
};
// dsCB gồm 1 nhiều CB , 1 mỗi CB có

// -- Cấu truc đánh dấu chuyến bay
struct markCB {
    CB* mark;
    markCB* next;
    markCB();
};

struct markList {
    markCB* head;
    markList();
};

// --- cấu trúc máy bay ---
struct MB {
    char soHieuMB[soHieuMB_max];
    char loaiMB[loaiMB_max];
    int socho;
    int SLB;
    markList dsHD;
    MB();
    bool set_soHieuMB(char *shmb);
    bool set_loaiMB(char *loai);
    bool set_socho(int c);
};

struct listMB {
    int slMB;
    MB* list[slMB_max];
    void Clear();
    listMB();
};


// --- cấu trúc hành khách cây nhị phân ---
struct HK{
    char *ho;                           // họ và tên đệm chứa 40 kí tự
    char *ten;                          // ten cho 15 ki tu
    char *cmnd;                         // cmnd có 13 kí tự
    bool phai;                          // true = gái   ;false = trai
    HK *left, *right;
    markList dsDatVe;
    HK();
    ~HK();                             
    bool set_ho(char *new_ho);          // true thì đã cập nhật tên họ mới 
    bool set_ten(char *new_ten);
    char* get_ho();                     // trả về tên đệm vầ họ
    bool set_cmnd(char *new_cmnd);      // nhâp cmnd
    char* get_cmnd();                   // trả về cmnd
    char* get_ten();
    bool set_phai(bool gt);
    bool get_phai();
};  

struct listHK{
    HK *goc ;
    int slHK;
    listHK();
};


#endif
