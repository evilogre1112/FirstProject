//Hạn chế sử dụng bits/stdc++.h, tốn bộ nhớ
#include <iostream>
#include <string>
#include <iomanip>

// Chỉ thêm windows.h nếu đang chạy trên Windows
#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>
#else
    #include <sys/ioctl.h>
    #include <unistd.h>
#endif


using namespace std ;

int TotalWidth = 0;
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

// ╔ ═ ╗ ║ ╠ ╚ ╝ ▶ ■
int GetTerminalWidth();
int visualLength(const string& s);
void PrintBox(string text,bool ABOVE , bool UNDER );
void MainScreen();

int main(){
    // Chỉ ép kiểu UTF-8 nếu đang chạy trên Windows
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    #endif

    TotalWidth = GetTerminalWidth();
    MainScreen();
    cout << "Nhan Enter de tiep tuc...";
    cin.get();    // Đợi nhấn Enter
    return 0;
}

int GetTerminalWidth() {
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

void PrintBox(string text, bool ABOVE = true , bool UNDER = true){
    
    //Tính khoảng trắng lề trái để đẩy toàn bộ khung ra giữa màn hình
    int marginLeft = (TotalWidth - boxWidth) / 2;
    // -|---|----
    string margin(marginLeft, ' '); // Tạo chuỗi khoảng trắng đẩy lề
    
    int innerWidth = boxWidth - 2; // Khoảng trống bên trong khung
    
    string line = "";
    for(int i = 0; i < innerWidth; i++){ 
        line += "═";
    }
    
    // Chèn khoảng trắng lề trái vào trước viền trên và dưới
    string Above , Under ;
    ABOVE ? Above = margin + "╔" + line + "╗" : Above = margin + "║" + string(innerWidth,' ') + "║";
    UNDER ? Under = margin + "╚" + line + "╝" : Under = margin + "║" + string(innerWidth,' ') + "║";
    
    // Tính toán để căn giữa phần chữ bên trong khung

    int textLen = visualLength(text); 
    int padLeft = (innerWidth - textLen) / 2;
    int padRight = innerWidth - textLen - padLeft; 
    
    cout << Above << endl;

    // Nhớ chèn lề trái (margin) vào trước bức tường bên trái "║"
    cout << margin << "║" << string(padLeft, ' ') << text << string(padRight, ' ') << "║" << endl;
    
    cout << Under << endl;
}

void MainScreen(){
    int selectedIdx = 0;
    int numOptions = 8;
    string options[] = {"1. Quan Li May Bay", 
                        "2. Quan Li Chuyen Bay", 
                        "3. Dat Ve", 
                        "4. Huy Ve", 
                        "5. In Danh Sach Hanh Khach", 
                        "6. Tra Cuu Chuyen Bay",
                        "7. In Danh Sach Ve Trong", 
                        "8. Thong Ke Luot Bay"};

    while(true){
        system("cls"); // Xóa màn hình

        PrintBox("QUAN LI CHUYEN BAY NOI DIA", true , false);
        
        // Vẽ nắp trên của khung Menu
        int boxText = 60;
        int marginLeft = (TotalWidth - boxText) / 2;
        string margin(marginLeft, ' '); // Tạo chuỗi khoảng trắng đẩy lề
        int innerWidth = boxText - 2;
        string horizontalLine = "";
        for(int i = 0; i < innerWidth; i++) horizontalLine += "═";
        

        // In từng mục menu theo hàng dọc
        for (int i = 0; i < numOptions; i++) {
            // Nếu là mục đang chọn thì đổi sang màu BOLDYELLOW, ngược lại để màu thường
            cout << margin << "║" << string(innerWidth, ' ') << "║" << endl;
            string s = (selectedIdx == i) ? string(BOLDYELLOW) + "▶ " + options[i] + " ◀" + string(RESET) : "  " + options[i];
            
            int visLen = visualLength(s);
            
            // Căn lề trái thụt vào để nguyên khối menu nằm ở giữa màn hình
            // (Giả sử khối menu rộng trung bình 30 ký tự)
            int padLeft = (innerWidth - 30) / 2; 
            int padRight = innerWidth - padLeft - visLen;
            
            cout << margin << "║" << string(5, ' ') << s << string(padRight + padLeft - 5, ' ') << "║" << endl;

        }
        
        cout << margin << "╚" << horizontalLine << "╝" << endl;

        // Bắt sự kiện phím bấm
        int key = _getch(); 
        
        if (key == 224) { // Phím điều hướng
            key = _getch(); 
            switch(key) {
                case 72: // Mũi tên Lên
                    if (selectedIdx > 0) selectedIdx--;
                    else selectedIdx = numOptions - 1; // Nhảy vòng xuống cuối
                    break;
                case 80: // Mũi tên Xuống
                    if (selectedIdx < numOptions - 1) selectedIdx++;
                    else selectedIdx = 0; // Nhảy vòng lên đầu
                    break;
            }
        } 
        else if (key == 13) { // Phím Enter
            system("cls");
            // Tạm thời in ra xem chọn đúng mục chưa
            cout << "Ban da chon: " << BOLDGREEN << options[selectedIdx] << RESET << endl;
            cout << "\nNhan phim bat ky de quay lai menu...";
            _getch();
        }
        else if (key == 27) { // Phím ESC để thoát
            break; 
        }
    }
}