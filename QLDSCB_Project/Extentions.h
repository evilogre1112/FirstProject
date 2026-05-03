#ifndef EXTENTIONS_H
#define EXTENTIONS_H

#include "Global.h"
#include "Logic.h"
#include <fstream>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <string>
#include <ctime>

using namespace std;

/**
 * @brief chuyển đổi DateTime thành chuỗi định dạng dd/mm/yyyy hh:mm
 * @param dt đối tượng DateTime cần chuyển đổi
 * @return con trỏ đến chuỗi đã được cấp phát động, cần giải phóng sau khi sử dụng
 */

char *time_to_string(const DateTime &dt);
/**
 * @brief đọc file chuyến bay
 * @param dsMB danh sách chuyến bay
 * @param path_file_MB đường dẫn file chuyến bay
 * @return true nếu lấy thành công
 */

//=========== các hàm đọc dữ liệu ===========//

bool Get_Data_CB(listCB &dsCB, listMB &dsMB, const char *path_file_CB);
/**
 * @brief đọc file máy bay
 * @param dsMB danh sách máy bay
 * @param path_file_MB đường dẫn file máy bay
 * @return true nếu lấy thành công
 */
bool Get_Data_MB(listMB &dsMB, const char *path_file_MB);
/**
 * @brief đọc file hành khách
 * @param dsHK danh sách hành khách
 * @param path_file_HK đường dẫn file hành khách
 * @return true nếu lấy thành công
 */
bool Get_Data_HK(listHK &dsHK, const char *path_file_HK);

//=========== các hàm ghi hết dữ liệu ===========//

/**
 * @brief ghi dữ liệu chuyến bay vào file
 * @param dsCB danh sách chuyến bay
 * @param path_file_CB đường dẫn file chuyến bay
 * @return true nếu ghi thành công
 */
bool Set_Data_CB(listCB &dsCB, const char *path_file_CB);
/**
 * @brief ghi dữ liệu máy bay vào file
 * @param dsMB danh sách máy bay
 * @param path_file_MB đường dẫn file máy bay
 * @return true nếu ghi thành công
 */
bool Set_Data_MB(listMB &dsMB, const char *path_file_MB);
/**
 * @brief ghi dữ liệu hành khách vào file
 * @param dsHK danh sách hành khách
 * @param path_file_HK đường dẫn file hành khách
 * @return true nếu ghi thành công
 */
bool Set_Data_HK(listHK &dsHK, const char *path_file_HK);

//=========== các hàm ghi 1 dữ liệu ===========//

/**
* @brief ghi dữ liệu chuyến bay vào file
* @param CB chuyến bay cần ghi  
* @param path_file_CB đường dẫn file chuyến bay
* @return true nếu ghi thành công
*/
bool Set_Data_CB(CB &CB, const char *path_file_CB);
/**
* @brief ghi dữ 1 dữ liệu máy bay vào file
* @param MB máy bay cần ghi
* @param path_file_MB đường dẫn file máy bay
* @return true nếu ghi thành công
*/
bool Set_Data_MB(MB &MB, const char *path_file_MB);
/**
* @brief ghi dữ liệu 1 hành khách vào file
* @param HK hành khách cần ghi
* @param path_file_HK đường dẫn file hành khách
* @return true nếu ghi thành công
*/
bool Set_Data_HK(HK &HK, const char *path_file_HK);

#endif