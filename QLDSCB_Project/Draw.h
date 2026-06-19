#ifndef DRAW_H
#define DRAW_H


#include <iostream>
#include "UI.h"
#include "Global.h"
#include <string>


using namespace std;


struct LineInfo {
    int byteCount;   // Số byte để dùng trong substr()
    int visualWidth; // Độ dài hiển thị thực tế để tính căn giữa
};

/**
 * @brief Hàm này dùng để tính toán số byte cần đọc từ chuỗi text bắt đầu từ startByte để vừa đủ hiển thị trong maxWidth ký tự, đồng thời trả về độ dài hiển thị thực tế của phần đó
 * @param text Chuỗi văn bản cần hiển thị trong hộp
 * @param startByte Vị trí byte bắt đầu trong chuỗi text để tính toán
 * @param maxWidth Số ký tự tối đa có thể hiển thị trong một dòng của hộp
 * @return LineInfo Cấu trúc chứa số byte cần đọc và độ dài hiển
 */
LineInfo getLineInfo(const string& text, int startByte, int maxWidth);

/**
 * @brief Hàm này dùng để đếm số dòng cần thiết để hiển thị toàn bộ text trong một hộp có chiều rộng innerWidth, tính cả việc xuống dòng khi vượt quá chiều rộng
 * @param text Chuỗi văn bản cần hiển thị trong hộp
 * @param innerWidth Độ rộng thực tế bên trong hộp, đã trừ đi 2 ký tự viền
 * @return int Số dòng cần thiết để hiển thị toàn bộ text trong hộp
 */
int countLinesNeeded(const string& text, int innerWidth) ;

/**
 * @brief hàm SmallBox nhưng ít tham số hơn
 */
void SmallBox(string text, int Width = 5, int Height = 40, string color = WHITE) ;
/**
 * @brief Hàm này dùng để vẽ một hộp nhỏ với tùy chọn có/không cạnh
 * @param text Nội dung sẽ được hiển thị trong hộp
 * @param ABOVE Nếu true, vẽ cạnh trên của hộp, nếu false thì không
 * @param UNDER Nếu true, vẽ cạnh dưới của hộp, nếu false thì không
 * @param LEFT Nếu true, vẽ cạnh trái của hộp, nếu false thì không
 * @param RIGHT Nếu true, vẽ cạnh phải của hộp, nếu false thì không
 * @param Height Chiều cao của hộp
 * @param Width Chiều rộng của hộp
 * @param color Màu sắc của hộp
 */
void SmallBox(string text = "" , bool ABOVE = true , bool UNDER = true , bool LEFT = true , bool RIGHT = true, int Width = 15, int Height = 5, string color = WHITE);

/**
 * @brief Vẽ con mèo tại vị trí x và y :D
 */
void  DrawACat(int x = 59,int y =25);

/**
 * @brief In ký tự xuống dòng bao gồm ký tự hỗn hợp, xuống dòng
 * @param StartX vị trí bắt đầu X
 * @param StartY vị trí bắt đầu Y
 * @param text chuỗi truyền vào 
 */
void PrintMultiLineText(int startX, int startY, const string& text);


/**
 * @brief Các hàm vẽ lịch tại vị trí X và Y có cho chỉnh giờ hoặc không 
 * @param rowpadding độ giãn cách mỗi hàng
 * @param colpadding độ giãn cách mỗi cột
 * @param CanPast có thể chọn ngày quá khứ không
 */
void    UiDrawCalendarWithoutTime(int x , int y,int rowpadding , int colpadding,string DateString);
void    UiDrawCalendar(int x , int y,int rowpadding , int colpadding,string DateString);
NavKey  DrawCalendar(int x, int y,int rowpadding , int colpadding, string& DateString,bool CanPast = false);
NavKey  DrawCalendarWithoutTime(int x , int y,int rowpadding , int colpadding,string& DateString,bool CanPast = false);
void    UiDrawCalendarWithoutTime(int x , int y,int rowpadding , int colpadding,string DateString);
void    UiDrawCalendar(int x , int y,int rowpadding , int colpadding,string DateString);

#endif