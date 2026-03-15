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

string subOptions4[4] = {
    "Thêm",
    "Sữa",
    "Hiệu Chỉnh",
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

NavKey GetNavKey() {

#ifdef _WIN32
    int key = _getch();
    if(key == 224 || key == 0) {
        key = _getch();
        if(key == 72) return NAV_UP;
        if(key == 80) return NAV_DOWN;
        if(key == 75) return NAV_LEFT;  // Thêm mũi tên trái
        if(key == 77) return NAV_RIGHT; // Thêm mũi tên phải
    } 
    else if(key == 13) return NAV_ENTER;
    else if(key == 27) return NAV_ESC;
    // Thêm phím chữ cho Windows nếu muốn đồng bộ với Linux
    else if(key == 'w' || key == 'W') return NAV_UP;
    else if(key == 's' || key == 'S') return NAV_DOWN;
    else if(key == 'a' || key == 'A') return NAV_LEFT;
    else if(key == 'd' || key == 'D') return NAV_RIGHT;

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
        if (seq[0] == 27) { 
            // Đọc thêm 2 byte nếu là phím mũi tên (ESC [ + A/B/C/D)
            if (read(STDIN_FILENO, &seq[1], 1) == 1 && read(STDIN_FILENO, &seq[2], 1) == 1) {
                if (seq[1] == 91) {
                    if (seq[2] == 65) result = NAV_UP;
                    else if (seq[2] == 66) result = NAV_DOWN;
                    else if (seq[2] == 67) result = NAV_RIGHT; // Thêm mũi tên phải (ANSI 'C')
                    else if (seq[2] == 68) result = NAV_LEFT;  // Thêm mũi tên trái (ANSI 'D')
                }
            } else {
                result = NAV_ESC; 
            }
        } 
        else if (seq[0] == 10 || seq[0] == 13) result = NAV_ENTER;
        else if (seq[0] == 'w' || seq[0] == 'W') result = NAV_UP;
        else if (seq[0] == 's' || seq[0] == 'S') result = NAV_DOWN;
        else if (seq[0] == 'a' || seq[0] == 'A') result = NAV_LEFT;  // Thêm phím A
        else if (seq[0] == 'd' || seq[0] == 'D') result = NAV_RIGHT; // Thêm phím D
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); 
    return result;
#endif

    return NAV_UNKNOWN;
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

// Trả về: 0 (ESC), 1 (ENTER), 2 (UP), 3 (DOWN)
int InputString(string &result, int x, int y, int maxLength, char placeholder = '_', bool onlyNumbers = false) {
    // Không reset 'result' về rỗng để giữ lại chữ cũ khi người dùng quay lại ô này
    Gotoxy(x, y);
    cout << string(maxLength, placeholder); 
    Gotoxy(x, y);
    cout << result; // In đè chữ cũ lên trên hàng gạch

    while (true) {
        if (_kbhit()) {
            int ch = _getch();

            // Xử lý phím mũi tên
            if (ch == 224 || ch == 0) {
                ch = _getch(); 
                if (ch == 72) return 2; // Nhấn Mũi tên LÊN
                if (ch == 80) return 3; // Nhấn Mũi tên XUỐNG
                continue; 
            }

            if (ch == 13) return 1; // Phím ENTER
            if (ch == 27) return 0; // Phím ESC

            if (ch == 8) { // Phím BACKSPACE
                if (!result.empty()) {
                    result.pop_back();
                    cout << "\b" << placeholder << "\b";
                }
            } 
            else if (result.length() < maxLength) {
                // CHẶN CHỮ NGAY TỪ LÚC GÕ
                if (onlyNumbers && !isdigit(ch)) continue; 
                
                // Các ràng buộc khác
                if (!onlyNumbers && !isprint(ch)) continue;
                if (result.empty() && ch == ' ') continue;

                result += (char)ch;
                cout << (char)ch;
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
            if (c >= 240) step = 4;
            else if (c >= 224) step = 3;
            else step = 2;
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
    Gotoxy(startX,startY);
    int space = 1 ;
    for(int i = 0 ; i < 4 ;i++){
        SmallBox(subOptions4[i],16,5);
        startX = whereX();
        Gotoxy(startX + 16 + space, startY);
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
            // Tham số TRUE cuối cùng sẽ chặn toàn bộ các ký tự không phải là số
            action = InputString(inputs[2], inputX, formY + 10, 3, '_', true); 
        }

        // Xử lý sự kiện trả về từ InputString
        if (action == 0){
            ClearScreen();
            return; // Nhấn ESC -> Hủy hoàn toàn form
        } 
        
        else if (action == 2) { // Nhấn LÊN
            currentField = (currentField == 0) ? 2 : currentField - 1;
        } 
        else if (action == 3) { // Nhấn XUỐNG
            currentField = (currentField == 2) ? 0 : currentField + 1;
        }else if (action == 1) { // Nhấn ENTER
            // --- BƯỚC 1: KIỂM TRA ĐÃ ĐIỀN ĐỦ 3 MỤC CHƯA ---
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
                    Gotoxy(startX + 20, formY + 16);
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
                // --- BƯỚC 2: CHƯA ĐIỀN ĐỦ MỤC ---
                if (currentField < 2) {
                    currentField++; // Nếu đang ở trên thì tự xuống dòng
                } else {
                    // Nếu đang ở dòng cuối cùng (Số chỗ) mà vẫn có dòng trống ở trên
                    Gotoxy(startX + 5, formY + 12);
                    cout << RED << "Lỗi: Không được để trống bất kỳ trường nào! Bấm phím bất kỳ..." << RESET;
                    _getch();
                    
                    // Tính năng thông minh: Tự động nhảy con trỏ tìm ô trống đầu tiên
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

void Router_B(int mainIdx, function<void()> func_1, 
                  function<void()> func_2, 
                  function<void()> func_3){
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

            SmallBox(s, 16, 5, color);
            startX = whereX();
            Gotoxy(startX + 16 + space, startY);
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
                Router_Board(chosen, [](){ 
                                        Router_B(3, []() { 
                                            // Lệnh ảo thuật: Mở màn hình phụ
                                            cout << NEWTAB; 
                                            CustomerAddMB(); 
                                            // Lệnh ảo thuật: Đóng màn hình phụ, khôi phục giao diện
                                            cout << CLOSETAB; 
                                        }, 
                                        [](){ /* Hàm sửa */ }, 
                                        [](){ /* Hàm hiệu chỉnh */ }
                                        ); 
                                    }, 
                                    [](){ Router_B(3); }, 
                                    [](){ Router_B(3); });
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
