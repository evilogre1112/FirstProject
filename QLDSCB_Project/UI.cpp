#include <iostream>
#include <string>
#include <sstream>
#include <functional>
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

string* Menus[] = {
        subOptions1,
        subOptions2,
        subOptions3
};

int MenusSize[] = {
    sizeof(subOptions1)/sizeof(string),
    sizeof(subOptions2)/sizeof(string),
    sizeof(subOptions3)/sizeof(string),
};

int TotalWidth = 0;

NavKey GetNavKey() {

#ifdef _WIN32
    int key = _getch();
    if(key == 224 || key == 0) {
        key = _getch();
        if(key == 72) return NAV_UP;
        if(key == 80) return NAV_DOWN;
    } 
    else if(key == 13) return NAV_ENTER;
    else if(key == 27) return NAV_ESC;

#else
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    
    // Thiết lập đọc không khóa (Non-blocking read) cho chuỗi ANSI
    newt.c_cc[VMIN] = 0; 
    newt.c_cc[VTIME] = 1; // Đợi tối đa 0.1s
    
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    char seq[3];
    int bytesRead = read(STDIN_FILENO, &seq[0], 1); // Đọc byte đầu tiên
    NavKey result = NAV_UNKNOWN;

    if (bytesRead == 1) {
        if (seq[0] == 27) { // Nếu là phím ESC hoặc bắt đầu phím mũi tên
            if (read(STDIN_FILENO, &seq[1], 1) == 1 && read(STDIN_FILENO, &seq[2], 1) == 1) {
                if (seq[1] == 91) {
                    if (seq[2] == 65) result = NAV_UP;
                    else if (seq[2] == 66) result = NAV_DOWN;
                }
            } else {
                result = NAV_ESC; // Bấm ESC thật sự
            }
        } 
        else if (seq[0] == 10 || seq[0] == 13) result = NAV_ENTER;
        else if (seq[0] == 'w' || seq[0] == 'W') result = NAV_UP;
        else if (seq[0] == 's' || seq[0] == 'S') result = NAV_DOWN;
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // Trả lại cấu hình Terminal
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


void Router_Board(int mainMenuIdx, function<void()> func_1 = [](){} , 
                  function<void()> func_2 = [](){}, 
                  function<void()> func_3 = [](){}){
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
        SmallBox("Quản lí chuyến Bay Nội Địa",true,true,true,true,120,3);
        // nhảy xuống dòng do SmallBox k tự động xuống dòng
        Gotoxy(whereX(),whereY()+1);
        int chosen = MainMenuOptionInBoard(options,sizeof(options)/sizeof(string));
        if(chosen >= 0 && chosen <= 2)
        {
            Router_Board(chosen);
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
