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

string time_now();
/**
 * @brief chuyển đổi chuỗi định dạng dd/mm/yyyy hh:mm thành DateTime
 * @param time_str chuỗi thời gian cần chuyển đổi
 * @return đối tượng DateTime đã được chuyển đổi, nếu có lỗi sẽ trả về DateTime với tất cả các trường bằng 0
 */
DateTime time_now(string time_str);

int getFirstDayOfWeek(int month, int year) ;

bool isNotPastTime(const DateTime& t, const DateTime& cur);

bool isLeap(int y);

int daysInMonth(int m, int y);

long long toMinutes(DateTime dt);

long long ss_ngay(DateTime const &a, DateTime const &b);

// Hàm lấy Ngày (DD) - Cắt 2 ký tự từ vị trí 0
int GetDayFromStr(const string &datetimeStr);
// Hàm lấy Tháng (MM) - Cắt 2 ký tự từ vị trí 3
int GetMonthFromStr(const string &datetimeStr);
// Hàm lấy Năm (YYYY) - Cắt 4 ký tự từ vị trí 6
int GetYearFromStr(const string &datetimeStr);
// Hàm lấy Giờ (HH) - Cắt 2 ký tự từ vị trí 11
int GetHourFromStr(const string &datetimeStr);
// Hàm lấy Phút (MM) - Cắt 2 ký tự từ vị trí 14
int GetMinuteFromStr(const string &datetimeStr);
// Hàm chuyển đổi DateTime thành chuỗi định dạng "DD/MM/YYYY HH:MM"
string ToStringDate(DateTime dt);

DateTime time_now(string time_str);
/**
 * @brief  đưa về Thứ của ngày tháng năm muốn xét 
 * @param dt là chứa ngày tháng năm theo form dd/mt/yyyy hh:mm tính Thứ của ngày tháng năm muốn xét 
 * @return thứ dày tiên trong tháng đó
 *          0 là chủ nhật
 *          1 là thứ 2
 *          ...
 *          6 là thứ 7
 */
int Get_Weekday(DateTime dt);
/**
 * @brief  đưa về Thứ của ngày tháng năm muốn xét 
 * @param time_str là chứa ngày tháng năm muốn tính thứ "dd/mt/yyyy hh:mm" Thứ của ngày tháng năm muốn xét 
 * @return thứ dày tiên trong tháng đó
 *          0 là chủ nhật
 *          1 là thứ 2
 *          ...
 *          6 là thứ 7
 */
int Get_Weekday(string time_str);

//=========== các hàm đọc dữ liệu ===========//
/**
 * @brief đọc file chuyến bay
 * @param dsMB danh sách chuyến bay
 * @param path_file_MB đường dẫn file chuyến bay
 * @return true nếu lấy thành công
 */
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