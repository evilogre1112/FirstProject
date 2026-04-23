#include <iostream>
#include <string>
#include <sstream>
#include <functional>
#include <cstring>
#include "Logic.h"
// Chỉ thêm windows.h nếu đang chạy trên Windows
#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <conio.h>
#else
    #include <sys/ioctl.h>
    #include <unistd.h>
    #include <termios.h>
    int _getch() {
        struct termios oldt, newt;
        int ch;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return ch;
    }
#endif

#include "UI.h"

using namespace std ;

string subOptions1[4] = {
    "Quản Lí Máy Bay",
    "Quản Lí Chuyến Bay",
    "Quản Lí Hành Khách",
    "Quay lại Main Menu"
};

string subOptions2[4] = {
    "Đặt Vé",
    "Huỷ Vé",
    "In Danh Sách Vé Trống",
    "Quay Lại Main Menu"
};

string subOptions3[4] = {
    "Tra Cứu Chuyến Bay",
    "Thống Kê Lượt Bay",
    "In Danh Sách Hành Khách",
    "Quay Lại Main Menu"
};

string subOptions4[5] = {
    "Thêm",
    "Sữa",
    "Xoá",
    "In Danh Sách",
    "Quay Lại"
};

string* Menus[] = {
        subOptions1,
        subOptions2,
        subOptions3,
        subOptions4,
};

int MenusSize[] = {
    sizeof(subOptions1)/sizeof(string),
    sizeof(subOptions2)/sizeof(string),
    sizeof(subOptions3)/sizeof(string),
    sizeof(subOptions4)/sizeof(string)
};

int TotalWidth = 0;

NavKey GetNavKey(int &ch) {
#ifdef _WIN32
    ch = _getch();
    if (ch == 224 || ch == 0) {
        ch = _getch();
        if (ch == 72) return NAV_UP;
        if (ch == 80) return NAV_DOWN;
        if (ch == 75) return NAV_LEFT;
        if (ch == 77) return NAV_RIGHT;
        return NAV_UNKNOWN;
    }
    if (ch == 13) return NAV_ENTER;
    if (ch == 27) return NAV_ESC;
    if (ch == 8) return NAV_BACK;
#else
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    newt.c_cc[VMIN] = 0;
    newt.c_cc[VTIME] = 1;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    char seq[3];
    int bytesRead = read(STDIN_FILENO, &seq[0], 1);
    NavKey result = NAV_UNKNOWN;
    if (bytesRead == 1) {
        ch = (unsigned char)seq[0];
        if (seq[0] == 27) {
            if (read(STDIN_FILENO, &seq[1], 1) == 1 && read(STDIN_FILENO, &seq[2], 1) == 1) {
                if (seq[1] == 91) {
                    if (seq[2] == 65) result = NAV_UP;
                    else if (seq[2] == 66) result = NAV_DOWN;
                    else if (seq[2] == 67) result = NAV_RIGHT;
                    else if (seq[2] == 68) result = NAV_LEFT;
                }
            } else {
                result = NAV_ESC;
            }
        }
        else if (seq[0] == 10 || seq[0] == 13) result = NAV_ENTER;
        else if (seq[0] == 127 || seq[0] == 8) result = NAV_BACK;
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return result;
#endif
    return NAV_UNKNOWN;
}

NavKey GetNavKey() {
    int ch;
    NavKey key = GetNavKey(ch);
    if (key == NAV_UNKNOWN) {
        if (ch == 'w' || ch == 'W') return NAV_UP;
        if (ch == 's' || ch == 'S') return NAV_DOWN;
        if (ch == 'a' || ch == 'A') return NAV_LEFT;
        if (ch == 'd' || ch == 'D') return NAV_RIGHT;
    }
    return key;
}

int GetTerminalWidth(){
      #ifdef _WIN32
        // Code đo chiều rộng dành cho Windows
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        return csbi.srWindow.Right - csbi.srWindow.Left + 1;
    #else
        // Code đo chiều rộng dành cho Mac / Linux
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        return w.ws_col;
    #endif
}

void ClearScreen() {
#ifdef _WIN32
    system("cls");
#else
    // Các hệ điều hành dựa trên UNIX (Linux, macOS)
    system("clear");
#endif
}
void Gotoxy(int x, int y) {
#ifdef _WIN32
        HANDLE hConsoleOutput;
        COORD Cursor_an_Pos = { (SHORT)x, (SHORT)y };
        hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleCursorPosition(hConsoleOutput, Cursor_an_Pos);
#else
        printf("\033[%d;%dH", y + 1, x + 1); 
#endif
}

#ifndef _WIN32
void getCursorPosMac(int &x, int &y) {
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    write(STDOUT_FILENO, "\033[6n", 4);
    char buf[32];
    int i = 0;
    while (i < sizeof(buf) - 1) {
        if (read(STDIN_FILENO, &buf[i], 1) != 1) break;
        if (buf[i] == 'R') break;
        i++;
    }
    buf[i] = '\0';
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    int rows, cols;
    if (sscanf(buf, "\033[%d;%d", &rows, &cols) == 2) {
        x = cols - 1; y = rows - 1;
    }
}
#endif

// Lấy tọa độ X hiện tại của con trỏ
int whereX() {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.dwCursorPosition.X;
#else
    int x, y;
    getCursorPosMac(x, y);
    return x;
#endif
}

// Lấy tọa độ Y hiện tại của con trỏ
int whereY() {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.dwCursorPosition.Y;
#else
    int x, y;
    getCursorPosMac(x, y);
    return y;
#endif
}

// Hàm lấy Ngày (DD) - Cắt 2 ký tự từ vị trí 0
int GetDayFromStr(const string &datetimeStr) {
    if (datetimeStr.length() < 16) return -1; 
    try { return stoi(datetimeStr.substr(0, 2)); } 
    catch (...) { return -1; }
}


int GetMonthFromStr(const string &datetimeStr) {
    if (datetimeStr.length() < 16) return -1;
    try { return stoi(datetimeStr.substr(3, 2)); } 
    catch (...) { return -1; }
}


int GetYearFromStr(const string &datetimeStr) {
    if (datetimeStr.length() < 16) return -1;
    try { return stoi(datetimeStr.substr(6, 4)); } 
    catch (...) { return -1; }
}

int GetHourFromStr(const string &datetimeStr) {
    if (datetimeStr.length() < 16) return -1;
    try { return stoi(datetimeStr.substr(11, 2)); } 
    catch (...) { return -1; }
}

int GetMinuteFromStr(const string &datetimeStr) {
    if (datetimeStr.length() < 16) return -1;
    try { return stoi(datetimeStr.substr(14, 2)); } 
    catch (...) { return -1; }
}

string ToStringDate(DateTime dt) {
    stringstream ss;
    
    // Sử dụng setw(2) và setfill('0') để đảm bảo luôn có 2 chữ số (vd: 09 thay vì 9)
    ss << setfill('0') << setw(2) << dt.get_dd() << "/"
       << setfill('0') << setw(2) << dt.get_mt() << "/"
       << setw(4) << dt.get_yy() << " "
       << setfill('0') << setw(2) << dt.get_hh() << ":"
       << setfill('0') << setw(2) << dt.get_mm();
       
    return ss.str();
}

string TranFormSatus(int st){
     string SelectedColors[] = {RED, GREEN, YELLOW, CYAN};
     return SelectedColors[st];
}

string TranFormGender(int st){
     string SelectedColors[] = {"Nam","Nữ"};
     return SelectedColors[st];
}


int visualLength(const string& s) {
    int length = 0;
    bool inCode = false;
    for (size_t i = 0; i < s.length(); i++) {
        if (s[i] == '\033') {
            inCode = true;
            continue; 
        }

        if (inCode) {
            if (isalpha((unsigned char)s[i])) {
                inCode = false;
            }
            continue;
        }
        if ((unsigned char)(s[i] & 0xc0) != 0x80) {
            length++;
        }
    }
    return length;
}

int InputStatus(int &result, int x, int y) {
    string Label[] = {"Huỷ Chuyến", "Còn Vé", "Hết Vé", "Hoàn Tất"};
    string Color[4];
    string SelectedColors[] = {RED, GREEN, YELLOW, CYAN};
    string gap(5, ' '); // Khoảng cách giữa 2 lựa chọn

    while (true) {
        // LUÔN ĐƯA CON TRỎ VỀ VỊ TRÍ CŨ TRƯỚC KHI IN
        Gotoxy(x, y);
        for(int i = 0 ; i < 4 ; i++) {
            if (result == i) {
                Color[i] = SelectedColors[i];
            } else {
                Color[i] = GRAY;
            }
        }

        for(int i = 0 ; i < 4 ;i++){
            cout << Color[i] << SQ << Label[i] << RESET;
            cout << gap;
        }

        int ch;
        NavKey key = GetNavKey(ch); 

        if (key == NAV_UP) return NAV_UP;
        if (key == NAV_DOWN) return NAV_DOWN;
        if (key == NAV_ENTER) return NAV_ENTER;
        if (key == NAV_ESC) return NAV_ESC;

        if (key == NAV_RIGHT) {
            result = (result + 1 > 3) ? 0 : result + 1; 
        } else if (key == NAV_LEFT) {
            result = (result - 1 < 0) ? 3 : result - 1; 
        }
    }
}


// Trả về: 0 (ESC), 1 (ENTER), 2 (UP), 3 (DOWN)
int InputString(string &result, int x, int y, int maxLength, char placeholder , bool onlyNumbers ) {
    // Không reset 'result' về rỗng để giữ lại chữ cũ khi người dùng quay lại ô này
    Gotoxy(x, y);
    cout << string(maxLength, placeholder); 
    Gotoxy(x, y);
    cout << result; // In đè chữ cũ lên trên hàng gạch

    while (true) {
        int ch ;
        NavKey key = GetNavKey(ch); 

        if (key == NAV_UP) return NAV_UP;
        if (key == NAV_DOWN) return NAV_DOWN;
        if (key == NAV_ENTER) return NAV_ENTER;
        if (key == NAV_ESC) return NAV_ESC;

        if (key == NAV_BACK) { // Phím BACKSPACE
            if (!result.empty()) {
                result.pop_back();
                cout << "\b" << placeholder << "\b";
            }
        } 
        else if (result.length() < maxLength) {
            // CHẶN CHỮ NGAY TỪ LÚC GÕ
            if (onlyNumbers && !isdigit(ch)) continue; 
            if(ch != ' ' && ch != '-' && !isalpha(ch) && !isdigit(ch)) continue ;
            // Các ràng buộc khác
            if (!onlyNumbers && !isprint(ch)) continue;
            if (result.empty() && ch == ' ') continue;

            result += (char)ch;
            cout << (char)ch;
        }
    }
}

int InputGender(int &result, int x, int y) {
    string boyLabel = " Nam ";
    string girlLabel = " Nữ ";
    string gap(5, ' '); // Khoảng cách giữa 2 lựa chọn

    while (true) {
        // LUÔN ĐƯA CON TRỎ VỀ VỊ TRÍ CŨ TRƯỚC KHI IN
        Gotoxy(x, y);

        string colorBoy = (result == 0) ? GREEN : GRAY;
        string colorGirl = (result == 1) ? GREEN : GRAY;

        // In ra giao diện: [■ Nam]     [□ Nữ]
        cout << colorBoy << SQ << boyLabel << RESET;
        cout << gap;
        cout << colorGirl << SQ << girlLabel << RESET;

        int ch;
        NavKey key = GetNavKey(ch); 

        if (key == NAV_UP) return NAV_UP;
        if (key == NAV_DOWN) return NAV_DOWN;
        if (key == NAV_ENTER) return NAV_ENTER;
        if (key == NAV_ESC) return NAV_ESC;

        if (key == NAV_RIGHT || key == NAV_LEFT) {
            result = 1 - result; 
        }
    }
}

int InputDateTime(string &result, int x, int y, char placeholder = '_') {
    int maxLength = 16; 
    Gotoxy(x, y); cout << string(maxLength, placeholder);
    Gotoxy(x, y); cout << result;

    while (true) {
        int ch ;
        NavKey key = GetNavKey(ch); 

        if (key == NAV_UP) return NAV_UP;
        if (key == NAV_DOWN) return NAV_DOWN;
        if (key == NAV_ENTER) return NAV_ENTER;
        if (key == NAV_ESC) return NAV_ESC;

        if (key == NAV_BACK) { // BACKSPACE
            if (!result.empty()) {
                char last = result.back();
                if (last == '/' || last == ' ' || last == ':') {
                    result.pop_back();
                    cout << "\b" << placeholder << "\b";
                }
                if (!result.empty()) {
                    result.pop_back();
                    cout << "\b" << placeholder << "\b";
                }
            }
        } 
        else if (result.length() < maxLength) {
            if (!isdigit(ch)) continue; // Chỉ cho phép nhập số

            int len = result.length();
            
            // 1. Kiểm tra Ngày (01 - 31)
            if (len == 0 && (ch < '0' || ch > '3')) continue; 
            if (len == 1) {
                if (result[0] == '3' && (ch < '0' || ch > '1')) continue; 
                if (result[0] == '0' && ch == '0') continue; // Chặn ngày 00
            }

            // 2. Kiểm tra Tháng (01 - 12) và tương quan với Ngày
            if (len == 3 && (ch < '0' || ch > '1')) continue; 
            if (len == 4) {
                if (result[3] == '1' && (ch < '0' || ch > '2')) continue; 
                if (result[3] == '0' && ch == '0') continue; // Chặn tháng 00

                int d = (result[0] - '0') * 10 + (result[1] - '0');
                int m = (result[3] - '0') * 10 + (ch - '0');
                if (d == 31 && (m == 2 || m == 4 || m == 6 || m == 9 || m == 11)) continue;
                
                // Chặn gõ tháng 2 nếu ngày đang là 30
                if (d >= 30 && m == 2) continue; 
            }

            // 3. Kiểm tra Năm (Giới hạn chặt: 1900 - 2100)
            if (len == 6) {
                if (ch != '1' && ch != '2') continue; // Chỉ cho phép bắt đầu bằng 1 hoặc 2
            }
            if (len == 7) {
                if (result[6] == '1' && ch != '9') continue; // 1 -> 19
                if (result[6] == '2' && ch != '0' && ch != '1') continue; // 2 -> 20 hoặc 21
            }
            if (len == 8) {
                if (result[6] == '2' && result[7] == '1' && ch != '0') continue; 
            }
            if (len == 9) {
                if (result[6] == '2' && result[7] == '1' && result[8] == '0' && ch != '0') continue;

                // Kiểm tra năm nhuận nếu người dùng đã gõ ngày 29/02 trước đó
                int d = (result[0] - '0') * 10 + (result[1] - '0');
                int m = (result[3] - '0') * 10 + (result[4] - '0');
                if (d == 29 && m == 2) {
                    int y = (result[6] - '0') * 1000 + (result[7] - '0') * 100 + (result[8] - '0') * 10 + (ch - '0');
                    bool isLeap = (y % 400 == 0) || (y % 4 == 0 && y % 100 != 0);
                    if (!isLeap) continue; // Chặn phím nếu năm tạo thành không phải năm nhuận
                }
            }

            // 4. Kiểm tra Giờ (00 - 23)
            if (len == 11 && (ch < '0' || ch > '2')) continue; 
            if (len == 12) {
                if (result[11] == '2' && (ch < '0' || ch > '3')) continue; 
            }

            // 5. Kiểm tra Phút (00 - 59)
            if (len == 14 && (ch < '0' || ch > '5')) continue; 

            result += (char)ch;
            cout << (char)ch;

            // Tự động chèn ký tự phân cách
            len = result.length(); 
            if ((len == 2 || len == 5) && len < maxLength) {
                result += '/'; cout << '/';
            } else if (len == 10 && len < maxLength) {
                result += ' '; cout << ' ';
            } else if (len == 13 && len < maxLength) {
                result += ':'; cout << ':';
            }
        }
    }
}

// Hàm này trả về số byte và độ dài hiển thị của một dòng con trong chuỗi text bắt đầu từ startByte, với độ rộng tối đa maxWidth
LineInfo getLineInfo(const string& text, int startByte, int maxWidth) {
    int bytes = 0;
    int visual = 0;
    int lastSpaceByte = -1;
    int lastSpaceVisual = -1;
    int textLen = text.length();
    bool inANSI = false;

    int i = startByte;
    while (i < textLen) {
        // Kiểm tra bắt đầu mã ANSI
        if (text[i] == '\033') {
            inANSI = true;
        }

        if (inANSI) {
            bytes++;
            // Mã ANSI kết thúc bằng chữ cái (thường là 'm')
            if (isalpha((unsigned char)text[i])) inANSI = false;
            i++;
            continue; // Không tăng visual khi đang ở trong mã ANSI
        }

        unsigned char c = (unsigned char)text[i];
        int step = 1;
        if (c >= 192) {
            if (c >= 240) step = 4; //Emoji
            else if (c >= 224) step = 3; // Tiếng Việt , Trung Hàn
            else step = 2; //các chữ có dấu cơ bản
        }

        // Nếu thêm 1 ký tự visual mà vượt quá maxWidth
        if (visual + 1 > maxWidth) {
            if (lastSpaceByte != -1) {
                return { lastSpaceByte - startByte, lastSpaceVisual };
            }
            return { bytes, visual };
        }

        if (c == ' ') {
            lastSpaceByte = i + step; // Bao gồm cả khoảng trắng để substr cắt đúng
            lastSpaceVisual = visual + 1;
        }

        bytes += step;
        visual += 1;
        i += step;
    }
    return { bytes, visual };
}
//Ham này đếm số dòng cần thiết để hiển thị toàn bộ text trong một hộp có độ rộng innerWidth, dựa trên việc gọi getLineInfo liên tục cho đến hết text
// innerWidth là độ rộng thực tế bên trong hộp, đã trừ đi 2 ký tự viền
int countLinesNeeded(const string& text, int innerWidth) {
    if (innerWidth <= 0) return 0;
    int count = 0;
    int currentByte = 0;
    int textLen = text.length();

    while (currentByte < textLen) {
        LineInfo info = getLineInfo(text, currentByte, innerWidth);
        if (info.byteCount == 0) break;
        currentByte += info.byteCount;
        while (currentByte < textLen && text[currentByte] == ' ') currentByte++;
        
        count++;
    }
    return count;
}

void ClearRegion(int x, int y, int width, int height) {
    string blank(width, ' '); // Tạo sẵn chuỗi khoảng trắng
    for (int i = 0; i < height; i++) {
        Gotoxy(x, y + i);
        cout << blank; 
    }
    Gotoxy(x,y);
}

void PrintBox(string text, bool ABOVE , bool UNDER ){
    //Tính khoảng trắng lề trái để đẩy toàn bộ khung ra giữa màn hình
    int marginLeft = (TotalWidth > boxWidth) ? (TotalWidth - boxWidth)/2 : 0;
    string margin = string(marginLeft,' ');   // Tạo chuỗi khoảng trắng đẩy lề
    string line = "";               // Tạo chuỗi đường kẻ ngang cho viền trên và dưới
    for(int i=0;i<boxWidth-2;i++) line += "═";
    // Chèn khoảng trắng lề trái vào trước viền trên và dưới
    string above , under ;
    ABOVE ? above = margin + "╔" + line + "╗" : above = margin + "║" + string(boxWidth-2,' ') + "║";
    UNDER ? under = margin + "╚" + line + "╝" : under = margin + "║" + string(boxWidth-2,' ') + "║";
    // Tính toán để căn giữa phần chữ bên trong khung
    int textLen = visualLength(text);
    int padleft = (boxWidth - 2 - textLen)/2 ;
    int padright = boxWidth - 2 - textLen - padleft ;
    cout << above << endl ; 
    cout << margin << "║" << string(padleft,' ') << text << string(padright,' ') << "║" << endl ;
    cout << under << endl ;
}
int GetKey(int chosen,int count){
    while(true){
        NavKey key = GetNavKey();
        switch (key) {
            case NAV_UP:
                chosen = (chosen - 1 >= 0) ? chosen - 1 : count - 1;
                return chosen;
            case NAV_DOWN:
                chosen = (chosen + 1 < count) ? chosen + 1 : 0;
                return chosen;
            case NAV_ENTER:
                return 999; 
            case NAV_ESC:
                return 1000; // Thoát không chọn
        }
    }
}

void SmallBox(string text, int Width, int Height, string color) {
    SmallBox(text, true, true, true, true, Width, Height, color); // Gọi hàm gốc với các giá trị mặc định
}

// Hàm này dùng để vẽ một hộp nhỏ với nhiều tùy chọn hơn như có/không cạnh trên/dưới/trái/phải, chiều cao, chiều rộng và màu sắc của text
void SmallBox(string text, bool ABOVE, bool UNDER, bool LEFT, bool RIGHT,int Width, int Height, string color ) {
    int startX = whereX();
    int startY = whereY();
    int innerWidth = Width - 2;
    if (innerWidth <= 0) return;

    int totalLinesNeeded = countLinesNeeded(text, innerWidth);
    int actualLinesToPrint = (totalLinesNeeded > Height) ? Height : totalLinesNeeded;
    int startTextRow = (Height - actualLinesToPrint) / 2;

    string lineBorder = "";
    for (int i = 0; i < innerWidth; i++) lineBorder += HZ;

    // --- 1. Vẽ cạnh trên ---
    if (ABOVE) {
        Gotoxy(startX, startY);
        // Nếu không có LEFT/RIGHT thì dùng HZ để nối dài đường kẻ
        cout << RESET << (LEFT ? TLC : HZ) << lineBorder << (RIGHT ? TRC : HZ);
    }

    // --- 2. Vẽ phần thân ---
    int bodyStartRow = startY + (ABOVE ? 1 : 0);

    for (int i = 0; i < Height; i++) {
        Gotoxy(startX, bodyStartRow + i);
        
        cout << RESET; 
        if (LEFT) cout << VT; else cout << " "; // Nếu không có cạnh trái, để trống để không đè hộp bên cạnh

        if (i >= startTextRow && i < startTextRow + actualLinesToPrint) {
            int currentBytePos = 0; 
            int tempPos = 0;
            for(int j = 0; j < i - startTextRow; j++) {
                tempPos += getLineInfo(text, tempPos, innerWidth).byteCount;
                while (tempPos < text.length() && text[tempPos] == ' ') tempPos++;
            }
            
            if (tempPos < text.length()) {
                LineInfo info = getLineInfo(text, tempPos, innerWidth);
                string currentLine = text.substr(tempPos, info.byteCount);
                int padLeft = (innerWidth - info.visualWidth) / 2;
                int padRight = innerWidth - info.visualWidth - padLeft;
                cout << string(padLeft, ' ') << color << currentLine << RESET << string(padRight, ' ');
            } else cout << string(innerWidth, ' ');
            
        } else {
            cout << string(innerWidth, ' ');
        }

        cout << RESET;
        if (RIGHT) cout << VT; else cout << " ";
    }

    // --- 3. Vẽ cạnh dưới ---
    int footerRow = bodyStartRow + Height;
    if (UNDER) {
        Gotoxy(startX, footerRow);
        cout << RESET << (LEFT ? BLC : HZ) << lineBorder << (RIGHT ? BRC : HZ);
    }

    // Đưa con trỏ về dòng cuối cùng đã vẽ để whereY() của hộp sau trả về đúng vị trí đó
    Gotoxy(startX, footerRow); 
}
// Hiển thị giao diện bên tay phải
void ViewOptions(int mainMenuIdx) {
    Gotoxy(71, 6); 
    int startX = whereX();
    int startY = whereY();
    string* subOptions = Menus[mainMenuIdx];
    int count = MenusSize[mainMenuIdx];
    // 1. Xóa vùng cũ 
    ClearRegion(startX, startY, 30, 20); 

    int space = 2;
    // 3. Vẽ nội dung ra
    for(int i = 0 ; i < count ; i++) {
        // con trỏ sẽ nhảy xuống dưới, nên vẫn cần Gotoxy để căn dòng
        SmallBox(subOptions[i], true, true, true, true, 30, 1, WHITE);
        Gotoxy(whereX(),whereY()+space);
    }
}

void ViewQLMB(){
    int startX = 1 ;
    int startY = 22;
    int lenght = MenusSize[3];
    Gotoxy(startX,startY);
    int space = 1 ;
    for(int i = 0 ; i < lenght ;i++){
        SmallBox(subOptions4[i],18,5);
        startX = whereX();
        Gotoxy(startX + 19 + space, startY);
    }  
}

void CustomerAddMB() {
    int width = 80;
    int startX = (GetTerminalWidth() - width) / 2;
    int formY = 6;

    if (dsMB.slMB >= slMB_max) {
        ClearScreen();
        Gotoxy(startX, 10);
        cout << RED << "Lỗi: Danh sách máy bay đã đầy! Không thể thêm." << RESET;
        _getch();
        return; 
    }

    ClearScreen();
    Gotoxy(startX, 2);
    SmallBox("THÊM MÁY BAY MỚI", (int)width, (int)3, (string)YELLOW);
    
    Gotoxy(startX, formY);
    SmallBox("", true, true, true, true, width, 14, WHITE);

    string labels[] = {"Số hiệu máy bay:", "Loại máy bay:", "Số chỗ ngồi:"};
    int labelX = startX + 5;
    int inputX = startX + 25;

    for (int i = 0; i < 3; i++) {
        Gotoxy(labelX, formY + 2 + (i * 4));
        cout << labels[i];
        Gotoxy(inputX, formY + 2 + (i * 4));
        cout << string(50, '_'); 
    }

    Gotoxy(startX, formY + 15);
    SmallBox("ENTER/DOWN: Tiếp tục - UP: Lên trên - ESC: Hủy", (int)width, (int)3, (string)WHITE);

    // --- LOGIC NHẬP LIỆU VÀ ĐIỀU HƯỚNG ---
    string inputs[3] = {"", "", ""}; 
    int currentField = 0; // 0: Số hiệu, 1: Loại, 2: Số chỗ

    while (true) {
        // Xóa thông báo lỗi cũ mỗi khi chuyển dòng
        Gotoxy(startX + 5, formY + 12); cout << string(width - 10, ' '); 

        int action;
        if (currentField == 0) {
            action = InputString(inputs[0], inputX, formY + 2, soHieuMB_max, '_', false);
        } else if (currentField == 1) {
            action = InputString(inputs[1], inputX, formY + 6, loaiMB_max, '_', false);
        } else if (currentField == 2) {
            action = InputString(inputs[2], inputX, formY + 10, 3, '_', true); 
        }

        if (action == NAV_ESC){
            ClearScreen();
            return; 
        } 
        else if (action == NAV_UP) { 
            currentField = (currentField == 0) ? 2 : currentField - 1;
        } 
        else if (action == NAV_DOWN) { 
            currentField = (currentField == 2) ? 0 : currentField + 1;
        }else if (action == NAV_ENTER) { 
            // --- KIỂM TRA ĐÃ ĐIỀN ĐỦ 3 MỤC CHƯA ---
            if (!inputs[0].empty() && !inputs[1].empty() && !inputs[2].empty()) {
                
                // --- Đã điền đủ: Tiến hành kiểm tra logic và hỏi lưu ---
                int soCho = stoi(inputs[2]);
                if (soCho <= 0 || soCho > socho_max) {
                    Gotoxy(startX + 5, formY + 12);
                    cout << RED << "Lỗi: Số chỗ phải từ 1 đến " << socho_max << "! Bấm phím bất kỳ..." << RESET;
                    _getch(); 
                    currentField = 2; // Ép con trỏ nhảy về ô Số chỗ để sửa
                    continue;
                }

                Gotoxy(startX, formY + 15);
                SmallBox("Xác nhận thêm máy bay này? (ENTER: Đồng ý - ESC: Quay lại sửa)", (int)width, (int)3, (string)YELLOW);
                
                bool isConfirm = false;
                while (true) {
                    NavKey confirmKey = GetNavKey();
                    if (confirmKey == NAV_ENTER) {
                        isConfirm = true;
                        break;
                    } else if (confirmKey == NAV_ESC) {
                        isConfirm = false;
                        break;
                    }
                }

                if (!isConfirm) {
                    Gotoxy(startX, formY + 15);
                    SmallBox("ENTER/DOWN: Tiếp tục - UP: Lên trên - ESC: Hủy", (int)width, (int)3, (string)WHITE);
                    continue; 
                }

                // --- TIẾN HÀNH LƯU ---
                Gotoxy(startX, formY + 15);
                SmallBox("ĐANG LƯU DỮ LIỆU...", (int)width, (int)3, (string)GREEN);

                MB* NewMB = new MB; 
                strcpy(NewMB->soHieuMB, inputs[0].c_str());
                strcpy(NewMB->loaiMB, inputs[1].c_str());
                NewMB->socho = soCho;

                if(Add_MB(dsMB, NewMB)) {
                    Gotoxy(startX + 20, formY + 18);
                    cout << YELLOW << "Đã lưu thành công! Nhấn phím bất kỳ để thoát." << RESET;
                    _getch();
                    ClearScreen();
                    return; 
                } else {
                    Gotoxy(startX + 15, formY + 16);
                    cout << RED << "Lỗi: Số hiệu [" << inputs[0] << "] đã tồn tại! Bấm phím bất kỳ..." << RESET;
                    delete NewMB;
                    _getch(); 
                    
                    Gotoxy(startX, formY + 15);
                    SmallBox("ENTER/DOWN: Tiếp tục - UP: Lên trên - ESC: Hủy", (int)width, (int)3, (string)WHITE);
                    currentField = 0; // Trùng mã thì đưa về ô đầu tiên để sửa
                }

            } else {
                // --- CHƯA ĐIỀN ĐỦ MỤC ---
                if (currentField < 2) {
                    currentField++; // Nếu đang ở trên thì tự xuống dòng
                } else {
                    // Nếu đang ở dòng cuối cùng (Số chỗ) mà vẫn có dòng trống ở trên
                    Gotoxy(startX + 5, formY + 12);
                    cout << RED << "Lỗi: Không được để trống bất kỳ trường nào! Bấm phím bất kỳ..." << RESET;
                    _getch();
                    
                    for (int i = 0; i < 3; i++) {
                        if (inputs[i].empty()) {
                            currentField = i;
                            break;
                        }
                    }
                }
            }
        }
    }
}

void CustomerAddCB() {
    int width = 80;
    int startX = (GetTerminalWidth() - width) / 2;
    int formY = 6;

    ClearScreen();
    Gotoxy(startX, 1);
    SmallBox("THÊM CHUYẾN BAY MỚI", (int)width, (int)3, (string)YELLOW);
    
    Gotoxy(startX, formY);
    SmallBox("", true, true, true, true, width, 13, WHITE); // Giảm chiều cao box xuống 15

    string labels[] = {"Mã Chuyến bay:", "Ngày giờ khởi hành:", "Sân bay đến:", "Số hiệu máy bay:","Trạng Thái:"};
    int labelX = startX + 5;
    int inputX = startX + 25;
    string Label[] = {"Huỷ Chuyến", "Còn Vé", "Hết Vé", "Hoàn Tất"};
    string gap(5, ' '); // Khoảng cách giữa 2 lựa chọn
    // Đổi khoảng cách từ (i * 4) thành (i * 2)
    for (int i = 0; i < 5; i++) {
        Gotoxy(labelX, formY + 2 + (i * 2));
        cout << labels[i];
        Gotoxy(inputX, formY + 2 + (i * 2));
        if(i != 4){
            cout << string(50, '_'); 
        }else{
            cout << string(RED) << SQ << Label[0] << string(RESET) ;
            cout << gap ;
            for(int i = 1 ; i < 4;i++){
                cout << string(GRAY) << SQ << Label[i] << string(RESET); 
                cout << gap ;
            }
         
        }
    }
    
    string text = "ENTER/DOWN: Tiếp tục - UP: Lên trên - ESC: Hủy";
    Gotoxy(startX, formY + 15); // Đưa dòng hướng dẫn lên sát mép dưới form
    SmallBox(text, (int)width, (int)3, (string)WHITE);
    // --- LOGIC NHẬP LIỆU VÀ ĐIỀU HƯỚNG ---
    string inputs[5] = {"", "", "","",""}; 
    int currentField = 0; // 0: Mã_CB, 1: Ngày giờ khởi hành, 2: Sbd , 3: shMB, 4: status 
    int stat = 0 ;
    while (true) {
        Gotoxy(startX + 5, formY + 12); cout << string(width - 10, ' ');

        int action;
        if (currentField == 0) {
            action = InputString(inputs[0], inputX, formY + 2, maCB_max, '_', false);
        } else if (currentField == 1) {
            action = InputDateTime(inputs[1], inputX, formY + 4, '_');
        } else if (currentField == 2) {
            // Tham số TRUE cuối cùng sẽ chặn toàn bộ các ký tự không phải là số
            action = InputString(inputs[2], inputX, formY + 6, sbDich_max, '_', false); 
        }else if (currentField == 3){
            action = InputString(inputs[3], inputX, formY + 8, soHieuMB_max , '_', false);
        }else if(currentField == 4){
            action = InputStatus(stat, inputX, formY + 10);
        }

        // Xử lý sự kiện trả về từ InputString
        if (action == NAV_ESC){
            ClearScreen();
            return; 
        } 
        
        else if (action == NAV_UP) { 
            currentField = (currentField == 0) ? 4 : currentField - 1;
        } 
        else if (action == NAV_DOWN) { 
            currentField = (currentField == 4) ? 0 : currentField + 1;
        }else if (action == NAV_ENTER) { // Nhấn ENTER
            if (!inputs[0].empty() && !inputs[1].empty() && !inputs[2].empty() && !inputs[3].empty()) {
                if (GetDayFromStr(inputs[1]) == -1) {
                    Gotoxy(startX + 5, formY + 12); 
                    cout << RED << "Lỗi: Ngày giờ khởi hành chưa điền đủ định dạng! Bấm phím bất kỳ..." << RESET;
                    _getch(); 
                    currentField = 1; 
                    continue;
                }
                Gotoxy(startX, formY + 15);
                SmallBox("Xác nhận thêm chuyến bay này? (ENTER: Đồng ý - ESC: Quay lại sửa)", (int)width, (int)3, (string)YELLOW);
                
                bool isConfirm = false;
                while (true) {
                    NavKey confirmKey = GetNavKey();
                    if (confirmKey == NAV_ENTER) {
                        isConfirm = true;
                        break;
                    } else if (confirmKey == NAV_ESC) {
                        isConfirm = false;
                        break;
                    }
                }

                if (!isConfirm) {
                    Gotoxy(startX, formY + 15);
                    SmallBox("ENTER/DOWN: Tiếp tục - UP: Lên trên - ESC: Hủy", (int)width, (int)3, (string)WHITE);
                    continue; 
                }

                // --- TIẾN HÀNH LƯU ---
                Gotoxy(startX, formY + 15);
                SmallBox("ĐANG LƯU DỮ LIỆU...", (int)width, (int)3, (string)GREEN);

                CB* NewCB = new CB; 
                NewCB->set_maCB(const_cast<char*>(inputs[0].c_str()));
                NewCB->ngayKH.set_dd(GetDayFromStr(inputs[1]));
                NewCB->ngayKH.set_mm(GetMonthFromStr(inputs[1]));
                NewCB->ngayKH.set_yy(GetYearFromStr(inputs[1]));
                NewCB->ngayKH.set_hh(GetHourFromStr(inputs[1]));
                NewCB->ngayKH.set_mt(GetMinuteFromStr(inputs[1]));
                
                NewCB->set_sbDich(const_cast<char*>(inputs[2].c_str()));
                NewCB->set_soHieuMB(const_cast<char*>(inputs[3].c_str()));
                NewCB->trangThai = stat;

                int pos = Find_MB(dsMB, NewCB->soHieuMB) ; 
                if (pos != -1) {
                    NewCB->set_socho(dsMB.list[pos]->socho);
                }

                if(Add_CB(dsCB,dsMB, NewCB)) {
                    Gotoxy(startX + 15, formY + 18);
                    cout << YELLOW << "Đã lưu thành công! Nhấn phím bất kỳ để thoát." << RESET;
                    _getch();
                    ClearScreen();
                    return; 
                } else {
                    Gotoxy(startX + 5, formY + 17); cout << string(width - 10, ' '); 
                    Gotoxy(startX + 10, formY + 17);
                    
                    if(Find_MB(dsMB, NewCB->soHieuMB) == -1) {
                        cout << RED << "Lỗi: Số hiệu [" << inputs[3] << "] không tồn tại trong bãi!" << RESET;
                    }
                    else if(Find_CB(dsCB, NewCB->maCB) != nullptr) {
                        cout << RED << "Lỗi: Mã chuyến bay [" << inputs[0] << "] đã tồn tại!" << RESET;
                    }
                    else {
                        cout << RED << "Lỗi: Máy bay đang bận bay chuyến khác!" << RESET;
                    }
                    
                    delete NewCB; 
                    _getch(); 
                    
                    Gotoxy(startX, formY + 15);
                    SmallBox("ENTER/DOWN: Tiếp tục - UP: Lên trên - ESC: Hủy", (int)width, (int)3, (string)WHITE);
                    currentField = 0; 
                }
            } else {
                if (currentField < 4) {
                    currentField++; 
                } else {
                    Gotoxy(startX + 5, formY + 12);
                    cout << RED << "Lỗi: Không được để trống bất kỳ trường nào! Bấm phím bất kỳ..." << RESET;
                    _getch();
                    
                    for (int i = 0; i < 5; i++) {
                        if (inputs[i].empty()) {
                            currentField = i;
                            break;
                        }
                    }
                }
            }
        }
    }
}

void CustomerAddHK() {
    int width = 80;
    int startX = (GetTerminalWidth() - width) / 2;
    int formY = 6;

    if (dsHK.slHK >= slHK_max) {
        ClearScreen();
        Gotoxy(startX, 10);
        cout << RED << "Lỗi: Danh sách hành khách đã đầy" << RESET;
        _getch();
        return; 
    }

    ClearScreen();
    Gotoxy(startX, 2);
    SmallBox("THÊM HÀNH KHÁCH MỚI", (int)width, (int)3, (string)YELLOW);
    
    Gotoxy(startX, formY);
    SmallBox("", true, true, true, true, width, 14, WHITE);

    string labels[] = {"CMND", "Họ", "Tên", "Phái"};
    int labelX = startX + 5;
    int inputX = startX + 25;
    string gap(5, ' '); // Khoảng cách giữa 2 lựa chọn
    for (int i = 0; i < 4; i++) {
        Gotoxy(labelX, formY + 2 + (i * 4));
        cout << labels[i];
        Gotoxy(inputX, formY + 2 + (i * 4));
        if(i != 3){
            cout << string(50, '_'); 
        }else{
            cout << string(GREEN) << SQ << " Nam " << string(RESET) ;
            cout << gap ;
            cout << string(GRAY) << SQ << " Nữ " << string(RESET); 

        }
       
    }

    Gotoxy(startX, formY + 17);
    SmallBox("ENTER/DOWN: Tiếp tục - UP: Lên trên - ESC: Hủy", (int)width, (int)3, (string)WHITE);

    // --- LOGIC NHẬP LIỆU VÀ ĐIỀU HƯỚNG ---
    string inputs[4] = {"", "", ""}; 
    int currentField = 0; // 0: Số hiệu, 1: Loại, 2: Số chỗ

    while (true) {
        // Xóa thông báo lỗi cũ mỗi khi chuyển dòng
        Gotoxy(startX + 5, formY + 12); cout << string(width - 10, ' '); 
        int gender = 0 ;
        int action;
        if (currentField == 0) {
            action = InputString(inputs[0], inputX, formY + 2, cmnd_max, '_', true);
        } else if (currentField == 1) {
            action = InputString(inputs[1], inputX, formY + 6, ho_max, '_', false);
        } else if (currentField == 2) {
            action = InputString(inputs[2], inputX, formY + 10, ten_max, '_', false); 
        }else if (currentField == 3){
            action = InputGender(gender, inputX, formY + 14); 
        }

        if (action == NAV_ESC){
            ClearScreen();
            return; 
        } 
        else if (action == NAV_UP) { 
            currentField = (currentField == 0) ? 3 : currentField - 1;
        } 
        else if (action == NAV_DOWN) { 
            currentField = (currentField == 3) ? 0 : currentField + 1;
        }else if (action == NAV_ENTER) { 
            // --- KIỂM TRA ĐÃ ĐIỀN ĐỦ 3 MỤC CHƯA ---
            if (!inputs[0].empty() && !inputs[1].empty() && !inputs[2].empty()) {

                Gotoxy(startX, formY + 15);
                SmallBox("Xác nhận thêm hành khách này? (ENTER: Đồng ý - ESC: Quay lại sửa)", (int)width, (int)3, (string)YELLOW);
                
                bool isConfirm = false;
                while (true) {
                    NavKey confirmKey = GetNavKey();
                    if (confirmKey == NAV_ENTER) {
                        isConfirm = true;
                        break;
                    } else if (confirmKey == NAV_ESC) {
                        isConfirm = false;
                        break;
                    }
                }

                if (!isConfirm) {
                    Gotoxy(startX, formY + 15);
                    SmallBox("ENTER/DOWN: Tiếp tục - UP: Lên trên - ESC: Hủy", (int)width, (int)3, (string)WHITE);
                    continue; 
                }

                // --- TIẾN HÀNH LƯU ---
                Gotoxy(startX, formY + 15);
                SmallBox("ĐANG LƯU DỮ LIỆU...", (int)width, (int)3, (string)GREEN);

                HK* NewHK = new HK;
                char* cmnd = const_cast<char*>(inputs[0].c_str());
                char* ho = const_cast<char*>(inputs[1].c_str());
                char* ten = const_cast<char*>(inputs[2].c_str());
                

                if(Add_HK(dsHK, ho, ten, cmnd, gender)) {
                    Gotoxy(startX + 20, formY + 18);
                    cout << YELLOW << "Đã lưu thành công! Nhấn phím bất kỳ để thoát." << RESET;
                    _getch();
                    ClearScreen();
                    return; 
                } else {
                    Gotoxy(startX + 15, formY + 16);
                    cout << RED << "Lỗi: CMND [" << inputs[0] << "] đã tồn tại! Bấm phím bất kỳ..." << RESET;
                    delete NewHK;
                    _getch(); 
                    
                    Gotoxy(startX, formY + 15);
                    SmallBox("ENTER/DOWN: Tiếp tục - UP: Lên trên - ESC: Hủy", (int)width, (int)3, (string)WHITE);
                    currentField = 0; // Trùng mã thì đưa về ô đầu tiên để sửa
                }

            } else {
                // --- CHƯA ĐIỀN ĐỦ MỤC ---
                if (currentField < 2) {
                    currentField++; // Nếu đang ở trên thì tự xuống dòng
                } else {
                    // Nếu đang ở dòng cuối cùng (Số chỗ) mà vẫn có dòng trống ở trên
                    Gotoxy(startX + 5, formY + 12);
                    cout << RED << "Lỗi: Không được để trống bất kỳ trường nào! Bấm phím bất kỳ..." << RESET;
                    _getch();
                    
                    for (int i = 0; i < 3; i++) {
                        if (inputs[i].empty()) {
                            currentField = i;
                            break;
                        }
                    }
                }
            }
        }
    }
}

listMB UiFindMB(string& res,NavKey key, int& ch, listMB* A ){
    
    if(key == NAV_BACK && !res.empty()){
        res.pop_back();
        Gotoxy(20,4);
        cout << res << " " ;
        for(int i = 0 ; i < A->slMB ;i++){
            delete A->list[i];
        }
        return Find_MB_OnRage(dsMB, const_cast<char*>(res.c_str()));
    }else if (isalnum(ch) && key == NAV_UNKNOWN) { 
        res += toupper((char)ch); 
        Gotoxy(20,4);
        cout << res ; 
        for(int i = 0 ; i < A->slMB ;i++){
            delete A->list[i];
        }
        return  Find_MB_OnRage(dsMB, const_cast<char*>(res.c_str()));
    }
  
    return  Find_MB_OnRage(dsMB, const_cast<char*>(res.c_str()));
}

void OnlyPrint_List_MB(listMB& dsMB){
    
    int Temp[] = {10,20,50,90,100};
    int Temp2[] ={10,30,40,10,15};
    int currentPage = 0;
    // Thiết lập thông số phân trang
    int itemsPerPage = 15; // Ví dụ: màn hình chỉ chứa được 15 máy bay
    int startIndex = currentPage * itemsPerPage;
    int endIndex = startIndex + itemsPerPage;
    
    // Đảm bảo không in lố số lượng máy bay thực tế
    if (endIndex > dsMB.slMB) {
        endIndex = dsMB.slMB;
    }
    int itemsOnThisPage = endIndex - startIndex;
    int rowOnScreen = 0 ;
    ClearRegion(10,8,110,16);
    if(dsMB.slMB == 0){
        Gotoxy(50,8);
        cout << RED << "No Result Remaining" << RESET ;
        return ;
    }
    // Chỉ in từ startIndex đến endIndex
    for(int i = startIndex; i < endIndex; i++){
        rowOnScreen = i - startIndex; 
        string s = to_string(i+1);
        Gotoxy(10, 8 + rowOnScreen);
        SmallBox(s,false, false,true, true, 10, 1);
        Gotoxy(20, 8 + rowOnScreen);
        s = dsMB.list[i]->soHieuMB;
        SmallBox(s,false, false,true, true, 30, 1);
        Gotoxy(50, 8 + rowOnScreen);
        s = dsMB.list[i]->loaiMB ;
        SmallBox(s,false, false,true, true, 40, 1);
        Gotoxy(90,8 + rowOnScreen);
        s = to_string(dsMB.list[i]->socho);
        SmallBox(s,false, false,true, true, 10, 1);
        Gotoxy(100,8 + rowOnScreen);
        s = to_string(dsMB.list[i]->SLB);
        SmallBox(s,false, false,true, true, 15, 1);
    }
    for(int i = 0 ; i < 5 ;i++){
        Gotoxy(Temp[i],8 + rowOnScreen+1);
        SmallBox("...",false, false,true, true, Temp2[i], 1,GRAY);
    }
    
}

 NavKey It_Sub_List_MB(listMB& dsMB,int mod){
        int Temp[] = {10,20,50,90,100};
        int Temp2[] ={10,30,40,10,15};
        int currentPage = 0 ;
        while(true){
            // Thiết lập thông số phân trang
            int itemsPerPage = 15; // Ví dụ: màn hình chỉ chứa được 15 máy bay
            int startIndex = currentPage * itemsPerPage;
            int endIndex = startIndex + itemsPerPage;
              // Đảm bảo không in lố số lượng máy bay thực tế
            if (endIndex > dsMB.slMB) {
                endIndex = dsMB.slMB;
            }
            int itemsOnThisPage = endIndex - startIndex;
            int rowOnScreen = 0 ;
            ClearRegion(10,8+itemsOnThisPage,110,16-itemsOnThisPage);
            // Chỉ in từ startIndex đến endIndex
            for(int i = startIndex; i < endIndex; i++){
                rowOnScreen = i - startIndex; 
                string s = to_string(i+1);
                Gotoxy(10, 8 + rowOnScreen);
                SmallBox(s,false, false,true, true, 10, 1);
                Gotoxy(20, 8 + rowOnScreen);
                s = dsMB.list[i]->soHieuMB;
                SmallBox(s,false, false,true, true, 30, 1);
                Gotoxy(50, 8 + rowOnScreen);
                s = dsMB.list[i]->loaiMB ;
                SmallBox(s,false, false,true, true, 40, 1);
                Gotoxy(90,8 + rowOnScreen);
                s = to_string(dsMB.list[i]->socho);
                SmallBox(s,false, false,true, true, 10, 1);
                Gotoxy(100,8 + rowOnScreen);
                s = to_string(dsMB.list[i]->SLB);
                SmallBox(s,false, false,true, true, 15, 1);
            }
            for(int i = 0 ; i < 5 ;i++){
                Gotoxy(Temp[i],8 + rowOnScreen+1);
                SmallBox("...",false, false,true, true, Temp2[i], 1,GRAY);
            }
            
            int totalPages = (dsMB.slMB + itemsPerPage - 1) / itemsPerPage; // Tính tổng số trang
       
            rowOnScreen = 0 ;
            while(true){
                string s ;
                int index = rowOnScreen + startIndex;// rowOnScreen + startIndex -1
                for(int i = 0; i < 5 ;i++){
                    Gotoxy(Temp[i],8 + rowOnScreen);
                    if(i == 1){
                        s = dsMB.list[index]->soHieuMB;
                    }else if (i == 2){
                        s = dsMB.list[index]->loaiMB;
                    }else if (i == 3){
                        s = to_string(dsMB.list[index]->socho);
                    }else{
                        s = to_string(dsMB.list[index]->SLB);
                    }
                    if(i == 0) s = to_string(index+1);
                    SmallBox(s,false, false,true, true, Temp2[i],1, HIGHLIGHT);
                }
                NavKey ch = GetNavKey();
                for(int i = 0; i < 5 ;i++){
                    Gotoxy(Temp[i],8 + rowOnScreen);
                    if(i == 1){
                        s = dsMB.list[index]->soHieuMB;
                    }else if (i == 2){
                        s = dsMB.list[index]->loaiMB;
                    }else if (i == 3){
                        s = to_string(dsMB.list[index]->socho);
                    }else{
                        s = to_string(dsMB.list[index]->SLB);
                    }
                    if(i == 0) s = to_string(index+1);
                    SmallBox(s,false, false,true, true, Temp2[i],1);
                }
                if(ch == NAV_UP){
                    if(rowOnScreen -1 >= 0) rowOnScreen--;
                    else {
                       return ch;
                    } 
                }
                else if (ch == NAV_DOWN){
                    if(rowOnScreen +1 < itemsOnThisPage) rowOnScreen++;
                    else{
                        if(currentPage+1 <totalPages ) currentPage++;
                        else currentPage = 0 ;
                        break ;
                    }
                }else if (ch == NAV_RIGHT){
                    if(currentPage+1 <totalPages ) currentPage++;
                    else currentPage = 0 ;
                    break ;
                }else if(ch == NAV_LEFT){
                    if(currentPage-1 >=0 ) currentPage--;
                    else currentPage = totalPages-1 ;
                    break ;
                }else if (ch == NAV_ENTER){
                    if(mod == 0) break ;
                }else if (ch == NAV_ESC){
                    return ch;
                }
            }
    }
    return NAV_UNKNOWN;
       
 }


void It_list_MB(int mod, bool main , listMB& dsMB, string res){
    Gotoxy(50,2);
    cout << "Danh Sách Máy Bay";
    Gotoxy(10,4);
    int field = 5 ;
    int Temp[] = {10,20,50,90,100};
    int Temp2[] ={10,30,40,10,15};
    cout << GRAY << "Tìm Kiếm: " << res << RESET ;
    string typemode[] = {"In danh sách","Hiệu Chỉnh", "Xoá"};
    Gotoxy(90,4) ;
    cout << GRAY <<"MODE: "<< typemode[mod] << RESET ;
    Gotoxy(10,5);
    SmallBox("STT",10,1);
    Gotoxy(20,5);
    SmallBox("Số Hiệu Máy Bay",30,1);
    Gotoxy(50,5);
    SmallBox("Loại Máy Bay",40,1);
    Gotoxy(90,5);
    SmallBox("Số Chỗ",10,1);
    Gotoxy(100,5);
    SmallBox("Số Lần Bay",15,1);
    int currentPage = 0;
    while(true){
         // Thiết lập thông số phân trang
        int itemsPerPage = 15; // Ví dụ: màn hình chỉ chứa được 15 máy bay
        int startIndex = currentPage * itemsPerPage;
        int endIndex = startIndex + itemsPerPage;
        
        // Đảm bảo không in lố số lượng máy bay thực tế
        if (endIndex > dsMB.slMB) {
            endIndex = dsMB.slMB;
        }
        int itemsOnThisPage = endIndex - startIndex;
        int rowOnScreen = 0 ;
        ClearRegion(10,8+itemsOnThisPage,110,16-itemsOnThisPage);
        // Chỉ in từ startIndex đến endIndex
        for(int i = startIndex; i < endIndex; i++){
            rowOnScreen = i - startIndex; 
            string s = to_string(i+1);
            Gotoxy(10, 8 + rowOnScreen);
            SmallBox(s,false, false,true, true, 10, 1);
            Gotoxy(20, 8 + rowOnScreen);
            s = dsMB.list[i]->soHieuMB;
            SmallBox(s,false, false,true, true, 30, 1);
            Gotoxy(50, 8 + rowOnScreen);
            s = dsMB.list[i]->loaiMB ;
            SmallBox(s,false, false,true, true, 40, 1);
            Gotoxy(90,8 + rowOnScreen);
            s = to_string(dsMB.list[i]->socho);
            SmallBox(s,false, false,true, true, 10, 1);
            Gotoxy(100,8 + rowOnScreen);
            s = to_string(dsMB.list[i]->SLB);
            SmallBox(s,false, false,true, true, 15, 1);
        }
        for(int i = 0 ; i < 5 ;i++){
            Gotoxy(Temp[i],8 + rowOnScreen+1);
            SmallBox("...",false, false,true, true, Temp2[i], 1,GRAY);
        }
        
        
        //In thông tin điều hướng ở cuối
        Gotoxy(35, 8 + itemsPerPage + 1);
        int totalPages = (dsMB.slMB + itemsPerPage - 1) / itemsPerPage; // Tính tổng số trang
        string m = "Trang " + to_string(currentPage + 1) + "/" + to_string(totalPages) + "  |  Dùng phím ◄/► để chuyển trang, ▲/▼ đổi lựa chọn, ESC để thoát." ;
      
        SmallBox(m,50,4,string(GRAY));
        rowOnScreen = 0 ;
        while(true){
            string s ;
            int index = rowOnScreen + startIndex;// rowOnScreen + startIndex -1
            for(int i = 0; i < 5 ;i++){
                Gotoxy(Temp[i],8 + rowOnScreen);
                if(i == 1){
                    s = dsMB.list[index]->soHieuMB;
                }else if (i == 2){
                    s = dsMB.list[index]->loaiMB;
                }else if (i == 3){
                    s = to_string(dsMB.list[index]->socho);
                }else{
                    s = to_string(dsMB.list[index]->SLB);
                }
                if(i == 0) s = to_string(index+1);
                SmallBox(s,false, false,true, true, Temp2[i],1, HIGHLIGHT);
            }
            NavKey ch = GetNavKey();
            for(int i = 0; i < 5 ;i++){
                Gotoxy(Temp[i],8 + rowOnScreen);
                 if(i == 1){
                    s = dsMB.list[index]->soHieuMB;
                }else if (i == 2){
                    s = dsMB.list[index]->loaiMB;
                }else if (i == 3){
                    s = to_string(dsMB.list[index]->socho);
                }else{
                    s = to_string(dsMB.list[index]->SLB);
                }
                if(i == 0) s = to_string(index+1);
                SmallBox(s,false, false,true, true, Temp2[i],1);
            }
            if(ch == NAV_UP){
                if(rowOnScreen -1 >= 0) rowOnScreen--;
                else {
                    listMB tmp ;
                    while(true){
                        Gotoxy(20 + res.length(),4);
                        int ch = 0 ;
                        NavKey key = GetNavKey(ch);
                        tmp = UiFindMB(res,key,ch,&tmp);
                        if(key == NAV_ESC){
                            for(int i = 0 ; i < tmp.slMB;i++) delete tmp.list[i];
                            return ;
                        } 
                        OnlyPrint_List_MB(tmp);
                        if(key == NAV_DOWN && tmp.slMB > 0) {
                            if(tmp.slMB == dsMB.slMB) break ;
                            NavKey c = It_Sub_List_MB(tmp,mod);
                            if(c == NAV_ESC){
                                for(int i = 0 ; i < tmp.slMB;i++) delete tmp.list[i];
                                return ;
                            }
                        }
                    }
                    for(int i = 0 ; i < tmp.slMB;i++) delete tmp.list[i];
                } 
            }
            else if (ch == NAV_DOWN){
                if(rowOnScreen +1 < itemsOnThisPage) rowOnScreen++;
                else{
                    if(currentPage+1 <totalPages ) currentPage++;
                    else currentPage = 0 ;
                    break ;
                }
            }else if (ch == NAV_RIGHT){
                if(currentPage+1 <totalPages ) currentPage++;
                else currentPage = 0 ;
                break ;
            }else if(ch == NAV_LEFT){
                if(currentPage-1 >=0 ) currentPage--;
                else currentPage = totalPages-1 ;
                break ;
            }else if (ch == NAV_ENTER){
                if(mod == 0) break ;
            }else if (ch == NAV_ESC){
                return ;
            }
        }
    }
    
}

void It_list_CB(int mod){
    Gotoxy(50,2);
    cout << "Danh Sách Chuyến Bay";
    Gotoxy(10,4);
    int field = 5 ;
    int size = 6 ;
    int Temp[] = {10,20,50,80,90,105};
    int Temp2[] ={10,30,30,10,15,10};
    cout << GRAY << "Tìm Kiếm" << RESET ;
    string typemode[] = {"In danh sách","Hiệu Chỉnh", "Huỷ Chuyến"};
    Gotoxy(90,4) ;
    cout << GRAY <<"MODE: "<< typemode[mod] << RESET ;
    Gotoxy(10,5);
    SmallBox("STT",10,1);
    Gotoxy(20,5);
    SmallBox("Mã Chuyến Bay",30,1);
    Gotoxy(50,5);
    SmallBox("Ngày Khởi Hành",30,1);
    Gotoxy(80,5);
    SmallBox("Số Vé",10,1);
    Gotoxy(90,5);
    SmallBox("Sân bay đích",15,1);
    Gotoxy(105,5);
    SmallBox("Status",10,1);
    string q[] = {"","","",""};
    string Type[] = {RED, GREEN, YELLOW, CYAN};
    q[0] = Type[0] + SQ + RESET + ": Chuyến bay bị huỷ " ;
    q[1] = Type[1] + SQ + RESET + ": Chuyến bay còn vé " ;
    q[2] = Type[2] + SQ + RESET + ": Chuyến bay hết vé " ;
    q[3] = Type[3] + SQ + RESET + ": Chuyến bay hoàn tất " ;
    Gotoxy(70,24);
    string u = "  " + q[0] + q[1] +"    " +q[2] + q[3];
    SmallBox(u,50,4);
    int currentPage = 0;
    while(true){
         // Thiết lập thông số phân trang
        int itemsPerPage = 15; // Ví dụ: màn hình chỉ chứa được 15 CB
        int startIndex = currentPage * itemsPerPage;
        int endIndex = startIndex + itemsPerPage;
        
        // Đảm bảo không in lố số lượng CB thực tế
        if (endIndex > dsCB.slCB) {
            endIndex = dsCB.slCB;
        }
        int itemsOnThisPage = endIndex - startIndex;
        int rowOnScreen = 0 ;
        ClearRegion(10,8+itemsOnThisPage,110,16-itemsOnThisPage);
        CB* current = dsCB.head;
        for (int k = 0; k < startIndex && current != NULL; k++) {
            current = current->next; // Nhảy qua các phần tử của trang trước
        }
        // Chỉ in từ startIndex đến endIndex
        for(int i = startIndex; i < endIndex; i++){
            rowOnScreen = i - startIndex; 
            string s = to_string(i+1);
            Gotoxy(10, 8 + rowOnScreen);
            SmallBox(s,false, false,true, true, 10, 1);
            Gotoxy(20, 8 + rowOnScreen);
            s = current->maCB;
            SmallBox(s,false, false,true, true, 30, 1);
            Gotoxy(50, 8 + rowOnScreen);
            s = ToStringDate(current->ngayKH) ;
            SmallBox(s,false, false,true, true, 30, 1);
            Gotoxy(80,8 + rowOnScreen);
            s = to_string(current->socho);
            SmallBox(s,false, false,true, true, 10, 1);
            Gotoxy(90,8 + rowOnScreen);
            s = current->sbDich;
            SmallBox(s,false, false,true, true, 15, 1);
            Gotoxy(105,8 + rowOnScreen);
            s = TranFormSatus(current->trangThai);
            SmallBox(SQ,false, false,true, true, 10, 1,s);
            current = current->next;
        }
        for(int i = 0 ; i < size ;i++){
            Gotoxy(Temp[i],8 + rowOnScreen+1);
            SmallBox("...",false, false,true, true, Temp2[i], 1,GRAY);
        }
        
        //In thông tin điều hướng ở cuối
        Gotoxy(5, 8 + itemsPerPage + 1);
        int totalPages = (dsCB.slCB + itemsPerPage - 1) / itemsPerPage; // Tính tổng số trang
        string m = "Trang " + to_string(currentPage + 1) + "/" + to_string(totalPages) + "  |  Dùng phím ◄/► để chuyển trang, ▲/▼ đổi lựa chọn, ESC để thoát." ;
        SmallBox(m,50,4,string(GRAY));
        Gotoxy(59, 8 + itemsPerPage + 2);
        cout << " /\\_/\\";
        Gotoxy(59, 8 + itemsPerPage + 3);
        cout <<"( o.o )";
        Gotoxy(59, 8 + itemsPerPage + 4);
        cout << " > ^ <";

        current = dsCB.head;
        for (int k = 0; k < startIndex && current != NULL; k++) {
            current = current->next; // Nhảy qua các phần tử của trang trước
        }
        // 2. LƯU CÁC NODE CỦA TRANG NÀY VÀO MẢNG TẠM
        CB* pageNodes[itemsPerPage]; 
        int nodeCount = 0;
        CB* temp = current;
        for (int k = 0; k < itemsOnThisPage && temp != NULL; k++) {
            pageNodes[nodeCount] = temp;
            nodeCount++;
            temp = temp->next;
        }

        rowOnScreen = 0 ;
        while(true){
            string s ;
            int index = rowOnScreen + startIndex;// rowOnScreen + startIndex
            CB* SelectedNode = pageNodes[rowOnScreen];
            for(int i = 0; i < size ;i++){
                Gotoxy(Temp[i],8 + rowOnScreen);
                if(i == 0) s = to_string(index+1);
                else if(i == 1){
                    s = SelectedNode->maCB;
                }else if (i == 2){
                    s = ToStringDate(SelectedNode->ngayKH);
                }else if (i == 3){
                    s = to_string(SelectedNode->socho);
                }else if (i == 4){
                    s = SelectedNode->sbDich;
                }else{
                    s = TranFormSatus(SelectedNode->trangThai);
                    SmallBox(SQ,false, false,true, true, Temp2[i],1,s);
                }
                if(i!= 5)SmallBox(s,false, false,true, true, Temp2[i],1,HIGHLIGHT);
            }
            NavKey ch = GetNavKey();
            for(int i = 0; i < size ;i++){
                Gotoxy(Temp[i],8 + rowOnScreen);
                if(i == 0) s = to_string(index+1);
                else if(i == 1){
                    s = SelectedNode->maCB;
                }else if (i == 2){
                    s = ToStringDate(SelectedNode->ngayKH);
                }else if (i == 3){
                    s = to_string(SelectedNode->socho);
                }else if (i == 4){
                    s = SelectedNode->sbDich;
                }else{
                    s = TranFormSatus(SelectedNode->trangThai);
                    SmallBox(SQ,false, false,true, true, Temp2[i],1,s);
                }
                if(i != 5)SmallBox(s,false, false,true, true, Temp2[i],1);
            }
            if(ch == NAV_UP){
                if(rowOnScreen -1 >= 0) rowOnScreen--;
                else break ;
            }
            else if (ch == NAV_DOWN){
                if(rowOnScreen +1 < itemsOnThisPage) rowOnScreen++;
                else{
                    if(currentPage+1 <totalPages ) currentPage++;
                    else currentPage = 0 ;
                    break ;
                }
            }else if (ch == NAV_RIGHT){
                if(currentPage+1 <totalPages ) currentPage++;
                else currentPage = 0 ;
                break ;
            }else if(ch == NAV_LEFT){
                if(currentPage-1 >=0 ) currentPage--;
                else currentPage = totalPages-1 ;
                break ;
            }else if (ch == NAV_ENTER){
                if(mod == 0) break ;
            }else if (ch == NAV_ESC){
                return ;
            }
        }
    }
    
}


void StatiscalMB(){
    listMB dsMB_Stat = Get_Flight_Stats(dsMB);
    Gotoxy(50,2);
    cout << "Danh Sách Máy Bay";
    Gotoxy(10,4);
    int field = 5 ;
    int Temp[] = {10,20,50,90,100};
    int Temp2[] ={10,30,40,10,15};
    cout << GRAY << "Tìm Kiếm" << RESET ;
    string typemode[] = {"In danh sách","Hiệu Chỉnh", "Xoá"};
    Gotoxy(90,4) ;
    Gotoxy(10,5);
    SmallBox("STT",10,1);
    Gotoxy(20,5);
    SmallBox("Số Hiệu Máy Bay",30,1);
    Gotoxy(50,5);
    SmallBox("Loại Máy Bay",40,1);
    Gotoxy(90,5);
    SmallBox("Số Chỗ",10,1);
    Gotoxy(100,5);
    SmallBox("Số Lần Bay",15,1);
    int currentPage = 0;
    while(true){
         // Thiết lập thông số phân trang
        int itemsPerPage = 15; // Ví dụ: màn hình chỉ chứa được 15 máy bay
        int startIndex = currentPage * itemsPerPage;
        int endIndex = startIndex + itemsPerPage;
        
        // Đảm bảo không in lố số lượng máy bay thực tế
        if (endIndex > dsMB_Stat.slMB) {
            endIndex = dsMB_Stat.slMB;
        }
        int itemsOnThisPage = endIndex - startIndex;
        int rowOnScreen = 0 ;
        ClearRegion(10,8+itemsOnThisPage,110,16-itemsOnThisPage);
        // Chỉ in từ startIndex đến endIndex
        for(int i = startIndex; i < endIndex; i++){
            rowOnScreen = i - startIndex; 
            string s = to_string(i+1);
            Gotoxy(10, 8 + rowOnScreen);
            SmallBox(s,false, false,true, true, 10, 1);
            Gotoxy(20, 8 + rowOnScreen);
            s = dsMB_Stat.list[i]->soHieuMB;
            SmallBox(s,false, false,true, true, 30, 1);
            Gotoxy(50, 8 + rowOnScreen);
            s = dsMB_Stat.list[i]->loaiMB ;
            SmallBox(s,false, false,true, true, 40, 1);
            Gotoxy(90,8 + rowOnScreen);
            s = to_string(dsMB_Stat.list[i]->socho);
            SmallBox(s,false, false,true, true, 10, 1);
            Gotoxy(100,8 + rowOnScreen);
            s = to_string(dsMB_Stat.list[i]->SLB);
            SmallBox(s,false, false,true, true, 15, 1);
        }
        for(int i = 0 ; i < 5 ;i++){
            Gotoxy(Temp[i],8 + rowOnScreen+1);
            SmallBox("...",false, false,true, true, Temp2[i], 1,GRAY);
        }
        
        
        //In thông tin điều hướng ở cuối
        Gotoxy(35, 8 + itemsPerPage + 1);
        int totalPages = (dsMB_Stat.slMB + itemsPerPage - 1) / itemsPerPage; // Tính tổng số trang
        string m = "Trang " + to_string(currentPage + 1) + "/" + to_string(totalPages) + "  |  Dùng phím ◄/► để chuyển trang, ▲/▼ đổi lựa chọn, ESC để thoát." ;
      
        SmallBox(m,50,4,string(GRAY));
        rowOnScreen = 0 ;
        while(true){
            string s ;
            int index = rowOnScreen + startIndex;// rowOnScreen + startIndex -1
            for(int i = 0; i < 5 ;i++){
                Gotoxy(Temp[i],8 + rowOnScreen);
                if(i == 1){
                    s = dsMB_Stat.list[index]->soHieuMB;
                }else if (i == 2){
                    s = dsMB_Stat.list[index]->loaiMB;
                }else if (i == 3){
                    s = to_string(dsMB_Stat.list[index]->socho);
                }else{
                    s = to_string(dsMB_Stat.list[index]->SLB);
                }
                if(i == 0) s = to_string(index+1);
                SmallBox(s,false, false,true, true, Temp2[i],1, HIGHLIGHT);
            }
            NavKey ch = GetNavKey();
            for(int i = 0; i < 5 ;i++){
                Gotoxy(Temp[i],8 + rowOnScreen);
                 if(i == 1){
                    s = dsMB_Stat.list[index]->soHieuMB;
                }else if (i == 2){
                    s = dsMB_Stat.list[index]->loaiMB;
                }else if (i == 3){
                    s = to_string(dsMB_Stat.list[index]->socho);
                }else{
                    s = to_string(dsMB_Stat.list[index]->SLB);
                }
                if(i == 0) s = to_string(index+1);
                SmallBox(s,false, false,true, true, Temp2[i],1);
            }
            if(ch == NAV_UP){
                if(rowOnScreen -1 >= 0) rowOnScreen--;
                else break ;
            }
            else if (ch == NAV_DOWN){
                if(rowOnScreen +1 < itemsOnThisPage) rowOnScreen++;
                else{
                    if(currentPage+1 <totalPages ) currentPage++;
                    else currentPage = 0 ;
                    break ;
                }
            }else if (ch == NAV_RIGHT){
                if(currentPage+1 <totalPages ) currentPage++;
                else currentPage = 0 ;
                break ;
            }else if(ch == NAV_LEFT){
                if(currentPage-1 >=0 ) currentPage--;
                else currentPage = totalPages-1 ;
                break ;
            }else if (ch == NAV_ENTER){
                break ;
            }else if (ch == NAV_ESC){
                for (int i = 0; i < dsMB_Stat.slMB; i++) {
                    delete dsMB_Stat.list[i]; 
                }
                return ;
            }
        }
    }

}

void Router_B(int mainIdx, function<void()> func_1, 
                  function<void()> func_2, 
                  function<void()> func_3,
                  function<void()> func_4){
    int chosen = 0; // Vị trí lựa chọn trong menu con
    int startX, startY ;
    int space = 1;
    string* Options = Menus[mainIdx];
    int length = MenusSize[mainIdx];
    while (true) {
        // Vẽ lại vùng SideBar
        startX = 1;
        startY = 22;
        Gotoxy(startX, startY);
        // ClearRegion đã reset lại vị trí bắt đầu của con trỏ
        ClearRegion(startX, startY, 60, 5); 

        // 2. Vẽ danh sách các tùy chọn
        for (int i = 0; i < length; i++) {
            string s = Options[i];
            string color = WHITE;

            if (i == chosen) {
                color = YELLOW;
                s = string(ARR) + " " + s + " " + ARL; // Thêm mũi tên khi chọn
            }

            SmallBox(s, 18, 5, color);
            startX = whereX();
            Gotoxy(startX + 19 + space, startY);
        }
        NavKey key = GetNavKey();
        switch (key) {
            case NAV_LEFT:
                chosen = (chosen - 1 >= 0) ? chosen - 1 : length - 1;
                break;
            case NAV_RIGHT:
                chosen = (chosen + 1 < length) ? chosen + 1 : 0;
                break;
            case NAV_ENTER:
                switch(chosen){
                    case 0 :
                        func_1();
                        break ;
                    case 1 :
                        func_2();
                        break;
                    case 2 :
                        func_3();
                        break;
                    case 3 :
                        func_4();
                        break;
                    case 4:
                        return;

                }
                break; 
            case NAV_ESC:
                return ; // Thoát không chọn
        }
    }
}

void Router_Board(int mainMenuIdx, function<void()> func_1, 
                  function<void()> func_2, 
                  function<void()> func_3){
    int chosen = 0; // Vị trí lựa chọn trong menu con
    int startX = 71;
    int startY = 6;
    int space = 1;
    string* Options = Menus[mainMenuIdx];
    int length = MenusSize[mainMenuIdx];
    while (true) {
        // Vẽ lại vùng SideBar
        Gotoxy(startX, startY);
        // ClearRegion đã reset lại vị trí bắt đầu của con trỏ
        ClearRegion(startX, startY, 30, 20); 

        // 2. Vẽ danh sách các tùy chọn
        for (int i = 0; i < length; i++) {
            string s = Options[i];
            string color = WHITE;

            if (i == chosen) {
                color = YELLOW;
                s = string(ARR) + " " + s + " " + ARL; // Thêm mũi tên khi chọn
            }

            // Di chuyển con trỏ đến đúng dòng để vẽ Box tiếp theo
            // Mỗi SmallBox cao 3 dòng + khoảng cách space
            Gotoxy(startX, startY + (i * (3 + space))); 

            SmallBox(s, true, true, true, true, 30, 1, color);
            Gotoxy(whereX(),whereY() + 1);
        }
        if(mainMenuIdx == 0) ViewQLMB();
        // 3. Bắt phím điều khiển
        NavKey key = GetNavKey();
        switch (key) {
            case NAV_UP:
                chosen = (chosen - 1 >= 0) ? chosen - 1 : length - 1;
                break;
            case NAV_DOWN:
                chosen = (chosen + 1 < length) ? chosen + 1 : 0;
                break;
            case NAV_ENTER:
                switch(chosen){
                    case 0 :
                        func_1();
                        break ;
                    case 1 :
                        func_2();
                        break;
                    case 2 :
                        func_3();
                        break;
                    case 3 :
                        return;
                }
                break; 
            case NAV_ESC:
                return ; // Thoát không chọn
        }
    }
}

int SubMenu(string options[], int length) {
    int chosen = 0;
    // Lưu lại vị trí bắt đầu để menu không bị "trôi"
    int startX = whereX();
    int startY = whereY();

    while(true) {
        ClearRegion(startX, startY, 70, (length * 3)); // (Mỗi box cao 3 dòng)

        for(int i = 0 ; i < length ; i++) {
            string s = options[i];
            if (i == chosen) {
                s = string(ARR) + " " + s + " " + ARL;
            }

            // Chọn màu trước
            string color = (i == chosen) ? YELLOW : WHITE;

            if (i == 0) {
                SmallBox(s, true, false, true, true, 70, 3, color);
            } else if (i < length - 1) {
                SmallBox(s, false, false, true, true, 70, 3, color);
            } else {
                SmallBox(s, false, true, true, true, 70, 3, color);
            }
        }

        int pre_chosen = chosen ;
        chosen = GetKey(chosen,length);
        if(chosen == 999) return pre_chosen ;
        if(chosen == 1000) return 4;
    }
}

int MainMenuOptionInBoard(string options[], int length) {
    int chosen = 0;
    // Lưu lại vị trí bắt đầu để menu không bị "trôi"
    int startX = whereX();
    int startY = whereY();

    while(true) {
        ClearRegion(startX, startY, 70, (length * 3)); // (Mỗi box cao 3 dòng)

        for(int i = 0 ; i < length ; i++) {
            string s = options[i];
            if (i == chosen) {
                s = string(ARR) + " " + s + " " + ARL;
            }

            // Chọn màu trước
            string color = (i == chosen) ? YELLOW : WHITE;

            if (i == 0) {
                SmallBox(s, true, false, true, true, 70, 3, color);
            } else if (i < length - 1) {
                SmallBox(s, false, false, true, true, 70, 3, color);
            } else {
                SmallBox(s, false, true, true, true, 70, 3, color);
            }
        }
        if(chosen < 3) ViewOptions(chosen);
        //if(chosen == 3) //Hướng dẫn sử dụng 
        int pre_chosen = chosen ;
        chosen = GetKey(chosen,length);
        if(chosen == 999) return pre_chosen ;
        if(chosen == 1000) return 4;
    }
}

void RunInNewTab(void (*func)()) {
    cout << NEWTAB;
    func();
    cout << CLOSETAB;
}

// []() { func(<type> Name);}

void Menu_QuanLyMayBay() {
    Router_B(3, 
        []() { RunInNewTab(CustomerAddMB); }, 
        []() { RunInNewTab([]() { It_list_MB(1,true,dsMB); });}, 
        []() { RunInNewTab([]() { It_list_MB(2,true,dsMB); });},
        []() { RunInNewTab([]() { It_list_MB(0,true,dsMB); });} 
    );
}

void Menu_QuanLyChuyenBay() {
    Router_B(3, 
        []() { RunInNewTab(CustomerAddCB); }, // Thêm
        []() { RunInNewTab([]() { It_list_CB(1); });}, // Sửa
        []() { RunInNewTab([]() { It_list_CB(2); }); }, // Xóa
        []() { RunInNewTab([]() { It_list_CB(0); });} 
    );
}

void Menu_QuanLyHanhKhach() {
    Router_B(3, 
        []() { RunInNewTab(CustomerAddHK); }, // Thêm
        []() { /* Gọi hàm Sửa HK ở đây */ }, // Sửa
        []() { /* Gọi hàm Xóa HK ở đây */ }  // Xóa
    );
}

void Menu_TraCuuChuyenBay() {
     RunInNewTab();
}

void Menu_ThongKeLuotBay() {
     RunInNewTab(StatiscalMB);
}

void Menu_InDanhSachHanhKhach() {
     RunInNewTab();
}

void MainScreen(){
    string options[] = {"1. Quản Lý Hệ Thống", 
                        "2. Quản Lý Vé", 
                        "3. Tra Cứu & Thống Kê", 
                        "4. Hướng Dẫn Sử Dụng",
                        "5. Thoát"};
    while(true){
        ClearScreen();
        SmallBox("Quản lí chuyến Bay Nội Địa",120,3);
        // nhảy xuống dòng do SmallBox k tự động xuống dòng
        Gotoxy(whereX(),whereY()+1);
        int chosen = MainMenuOptionInBoard(options,sizeof(options)/sizeof(string));
        if(chosen >= 0 && chosen <= 2)
        {
            switch (chosen)
            {
            case 0:
                // Gọi Router_Board và truyền tên các hàm menu vào
                Router_Board(chosen, Menu_QuanLyMayBay, Menu_QuanLyChuyenBay, Menu_QuanLyHanhKhach);
                break;
            case 1:
                break;
            case 2:
                Router_Board(chosen, Menu_TraCuuChuyenBay, Menu_ThongKeLuotBay, Menu_InDanhSachHanhKhach);
                break;
            default:
                break;
            }
        }
        else if(chosen == 3)
        {
            // Hướng dẫn sử dụng
        }
        else if(chosen == 4)
        {
            exit(0); // thoát chương trình
        }
    }
}
