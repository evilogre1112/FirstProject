
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

using namespace std;

// --- BIẾN TOÀN CỤC (EXTERN) ---
extern int slMB;    // Số lượng máy bay hiện có
extern int sldsHK;  // Số lượng hành khách (dùng để thống kê)
extern int sldsCB;  // Số lượng chuyến bay
extern int sldsVT;  // Số lượng vé trống (dùng tạm trong các hàm tra cứu)

// --- struct cần thiết may bay ---
struct MB {
    char ShMB[15];      // số hiệu máy bay tối đa 15
    char typeMB[40];    // loại máy bay tối đa 40
    int socho;          // sô chỗ trên máy bay
};
struct MB_Stat {
    char SHMB[15];  // Lấy từ cấu trúc Máy bay
    int SLB;        // Biến đếm số lần xuất hiện trong DSCB
};
struct dsMB {
    int n = 0;
    MB* nodes[300]; // mang con tro toi da 300
};
// --- struct chuyến bay lien ket don ---
struct Datetime {
    int hh, mm, dd,mt, yyyy;          
}
struct CB {
    char maCB[15];      // mã chuyên bay
    Datetime date;      //ngày khơi hành
    char address;       // sân bay đích
    string trang_thai["huy chuyen", "con ve", "het ve", "hoan tat"];
    char ShMB[15];      // số hiệu máy bay tối đa 15
    char **Dsve;        //mảng các chuỗi  CMND, kích thước = MB.socho
    CB* next;           // truy cập phần tử típ theo
};
struct dsCB {
    int n = 0;
    CB* head = nullptr; // trỏ vào node đầu
};

// --- cấu trúc khách hàng ---
struct KH {
    char CMND[13];         // CMND có 12 kí tự chính 1 kết thúc
    char 
};
#endif
