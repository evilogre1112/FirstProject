#include <iostream>
#include <string>

// Chỉ thêm windows.h nếu đang chạy trên Windows
#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <conio.h>
#else
    #include <sys/ioctl.h>
    #include <unistd.h>
    #include <termios.h>
    #include <unistd.h>
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

int TotalWidth = 0;


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

int visualLength(const string& s) {
    string plain = "";
    bool inCode = false;
    for (size_t i = 0; i < s.length(); i++) {
        if (s[i] == '\033') inCode = true; // Bắt đầu mã ANSI
        if (!inCode) {
            // Kiểm tra ký tự UTF-8 (ký tự đa byte như ▶)
            if ((s[i] & 0xc0) != 0x80) plain += s[i]; 
        }
        if (inCode && s[i] == 'm') inCode = false; // Kết thúc mã ANSI
    }
    return plain.length();
}

void PrintBox(string text, bool ABOVE , bool UNDER ){
    //Tính khoảng trắng lề trái để đẩy toàn bộ khung ra giữa màn hình
    int marginLeft = (TotalWidth - boxWidth)/2 ;
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

void MainScreen(){
    int selectedIdx = 0;
    string options[] = {"1. Quản Lý Hệ Thống", 
                        "2. Quản Lý Vé", 
                        "3. Tra Cứu & Thống Kê", 
                        "4. Hướng Dẫn Sử Dụng",
                        "5. Thoát"};
    int numOptions = sizeof(options) / sizeof(options[0]);
    while(true){
        ClearScreen();
        PrintBox("QUẢN LÝ CHUYẾN BAY NỘI ĐỊA", true , false);
        for(int i = 0 ; i < numOptions;i++){
            string s = (selectedIdx == i) ? string(BOLDYELLOW) + "▶ " + options[i] + " ◀" + string(RESET) : "  " + options[i];
            if(i != numOptions -1){
                PrintBox(s,false,false);
            }else{
                PrintBox(s,false,true);
            }
        }
        cout << string((TotalWidth - boxWidth)/2, ' ') << string(GRAY) << "Dùng phím ↑/↓ để di chuyển || Enter để chọn || ESC để thoát" << RESET << endl; 
        int key = _getch();
        if(key == 224){
            key = _getch();
            switch(key){
                case 72: // Mũi tên Lên
                    if(selectedIdx > 0) selectedIdx--;
                    else selectedIdx = numOptions - 1; // Nhảy vòng xuống cuối
                    break;
                case 80: // Mũi tên Xuống
                    if(selectedIdx < numOptions - 1) selectedIdx++;
                    else selectedIdx = 0; // Nhảy vòng lên đầu
                    break;
            }
        }
        else if(key == 13){ // Phím Enter
            ClearScreen();
            cout << "Bạn đã chọn: " << BOLDGREEN << options[selectedIdx] << RESET << endl;
            cout << "\nNhấn phím bất kỳ để quay lại menu...";
            _getch();
        }
        else if(key == 27){ // Phím ESC để thoát
            break;
        }

    }
}