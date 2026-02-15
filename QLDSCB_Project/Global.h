
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

using namespace std;

// --- BIẾN TOÀN CỤC (EXTERN) ---
extern int slMB;    // Số lượng máy bay hiện có
extern int sldsHK;  // Số lượng hành khách (dùng để thống kê)
extern int sldsCB;  // Số lượng chuyến bay
extern int sldsVT;  // Số lượng vé trống (dùng tạm trong các hàm tra cứu)


#endif