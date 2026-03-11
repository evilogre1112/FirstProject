#ifndef UI_H
#define UI_H

#include <iostream>
#include <string>
#include "Global.h"
#include "Logic.h"
using namespace std;
extern int TotalWidth;

#define boxWidth 60

#define RESET       "\033[0m"
#define YELLOW      "\033[33m"  
#define GREEN       "\033[32m"
#define GRAY        "\033[90m"
#define WHITE       "\033[37m" 
#define RED         "\033[31m"
#define MAGENTA     "\033[35m"
#define CYAN        "\033[36m"
#define BLUE        "\033[34m"
#define BOLD        "\033[1m"

// Các màu Bold (In đậm/Sáng hơn)
#define BOLDRED     "\033[1;31m"
#define BOLDGREEN   "\033[1;32m"
#define BOLDYELLOW  "\033[1;33m"
#define BOLDBLUE    "\033[1;34m"
#define BOLDMAGENTA "\033[1;35m"
#define BOLDCYAN    "\033[1;36m"
#define BOLDWHITE   "\033[1;37m"
#define BOLDUNDERLINE "\033[1;4m"

#define BG_YELLOW        "\033[43m"
#define BG_BRIGHT_YELLOW "\033[103m"
#define TEXT_BLACK       "\033[30m"

// Ký tự vẽ khung
#define AL "╔"
#define AR "╗"
#define BL "╚"
#define BR "╝"
#define HL "═"
#define VL "║"
#define CL "╠"
#define CR "╣"
#define AP "▲"
#define AD "▼"
#define ARL "◄"  // Hoặc "◀"
#define ARR "►"  // Hoặc "▶"
#define SAP "↑"
#define SAD "↓"
#define SQ "■"
#define NSQ "□"
#define CI "○"
#define PT "◉"
#define SB "█"
#define TLC "╭"
#define TRC "╮"
#define VT  "│"
#define BRC "╯"
#define BLC "╰"
#define HZ "─" 
#define BLC "╰"
#define DM "◈"

enum NavKey {
    NAV_UP,
    NAV_DOWN,
    NAV_ENTER,
    NAV_ESC,
    NAV_UNKNOWN
};

struct LineInfo {
    int byteCount;   // Số byte để dùng trong substr()
    int visualWidth; // Độ dài hiển thị thực tế để tính căn giữa
};



/**
 * @brief Hàm này dùng để đo chiều rộng của terminal hiện tại, tương thích với cả Windows và UNIX (Linux/macOS)
 * @return int Chiều rộng của terminal tính bằng số cột ký tự
 */
int GetTerminalWidth();

/**
 * @brief Xoá màn hình, tương thích với cả Windows và UNIX (Linux/macOS)
 */
void ClearScreen();

/**
 * @brief Hàm này dùng để tính độ dài hiển thị thực tế của một chuỗi, bỏ qua các mã ANSI và tính đúng các ký tự Unicode có thể chiếm nhiều byte
 * @param s Chuỗi cần tính độ dài hiển thị
 * @return int Độ dài hiển thị thực tế của chuỗi
 */
int visualLength(const string& s);

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
 * @brief Hàm này dùng để di chuyển con trỏ đến vị trí (x, y
 * @param x Tọa độ X (cột) mà con trỏ sẽ di chuyển đến
 * @param y Tọa độ Y (dòng) mà con trỏ sẽ di
 */
void Gotoxy(int x, int y);

/**
 * @brief Hàm này dùng để lấy vị trí con trỏ hiện tại trên macOS/Linux bằng cách đọc chuỗi ANSI trả về từ terminal
 * @param x Tham chiếu để lưu giá trị X của con trỏ
 * @param y Tham chiếu để lưu giá trị Y của con trỏ
 */
void getCursorPosMac(int &x, int &y);

/**
 * @brief Hàm này sẽ trả về vị trí con trỏ hiện tại
 * @return int Giá trị X của con trỏ hiện tại.
 */
int whereX();

/**
 * @brief Hàm này sẽ trả về vị trí con trỏ hiện tại
 * @return int Giá trị X của con trỏ hiện tại.
 */
int whereY();

/**
 * @brief Hàm này dùng để vẽ một hộp đơn giản chỉ có tiêu đề ở giữa, với tùy chọn có/không cạnh trên và dưới
 * @param text Nội dung tiêu đề sẽ được căn giữa trong hộp
 * @param ABOVE Nếu true, vẽ cạnh trên của hộp, nếu false thì không
 * @param UNDER Nếu true, vẽ cạnh dưới của hộp, nếu false thì không
 */
void PrintBox(string text,bool ABOVE , bool UNDER );

void ClearRegion(int x, int y, int width, int height);

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
void SmallBox(string text = "" , bool ABOVE = true , bool UNDER = true , bool LEFT = true , bool RIGHT = true, int Height = 5, int Width = 15, string color = WHITE);

int SubMenu(string options[], int length);
int MainMenuOptionInBoard(string options[], int length);
// Menu chính
void MainScreen();

// Hàm này sẽ đọc phím bấm của người dùng và trả về một giá trị NavKey tương ứng
NavKey GetNavKey();

#endif