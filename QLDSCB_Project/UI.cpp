#include <iostream>
#include <string>
#include <sstream>
#include <functional>
#include <cstring>
#include "Logic.h"
#include "Extentions.h"
// Chỉ thêm windows.h nếu đang chạy trên Windows
#ifndef _WIN32
static int currentCursorX = 0;
static int currentCursorY = 0;
#endif

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
    "Quay Lại Main Menu"
};

string subOptions3[5] = {
    "Đặt Vé",
    "Huỷ Vé",
    "DS Vé Trống",
    "DS Hành Khách",
    "Quay Lại"
};

string subOptions2[4] = {
    "Máy Bay Của Chuyến Bay",
    "Thống Kê Lượt Bay",
    "In Danh Sách Hành Khách",
    "Quay Lại Main Menu"
};

string subOptions4[5] = {
    "Thêm",
    "Sửa",
    "Xoá",
    "In Danh Sách",
    "Quay Lại"
};

string subOptions5[5] = {
    "Thêm",
    "Hiệu Chỉnh",
    "Tìm Theo Ngày/SB",
    "In Danh Sách",
    "Quay Lại"
};
static listMB* pDsMB = nullptr;
static listCB* pDsCB = nullptr;
static listHK* pDsHK = nullptr;
static listSBD* pDsSBD = nullptr;

#define dsMB (*pDsMB)
#define dsCB (*pDsCB)
#define dsHK (*pDsHK)
#define dsSBD (*pDsSBD)

void TruyenThamSo(listMB &A, listCB& B , listHK &C, listSBD& D){
    pDsMB = &A ;
    pDsCB = &B ;
    pDsHK = &C ; 
    pDsSBD = &D;
}

string* Menus[] = {
        subOptions1,
        subOptions2,
        subOptions3,
        subOptions4,
        subOptions5
};

int MenusSize[] = {
    sizeof(subOptions1)/sizeof(string),
    sizeof(subOptions2)/sizeof(string),
    sizeof(subOptions3)/sizeof(string),
    sizeof(subOptions4)/sizeof(string),
    sizeof(subOptions5)/sizeof(string)
};

int TotalWidth = 0;

NavKey GetNavKey(int &ch) {
    ch = 0;
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
int GetTerminalHeight() {
    #ifdef _WIN32
        // Code đo chiều cao dành cho Windows
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    #else
        // Code đo chiều cao dành cho Mac / Linux
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        return w.ws_row;
    #endif
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
    system("clear");
    currentCursorX = 0;
    currentCursorY = 0;
#endif
}
void Gotoxy(int x, int y) {
#ifdef _WIN32
        HANDLE hConsoleOutput;
        COORD Cursor_an_Pos = { (SHORT)x, (SHORT)y };
        hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleCursorPosition(hConsoleOutput, Cursor_an_Pos);
#else
        currentCursorX = x; // Lưu lại vết con trỏ
        currentCursorY = y;
        printf("\033[%d;%dH", y + 1, x + 1); 
        fflush(stdout);     // Ép terminal nhả bộ đệm và vẽ ngay lập tức
#endif
}


// Lấy tọa độ X hiện tại của con trỏ
int whereX() {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.dwCursorPosition.X;
#else
    return currentCursorX; 
#endif
}

// Lấy tọa độ Y hiện tại của con trỏ
int whereY() {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.dwCursorPosition.Y;
#else
    return currentCursorY;
#endif
}

void DrawACat(int x  ,int y){
    Gotoxy(x, y);
    cout << " /\\_/\\";
    Gotoxy(x, y+1);
    cout <<"( o.o )";
    Gotoxy(x, y+2);
    cout << " > ^ <";
}
void UiDrawCalendar(int x , int y,int rowpadding , int colpadding,string DateString){
    if (rowpadding < 4) rowpadding = 4;

    int year = GetYearFromStr(DateString);
    int month = GetMonthFromStr(DateString);
    int SelectedDay = GetDayFromStr(DateString);
    int hours = GetHourFromStr(DateString);
    int minute = GetMinuteFromStr(DateString);
    int Days_In_Month = daysInMonth(month, year);
    // Lấy mốc thời gian thực tế ngoài đời để tô màu chuẩn xác
    UiDrawCalendarWithoutTime(x , y,rowpadding , colpadding,DateString);
    
    // --- BẮT ĐẦU VẼ KHUNG GIỜ/PHÚT CO GIÃN ĐỘNG ---
    int calWidth = rowpadding * 7; 
    int timeY = y + colpadding * 6 + 7;
    
    // Bóp nhỏ cụm Giờ/Phút lại (Tổng rộng khoảng 28 ký tự)
    int timeStartX = x ; 

    Gotoxy(timeStartX, timeY); cout << "Giờ" ;
    Gotoxy(timeStartX + 4, timeY - 1); SmallBox(string(3,' ') + AD, 8, 1); // Rộng 8
    
    Gotoxy(timeStartX + 14, timeY); cout << "Phút" ;
    Gotoxy(timeStartX + 19, timeY - 1); SmallBox(string(3,' ') + AD, 8, 1); // Rộng 8 

    int H = hours; 
    int M = ((minute + 14) / 15) * 15; 
    
    // Nếu phút nhảy lên 60 thì reset về 0 và tăng 1 giờ
    if (M == 60) {
        M = 0;
        H = (H + 1) % 24; // Nếu 23h thì vòng lại 0h
    }
    int timeFocus = 1; // 1: Đang chọn Giờ, 2: Đang chọn Phút

    int textY = timeY;
    // Dịch tọa độ in số (cách viền trái box 2 ký tự)
    int hourX = timeStartX + 4 + 2; 
    int minX = timeStartX + 19 + 2;
    
    // 1. In Giờ
    Gotoxy(hourX, textY);
    if (timeFocus == 1) cout << YELLOW; else cout << RESET;
    cout << (H < 10 ? "0" : "") << H << RESET;
    
    // 2. In Phút
    Gotoxy(minX, textY);
    if (timeFocus == 2) cout << YELLOW; else cout << RESET;
    cout << (M < 10 ? "0" : "") << M << RESET;
}

void UiDrawCalendarWithoutTime(int x , int y,int rowpadding , int colpadding,string DateString){
    if (rowpadding < 4) rowpadding = 4;
    
    string label[] = {"Sun","Mon","Tue","Wed","Thur","Fri","Sat"};
    int size = 7 ;
    int lens = visualLength(DateString);
    Gotoxy(x + (rowpadding*7)/2 - lens/2 , y);
    cout << DateString ; 
    int year = GetYearFromStr(DateString);
    int month = GetMonthFromStr(DateString);
    int SelectedDay = GetDayFromStr(DateString);
    int hours = GetHourFromStr(DateString);
    int minute = GetMinuteFromStr(DateString);
    int Days_In_Month = daysInMonth(month, year);
    // Lấy mốc thời gian thực tế ngoài đời để tô màu chuẩn xác
    string nowStr = time_now();
    int realMonth = GetMonthFromStr(nowStr);
    int realYear = GetYearFromStr(nowStr);
    int realToday = GetDayFromStr(nowStr);
    // dd/mtmt/yyyy hh:mm
    DateTime a ;
    a.set_dd(1);
    a.set_mt(month);
    a.set_yy(year);
    a.set_hh(0);
    a.set_mm(0);
    int FirstDayOfMonth = Get_Weekday(a);
    for(int i = 0 ; i < 7; i++){
        Gotoxy(x + rowpadding * i, y + 2);
        cout << label[i];
    }
    // VẼ NGÀY TĨNH
    for(int Day = 1 ; Day <= Days_In_Month; Day++){
        int cx = x + rowpadding * ((FirstDayOfMonth + Day - 1) % 7);
        int cy = y + ((FirstDayOfMonth + Day - 1) / 7) * colpadding + 5;
        
        Gotoxy(cx, cy);
        
        // Chỉ tô xám quá khứ hoặc xanh hôm nay nếu đang vẽ tháng hiện tại
        if (month == realMonth && year == realYear) {
            if (Day < realToday) cout << GRAY;
            else if (Day == realToday) cout << GREEN;
        }
        
        // Vẽ ngoặc cho ngày đang được chọn (SelectedDay)
        if (Day == SelectedDay){
            Gotoxy(cx - 3, cy); cout << "[";
            Gotoxy(cx + 3, cy); cout << "]";
            Gotoxy(cx, cy);
        }
        
        cout << Day << RESET;
    }    
}

NavKey DrawCalendar(int x , int y,int rowpadding , int colpadding,string& DateString,bool CanPast){
    if (rowpadding < 4) rowpadding = 4;
    int lens = visualLength(DateString);
    Gotoxy(x + (rowpadding*7)/2 - lens/2 , y);
    cout << DateString ; 
    int year = GetYearFromStr(DateString);
    int month = GetMonthFromStr(DateString);
    int SelectedDay = GetDayFromStr(DateString);
    int hours = GetHourFromStr(DateString);
    int minute = GetMinuteFromStr(DateString);
    int Days_In_Month = daysInMonth(month, year);
   
    // --- VẼ KHUNG GIỜ/PHÚT CO GIÃN ĐỘNG ---
    int calWidth = rowpadding * 7; 
    int timeY = y + colpadding * 6 + 7;
    
    // Bóp nhỏ cụm Giờ/Phút lại (Tổng rộng khoảng 28 ký tự)
    int timeStartX = x ; 

    int H = hours; 
    int M = ((minute + 14) / 15) * 15; 

    if (M == 60) {
        M = 0;
        H = (H + 1) % 24;
    }
    
    int timeFocus = 1; // 1: Đang chọn Giờ, 2: Đang chọn Phút

    int textY = timeY;
    // Dịch tọa độ in số (cách viền trái box 2 ký tự)
    int hourX = timeStartX + 4 + 2; 
    int minX = timeStartX + 19 + 2;
    
    UiDrawCalendar(x, y, rowpadding, colpadding, DateString);
    
    // Nếu đang ở tháng hiện tại, mốc nhỏ nhất là realToday. Nếu là tháng tương lai, mốc nhỏ nhất là 1.
 
    while (true){
        NavKey kt = DrawCalendarWithoutTime( x , y, rowpadding , colpadding, DateString ,CanPast);

        if(kt == NAV_RIGHT || kt == NAV_LEFT || kt == NAV_ESC || kt == NAV_BACK) return kt;

        DateTime ChossenDay = StringtoTime(DateString);
        int D = ChossenDay.get_dd() ;
        while(true){
            // In Giờ
            Gotoxy(hourX, textY);
            if (timeFocus == 1) cout << YELLOW; else cout << RESET;
            cout << (H < 10 ? "0" : "") << H << RESET;
            
            // In Phút
            Gotoxy(minX, textY);
            if (timeFocus == 2) cout << YELLOW; else cout << RESET;
            cout << (M < 10 ? "0" : "") << M << RESET;

            int ch;
            NavKey key = GetNavKey(ch);
            
            if (key == NAV_ESC) {
                DateString = "";
                return NAV_ESC;
            }
            else if (key == NAV_ENTER) {
                ChossenDay.set_hh(H);
                ChossenDay.set_mm(M);
                DateString = ToStringDate(ChossenDay);
                return NAV_ENTER;
            }
            
            // Quay về Lịch (phím W)
            else if (key == NAV_UNKNOWN && tolower(ch) == 'w') {
                // Trả lại màu sáng bình thường cho Giờ/Phút trước khi thoát lên trên
                Gotoxy(hourX, textY); cout << (H < 10 ? "0" : "") << H;
                Gotoxy(minX, textY); cout << (M < 10 ? "0" : "") << M;
                ChossenDay.set_hh(H);
                ChossenDay.set_mm(M);
                DateString = ToStringDate(ChossenDay);
                break; // Break vòng lặp này -> Vòng lặp tổng sẽ quay ngược lên vòng chọn Ngày
            }
            
            // Đảo Focus giữa Giờ và Phút
            else if (key == NAV_LEFT || key == NAV_RIGHT) {
                timeFocus = (timeFocus == 1) ? 2 : 1;
            }
            else if (key == NAV_UNKNOWN && tolower(ch) == 'h') timeFocus = 1;
            else if (key == NAV_UNKNOWN && tolower(ch) == 'm') timeFocus = 2;
            
            else if (key == NAV_UP) {
                if (timeFocus == 1) H = (H + 1) % 24;
                else M = (M + 15) % 60;
            }
            else if (key == NAV_DOWN) {
                if (timeFocus == 1) H = (H - 1 + 24) % 24;
                else M = (M - 15 + 60) % 60;
            }
        }
    }
}

NavKey DrawCalendarWithoutTime(int x , int y,int rowpadding , int colpadding,string& DateString,bool CanPast){
    if (rowpadding < 4) rowpadding = 4;
    string label[] = {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
    int size = 7 ;
    int lens = visualLength(DateString);
    Gotoxy(x + (rowpadding*7)/2 - lens/2 , y);
    cout << DateString ; 
    int year = GetYearFromStr(DateString);
    int month = GetMonthFromStr(DateString);
    int SelectedDay = GetDayFromStr(DateString);
    int hours = GetHourFromStr(DateString);
    int minute = GetMinuteFromStr(DateString);
    int Days_In_Month = daysInMonth(month, year);
    // Lấy mốc thời gian thực tế ngoài đời để tô màu chuẩn xác
    string nowStr = time_now();
    int realMonth = GetMonthFromStr(nowStr);
    int realYear = GetYearFromStr(nowStr);
    int realToday = GetDayFromStr(nowStr);
   
    // dd/mtmt/yyyy hh:mm
    DateTime a ;
    a.set_dd(1);
    a.set_mt(month);
    a.set_yy(year);
    a.set_hh(0);
    a.set_mm(0);
    int FirstDayOfMonth = Get_Weekday(a);
    
    UiDrawCalendarWithoutTime(x , y,rowpadding , colpadding,DateString);
    
    int H = hours; 
    int M = ((minute + 14) / 15) * 15; 
    
    // Nếu phút nhảy lên 60 thì reset về 0 và tăng 1 giờ
    if (M == 60) {
        M = 0;
        H = (H + 1) % 24; // Nếu 23h thì vòng lại 0h
    }
    int timeFocus = 1; // 1: Đang chọn Giờ, 2: Đang chọn Phút
    
    DateTime ChossenDay = StringtoTime(DateString);
    
    // Nếu đang ở tháng hiện tại, mốc nhỏ nhất là realToday. Nếu là tháng tương lai, mốc nhỏ nhất là 1.
    int D = ChossenDay.get_dd() ;

    int minDay = (!CanPast && month == realMonth && year == realYear) ? realToday : 1;
    
    int initial_cx = x + rowpadding * ((FirstDayOfMonth + D - 1) % 7);
    int initial_cy = y + ((FirstDayOfMonth + D - 1) / 7) * colpadding + 5;

    Gotoxy(initial_cx - 3, initial_cy); cout << "[";
    Gotoxy(initial_cx + 3, initial_cy); cout << "]";
    
    while(true){
        int ch ;
        NavKey key = GetNavKey(ch);
        int old_cx = x + rowpadding * ((FirstDayOfMonth + D - 1) % 7);
        int old_cy = y + ((FirstDayOfMonth + D - 1) / 7) * colpadding + 5;
        
        Gotoxy(old_cx - 3, old_cy); cout << " "; 
        Gotoxy(old_cx + 3, old_cy); cout << " ";
        Gotoxy(old_cx, old_cy);
        

        if (month == realMonth && year == realYear) {
            if (D < realToday) cout << GRAY << D << RESET; 
            else if (D == realToday) cout << GREEN << D << RESET;
            else cout << D;
        }else if (year < realYear || (year == realYear && month < realMonth)) {
            cout << GRAY << D << RESET; // Thêm dòng này
        }else {
            cout << D; // Tháng tương lai thì in màu trắng bình thường
        }

        if (key == NAV_BACK) {
            DateString = "";
            return NAV_BACK;
        }
        else if (key == NAV_RIGHT) {
            if (D < Days_In_Month) D++;
        } 
        else if (key == NAV_LEFT) {
            if (D > 1) {
                D = max(minDay, D - 1); 
            }
        }
        else if (key == NAV_DOWN) {
            if (D + 7 <= Days_In_Month) D += 7;
            else {
                ChossenDay.set_dd(D); 
                Gotoxy(old_cx - 3, old_cy); cout << "["; 
                Gotoxy(old_cx + 3, old_cy); cout << "]";
                Gotoxy(old_cx, old_cy);
                cout << CYAN << D << RESET;
                DateString = ToStringDate(ChossenDay);
                break;
            }            
        }
        else if (key == NAV_UP) {
            // Đã fix: Dùng minDay để chặn nhảy lên tuần quá khứ
            if ((D - 7) >= minDay) D -= 7;
        }
        else if (key == NAV_ENTER){
            ChossenDay.set_dd(D);
            ChossenDay.set_hh(H);
            ChossenDay.set_mm(M); 
            Gotoxy(old_cx - 3, old_cy); cout << "["; 
            Gotoxy(old_cx + 3, old_cy); cout << "]";
            Gotoxy(old_cx, old_cy);
            cout << CYAN << D << RESET;
            DateString = ToStringDate(ChossenDay);
            return NAV_ENTER;
        }
        else if (key == NAV_UNKNOWN && tolower(ch) == 'd') {
            ChossenDay.set_dd(Days_In_Month); 
            DateString = NextDay(ChossenDay); 
            return NAV_RIGHT; 
        }
        else if (key == NAV_UNKNOWN && tolower(ch) == 'a') {
            int current_d = ChossenDay.get_dd(); 
            int m = ChossenDay.get_mt() - 1;
            int y = ChossenDay.get_yy();
            
            if (m < 1) { m = 12; y--; }
            
            int maxDays = daysInMonth(m, y);
            if (current_d > maxDays) current_d = maxDays;
            
            DateTime temp = ChossenDay;
            temp.set_dd(current_d);
            temp.set_mt(m);
            temp.set_yy(y);
            
            string prevMonthStr = ToStringDate(temp); 
            
            // Kiểm tra xem ngày tính ra có bị rơi vào quá khứ không?
            if (!CanPast && IsGreaterTime(time_now(), prevMonthStr) == 1) { 
                // Nếu lùi về TRÚNG tháng hiện tại ngoài đời -> Bẻ nó về đúng ngày hôm nay
                if (m == realMonth && y == realYear) {
                    temp.set_dd(realToday);
                    DateString = ToStringDate(temp);
                    return NAV_LEFT;
                }
                // Còn nếu lùi về hụt qua hẳn tháng trước (ví dụ tháng 4) -> Bị chặn, không làm gì cả
            } else {
                // Tương lai hợp lệ -> Nhảy lịch bình thường
                DateString = prevMonthStr;
                return NAV_LEFT;
            }
        }else if( key == NAV_ESC){
            DateString = ""; 
            return NAV_ESC;
        }
        
        int new_cx = x + rowpadding * ((FirstDayOfMonth + D - 1) % 7);
        int new_cy = y + ((FirstDayOfMonth + D - 1) / 7) * colpadding + 5;
        
        Gotoxy(new_cx - 3, new_cy); cout << "[";
        Gotoxy(new_cx + 3, new_cy); cout << "]";
    }
    return NAV_UNKNOWN;   
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

// Hàm bổ trợ để vẽ lại nội dung nhập liệu (hỗ trợ căn giữa)
void RedrawInput(string result, int x, int y, int maxLength, char placeholder, bool isCenter,bool isHighlight,string SugestionStr) {
    // 1. Xóa sạch vùng nhập liệu cũ bằng placeholder
    Gotoxy(x, y);
    if (isHighlight) cout << HIGHLIGHT; // Bật màu nền nếu cần
    cout << string(maxLength, placeholder);
    if (isHighlight) cout << RESET;

    // 2. Tính toán vị trí x bắt đầu in dựa trên visualLength 
    int printX = x;
    if (isCenter) {
        int vLen = visualLength(result + SugestionStr); 
        printX = x + (maxLength - vLen) / 2;
    }

    // 3. In kết quả thực tế lên màn hình
    Gotoxy(printX, y);
    if (isHighlight) cout << HIGHLIGHT;
    cout << result;
    if (SugestionStr != "") {
        cout << GRAY << SugestionStr << RESET;
        if (isHighlight) cout << HIGHLIGHT;
    }
    if (isHighlight) cout << RESET;
    Gotoxy(printX + visualLength(result), y);
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
int InputString(string &result, int x, int y, int maxLength, char placeholder, bool onlyNumbers, bool isCenter, bool isHighlight, bool isUppercase,bool isonlychar) {
    // Vẽ lần đầu tiên
    RedrawInput(result, x, y, maxLength, placeholder, isCenter, isHighlight);

    while (true) {
        int ch;
        NavKey key = GetNavKey(ch); 

        if (key != NAV_UNKNOWN && key != NAV_BACK) return key;

        if (key == NAV_BACK) {
            if (!result.empty()) {
                unsigned char c = (unsigned char)result.back();
                if (c >= 0x80) {
                    while (!result.empty() && ((unsigned char)result.back() & 0xc0) == 0x80) {
                        result.pop_back();
                    }
                }
                if (!result.empty()) result.pop_back();
                
                RedrawInput(result, x, y, maxLength, placeholder, isCenter, isHighlight);
            }
        } 
        else if (result.length() < maxLength) {
            if (result.empty() && ch == ' ') continue;
            if(!result.empty() && result.back() == ' ' && ch == ' ') continue;
            // Phân loại xử lý dựa trên bảng mã ASCII (0-127) và UTF-8 (>= 128)
            if ((unsigned char)ch < 128) {
                // VÙNG ASCII (Ký tự tiếng Anh, số, dấu)
                
                // Ràng buộc số và chữ
                if (onlyNumbers && !isdigit(ch)) continue; 
                if (isonlychar && isdigit(ch)) continue;
                
                if (ch != ' ' && ch != '-' && !isalnum(ch)) continue; 

                char finalChar = (isUppercase) ? (char)toupper(ch) : (char)ch;
                result += finalChar;

            } else {
                // UTF-8 (Byte của tiếng Việt có dấu, ký tự đặc biệt)
               
                if (onlyNumbers) continue; // Nếu chỉ nhập số thì chặn luôn các ký tự UTF-8
                
                result += (char)ch;
            }

            RedrawInput(result, x, y, maxLength, placeholder, isCenter, isHighlight);
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

int InputDateTime(string &result, int x, int y, int maxLength , char placeholder, bool isCenter, bool isHighlight, int errX, int errY) {
    string errMsg = "";
    RedrawInput(result, x, y, maxLength, placeholder, isCenter, isHighlight);
    if(errX != -1 && errY != -1)ClearRegion(errX, errY, 60, 1);

    while (true) {
        int ch;
        NavKey key = GetNavKey(ch); 
        errMsg = ""; 

        if (key == NAV_ENTER) {
            if (result.length() < 16) {
                errMsg = "Lỗi: Vui lòng nhập đầy đủ thời gian (dd/mm/yyyy hh:mm)!";
                if(errX != -1 && errY != -1)ClearRegion(errX, errY, 60, 1);
                Gotoxy(errX, errY);
                cout << errMsg;
                RedrawInput(result, x, y, maxLength, placeholder, isCenter, isHighlight);
                continue; 
            }
             if(errX != -1 && errY != -1)ClearRegion(errX, errY, 60, 1);
            return key;
        }

        if (key != NAV_UNKNOWN && key != NAV_BACK) {
             if(errX != -1 && errY != -1)ClearRegion(errX, errY, 60, 1);
            return key;
        }

        if (key == NAV_BACK) { 
            if (!result.empty()) {
                char last = result.back();
                if (last == '/' || last == ' ' || last == ':') {
                    result.pop_back();
                }
                if (!result.empty()) {
                    result.pop_back();
                }
                if(errX != -1 && errY != -1)ClearRegion(errX, errY, 60, 1); 
                RedrawInput(result, x, y, maxLength, placeholder, isCenter, isHighlight);
            }
            continue;
        } 
        
        if (result.length() < 16) {
            if (!isdigit(ch)) continue; 

            int len = result.length();
            
            // Cắt chuỗi lấy thời gian hiện tại
            string now_str = time_now();
            int curDay   = GetDayFromStr(now_str);
            int curMonth = GetMonthFromStr(now_str);
            int curYear  = GetYearFromStr(now_str);
            int curHour  = GetHourFromStr(now_str);
            int curMin   = GetMinuteFromStr(now_str);

            // 1. Kiểm tra Ngày
            if (len == 0 && (ch < '0' || ch > '3')) { errMsg = "Lỗi: Ngày phải bắt đầu từ 0 đến 3"; }
            if (len == 1) {
                if (result[0] == '3' && (ch < '0' || ch > '1')) { errMsg = "Lỗi: Ngày không hợp lệ!"; }
                if (result[0] == '0' && ch == '0') { errMsg = "Lỗi: Ngày không thể là 00!"; }
            }

            // 2. Kiểm tra Tháng
            if (len == 3 && (ch < '0' || ch > '1')) { errMsg = "Lỗi: Tháng phải bắt đầu từ 0 hoặc 1!"; }
            if (len == 4) {
                if (result[3] == '1' && (ch < '0' || ch > '2')) { errMsg = "LỗiP: Tháng không hợp lệ!"; }
                if (result[3] == '0' && ch == '0') { errMsg = "Lỗi: Tháng không thể là 00!"; }

                int d = (result[0] - '0') * 10 + (result[1] - '0');
                int m = (result[3] - '0') * 10 + (ch - '0');
                if (d == 31 && (m == 2 || m == 4 || m == 6 || m == 9 || m == 11)) { errMsg = "Loi: Thang nay khong co ngay 31!"; }
                if (d >= 30 && m == 2) { errMsg = "Lỗi: Tháng 2 không có ngày 30, 31!"; }
            }

            // 3. Kiểm tra Năm (Bỏ giới hạn 1900-2100, chỉ chặn năm quá khứ)
            if (len >= 6 && len <= 9) {
                int y_prefix = 0;
                int cur_prefix = 0;
                
                // So sánh từng chữ số được gõ với tiền tố của năm hiện tại
                if (len == 6) {
                    y_prefix = ch - '0';
                    cur_prefix = curYear / 1000;
                } else if (len == 7) {
                    y_prefix = (result[6] - '0') * 10 + (ch - '0');
                    cur_prefix = curYear / 100;
                } else if (len == 8) {
                    y_prefix = (result[6] - '0') * 100 + (result[7] - '0') * 10 + (ch - '0');
                    cur_prefix = curYear / 10;
                } else if (len == 9) {
                    y_prefix = (result[6] - '0') * 1000 + (result[7] - '0') * 100 + (result[8] - '0') * 10 + (ch - '0');
                    cur_prefix = curYear;

                    // Nếu gõ xong năm thì tiện thể kiểm tra luôn năm nhuận và ngày/tháng quá khứ
                    int d = (result[0] - '0') * 10 + (result[1] - '0');
                    int m = (result[3] - '0') * 10 + (result[4] - '0');
                    
                    if (d == 29 && m == 2) {
                        bool isLeap = (y_prefix % 400 == 0) || (y_prefix % 4 == 0 && y_prefix % 100 != 0);
                        if (!isLeap) { errMsg = "Lỗi: Năm bạn nhập không phải là năm nhuận!"; }
                    }

                    if (y_prefix == curYear && (m < curMonth || (m == curMonth && d < curDay))) {
                        errMsg = "Lỗii: Không được nhập ngày tháng trong quá khứ!";
                    }
                }

                // Nếu bất kỳ lúc nào tiền tố đang gõ nhỏ hơn tiền tố năm nay -> Chắc chắn là năm quá khứ
                if (y_prefix < cur_prefix) {
                    errMsg = "Lỗi: Năm không được trong quá khứ!";
                }
            }

            // 4. Kiểm tra Giờ & Quá khứ
            if (len == 11 && (ch < '0' || ch > '2')) { errMsg = "Lỗi: Giờ phải bắt đầu từ 0 đến 2!"; }
            if (len == 12) {
                if (result[11] == '2' && (ch < '0' || ch > '3')) { errMsg = "Lỗi: Giờ tối đa là 23!"; }

                int d = (result[0] - '0') * 10 + (result[1] - '0');
                int m = (result[3] - '0') * 10 + (result[4] - '0');
                int y_input = stoi(result.substr(6, 4));
                int h = (result[11] - '0') * 10 + (ch - '0');

                if (y_input == curYear && m == curMonth && d == curDay && h < curHour) {
                    errMsg = "Lỗi: Giờ này đã qua rồi, vui lòng nhập giờ khác!";
                }
            }

            // 5. Kiểm tra Phút & Quá khứ
            if (len == 14) {
                if (ch != '0' && ch != '1' && ch != '3' && ch != '4') { 
                    errMsg = "Lỗi: Phút chỉ được là 00, 15, 30 hoặc 45!"; 
                }
            }
            if (len == 15) {
                if ((result[14] == '0' && ch != '0') || 
                    (result[14] == '1' && ch != '5') || 
                    (result[14] == '3' && ch != '0') || 
                    (result[14] == '4' && ch != '5')) {
                    errMsg = "Lỗi: Phút chỉ được là 00, 15, 30 hoặc 45!";
                } else {
                    int d = (result[0] - '0') * 10 + (result[1] - '0');
                    int m = (result[3] - '0') * 10 + (result[4] - '0');
                    int y_input = stoi(result.substr(6, 4));
                    int h = stoi(result.substr(11, 2));
                    int minVal = (result[14] - '0') * 10 + (ch - '0');

                    if (y_input == curYear && m == curMonth && d == curDay && h == curHour && minVal < curMin) {
                        errMsg = "Lỗi: Thời gian này đã trôi qua rồi!";
                    }
                }
            }

            // Xử lý hiển thị hoặc nạp ký tự
            if (!errMsg.empty() && errX != -1 && errY != -1) {
                ClearRegion(errX, errY, 60, 1);
                Gotoxy(errX, errY);
                cout << RED << errMsg << RESET;
                RedrawInput(result, x, y, maxLength, placeholder, isCenter, isHighlight);
                continue; 
            }

             if(errX != -1 && errY != -1)ClearRegion(errX, errY, 60, 1); 
            
            result += (char)ch;

            len = result.length(); 
            if ((len == 2 || len == 5) && len < maxLength) {
                result += '/';
            } else if (len == 10 && len < maxLength) {
                result += ' ';
            } else if (len == 13 && len < maxLength) {
                result += ':';
            }

            RedrawInput(result, x, y, maxLength, placeholder, isCenter, isHighlight);
        }
    }
}

int InputStringSBD(string &result, int x, int y, int maxLength, char placeholder, bool isCenter, bool isHighlight) {
    // Vẽ lần đầu tiên
    bool isonlychar = true ;
    bool isUppercase = false ;
    string u = "";
    RedrawInput(result, x, y, maxLength, placeholder, isCenter, isHighlight);

    while (true) {
        int ch;
        NavKey key = GetNavKey(ch); 
        bool isChanged = false;
        if(key == NAV_ENTER){
            if(result.empty()) return key;
            listSBD tmp = Find_SBD_OnRage(dsSBD, const_cast<char*>(result.c_str()));
            result = tmp.list[0].sbDich;
            return key ;
        }
        if (key != NAV_UNKNOWN && key != NAV_BACK) return key;

        if (key == NAV_BACK) {
            if (!result.empty()) {
                unsigned char c = (unsigned char)result.back();
                if (c >= 0x80) {
                    while (!result.empty() && ((unsigned char)result.back() & 0xc0) == 0x80) {
                        result.pop_back();
                    }
                }
                if (!result.empty()) result.pop_back();
                
               isChanged = true;
            }
        } 
        else if (result.length() < maxLength) {
            if (result.empty() && ch == ' ') continue;
            // Phân loại xử lý dựa trên bảng mã ASCII (0-127) và UTF-8 (>= 128)
            if ((unsigned char)ch < 128) {
                // VÙNG ASCII (Ký tự tiếng Anh, số, dấu)
                
                // Ràng buộc số và chữ
                if (isonlychar && isdigit(ch)) continue;
                
                if (ch != ' ' && ch != '-' && !isalnum(ch)) continue; 

                char finalChar = (isUppercase) ? (char)toupper(ch) : (char)ch;
                result += finalChar;

            } else {
                // UTF-8 (Byte của tiếng Việt có dấu, ký tự đặc biệt)
                result += (char)ch;
            }
            isChanged = true;
           
        }
        if (isChanged) {
            if (result.empty()) {
                u = "";
            } else {
                listSBD tmp = Find_SBD_OnRage(dsSBD, const_cast<char*>(result.c_str()));
                
                if (tmp.slSBD > 0) {
                    string match = tmp.list[0].sbDich;
                    if (match.length() >= result.length()) {
                        u = match.substr(result.length());
                    } else {
                        u = "";
                    }
                } else {
                    u = "";
                }
            }
        }
        RedrawInput(result, x, y, maxLength, placeholder, isCenter, isHighlight,u);
       
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

// Hàm phục vụ MB trở xuống

NavKey CustomerAddMB() {
    int width = 80;
    int startX = (GetTerminalWidth() - width) / 2;
    int formY = 6;

    if (dsMB.slMB >= slMB_max) {
        ClearScreen();
        Gotoxy(startX, 10);
        cout << RED << "Lỗi: Danh sách máy bay đã đầy! Không thể thêm." << RESET;
        _getch();
        return NAV_ESC; 
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
            action = InputString(inputs[0], inputX, formY + 2, soHieuMB_max, '_', false, false, false, true, false);
        } else if (currentField == 1) {
            action = InputString(inputs[1], inputX, formY + 6, loaiMB_max, '_', false, false, false, false, false);
        } else if (currentField == 2) {
            action = InputString(inputs[2], inputX, formY + 10, 3, '_', true, false, false, false, false);
        }

        if (action == NAV_ESC){
            ClearScreen();
            return NAV_ESC; 
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
               
                if (Find_MB(dsMB, const_cast<char*>(inputs[0].c_str())) != -1) { 
                    Gotoxy(startX + 15, formY + 12);
                    cout << RED << "Lỗi: Số hiệu [" << inputs[0] << "] đã tồn tại! Bấm phím bất kỳ..." << RESET;
                    _getch(); 
                    
                    Gotoxy(startX, formY + 15);
                    SmallBox("ENTER/DOWN: Tiếp tục - UP: Lên trên - ESC: Hủy", (int)width, (int)3, (string)WHITE);
                    currentField = 0; 
                    continue;
                }
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
                }else{
                    // --- TIẾN HÀNH LƯU ---
                    Gotoxy(startX, formY + 15);
                    SmallBox("ĐANG LƯU DỮ LIỆU...", (int)width, (int)3, (string)GREEN);

                    Gotoxy(startX + 12, formY + 18);
                    MB* NewMB = new MB; 
                    strcpy(NewMB->soHieuMB, inputs[0].c_str());
                    strcpy(NewMB->loaiMB, inputs[1].c_str());
                    NewMB->socho = soCho;
                    Add_MB(dsMB, NewMB);
                    cout << YELLOW << "Đã lưu thành công! Nhấn phím bất kỳ để tiếp tục, ESC để thoát." << RESET;
                  
                    int key = GetNavKey();
                    ClearScreen();
                    if(key == NAV_ESC) return NAV_ESC;
                    else return NAV_ENTER;  
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

void UiThongTinMB(int x,int y , int width, int height,string s){
    int formY = y ;
    int startX = x ;
    ClearRegion(startX + width+1, formY+4, width/2-2, height-4);
    Gotoxy(startX + width+2, formY+4);
    int mbIndex = Find_MB(dsMB, const_cast<char*>(s.c_str()));
    cout << "Số hiệu: " << dsMB.list[mbIndex]->soHieuMB;
    Gotoxy(startX + width+2, formY+6);
    cout << "Loại máy bay: " << dsMB.list[mbIndex]->loaiMB;
    Gotoxy(startX + width+2, formY+8);
    cout << "Số chỗ: " << dsMB.list[mbIndex]->socho;
}

int SmallListShMB( string& res, int &index, int width , int x, int y, int items, listMB& A,int X, int Y, int w , int h){
    if (A.slMB == 0) return NAV_ESC;
    items = min(items, A.slMB);
    string s ;
    int here = 0 ;
    int pos_op = 0 ;
    while(true){
        for(int i = 0 ; i < items ;i++){
            Gotoxy(x+1,y + (i+1)*2+1);
            if(i + here < A.slMB){
                s = string(A.list[i+here]->soHieuMB);
                if (i == pos_op) s = HIGHLIGHT + s + RESET;
                int len = visualLength(s);
                int padding = max(0, width - len - 3);
                cout << s + string(padding, ' ');
            }
        }
        if(w != - 1 && h != -1) UiThongTinMB(X,Y,w,h,string(A.list[pos_op + here]->soHieuMB));
        int ch ;
        NavKey key = GetNavKey(ch);
        if(key == NAV_ESC) return NAV_ESC;
        if(key == NAV_DOWN){
            if (pos_op + here + 1 < A.slMB) { // Nếu chưa chạm đáy danh sách thật
                if (pos_op + 1 < items) {
                    pos_op++; // Thanh sáng dịch xuống
                } else {
                    here++;   // Đẩy danh sách xuống, thanh sáng giữ nguyên ở đáy
                }
            }
        }else if( key == NAV_UP){
           if (pos_op == 0) {
                if (here > 0) here--;
                else return NAV_UP;   
            } else {
                pos_op--; 
            }
        }else if(key == NAV_ENTER){
            index = pos_op + here; 
            
            res = string(A.list[index]->soHieuMB);
            return NAV_ENTER ; 
        }else if(key == NAV_UNKNOWN && char(ch) == 'f'){
            return NAV_UP;
        }
    }
    
}

void UiSmallListShMB(int width, int max_items, int x, int y, listMB& A){
    string s;
    if(A.slMB == 0) {
        Gotoxy(x+1, y + 3);
        int padding = max(0, width - 25);
        string u = "không tìm thấy máy bay nào!";
        cout << RED << u << string(padding, ' ') << RESET;
        ClearRegion(x+1, y + 4, width - 2, (max_items-1)*2); 
        return;
    }

    int renderCount = min(max_items, A.slMB);

    for(int i = 0 ; i < renderCount ; i++){
        Gotoxy(x+1, y + (i+1)*2+1);
        s = string(A.list[i]->soHieuMB);
        int len = visualLength(s);
        int padding = max(0, width - len - 3);
        cout << s + string(padding, ' ');
    }
    
    for(int i = renderCount; i < max_items; i++) {
        Gotoxy(x+1, y + (i+1)*2+1);
        int padding = max(0, width - 3);
        cout << string(padding, ' '); 
    }
}

// Hàm addCB trở xuống

NavKey CustomerAddCB() {
    int width = 80;
    int startX = 0;
    int formY = 1;

    ClearScreen();
    Gotoxy(startX, formY);
    SmallBox("", true, true, true, true, width, 22, WHITE); // Giảm chiều cao box xuống 15
    Gotoxy(startX+width, formY +15);
    SmallBox("THÔNG TIN MÁY BAY", (int)width/2, (int)3, (string)YELLOW);
    Gotoxy(startX + width, formY+20);
    SmallBox("", true, true, true, true, width/2 , 7 , WHITE); 

    string labels[] = {"Mã Chuyến bay:", "Ngày giờ khởi hành:", "Sân bay đến:", "Số hiệu máy bay:","Số vé khởi tạo:"};
    int labelX = startX + 5;
    int inputX = startX + 25;
    string gap(5, ' '); // Khoảng cách giữa 2 lựa chọn
    int size = 4 ;
    string o = "Hãy Chọn Máy Bay";
    // Đổi khoảng cách từ (i * 4) thành (i * 2)
    for (int i = 0; i < size; i++) {
       
        if(i != size - 1 && i != 1){
            Gotoxy(labelX, formY + 2 + (i * 3));
            cout << labels[i];
            Gotoxy(inputX, formY + 2 + (i * 3));
            cout << string(50, '_'); 
        }else if(i == 1){
            Gotoxy(labelX, formY + 3 + (i * 3)-1);
            cout << labels[i];
            Gotoxy(inputX, formY + 2 + (i * 3)-1);
            SmallBox(string(22,' ') + DM, 25,1);
        }
        else {
            Gotoxy(inputX + 5, formY + 3 + (i * 3));
            cout << labels[i];
            Gotoxy(inputX+25, formY + 2 + (i * 3));
            SmallBox(o + string(22-visualLength(o),' ') + AD, 25,1);
        }
    }
    Gotoxy(labelX,formY + 3 + (size - 1) * 3);
    cout << labels[4];
    string text = "ENTER/DOWN: Tiếp tục - UP: Lên trên - ESC: Hủy";
    Gotoxy(startX, formY + 24); // Đưa dòng hướng dẫn lên sát mép dưới form
    SmallBox(text, width , 3, (string)WHITE);
    // --- LOGIC NHẬP LIỆU VÀ ĐIỀU HƯỚNG ---
    string inputs[5] = {"", "", "","",""}; 
    inputs[1] = time_now(); 
    UiDrawCalendar(startX+width+3,1,5,1,inputs[1]);
    int currentField = 0; // 0: Mã_CB, 1: Ngày giờ khởi hành, 2: Sbd , 3: shMB, 4: số vé khơi tạo
    while (true) {
        Gotoxy(startX + 5, formY + 12);
        string s = "";
        int action;
        if (currentField == 0) {
            action = InputString(inputs[0], inputX, formY + 2, maCB_max, '_', false, false, false, true, false);
        } else if (currentField == 1) {
            string old = inputs[1];
            NavKey k = NAV_LEFT;
            bool dateError = false; //Kiểm tra lỗi ngày quá khứ
            
            // Vòng lặp do-while giữ Lịch không bị tắt khi đổi tháng HOẶC khi chọn sai ngày
            while (k == NAV_LEFT || k == NAV_RIGHT || dateError){
                ClearRegion(startX + width + 3, 1, TotalWidth - width-3, 13); // Xóa sạch không gian trước khi vẽ

                k = DrawCalendar(startX + width + 3, 1, 5, 1, inputs[1]);
                
                dateError = false; // Reset cờ mỗi lần vẽ lại lịch
                
                // Nếu người dùng nhấn ENTER để chốt ngày, ta tiến hành kiểm tra
                if (k == NAV_ENTER) {
                    // Nếu thời gian hiện tại LỚN HƠN thời gian vừa chọn -> Lỗi quá khứ
                    if (IsGreaterTime(time_now(), inputs[1]) == 1 || IsGreaterTime(NextDay(time_now()), inputs[1]) == 1) { 
                        Gotoxy(startX+1, formY + 20);
                        string u = "Lỗi: Thời gian khởi hành " + inputs[1] + " Phải lớn hơn ngày hiện tại ít nhất 24h! Bấm phím bất kỳ..." ;
                        SmallBox(u,false,true,false,false,width-2,3,RED);
                        _getch();
                        ClearRegion(startX+1, formY + 20, width-2, 3);
                        dateError = true; // Bật cờ lỗi để vòng lặp while quay lại mở Lịch
                    }
                }
            }
            if (k == NAV_ESC) {
                inputs[1] = old;      // Phục hồi ngày cũ
                action = NAV_DOWN; // Ép giữ con trỏ lại ô này
            } else {
                Gotoxy(inputX + 1, formY + 2 + (currentField * 3));
                cout << inputs[1] ;
                action = NAV_DOWN;    // Tự động nhảy xuống ô Sân bay đến
            }
        } else if (currentField == 2) {
            // Tham số TRUE cuối cùng sẽ chặn toàn bộ các ký tự không phải là số
            action = InputStringSBD(inputs[2], inputX, formY + 8, sbDich_max, '_', false); 
            RedrawInput(inputs[2], inputX, formY + 8, sbDich_max, '_', false, false, ""); 
        }else if (currentField == 3){
            Gotoxy(inputX+25, formY + 2 + (currentField * 3));
            SmallBox(string(22,' ') + AD, 25,1);
            int items = 5 ;
            listMB tmp ;
            tmp = Find_MB_OnRage(dsMB, const_cast<char*>(s.c_str()));
            UiSmallListShMB(15, items, inputX+26, formY + 2 + currentField*3, tmp);
            while(true){
                Gotoxy(inputX+26+s.size(), formY + 2 + currentField*3+1);
                int ch = 0 ;
                NavKey key = GetNavKey(ch);
                ClearRegion(inputX+24, formY + 2 + currentField*3+3, 30, items*2-1);
                if(key == NAV_UP || key  == NAV_RIGHT){
                    Gotoxy(inputX+25, formY + 2 + (currentField * 3));
                    SmallBox(o + string(22-visualLength(o),' ') + AD, 25,1);
                    ClearRegion(inputX+40, formY + 2 + currentField*3+3, 12, items*2-1);
                    action = key ;
                    break;
                }

                if(key == NAV_ESC){
                    return NAV_ESC  ;
                } 
                
                if(key == NAV_DOWN && tmp.slMB > 0) {
                    int idx = 0 ;
                    action = SmallListShMB(s,idx,15,inputX+26,formY + 2 + currentField*3,items,tmp,startX ,formY +18,width,7); // Hàm này hiển thị thông tin MB
                    if(action == NAV_ESC){
                        ClearRegion(inputX+1, formY + 2 + currentField*3+3, 30, items*2-1);
                        return NAV_ESC;
                    }else if(action == NAV_ENTER){
                        inputs[3] = s ;
                        inputs[4] = to_string(tmp.list[idx]->socho);
                        Gotoxy(inputX+25, formY + 2 + (currentField * 3));
                        SmallBox(o + string(22-visualLength(o),' ') + AD, 25,1);
                        RedrawInput(inputs[4],inputX, formY + 3 + (size - 1) * 3,3,' ',true,false)  ; 
                        ClearRegion(inputX+24, formY + 2 + currentField*3+3, 12, items*2-1);
                        break;
                    }
                }
                UiFindMB(s, key, ch, tmp, inputX+26, formY + 2 + currentField*3+1);
                UiSmallListShMB(15, items, inputX+26, formY + 2 + currentField*3, tmp);
            }
            
        }else if(currentField == 4){
            action = InputString(inputs[4], inputX, formY + 3 + (size - 1) * 3, 3, ' ', true, true, false, false, false); 
        }
        if(!inputs[3].empty()) {
           UiThongTinMB(startX,formY +18,width,7,inputs[3]);
        }
        // Xử lý sự kiện trả về từ InputString
        if (action == NAV_ESC){
            ClearScreen();
            return NAV_ESC; 
        } 
        
        else if (action == NAV_UP) { 
            currentField = (currentField <= 0) ? 3 : currentField - 1;
        } 
        else if (action == NAV_DOWN) { 
            currentField = (currentField >= 3) ? 0 : currentField + 1;
        }else if(action == NAV_RIGHT){ 
            currentField = (currentField == 4) ? 0 : 4;
        }else if(action == NAV_LEFT){
            currentField = (currentField == 0) ? 4 : 0;
        }else if (action == NAV_ENTER) { // Nhấn ENTER
            if (!inputs[0].empty() && !inputs[1].empty() && !inputs[2].empty() && !inputs[3].empty() && !inputs[4].empty()) {
                CB* NewCB = new CB; 
                NewCB->set_maCB(const_cast<char*>(inputs[0].c_str()));
                NewCB->ngayKH.set_dd(GetDayFromStr(inputs[1]));
                NewCB->ngayKH.set_mt(GetMonthFromStr(inputs[1]));
                NewCB->ngayKH.set_yy(GetYearFromStr(inputs[1]));
                NewCB->ngayKH.set_hh(GetHourFromStr(inputs[1]));
                NewCB->ngayKH.set_mm(GetMinuteFromStr(inputs[1]));
                
                NewCB->set_sbDich(const_cast<char*>(inputs[2].c_str()));
                NewCB->set_soHieuMB(const_cast<char*>(inputs[3].c_str()));
                Init_Tickets(NewCB, stoi(inputs[4]));
                NewCB->trangThai = 1;
            
                string error_code =Can_Add_CB(dsCB, dsMB, NewCB);
                if(error_code != "0") {
                    Gotoxy(startX+1, formY + 20);
                    if(error_code == "1") {
                        string u = "Lỗi: Số hiệu [" + inputs[3] + "] không tồn tại!";
                        SmallBox(u,false,true,true,false,width-2,3,RED); // tính cả hai cái viền của box xung quanh
                    }
                    else if(error_code == "2") {
                        string u = "Lỗi: Máy bay có số hiệu [" + inputs[3] + "] đang thực hiện chuyến bay khác vào thời điểm này! hãy đặt cách nhau 24h giữa 2 chuyến bay có cùng số hiệu máy bay.";
                        SmallBox(u,false,true,false,false,width-2,3,RED);
                    }
                    else if(error_code == "3") {
                        string u = "Lỗi: Trùng mã chuyến bay!";
                        SmallBox(u,false,true,false,false,width-2,3,RED);
                    }else if(error_code == "4") {
                        string u = "Lỗi: Không được đặt số vé khởi tạo lớn hơn số chỗ của máy bay [" + inputs[3] + "]";
                        SmallBox(u,false,true,false,false,width-2,3,RED);
                    }
                    delete NewCB; 
                    _getch(); 
                    ClearRegion(startX+1, formY + 20, width-2, 3);
                    
                    currentField = 0; 
                }else{
                    Gotoxy(startX, formY + 24);
                    SmallBox("Xác nhận thêm chuyến bay này? (ENTER: Đồng ý - ESC: Quay lại sửa)", width, 3, (string)YELLOW);
                    
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
                        Gotoxy(startX, formY + 24);
                        SmallBox(text, width, 3, (string)WHITE);
                        continue; 
                    }

                    // --- TIẾN HÀNH LƯU ---
                    Gotoxy(startX, formY + 24);
                    SmallBox("ĐANG LƯU DỮ LIỆU...", width, 3, (string)GREEN);
                    Add_CB(dsCB,dsMB, NewCB);
                    string succeededtext = "Đã lưu thành công! Nhấn phím bất kỳ để tiếp tục, ESC để thoát.";
                    Gotoxy(startX + width/2 -visualLength(succeededtext)/2, formY + 27);
                    cout << YELLOW << succeededtext << RESET;
                    int key = GetNavKey();
                    ClearScreen();
                    if(key == NAV_ESC){
                        return NAV_ESC;
                    }else return NAV_ENTER; 
                }
            } else {
               currentField = (currentField + 1)%5;
            }
        }
    }
    return NAV_ESC;
}

void UiThongTinCB(int x,int y , int width, int height,string MCB){
    int formY = y ;
    int startX = x ;
    ClearRegion(startX, formY + 4, width, height);
    Gotoxy(startX, formY+4);
    CB* ThisCB = Find_CB(dsCB, const_cast<char*>(MCB.c_str()));
    cout << "Mã Chuyến Bay: " << ThisCB->maCB;
    Gotoxy(startX, formY+6);
    cout << "Số Hiệu Máy Bay: " << ThisCB->soHieuMB;
    Gotoxy(startX, formY + 8);
	cout << "Ngày khởi hành: " << ToStringDate(ThisCB->ngayKH);
    Gotoxy(startX, formY+10);
    cout << "Sân Bay: " << string(ThisCB->sbDich);
    Gotoxy(startX + 24, formY + 10);
    cout << "Số vé: " << ThisCB->socho;
}

int SmallListMCB( string& res, CB* &thisCB, int width, int x, int y, int items, listCB &A,int X, int Y, int w , int h){
    if (A.slCB == 0) return NAV_ESC;
    items = min(items, A.slCB);
    string s ;
    int here = 0 ;
    int pos_op = 0 ;
    CB* selectedNode = nullptr;
    CB* StartHere = A.head;
    while(true){
        CB* current = StartHere;

        for(int i = 0 ; i < items ;i++){
            Gotoxy(x+1,y + (i+1)*2+1);
            if(i + here < A.slCB && current != nullptr){
                s = string(current->maCB);
                if (i == pos_op){
                    s = HIGHLIGHT + s + RESET;
                    selectedNode = current;
                }
                int len = visualLength(s);
                int padding = max(0, width - len - 3);
                cout << s + string(padding, ' ');
                current = current->next;
            }
        }
        if(w != - 1 && h != -1) UiThongTinCB(X,Y,w,h,string(selectedNode->maCB));
        int ch ;
        NavKey key = GetNavKey(ch);
        if(key == NAV_ESC) return NAV_ESC;
        if(key == NAV_DOWN){
            if (pos_op + here + 1 < A.slCB) { // Nếu chưa chạm đáy danh sách thật
                if (pos_op + 1 < items) {
                    pos_op++; // Thanh sáng dịch xuống
                } else {
                    here++;   // Đẩy danh sách xuống, thanh sáng giữ nguyên ở đáy
                    StartHere = StartHere->next;
                }
            }
        }else if( key == NAV_UP){
           if (pos_op == 0) {
                if (here > 0){
                    here--;
                    StartHere = A.head; 
                    for(int i = 0 ; i < here;i++){
                        StartHere = StartHere->next;
                    }
                } 
                else return NAV_UP;   
            } else {
                pos_op--; 
            }
        }else if(key == NAV_ENTER){
           if (selectedNode != nullptr) {
                thisCB = selectedNode;
                res = string(selectedNode->maCB);
                return NAV_ENTER; 
            }
        }else if(key == NAV_UNKNOWN && char(ch) == 'f'){
            return NAV_UP;
        }
    }
}

void UiSmallListMCB(int width, int items, int x, int y, listCB& A){
    if (A.slCB == 0) {
        Gotoxy(x+1, y + 3);
        string u = "Không có chuyến bay nào!";
        int len = visualLength(u);
        int padding = max(0, width - len - 3);
        cout << RED << u << string(padding, ' ') << RESET;
        ClearRegion(x+1, y + 4, width - 2, (items-1)*2); 
        return;
    }

    CB* current = A.head;
    string s;
    int renderCount = min(items, A.slCB);

    for(int i = 0 ; i < renderCount ; i++){
        Gotoxy(x+1, y + (i+1)*2+1);
        if(current != nullptr){
            s = string(current->maCB);
            int len = visualLength(s);
            int padding = max(0, width - len - 3);
            cout << s + string(padding, ' ');
            current = current->next;
        }
    }
    
    for(int i = renderCount; i < items; i++){
        Gotoxy(x+1, y + (i+1)*2+1);
        int padding = max(0, width - 3);
        cout << string(padding, ' '); 
    }
}

// Hàm này ở kế hàm CustomerAddHK nằm trong Block AddHK nên để đây, không được để lung tung ở các hàm CB
// Phần này của Add hành khách
NavKey CustomerAddHK(string cmnd) {
    int width = 120;
    int startX = (GetTerminalWidth() - width) / 2;
    int formY = 4;
    int startY = 0;

    Gotoxy(startX, formY + 14);
    SmallBox("", true, true, true, true, width - 43, 10, WHITE);

    Gotoxy(startX, formY + 11);
    SmallBox("CẬP NHẬT THÔNG TIN CHO KHÁCH HÀNG", (int)width - 43, (int)3, (string)YELLOW);

    string labels[] = { "CMND", "Họ", "Tên", "Phái" };
    int labelX = startX + 2;
    int inputX = startX + 25;
    string gap(5, ' '); // Khoảng cách giữa 2 lựa chọn
    for (int i = 0; i < 4; i++) {
        Gotoxy(labelX, formY + 16 + (i * 2));
        cout << labels[i];
        Gotoxy(inputX, formY + 16 + (i * 2));
        if (i != 3) {
            cout << string(50, '_');
        }
        else {
            cout << string(GREEN) << SQ << " Nam " << string(RESET);
            cout << gap;
            cout << string(GRAY) << SQ << " Nữ " << string(RESET);

        }
    }

    Gotoxy(startX + 2, formY + 24);
    string t = "ENTER/DOWN: Tiếp tục - UP: Lên trên - ESC: Hủy";
    cout << t;

    // --- LOGIC NHẬP LIỆU VÀ ĐIỀU HƯỚNG ---
    string inputs[4] = { "", "", "", "" };
    inputs[0] = cmnd; // Điền sẵn CMND nếu đã có từ trước
    int currentField = 0; // 0: Số hiệu, 1: Loại, 2: Số chỗ

    while (true) {
        // Xóa thông báo lỗi cũ mỗi khi chuyển dòng 
        int gender = 0;
        int action;
        if (currentField == 0) {
            action = InputString(inputs[0], inputX, formY + 16, cmnd_max - 1, '_', true, false, false, false, false);
        }
        else if (currentField == 1) {
            action = InputString(inputs[1], inputX, formY + 18, ho_max, '_', false, false, false, false, true);
        }
        else if (currentField == 2) {
            action = InputString(inputs[2], inputX, formY + 20, ten_max, '_', false, false, false, false, true);
        }
        else if (currentField == 3) {
            action = InputGender(gender, inputX, formY + 22);
        }

        if (action == NAV_ESC) {
            return NAV_ESC;
        }
        else if (action == NAV_UP) {
            currentField = (currentField == 0) ? 3 : currentField - 1;
        }
        else if (action == NAV_DOWN) {
            currentField = (currentField == 3) ? 0 : currentField + 1;
        }
        else if (action == NAV_ENTER) {
            // --- KIỂM TRA ĐÃ ĐIỀN ĐỦ 3 MỤC CHƯA ---
            if (!inputs[0].empty() && !inputs[1].empty() && !inputs[2].empty()) {

                if (inputs[0].size() < 12) {
                    Gotoxy(labelX, formY + 24);
                    string t = "Vui lòng điền một cmnd hợp lệ có đủ 12 chữ số!";
                    cout << RED << t << RESET;
                    currentField = 0;
                    _getch();
                    ClearRegion(labelX, formY + 24, visualLength(t), 1);
                    t = "ENTER/DOWN: Tiếp tục - UP: Lên trên - ESC: Hủy";
                    cout << t;
                    continue;
                }

                Gotoxy(startX + 2, formY + 24);
                string t = "Xác nhận thêm hành khách này? (ENTER: Đồng ý - ESC: Quay lại sửa)";
                cout << YELLOW << t << RESET;

                bool isConfirm = false;
                while (true) {
                    NavKey confirmKey = GetNavKey();
                    if (confirmKey == NAV_ENTER) {
                        isConfirm = true;
                        break;
                    }
                    else if (confirmKey == NAV_ESC) {
                        isConfirm = false;
                        break;
                    }
                }

                ClearRegion(startX + 2, formY + 24, visualLength(t), 1);

                if (!isConfirm) {
                    Gotoxy(startX + 2, formY + 24);
                    string t = "ENTER/DOWN: Tiếp tục - UP: Lên trên - ESC: Hủy";
                    cout << t;
                    continue;
                }

                // --- TIẾN HÀNH LƯU ---
                char buf_cmnd[cmnd_max], buf_ho[ho_max], buf_ten[ten_max];
                strncpy(buf_cmnd, inputs[0].c_str(), cmnd_max - 1); buf_cmnd[cmnd_max - 1] = '\0';
                strncpy(buf_ho, inputs[1].c_str(), ho_max - 1);   buf_ho[ho_max - 1] = '\0';
                strncpy(buf_ten, inputs[2].c_str(), ten_max - 1);  buf_ten[ten_max - 1] = '\0';

                if (Add_HK(dsHK, buf_ho, buf_ten, buf_cmnd, gender)) {
                    Gotoxy(startX + 2, formY + 24);
                    cout << YELLOW << "Đã lưu thành công! Nhấn phím bất kỳ để thoát." << RESET;
                    _getch();
                    return NAV_ESC;
                }
                else {
                    Gotoxy(startX + 2, formY + 24);
                    string t = "Lỗi: CMND [" + inputs[0] + "] đã tồn tại! Bấm phím bất kỳ...";
                    cout << RED << t << RESET;
                    _getch();
                    ClearRegion(startX + 2, formY + 24, visualLength(t), 1);

                    Gotoxy(startX + 2, formY + 24);
                    cout << "ENTER/DOWN: Tiếp tục - UP: Lên trên - ESC: Hủy";
                    currentField = 0; // Trùng mã thì đưa về ô đầu tiên để sửa
                }

            }
            else {
                // --- CHƯA ĐIỀN ĐỦ MỤC ---
                if (currentField < 2) {
                    currentField++; // Nếu đang ở trên thì tự xuống dòng
                }
                else {
                    // Nếu đang ở dòng cuối cùng mà vẫn có dòng trống ở trên
                    Gotoxy(startX + 2, formY + 24);
                    string t = "Lỗi: Không được để trống bất kỳ trường nào! Bấm phím bất kỳ...";
                    cout << RED << t << RESET;
                    _getch();
                    ClearRegion(startX + 2, formY + 24, visualLength(t), 1);
                    cout << "ENTER/DOWN: Tiếp tục - UP: Lên trên - ESC: Hủy";

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

void BookTicket() {
start:
    realTimeUpdateCB(dsCB, dsHK, dsMB);
    int width = 120;
    int startX = (GetTerminalWidth() - width) / 2;
    int formY = 2;
    int startY = 0;
    ClearScreen();

    int height = 28;
    Gotoxy(startX, startY);
    SmallBox("", (int)width - 43, (int)height, (string)WHITE);

    string strDate = time_now();
    UiDrawCalendarWithoutTime(81, 0, 5, 1, strDate);

    Gotoxy(startX + 81, startY + 19);
    SmallBox("", (int)39, (int)9, (string)WHITE);
    Gotoxy(startX + 81, startY + 16);
    SmallBox("THÔNG TIN CHUYẾN BAY", (int)39, (int)1, (string)YELLOW);

    Gotoxy(startX, formY + 13);
    SmallBox("THÔNG TIN KHÁCH HÀNG", (int)width - 43, (int)3, (string)YELLOW);
    Gotoxy(startX, formY + 16);
    SmallBox("", true, true, true, true, width - 43, 10, WHITE);

    int labelX = startX + 2;
    int inputX = startX + 25;
    string labels[] = { "CMND", "Ngày Giờ Khởi Hành", "Sân Bay Đích", "Chuyến Bay", "Số Ghế" };

    for (int i = 0; i <= labels->size(); i++) {
        if (i == labels->size()) {
            Gotoxy(labelX + 50, formY + 2 * (i - 1));
            cout << labels[i];
        }
        else {
            Gotoxy(labelX, formY + 2 * i);
            cout << labels[i];
        }
        if (i == 1 || i == 3 || i == 4) continue; // Bỏ qua dòng "Ngày Giờ Khởi Hành" vì có lịch riêng và Chuyên Bay, so ve vì có box riêng
        Gotoxy(inputX, formY + 2 * i);
        cout << string(50, '_');
    }

    Gotoxy(inputX - 1, formY + 1);
    SmallBox(string(22, ' ') + DM, 25, 1);
    Gotoxy(inputX - 1, formY + 5);
    SmallBox(string(22, ' ') + AD, 25, 1);
    Gotoxy(inputX + 34, formY + 5);
    SmallBox(string(5, ' ') + AD, 8, 1);

    string inputs[5] = { "", "", "", "", "" };
    int currentField = 0;
    int action = 0;
    inputs[1] = time_now();
    bool dateSelected = false; // Cờ để kiểm soát việc đã chọn ngày hay chưa
    while (true) {
        ClearRegion(labelX, formY + 10, 40, 1); // Xóa thông báo lỗi cũ
        if (currentField == 0) {
            Gotoxy(startX, formY + 13);
            SmallBox("THÔNG TIN KHÁCH HÀNG", (int)width - 43, (int)3, (string)YELLOW);
            action = InputString(inputs[currentField], inputX, formY + 2 * currentField, 12, '_', true, false, false, false, false);
            if (action == NAV_ESC) {
                ClearScreen();
                return;
            }
            if (inputs[0].size() < 12) {
                Gotoxy(labelX, formY + 10);
                string t = "Vui lòng điền một cmnd hợp lệ có đủ 12 chữ số!";
                cout << RED << t << RESET;
                currentField = 0;
                _getch();
                ClearRegion(labelX, formY + 10, visualLength(t), 1);
                continue;
            }
            char cmnd[cmnd_max];
            strcpy(cmnd, inputs[0].c_str());
            HK* UIHK = Find_HK_At_List(dsHK, cmnd);
            if (UIHK == NULL) {
                Gotoxy(labelX, formY + 10);
                string t1 = "Phát hiện số cmnd mới! Vui lòng cập nhật cho số cmnd này để tiếp tục!";
                cout << YELLOW << t1 << RESET;
                Gotoxy(labelX, formY + 11);
                string t2 = "(Enter: Tiếp tục ESC: Quay lại)";
                cout << GRAY << t2 << RESET;
                action = GetNavKey();
                ClearRegion(labelX, formY + 10, visualLength(t1), 1);
                ClearRegion(labelX, formY + 11, visualLength(t2), 1);
                if (action == NAV_ESC) {
                    currentField = 0;
                    continue;
                }
                else {
                    action = CustomerAddHK(inputs[0]);
                    ClearRegion(labelX, formY + 18, 73, 9);
                    currentField = 0;
                    continue;
                }
            }
            if (UIHK != NULL) {
                ClearRegion(labelX, formY + 18, 73, 9);
                string labels[] = { "CMND", "Họ", "Tên", "Phái" };
                string datas[] = { UIHK->cmnd, UIHK->ho, UIHK->ten, UIHK->phai == 0 ? "Nam" : "Nữ" };
                for (int i = 0; i < 4; i++) {
                    Gotoxy(labelX, formY + 19 + (i * 2));
                    cout << labels[i];
                    Gotoxy(inputX, formY + 19 + (i * 2));
                    cout << "[ " << datas[i] << string(30 - visualLength(datas[i]), '_') << " ]";
                }
            }
        }
        else if (currentField == 1) {
            string old = inputs[1];
            NavKey k = NAV_LEFT;
            bool dateError = false; //Kiểm tra lỗi ngày quá khứ

            // Vòng lặp do-while giữ Lịch không bị tắt khi đổi tháng HOẶC khi chọn sai ngày
            while (k == NAV_LEFT || k == NAV_RIGHT || dateError) {
                ClearRegion(81, 0, 39, 15); // Xóa sạch không gian trước khi vẽ
                k = DrawCalendarWithoutTime(81, 0, 5, 1, inputs[1]);

                dateError = false; // Reset cờ mỗi lần vẽ lại lịch

                // Nếu người dùng nhấn ENTER để chốt ngày, ta tiến hành kiểm tra
                if (k == NAV_ENTER) {
                    // Nếu thời gian hiện tại LỚN HƠN thời gian vừa chọn -> Lỗi quá khứ
                    if (IsGreaterTime(time_now(), inputs[1]) == 1) {
                        Gotoxy(labelX, formY + 10);
                        string u = "Lỗi: Thời gian khởi hành không được ở trong quá khứ! Bấm phím bất kỳ...";
                        cout << RED << u << RESET;
                        _getch();
                        ClearRegion(labelX, formY + 10, visualLength(u), 1);
                        dateError = true; // Bật cờ lỗi để vòng lặp while quay lại mở Lịch
                    }
                }
            }
            if (k == NAV_ESC) {
                inputs[1] = old;      // Phục hồi ngày cũ
                action = NAV_DOWN; // Ép giữ con trỏ lại ô này
            }
            else if (k == NAV_BACK) {
                inputs[1] = time_now();
                dateSelected = false; // Đánh dấu chưa chọn ngày
                ClearRegion(inputX, formY + 2 * currentField, visualLength(inputs[1]), 1);
                action = NAV_DOWN;    // Tự động nhảy xuống ô Sân bay đến
            }
            else {
                dateSelected = true; // Đánh dấu đã chọn ngày
                if (!inputs[3].empty()) {
                    CB* tempCB = Find_CB(dsCB, const_cast<char*>(inputs[3].c_str()));
                    if (ss_ngay(StringtoTime(inputs[1]), tempCB->ngayKH) != 0) {
                        ClearRegion(inputX, formY + 2 * 3, visualLength(inputs[3]), 1);
                        ClearRegion(startX + 84, startY + 20, 32, 8);
                        inputs[3] = ""; // Nếu ngày mới chọn không hợp lệ với chuyến bay đã chọn, xóa chuyến bay đó đi
                    }
                }
                Gotoxy(inputX, formY + 2 * currentField);
                cout << inputs[currentField].substr(0,10); //dd/mm/yyyy có độ dài là 10
                action = NAV_DOWN;    // Tự động nhảy xuống ô Sân bay đến
            }
        }
        else if (currentField == 2) {
            action = InputStringSBD(inputs[currentField], inputX, formY + 2 * currentField, 50, '_');
            RedrawInput(inputs[currentField], inputX, formY + 2 * currentField, 50, '_', false, false);
            if (!inputs[3].empty()) {
                CB* tempCB = Find_CB(dsCB, const_cast<char*>(inputs[3].c_str()));
                char sbDich[sbDich_max];
                strcpy(sbDich, inputs[2].c_str());
                if (ss_str(sbDich, tempCB->sbDich) != 0) {
                    ClearRegion(inputX, formY + 2 * 3, visualLength(inputs[3]), 1);
                    ClearRegion(startX + 84, startY + 20, 32, 8);
                    inputs[3] = ""; // Nếu sbDich mới chọn không hợp lệ với chuyến bay đã chọn, xóa chuyến bay đó đi
                }
            }
        }
        else if (currentField == 3) {
            CB* tempCB = NULL;
            listCB subCB = listCB();
            DateTime dateTemp = StringtoTime(inputs[1]);
            if (dateSelected == false) dateTemp = DateTime();
            char sbDich[sbDich_max];
            strcpy(sbDich, inputs[2].c_str());
            int slcb;
            CB** tempList = Search_CB(dsCB, dateTemp, sbDich, slcb);
            if (slcb == 0) {
                string t = "Không tìm thấy CB nào phù hợp với ngày KH hoặc sân bay đích mong muốn...";
                Gotoxy(labelX, formY + 10);
                cout << RED << t << RESET;
                _getch();
                ClearRegion(labelX, formY + 10, visualLength(t), 1);
                currentField = 1; // Quay về ô ngày giờ khởi hành để chọn lại
                continue;
            }
            CB* temp = subCB.head;
            if (temp == NULL && slcb > 0) {
                CB* newCB = new CB();
                copy_CB(newCB, tempList[0]);
                temp = newCB;
                subCB.head = temp;
            }
            for (int i = 1; i < slcb; i++) {
                CB* newCB = new CB();
                copy_CB(newCB, tempList[i]);
                temp->next = newCB;
                temp = temp->next;
            }
            subCB.slCB = slcb;
            action = SmallListMCB(inputs[currentField], tempCB, 20, inputX, formY + 2 * currentField - 1, 3, subCB, startX + 84, startY + 17, 35, 8);
            ClearRegion(inputX, startY + 2 * currentField + 4, 10, 5); // Xóa box thông tin chuyến bay phụ sau khi chọn xong
            if (action == NAV_ESC) {
                currentField++;
                continue;
            }
            if (action == NAV_UP) {
                currentField--;
                continue;
            }
            if (tempCB != NULL) {
                inputs[currentField] = string(tempCB->maCB);
                Gotoxy(inputX, formY + 2 * currentField);
                cout << inputs[currentField];
                inputs[1] = ToStringDate(tempCB->ngayKH);
                dateSelected = true; // Đánh dấu đã chọn ngày khi chọn chuyến bay
                Gotoxy(inputX, formY + 2 * 1);
                cout << inputs[1];
                inputs[2] = string(tempCB->sbDich);
                Gotoxy(inputX, formY + 2 * 2);
                cout << inputs[2];

            }
            temp = subCB.head;
            while (temp != NULL) { // Giải phóng bộ nhớ đã cấp phát cho subCB
                tempCB = temp;
                temp = temp->next;
                delete tempCB;
            }

        }
        else if (currentField == 4) {
            if (inputs[3].empty()) {
                Gotoxy(labelX, formY + 10);
                string t = "Vui lòng chọn chuyến bay trước khi chọn số ghế!";
                cout << RED << t << RESET;
                _getch();
                ClearRegion(labelX, formY + 10, visualLength(t), 1);
                currentField = 3; // Quay về ô chuyến bay để chọn lại
                continue;
            }
            char maCB[maCB_max];
            strcpy(maCB, inputs[3].c_str());
            CB* tempCB = Find_CB(dsCB, maCB);
            action = SmallListSoCho(inputs[currentField], dsCB, tempCB, 3, inputX + 35, formY + 2 * (currentField - 1) + 2, 3);
            ClearRegion(inputX + 35, formY + 2 * (currentField - 1), 3, 1);
            Gotoxy(inputX + 35, formY + 2 * (currentField - 1));
            cout << inputs[currentField];
            if (action == NAV_ESC) {
                currentField--;
                continue;
            }
        }

        if (action == NAV_ESC) {
            ClearScreen();
            return;
        }

        if (action == NAV_UP) {
            currentField--;
            if (currentField < 0) {
                currentField = 4;
            }
            continue;
        }

        if (action == NAV_DOWN) {
            currentField++;
            if (currentField > 4) {
                currentField = 0;
            }
            continue;
        }


        if (action == NAV_ENTER) {
            if (currentField < 5) {
                currentField++;
            }
            if (currentField == 5) {
                bool empty = false;
                for (int i = 0; i < 5; i++) {
                    if (inputs[i].empty() || dateSelected == false) {
                        empty = true;
                        Gotoxy(labelX, formY + 10);
                        string t = "Vui lòng điền đầy đủ thông tin!";
                        cout << RED << t << RESET;
                        currentField = i;
                        _getch();
                        ClearRegion(labelX, formY + 10, visualLength(t), 1);
                        break;
                    }
                }
                if (!empty) {
                    // Xử lý logic đặt vé ở đây
                    if (inputs[0].size() < 12) {
                        Gotoxy(labelX, formY + 10);
                        string t = "Vui lòng điền một cmnd hợp lệ có đủ 12 chữ số!";
                        cout << RED << t << RESET;
                        currentField = 0;
                        _getch();
                        ClearRegion(labelX, formY + 10, visualLength(t), 1);
                        continue;
                    }
                    char cmnd[cmnd_max];
                    strcpy(cmnd, inputs[0].c_str());
                    if (Find_HK_At_List(dsHK, cmnd) == NULL) {
                        Gotoxy(labelX, formY + 10);
                        string t1 = "Phát hiện số cmnd mới! Vui lòng cập nhật cho số cmnd này để tiếp tục!";
                        cout << YELLOW << t1 << RESET;
                        Gotoxy(labelX, formY + 11);
                        string t2 = "(Enter: Tiếp tục ESC: Quay lại)";
                        cout << GRAY << t2 << RESET;
                        action = GetNavKey();
                        ClearRegion(labelX, formY + 10, visualLength(t1), 1);
                        ClearRegion(labelX, formY + 11, visualLength(t2), 1);
                        if (action == NAV_ESC) {
                            currentField = 0;
                            continue;
                        }
                        else {
                            action = CustomerAddHK(inputs[0]);
                            ClearRegion(labelX, formY + 18, 73, 9);
                            currentField = 0;
                            continue;
                        }
                    }
                    char maCBTemp[maCB_max]; strcpy(maCBTemp, inputs[3].c_str());
                    char cmndTemp[cmnd_max]; strcpy(cmndTemp, inputs[0].c_str());
                    string result = Can_Book_Ticket(dsCB, dsHK, maCBTemp, cmndTemp, stoi(inputs[4]));
                    Gotoxy(labelX, formY + 10);
                    string errorMsg;
                    bool errorCode = false;
                    switch (result[0]) {
                    case '1':
                        errorMsg = "Lỗi: Chuyến bay không tồn tại!";
                        cout << RED << errorMsg << RESET;
                        currentField = 3;
                        errorCode = true;
                        _getch();
                        break;
                    case '2':
                        errorMsg = "Lỗi: Hành khách không tồn tại!";
                        cout << RED << errorMsg << RESET;
                        currentField = 0;
                        errorCode = true;
                        _getch();
                        break;
                    case '3':
                        errorMsg = "Lỗi: Hành khách đã đặt vé trên chuyến bay này!";
                        cout << RED << errorMsg << RESET;
                        currentField = 3;
                        errorCode = true;
                        _getch();
                        break;
                    case '4':
                        errorMsg = "Lỗi: Chuyến bay đã hết vé hoặc không còn hoạt động!";
                        cout << RED << errorMsg << RESET;
                        currentField = 3;
                        errorCode = true;
                        _getch();
                        break;
                    case '5':
                        errorMsg = "Lỗi: Đã có khách đặt vé tại ghế số " + inputs[4] + "!";
                        cout << RED << errorMsg << RESET;
                        currentField = 4;
                        errorCode = true;
                        _getch();
                        break;
                    case '6':
                        errorMsg = "Lỗi: Đăng ký nhiều chuyến thì ngày KH phải cách nhau ít nhất 1 ngày!";
                        cout << RED << errorMsg << RESET;
                        currentField = 4;
                        errorCode = true;
                        _getch();
                        break;
                    case '7':
                        errorMsg = "Lỗi: Số ghế không hợp lệ (Số ghế phải > 0 và <= số vé mở bán)!";
                        cout << RED << errorMsg << RESET;
                        currentField = 4;
                        errorCode = true;
                        _getch();
                        break;
                    }
                    ClearRegion(labelX, formY + 10, 70, 1);
                    if (errorCode == true) continue; // Nếu có lỗi thì không hỏi đặt vé nữa, cứ ở lại ô đó để sửa
                    Gotoxy(labelX, formY + 10);
                    string s = "Bạn có đồng ý đặt vé không ? (ENTER: Tiếp Tục | ESC: Thoát)";
                    cout << YELLOW << s << RESET;
                    action = GetNavKey();
                    ClearRegion(labelX, formY + 10, visualLength(s), 1);
                    if (action == NAV_ESC) {
                        currentField = 0;
                        continue;
                    }
                    Book_Ticket(dsCB, dsHK, maCBTemp, cmndTemp, stoi(inputs[4]));
                    Gotoxy(labelX, formY + 10);
                    string t = "Đặt vé thành công! (ENTER: Tiếp Tục | ESC: Thoát)";
                    cout << GREEN << t << RESET;
                    action = GetNavKey();
                    if (action == NAV_ESC) {
                        ClearScreen();
                        return;
                    }
                    ClearRegion(labelX, formY + 10, visualLength(t), 1);
                    goto start;
                }
                continue;
            }
        }

    }
}

int SmallListSoCho(string& res, listCB &dsSubCB, CB* &thisCB, int width, int x, int y, int items) {
    int sldsVT = 0;
	char maCB[maCB_max];
    strcpy(maCB, thisCB->maCB);
    int* soghe = Get_Empty_Seats(dsSubCB, maCB, sldsVT);
    if (sldsVT == 0) {
        Gotoxy(x, y);
        string t = "Hết Ghế!";
		cout << RED << t << RESET;
        _getch();
        ClearRegion(x, y, visualLength(t), 1);
		return NAV_ESC;
    }
    
    int l = 0, r = min(items, sldsVT);
    int select = 0;
    int action;
    while (true) {
        int count = 0;
        for (int i = l; i < r; i++) {
            Gotoxy(x, y + count * 2);
            if (i == select) cout << HIGHLIGHT << soghe[i] << RESET;
            else cout << soghe[i];
            count++;
        }
		action = GetNavKey();
        if (action == NAV_DOWN) {
            select++;
            if (select >= sldsVT) {
                select--;
                continue;
            }
            if (select >= r) {
                l++;
                r++;
                ClearRegion(x, y, width, items * 2 - 1);
                continue;
            }
        }

        if (action == NAV_UP) {
            select--;
            if (select < 0) {
                select++;
                continue;
            }
            if (select < l) {
                l--;
                r--;
                ClearRegion(x, y, width, items * 2 - 1);
                continue;
            }
        }

        if (action == NAV_ENTER) {
            ClearRegion(x, y, width, items * 2 - 1);
            res = to_string(soghe[select]);
            delete[] soghe;
            return NAV_ENTER;
        }

        if (action == NAV_ESC) {
            ClearRegion(x, y, width, items * 2 - 1);
            delete[] soghe;
            return NAV_ESC;
        }
    }
    
    return 0;
}

void UiThongTinHK(int x, int y, int width, int height, string s) {
    int formY = y ;
    int startX = x ;
    ClearRegion(startX, formY , width, height);
    HK* ThisHK = Find_HK_At_List(dsHK, const_cast<char*>(s.c_str()));
    if (ThisHK == NULL) return;
    Gotoxy(startX, formY);
    cout << "CMND: ";
    Gotoxy(startX + 9, formY);
    cout << ThisHK->cmnd;
    Gotoxy(startX, formY + 2);
    cout << "Họ tên: "; 
    Gotoxy(startX + 9, formY + 2);
    cout << ThisHK->ho << " " << ThisHK->ten;
	Gotoxy(startX, formY + 4);
    cout << "Phái: ";
    Gotoxy(startX + 9, formY + 4);
    cout << (ThisHK->phai == 0 ? "Nam" : "Nữ");
}

// Khối hàm thực hiện In danh sách MB và các thao tác sữa xoá ở đây 
bool SimpleInput(string& res, NavKey key, int& ch, int x, int y,int width){
    bool isChanged = false; 
    if (key == NAV_BACK && !res.empty()) {
        res.pop_back();
        Gotoxy(x, y);
        cout << res << "  ";
        isChanged = true;
    } 
    else if (visualLength(res) < width && (( (!res.empty() && res.back() != ' ' && ch == ' ') || ((isalnum(ch) || ch == '-') && key == NAV_UNKNOWN) ) )) { 
        res += toupper((char)ch); 
        Gotoxy(x, y);
        cout << res; 
        isChanged = true;
    }
    return isChanged;
}

void UiFindMB(string& res, NavKey key, int& ch, listMB& A, int x, int y) {
    bool isChanged = SimpleInput(res, key, ch, x, y,soHieuMB_max); 

    if (isChanged) {
        A = Find_MB_OnRage(dsMB, const_cast<char*>(res.c_str()));
    }
}

void PrintHeader(string res, int mod, string s, int size, string headers[], string typemode[], int Temp[], int Temp2[],int ResX, int FindX,int ModeX,int ResY){
    if(ResX != - 1){
        Gotoxy(ResX, ResY-2);
        cout << s;
    }
    if(FindX != -1){
        Gotoxy(FindX, ResY);
        cout << GRAY << "Tìm Kiếm: " << res << string(10, ' ') << RESET; 
    }
    if(ModeX != -1){
        Gotoxy(ModeX, ResY);
        cout << GRAY << "MODE: " << typemode[mod] << RESET;
    }
    for(int i = 0; i < size; i++) {
        Gotoxy(Temp[i], ResY+1);
        SmallBox(headers[i], Temp2[i], 1); // Dùng chính Temp2 để set độ rộng
    }
}

void Render_MB_Page(listMB& danhSach, int currentPage, int itemsPerPage) {
    int Temp[] = {10,20,50,90,100};
    int Temp2[] ={10,30,40,10,15};
    
    int startIndex = currentPage * itemsPerPage;
    int endIndex = startIndex + itemsPerPage;
    if (endIndex > danhSach.slMB) endIndex = danhSach.slMB;
    
    int itemsOnThisPage = endIndex - startIndex;
    int totalPages = (danhSach.slMB + itemsPerPage - 1) / itemsPerPage;
    if (totalPages == 0) totalPages = 1;

    int rowOnScreen = 0;
    string m = "Trang " + to_string(currentPage + 1) + "/" + to_string(totalPages) + "  |  Dùng phím ◄/► để chuyển trang, ▲/▼ đổi lựa chọn, ESC để thoát." ;
    Gotoxy(35,8+itemsPerPage + 1);
    SmallBox(m, 50, 4, string(GRAY));
    // Xóa vùng an toàn
    ClearRegion(10, 8 + itemsOnThisPage, 110, 16 - itemsOnThisPage);
    
    if (danhSach.slMB == 0) {
        Gotoxy(50, 8);
        cout << RED << "Không có dữ liệu máy bay nào!" << RESET;
        return;
    }

    for(int i = startIndex; i < endIndex; i++){
        rowOnScreen = i - startIndex; 
        string s = to_string(i+1);
        Gotoxy(10, 8 + rowOnScreen);
        SmallBox(s, false, false, true, true, 10, 1);
        
        Gotoxy(20, 8 + rowOnScreen);
        SmallBox(danhSach.list[i]->soHieuMB, false, false, true, true, 30, 1);
        
        Gotoxy(50, 8 + rowOnScreen);
        SmallBox(danhSach.list[i]->loaiMB, false, false, true, true, 40, 1);
        
        Gotoxy(90, 8 + rowOnScreen);
        SmallBox(to_string(danhSach.list[i]->socho), false, false, true, true, 10, 1);
        
        Gotoxy(100, 8 + rowOnScreen);
        SmallBox(to_string(danhSach.list[i]->SLB), false, false, true, true, 15, 1);
    }
    
    // Vẽ phần 3 chấm ở cuối
    for(int i = 0 ; i < 5 ;i++){
        Gotoxy(Temp[i], 8 + rowOnScreen + 1);
        SmallBox("...", false, false, true, true, Temp2[i], 1, GRAY);
    }
}

void OnlyPrint_List_MB(listMB& A,int currentPage){
     Render_MB_Page(A, currentPage, 15);
}

bool MODE2_MB(listMB &SubList,bool IsMainList, int &index, int &current_Row, int &currentPage, int rowOnScreen,int &itemsPerPage,int Temp[], int Temp2[]){
    string s = SubList.list[index]->soHieuMB;
    if(Can_DeL_MB(dsMB,dsCB,const_cast<char*>(s.c_str())) != "0"){
        Gotoxy(35, 8 + itemsPerPage + 1);
        SmallBox("Máy Bay Không Thể Xoá Vì Đang Có Khách",50,4,string(RED));
        return false ;
    }else{
        Gotoxy(35, 8 + itemsPerPage + 1);
        SmallBox("Bạn Có Muốn Xoá Máy Bay Này Không? ENTER/Đồng Ý Hoặc Phím Bất Kỳ Để Huỷ",50,4,string(RED));
        for(int i = 0; i < 5 ;i++){
            Gotoxy(Temp[i],8 + rowOnScreen);
            if(i == 1){
                s = SubList.list[index]->soHieuMB;
            }else if (i == 2){
                s = SubList.list[index]->loaiMB;
            }else if (i == 3){
                s = to_string(SubList.list[index]->socho);
            }else{
                s = to_string(SubList.list[index]->SLB);
            }
            if(i == 0) s = to_string(index+1);
            SmallBox(s,false, false,true, true, Temp2[i],1, HIGHLIGHT);
        }
        s = SubList.list[index]->soHieuMB;
        NavKey k = GetNavKey();
        if(k == NAV_ENTER){
            Del_MB(dsMB,dsCB, const_cast<char*>(s.c_str()));
            if(!IsMainList)Del_SubDsMB(SubList,const_cast<char*>(s.c_str()));
            Gotoxy(35, 8 + itemsPerPage + 1);
            SmallBox("Đã Xoá Thành Công",50,4,string(GREEN));
            GetNavKey();

            // Tính toán lại số lượng phần tử của trang hiện tại SAU KHI XÓA
            int new_itemsOnThisPage = SubList.slMB - (currentPage * itemsPerPage);
            if (new_itemsOnThisPage > itemsPerPage) new_itemsOnThisPage = itemsPerPage;

            // Xử lý các trường hợp ngoại lệ
            if (new_itemsOnThisPage <= 0 && currentPage > 0) {
                // Trường hợp: Xóa máy bay DUY NHẤT của trang cuối cùng
                // Phải lùi về trang trước đó và đặt con trỏ ở dòng cuối cùng của trang đó
                currentPage--;
                current_Row = itemsPerPage - 1; 
            } 
            else if (rowOnScreen >= new_itemsOnThisPage && new_itemsOnThisPage > 0) {
                // Trường hợp: Xóa máy bay ở DÒNG CUỐI CÙNG của trang hiện tại
                // Lùi con trỏ lên 1 dòng để không bị trỏ vào dòng trống
                current_Row = new_itemsOnThisPage - 1;
            } 
            else {
                // Trường hợp bình thường: Xóa ở giữa danh sách
                current_Row = rowOnScreen; 
            }
            return true; // Thoát ra vòng ngoài để vẽ lại danh sách mới
        }else{
            Gotoxy(35, 8 + itemsPerPage + 1);
            SmallBox("Đã Huỷ Xoá",50,4,string(CYAN));
            return false ;
        }
    }
}

bool MODE3_MB(listMB &SubList,bool IsMainList, int &index, int &current_Row, int &currentPage, int rowOnScreen,int &itemsPerPage,int Temp[], int Temp2[]){
   
    string input[] = {"","",""};
    char* Shmb = SubList.list[index]->soHieuMB; 
    input[0] = SubList.list[index]->soHieuMB;
    input[1] = SubList.list[index]->loaiMB;
    input[2] = to_string(SubList.list[index]->socho);
    int idx = 1;
    int ch = 1 ; 
    while(true){
        Gotoxy(35, 8 + itemsPerPage + 1);    
        SmallBox("Đang Hiệu Chỉnh - Dùng ◄/► để nhảy ô, ENTER để lưu, ESC thoát", 50, 4, string(BOLDCYAN));
        ch = InputString(input[idx], Temp[idx + 1] + 1, 8 + rowOnScreen, Temp2[idx + 1] - 2, ' ', (idx == 2),true,true, (idx ==0), false);
        RedrawInput(input[idx], Temp[idx + 1] + 1, 8 + rowOnScreen, Temp2[idx + 1] - 2, ' ', true, false);
        if(ch == NAV_ESC) return false ;
        if(ch == NAV_LEFT) idx == 1 ? idx = 2 : idx = idx - 1 ;
        if(ch == NAV_RIGHT)idx == 2 ? idx = 1 : idx = idx + 1 ; 
        if(ch == NAV_ENTER){
            if(input[0].empty() || input[1].empty() || input[2].empty()){
                Gotoxy(35, 8 + itemsPerPage + 1);
                SmallBox("Lỗi: Không được để trống thông tin!", 50, 4, string(RED));
                GetNavKey();
                continue; 
            }

            MB* tmp = new MB;
            tmp->set_soHieuMB(Shmb);
            tmp->set_socho(stoi(input[2]));
            tmp->set_loaiMB(const_cast<char*>(input[1].c_str()));
            string error_code = Can_Edit_MB(dsMB,dsCB,Shmb,tmp) ;
            if(stoi(input[2]) > socho_max){
                 Gotoxy(35, 8 + itemsPerPage + 1); 
                 SmallBox("Sửa Thất Bại, Số Chỗ Không Quá: " + to_string(socho_max), 50, 4, string(RED));
                 delete tmp ;
                 GetNavKey();
                 continue;
            }
            if(error_code != "0"){
                Gotoxy(35, 8 + itemsPerPage + 1);    
                if(error_code[0] == '1' ){   
                    string sub = error_code.substr(1);
                    SmallBox("Sửa Thất Bại, Số chỗ không được bé hơn số vé mở bán của CB " + sub + " đang sử dụng máy bay này", 50, 4, string(RED));
                    delete tmp ;
                }else{
                    SmallBox("Sửa Thất Bại, Số Hiệu: " + input[0] +" Bị Trùng", 50, 4, string(RED));
                    delete tmp ;
                }
                GetNavKey();
            }else{
                Gotoxy(35, 8 + itemsPerPage + 1);    
                SmallBox("Bạn Có Chắc Muốn Sửa Máy Bay Này Chứ? Nhấn ENTER Để Xác Nhận, Phím Bất Kỳ Để Hủy", 50, 4, string(YELLOW));
                NavKey k = GetNavKey();
                if(k != NAV_ENTER){
                    Gotoxy(35, 8 + itemsPerPage + 1);    
                    SmallBox("Đã Huỷ Chỉnh Sửa", 50, 4, string(CYAN));
                    delete tmp ;
                    GetNavKey();
                    continue ;
                }else{
                    Edit_MB(dsMB,dsCB,Shmb,tmp);
                    if(!IsMainList)Edit_SubDsMB(SubList,Shmb,tmp);
                    delete tmp ;
                    Gotoxy(35, 8 + itemsPerPage + 1);    
                    SmallBox("Sửa Thành Công!", 50, 4, string(GREEN));
                    GetNavKey();
                    return true ;
                }
            }
        }
    }
}

void Get_Input_MB(bool IsHighLight, listMB &tmp, int rowOnScreen,int startIndex, int Temp[], int Temp2[] ){
    string s ;
    int index = rowOnScreen + startIndex;// rowOnScreen + startIndex -1
    for(int i = 0; i < 5 ;i++){
        Gotoxy(Temp[i],8 + rowOnScreen);
        if(i == 1){
            s = tmp.list[index]->soHieuMB;
        }else if (i == 2){
            s = tmp.list[index]->loaiMB;
        }else if (i == 3){
            s = to_string(tmp.list[index]->socho);
        }else{
            s = to_string(tmp.list[index]->SLB);
        }
        if(i == 0) s = to_string(index+1);
        SmallBox(s,false, false,true, true, Temp2[i],1, IsHighLight ? HIGHLIGHT : WHITE);
    } 
}


void It_list_MB(int mod, string res){
    int field = 5 ;
    int Temp[] = {10,20,50,90,100};
    int Temp2[] ={10,30,40,10,15};
    string Typemode[] = {"In danh Sách","Hiệu Chỉnh","Xoá"};
    string headers[] = {"STT", "Số Hiệu Máy Bay", "Loại Máy Bay", "Số Chỗ", "Số Lần Bay"};
    PrintHeader(res, mod, "Danh Sách Máy Bay", 5, headers,Typemode, Temp, Temp2);
    int currentPage = 0;
    int current_Row = 0;
    int savedPage = 0;
    bool CanReDraw = true ; 
    while(true){
         // Thiết lập thông số phân trang
         if (res.empty()) {
            savedPage = currentPage;
        }
        int itemsPerPage = 15; // Ví dụ: màn hình chỉ chứa được 15 máy bay
        int startIndex = currentPage * itemsPerPage;
        int endIndex = startIndex + itemsPerPage;
        int rowOnScreen = 0 ;
        listMB tmp = Find_MB_OnRage(dsMB, const_cast<char*>(res.c_str()));
        
        if(CanReDraw){ 
            Render_MB_Page(tmp, currentPage, itemsPerPage);
        }
        CanReDraw = true ; 
        int itemsOnThisPage = min(itemsPerPage, tmp.slMB - (currentPage * itemsPerPage));
        if(itemsOnThisPage < 0) itemsOnThisPage = 0;

        //In thông tin điều hướng ở cuối
        Gotoxy(35, 8 + itemsPerPage + 1);
        int totalPages = (tmp.slMB + itemsPerPage - 1) / itemsPerPage; // Tính tổng số trang
        if(totalPages == 0) totalPages = 1;
        string m = "Trang " + to_string(currentPage + 1) + "/" + to_string(totalPages) + "  |  Dùng phím ◄/► để chuyển trang, ▲/▼ đổi lựa chọn, ESC để thoát." ;
        SmallBox(m,50,4,string(GRAY));
        if (itemsOnThisPage == 0) {
            NavKey ch = GetNavKey();
            if (ch == NAV_ESC) return; 
            continue;
        }
        rowOnScreen = current_Row ;
        while(true){
            int index = rowOnScreen + startIndex;// rowOnScreen + startIndex -1
            Get_Input_MB(true, tmp, rowOnScreen, startIndex, Temp, Temp2);
            Gotoxy(35, 8 + itemsPerPage + 1);
            SmallBox(m,50,4,string(GRAY));
            int oldPage = currentPage; 
            int oldRow = rowOnScreen;
            NavKey ch =  PaginationController(tmp.slMB, itemsPerPage, currentPage, current_Row);
            Get_Input_MB(false, tmp, rowOnScreen, startIndex, Temp, Temp2);
            rowOnScreen = current_Row;
            if(ch == NAV_UP && oldRow == 0){
                tmp = Find_MB_OnRage(dsMB, const_cast<char*>(res.c_str()));
                while(true){
                    Gotoxy(20 + res.length(),4);
                    int chr = 0 ;
                    NavKey key = NAV_UP;
                    while(key == NAV_UP || key == NAV_RIGHT || key == NAV_LEFT )key = GetNavKey(chr);
                    if(key == NAV_ESC)return ;
                    if((key == NAV_DOWN || key == NAV_ENTER) && tmp.slMB > 0) {
                        CanReDraw = false ;
                        break ;
                    }
                    bool Changed = res.empty();
                    UiFindMB(res, key, chr, tmp,20,4);
                    CanReDraw = true ;
                    if(!res.empty())
                        OnlyPrint_List_MB(tmp,0);
                    else if (res.empty() && !Changed)
                        OnlyPrint_List_MB(tmp,savedPage);
                }
                if (!res.empty()) {
                    current_Row = 0;
                    currentPage = 0;
                } else {
                    currentPage = savedPage;
                    current_Row = 0;
                }
                break;
            }else if (ch == NAV_ENTER){
                if(mod == 0){
                    Gotoxy(35, 8 + itemsPerPage + 1);
                    SmallBox("Đây Là Chế Độ In Danh Sách, Bạn Chỉ Có Quyền Xem Ở Chế Độ Này",50,4,string(RED));
                }else if(mod == 2){
                    bool IsDeleted = MODE2_MB(tmp, true, index, current_Row, currentPage, rowOnScreen, itemsPerPage, Temp, Temp2) ;
                    if (IsDeleted) break;
                }else if (mod == 1){
                    current_Row = rowOnScreen;
                    bool IsEdited = MODE3_MB(tmp, true, index, current_Row, currentPage, rowOnScreen, itemsPerPage, Temp, Temp2) ;
                    if (IsEdited) break;
                }
            }else if (ch == NAV_ESC){
                return ;
            }
            if (currentPage != oldPage) break ;
        }
    }
}

// Khối hàm In danh sách Chuyến Bay và các thao tác hiệu chỉnh ở đây không bao gồm các hàm thống kê đặt biệt

// Truyền listCB& A (tham chiếu) để sửa trực tiếp, không dùng lệnh return
void UiFindCB(string& res, NavKey key, int& ch, listCB& A, listCB& mainList) {
    int X = 20 , Y = 4 ;
    bool isChanged = SimpleInput(res, key, ch, X, Y,maCB_max);
  
    // CHỈ DỌN RAM VÀ TÌM LẠI NẾU NGƯỜI DÙNG THỰC SỰ GÕ PHÍM CHỮ/XÓA
    if (isChanged) {
        A.Clear();
        A = Find_CB_OnRage(mainList, const_cast<char*>(res.c_str()));
    }
}

void Render_CB_Page(listCB& danhSach, int currentPage, int itemsPerPage,int size, int Temp[],int Temp2[],int Bin[] ) {

    int startIndex = currentPage * itemsPerPage;
    int endIndex = startIndex + itemsPerPage;
    if (endIndex > danhSach.slCB) endIndex = danhSach.slCB;
    
    int itemsOnThisPage = endIndex - startIndex;
    int rowOnScreen = 0;
    int totalPages = (danhSach.slCB + itemsPerPage - 1) / itemsPerPage;
    if(totalPages == 0) totalPages = 1;

    int WidthClear = 0;
    for (int i = 0; i < size;i++) WidthClear += Temp2[i];
    ClearRegion(10, 8 + itemsOnThisPage, WidthClear, 13 - itemsOnThisPage);
    string m = "Trang " + to_string(currentPage + 1) + "/" + to_string(totalPages) + "  |  Dùng phím ◄/► để chuyển trang, ▲/▼ đổi lựa chọn, ESC để thoát.";
    Gotoxy(5, 24);
    SmallBox(m, 50, 4, string(GRAY));
    
    if (danhSach.slCB == 0) {
        Gotoxy(50, 8);
        cout << RED << "Không có dữ liệu chuyến bay nào!" << RESET;
        return;
    }

    CB* current = danhSach.head;

    for (int k = 0; k < startIndex && current != NULL; k++) {
        current = current->next;
    }
    for(int i = startIndex; i < endIndex && current != NULL; i++){
        rowOnScreen = i - startIndex; 
        int pha = 0 ; 
        for(int j = 0 ; j < size;j++){
            if(Bin[j] == 1){
                if(j == 0){
                    string s = to_string(i+1);
                    Gotoxy(Temp[j-pha], 8 + rowOnScreen);
                    SmallBox(s,false, false,true, true, Temp2[j-pha], 1);
                }else if( j == 1){
                    Gotoxy(Temp[j-pha], 8 + rowOnScreen);
                    SmallBox(current->maCB,false, false,true, true, Temp2[j-pha], 1);
                }else if(j == 2){
                    Gotoxy(Temp[j-pha], 8 + rowOnScreen);
                    SmallBox(ToStringDate(current->ngayKH),false, false,true, true, Temp2[j-pha], 1);
                }else if(j == 3){
                    Gotoxy(Temp[j-pha],8 + rowOnScreen);
                    SmallBox(to_string(current->socho),false, false,true, true, Temp2[j-pha], 1);
                }else if(j == 4){
                    Gotoxy(Temp[j-pha],8 + rowOnScreen);
                    SmallBox(current->sbDich,false, false,true, true, Temp2[j-pha], 1);
                }else{
                    Gotoxy(Temp[j-pha],8 + rowOnScreen);
                    SmallBox(SQ,false,false,true,true ,Temp2[j-pha] ,1 , TranFormSatus(current->trangThai));
                }
               
            }else{
                pha++;
            }
        }
        
        current = current->next ; 
    }
    
   
    int pha_dots = 0;
    for (int j = 0; j < size; j++) {
        if (Bin[j] == 1) {
            Gotoxy(Temp[j-pha_dots], 8 + rowOnScreen + 1);
            SmallBox("...", false, false, true, true, Temp2[j-pha_dots], 1, GRAY);
        } else {
            pha_dots++;
        }
    }
}

void Render_CB_Page(CB** CBSearch, int slCB,int Y,int errX,int errY, int currentPage, int itemsPerPage, int size, int Temp[], int Temp2[], int Bin[]) {

    int startIndex = currentPage * itemsPerPage;
    int endIndex = startIndex + itemsPerPage;
    if (endIndex > slCB) endIndex = slCB;
    
    int itemsOnThisPage = endIndex - startIndex;
    int rowOnScreen = 0;
    int totalPages = (slCB + itemsPerPage - 1) / itemsPerPage;
    if (totalPages == 0) totalPages = 1;
    if(slCB == 0)return ;
    // DUYỆT TRỰC TIẾP TỪ MẢNG CON TRỎ CB** 
    for (int i = startIndex; i < endIndex; i++) {
        rowOnScreen = i - startIndex; 
        int pha = 0; 
        
        CB* current = CBSearch[i]; // Lấy trực tiếp Node từ mảng tìm kiếm
        
        for (int j = 0; j < size; j++) {
            if (Bin[j] == 1) {
                if (j == 0) {
                    string s = to_string(i + 1);
                    Gotoxy(Temp[j - pha], Y + rowOnScreen);
                    SmallBox(s, false, false, true, true, Temp2[j - pha], 1);
                } else if (j == 1) {
                    Gotoxy(Temp[j - pha], Y + rowOnScreen);
                    SmallBox(current->maCB, false, false, true, true, Temp2[j - pha], 1);
                } else if (j == 2) {
                    Gotoxy(Temp[j - pha], Y + rowOnScreen);
                    SmallBox(ToStringDate(current->ngayKH), false, false, true, true, Temp2[j - pha], 1);
                } else if (j == 3) {
                    Gotoxy(Temp[j - pha], Y + rowOnScreen);
                    SmallBox(to_string(current->socho - current->sove), false, false, true, true, Temp2[j - pha], 1);
                } else if (j == 4) {
                    Gotoxy(Temp[j - pha], Y + rowOnScreen);
                    SmallBox(current->sbDich, false, false, true, true, Temp2[j - pha], 1);
                } else {
                    Gotoxy(Temp[j - pha], Y + rowOnScreen);
                    SmallBox(SQ, false, false, true, true, Temp2[j - pha], 1, TranFormSatus(current->trangThai));
                }
            } else {
                pha++;
            }
        }
    }
    int pha_dots = 0;
    for (int j = 0; j < size; j++) {
        if (Bin[j] == 1) {
            Gotoxy(Temp[j-pha_dots], Y + rowOnScreen + 1);
            SmallBox("...", false, true, true, true, Temp2[j-pha_dots], 1, GRAY);
        } else {
            pha_dots++;
        }
    }
}
// MODE 2 là sữa 
bool MODE2_CB(CB* pageNodes[], int &index, int current_Row, int currentPage, int rowOnScreen, int itemsPerPage, int Temp[], int Temp2[]){
    string m = "Đang Hiệu Chỉnh - Dùng ↑/↓ để điều chỉnh trạng thái ENTER để lưu, ESC thoát";
    string u = "";
    string inputs[] = {"","",""}; // Edit 3 field Ngày giờ , Sân Bay đích, trạng thái
    int ToaDoBoxX = 5 ;
    int ToaDoBoxY = 24 ;
    int WidthPerBox = 50 ;
    int HeightPerBox = 4 ;
    int ToaDoLichX = 40;
    int ToaDoLichY = 0 ;
    int width = 40 ;
    string Colors[] = {RED,GREEN,YELLOW};
    inputs[0] = ToStringDate(pageNodes[index]->ngayKH) ;
    inputs[1] = pageNodes[index]->sbDich ;
    inputs[2] = to_string(pageNodes[index]->trangThai);
    int thiscolors = pageNodes[index]->trangThai;
   if(pageNodes[index]->trangThai == 0 || pageNodes[index]->trangThai == 3){
        u = (pageNodes[index]->trangThai == 0) ? "Không thể chỉnh sửa chuyến đã huỷ" : "Không thể chỉnh sửa chuyến đã hoàn tất";
        Gotoxy(ToaDoBoxX, ToaDoBoxY);
        SmallBox(u,WidthPerBox,HeightPerBox,string(RED));
        _getch();
        return false ;
    }if(IsGreaterTime(time_now(),inputs[0]) == 1){
        u = "Không thể chỉnh sữa sau ngày giờ khởi hành";
        Gotoxy(ToaDoBoxX, ToaDoBoxY);
        SmallBox(u,WidthPerBox,HeightPerBox,string(RED));
        _getch();
        return false ;
    }if(pageNodes[index]->sove > 0){
        u = "Không thể chỉnh sữa khi chuyến bay đã có khách";
        Gotoxy(ToaDoBoxX, ToaDoBoxY);
        SmallBox(u,WidthPerBox,HeightPerBox,string(RED));
        _getch();
        return false ;
    }
    int EditField[] = {2,4,5};
    const int idx = 0 ;
    int ch = 0 ;

    Gotoxy(ToaDoBoxX, ToaDoBoxY);
    SmallBox(m,WidthPerBox,HeightPerBox,string(GRAY));
    while(true){
    
        ch = InputDateTime(inputs[idx],Temp[EditField[idx]]+1, 8 + rowOnScreen,Temp2[EditField[idx]]-2,' ',true,true, 6,22);
        RedrawInput(inputs[idx], Temp[EditField[idx]]+1, 8 + rowOnScreen, Temp2[EditField[idx]]-2, ' ', true, false);
        
        if(ch == NAV_ESC) return false ;
        if(ch == NAV_UP){
            thiscolors = (thiscolors + 1)%3;
            string stat = Colors[thiscolors] + SQ + RESET;
            inputs[2] = to_string(thiscolors);
            RedrawInput(stat, Temp[5] + 1, 8 + rowOnScreen, Temp2[5] - 2, ' ', true, false);
        }
        if ( ch == NAV_DOWN){
            thiscolors = (abs(thiscolors +2))%3;
            string stat = Colors[thiscolors] + SQ + RESET;
            inputs[2] = to_string(thiscolors);
            RedrawInput(stat, Temp[5] + 1, 8 + rowOnScreen, Temp2[5] - 2, ' ', true, false);
        }
        if( ch == NAV_ENTER){
            Gotoxy(ToaDoBoxX, ToaDoBoxY); 
            SmallBox("Bạn Có Chắc Muốn Sửa Chuyến Bay Này Chứ? Nhấn ENTER Để Xác Nhận, Phím Bất Kỳ Để Hủy", WidthPerBox,HeightPerBox, string(YELLOW));
            NavKey k = GetNavKey();
            if( k != NAV_ENTER){
                Gotoxy(ToaDoBoxX, ToaDoBoxY);
                SmallBox(m, WidthPerBox, HeightPerBox, string(GRAY));
                continue;
            }
            CB* tmp = pageNodes[index];

            // Sao lưu dữ liệu nếu từ chối lưu
            DateTime oldTime = tmp->ngayKH;
            string oldSb = tmp->sbDich;
            int oldSt = tmp->trangThai;

            DateTime T = StringtoTime(inputs[0]);
            tmp->ngayKH = T;
            tmp->set_sbDich(const_cast<char*>(inputs[1].c_str()));
            tmp->set_trangThai(stoi(inputs[2]));
            int result = Update_CB(dsCB,dsMB,tmp->maCB,tmp);
            Gotoxy(ToaDoBoxX, ToaDoBoxY); 
            if (result == 1) { 
                SmallBox("Đã Chỉnh sửa thành công, bất kì để tiếp tục, Esc để thoát", WidthPerBox,HeightPerBox, string(GREEN));
            } else {
                tmp->ngayKH = oldTime;
                tmp->set_sbDich(const_cast<char*>(oldSb.c_str()));
                tmp->trangThai = oldSt;
                string err = "Lỗi không xác định!";
                if (result == -6) err = "Xung đột thời gian với chuyến bay khác (dưới 24h)!";
                SmallBox(err, WidthPerBox,HeightPerBox, string(RED));
                _getch();
                return false ;
            }
            int t = GetNavKey();
            if(t == NAV_ESC){return true;}
            Gotoxy(ToaDoBoxX, ToaDoBoxY); 
            SmallBox(m,WidthPerBox,HeightPerBox,string(GRAY));
            return true;
        }
    }
 
}


void OnlyPrint_List_CB(listCB& A,int size,int CurrentPage,int itemsPerPage,int Temp[],int Temp2[], int Bin[]){
     Render_CB_Page(A, CurrentPage,itemsPerPage,size, Temp, Temp2,Bin);
}

void Get_Input_CB(bool IsHighLight,CB* pageNodes[], int rowOnScreen,int startIndex,int size, int Temp[], int Temp2[] ){
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
        if(i!= 5)SmallBox(s,false, false,true, true, Temp2[i],1, IsHighLight ? HIGHLIGHT : WHITE);
    }
}

void It_list_CB(int mod, listCB& A, string res, string cmnd){
    int field = 5;
    int size = 6;
    int Temp[] = { 10,20,50,80,90,105 };
    int Temp2[] = { 10,30,30,10,15,10 };
    int Bin[] = { 1,1,1,1,1,1 };
    string typemode[] = { "In danh sách","Hiệu Chỉnh/Huỷ Chuyến","Hủy Vé Theo Hành Khách","Thông tin kỹ thuật" };
    string headers[] = { "STT","Mã Chuyến Bay", "Ngày Khởi Hành", "Số Vé","Sân bay đích","Status" };
    string q[] = { "","","","" };
    string Type[] = { RED, GREEN, YELLOW, CYAN };
    q[0] = Type[0] + SQ + RESET + ": Chuyến bay bị huỷ ";
    q[1] = Type[1] + SQ + RESET + ": Chuyến bay còn vé ";
    q[2] = Type[2] + SQ + RESET + ": Chuyến bay hết vé ";
    q[3] = Type[3] + SQ + RESET + ": Chuyến bay hoàn tất ";
    start:
    ClearScreen();
    realTimeUpdateCB(dsCB, dsHK, dsMB);
    string header[] = {"Danh Sách Chuyến Bay","Danh Sách Chuyến Bay","Danh Sách Chuyến Bay Đã Được Đặt Theo Số CMND: " + cmnd,"Danh Sách Chuyến Bay"};
    PrintHeader(res, mod, header[mod], size, headers, typemode, Temp, Temp2, (TotalWidth / 2) - (visualLength(header[mod]) / 2));
    Gotoxy(70,24);
    string u = "  " + q[0] + q[1] +"    " +q[2] + q[3];
    SmallBox(u,50,4);
    int currentPage = 0;
    int current_Row = 0;
    const int itemsPerPage = 12; // Ví dụ: màn hình chỉ chứa được 12 CB
    int savedPage = 0;
    bool CanReDraw = true ; 
    listCB tmp = Find_CB_OnRage(A, const_cast<char*>(res.c_str()));
    while(true){
        // Lưu lại trang hiện tại nếu không tìm kiếm
        if (res.empty()) savedPage = currentPage;
        
         // Thiết lập thông số phân trang
        int startIndex = currentPage * itemsPerPage;
        int endIndex = startIndex + itemsPerPage;
        
        // Đảm bảo không in lố số lượng CB thực tế
        if (endIndex > tmp.slCB) {
            endIndex = tmp.slCB;
        }
        int itemsOnThisPage = endIndex - startIndex;
      
        if(CanReDraw)Render_CB_Page(tmp, currentPage, itemsPerPage,size,Temp,Temp2,Bin);
        if (tmp.slCB == 0) {
            _getch();
            return;
        }
   
        CanReDraw = true ; 
        //In thông tin điều hướng ở cuối
        Gotoxy(5, 24);
        int totalPages = (tmp.slCB + itemsPerPage - 1) / itemsPerPage; // Tính tổng số trang
        string m = "Trang " + to_string(currentPage + 1) + "/" + to_string(totalPages) + "  |  Dùng phím ◄/► để chuyển trang, ▲/▼ đổi lựa chọn, ESC để thoát." ;
        SmallBox(m,50,4,string(GRAY));
        DrawACat();

        CB* current = tmp.head;
        for (int k = 0; k < startIndex && current != NULL; k++) {
            current = current->next; // Nhảy qua các phần tử của trang trước
        }
        //Lưu các node vào mảng tạm
        CB* pageNodes[itemsPerPage]; 
        int nodeCount = 0;
        CB* temp = current;
        for (int k = 0; k < itemsOnThisPage && temp != NULL; k++) {
            pageNodes[nodeCount] = temp;
            nodeCount++;
            temp = temp->next;
        }

        int rowOnScreen = current_Row ;
       
        while(true){
            int index = rowOnScreen + startIndex;// rowOnScreen + startIndex
            int oldPage = currentPage; 
            int oldRow = rowOnScreen;
            CB* SelectedNode = pageNodes[rowOnScreen];
            Get_Input_CB(true,pageNodes, rowOnScreen, startIndex, size, Temp, Temp2 );
            NavKey ch =  PaginationController(tmp.slCB, itemsPerPage, currentPage, current_Row);
            Gotoxy(5, 24);
            SmallBox(m,50,4,string(GRAY));
            Get_Input_CB(false,pageNodes, rowOnScreen, startIndex, size, Temp, Temp2 );
            rowOnScreen = current_Row ;
            if(ch == NAV_UP&& oldRow == 0){
                tmp.Clear(); 
                tmp = Find_CB_OnRage(A, const_cast<char*>(res.c_str()));
                
                while(true){
                    Gotoxy(20 + res.length(),4);
                    int chr = 0 ;
                    NavKey key = NAV_UP;
                    while (key == NAV_UP||key == NAV_LEFT||key == NAV_RIGHT) key = GetNavKey(chr);
                    
                    if(key == NAV_ESC){
                        tmp.Clear();
                        return ;
                    } 
                    if((key == NAV_DOWN || key == NAV_ENTER) && tmp.slCB > 0) {
                        CanReDraw = false ; 
                        break ; 
                    }
                    bool changed = res.empty();
                    string c = res;    // tránh nháy màn hình
                    UiFindCB(res, key, chr, tmp, A);
                    if (c == res) continue; // tránh nháy màn hình nếu chuổi k đổi
                    CanReDraw = true ;
                    
                    if(!res.empty())
                        OnlyPrint_List_CB(tmp, size,0, itemsPerPage, Temp, Temp2, Bin);
                    else if(res.empty() && !changed)
                        OnlyPrint_List_CB(tmp, size,savedPage, itemsPerPage, Temp, Temp2, Bin);
                }
                
                if (!res.empty()) {
                    currentPage = 0;
                    current_Row = 0;
                } else {
                    currentPage = savedPage;
                    current_Row = 0;
                }
                break; 
            }else if (ch == NAV_ENTER){
                if(mod == 0){
                    
                    View_DSHK_CB(SelectedNode);
                    ClearScreen();
                    PrintHeader(res, mod, "Danh Sách Chuyến Bay", size, headers,typemode, Temp, Temp2);
                    Gotoxy(70,24);
                    SmallBox(u,50,4);
                    Render_CB_Page(tmp, currentPage, itemsPerPage,size,Temp,Temp2,Bin);
                    DrawACat();
                }
                else if (mod == 1){
                    bool isedited = MODE2_CB( pageNodes, rowOnScreen,current_Row,currentPage,rowOnScreen,itemsPerPage,Temp,Temp2 ); 
                    if(isedited) break ;
                }
                else if (mod == 2) {
					DateTime now = StringtoTime(time_now());
                    if (ss_ngay(pageNodes[index]->ngayKH, now) <= 60 && isNotPastTime(pageNodes[index]->ngayKH, now)) {
                        Gotoxy(5, 24);
                        SmallBox("Không thể hủy vé trên chuyến bay sắp khởi hành!", 50, 4, string(RED));
                        _getch();
                        Gotoxy(5, 24);
                        SmallBox(m, 50, 4, string(GRAY));
                        continue;
                    }
                    else if (!isNotPastTime(pageNodes[index]->ngayKH, now)) {
                        Gotoxy(5, 24);
                        SmallBox("Chuyến bay đã khởi hành, không thể hủy vé!", 50, 4, string(RED));
                        _getch();
                        Gotoxy(5, 24);
                        SmallBox(m, 50, 4, string(GRAY));
                        continue;
                    }
                    Gotoxy(5, 24);
                    SmallBox("Bạn Có Chắc Muốn Huỷ Chuyến Bay Này Chứ? Nhấn ENTER Để Xác Nhận, Phím Bất Kỳ Để Hủy", 50, 4, string(YELLOW));
					int k = GetNavKey();
                    if (k == NAV_ENTER){
				        CB* tmpCB = pageNodes[index];
                        char cmndtmp[cmnd_max];
                        strcpy(cmndtmp, cmnd.c_str());
                        for (int i = 0; i < tmpCB->socho; i++) {
                            if (ss_str(tmpCB->DSV[i], cmndtmp) == 0) {
                                Cancel_Ticket(dsCB, dsHK, tmpCB->maCB, i + 1);
                                // Xoa CB trong A de dong bo
                                A.Clear();
                                CopyMarkList(A, dsHK, cmndtmp);
                                listCB tmp = Find_CB_OnRage(A, const_cast<char*>(res.c_str()));
                                break;
                            }
                        }
                        goto start;
					}
                    Gotoxy(5, 24);
                    SmallBox(m, 50, 4, string(GRAY));
                }else if(mod == 3){
                    View_MB_CB(SelectedNode);
                    ClearScreen();
                    PrintHeader(res, mod, "Danh Sách Chuyến Bay", size, headers,typemode, Temp, Temp2);
                    Gotoxy(70,24);
                    SmallBox(u,50,4);
                    Render_CB_Page(tmp, currentPage, itemsPerPage,size,Temp,Temp2,Bin);
                    DrawACat();
                }
            }else if (ch == NAV_ESC){
                tmp.Clear();
                return ;
            }
            if(currentPage != oldPage) break ;
        }
    }
}

void  RenderHKBySample (HK** displayArr, CB* thisCB,int fields, int currentPage, int CustomerPerPage, int displayTotal,int tWidth,int Temp[], int Temp2[]) {
        int startIndex = currentPage * CustomerPerPage;
        int endIndex = min(startIndex + CustomerPerPage, displayTotal);
        int totalPages = max(1, (displayTotal + CustomerPerPage - 1) / CustomerPerPage);
        int itemsOnThisPage = max(0, endIndex - startIndex);

        ClearRegion(10, 8, tWidth - 10, 15); 
        string inputs[5] ;
        if (displayTotal == 0) {
            Gotoxy(0, 10);
            SmallBox("Không tìm thấy hành khách nào phù hợp!", false, false, false, false, tWidth, 1, string(RED));
        } else {
            for(int i = startIndex; i < endIndex; i++) {
                int r = i - startIndex;
                inputs[0] = to_string(i + 1);
                inputs[1] = "?";
                inputs[2] = string(displayArr[i]->get_cmnd());
                inputs[3] = string(displayArr[i]->get_ho()) + " " + string(displayArr[i]->get_ten());
                inputs[4] = displayArr[i]->get_phai() ? "Nữ" : "Nam";
                
                for(int v = 0; v < thisCB->socho; v++){
                    if(strcmp(thisCB->DSV[v], displayArr[i]->get_cmnd()) == 0){
                        inputs[1] = to_string(v + 1); 
                        inputs[1] = string(3 - inputs[1].size(), '0') + inputs[1];
                        break;
                    }
                }
                for(int j = 0 ; j < fields ; j++){
                    Gotoxy(Temp[j], 8 + r);
                    SmallBox(inputs[j], false, false, true, true, Temp2[j], 1);
                }
            }
            
            for(int i = 0 ; i < 5 ; i++){
                Gotoxy(Temp[i], 8 + itemsOnThisPage);
                SmallBox("...", false, false, true, true, Temp2[i], 1, string(GRAY)); 
            }
        }

        string m = "Trang " + to_string(currentPage + 1) + "/" + to_string(totalPages) + "  |  ◄/►: Chuyển trang, ▲/▼: Chọn, ESC: Thoát";
        Gotoxy((tWidth - 65)/2, 22); 
        SmallBox(m, 65, 3, string(GRAY)); 
}

void View_DSHK_CB(CB* thisCB) {
    if (thisCB == NULL) return;

    int displayTotal = 0;
    HK**  displayArr = Get_DSHKCB(dsCB, dsHK, thisCB->maCB, displayTotal);

    string searchCMND = "";
    int currentPage = 0;
    int current_Row = 0;
    int fields = 5 ;
    const int CustomerPerPage = 12;
    int tWidth = GetTerminalWidth();

    int Temp[] = {10, 20, 35, 60, 100};   
    int Temp2[] = {10, 15, 25, 40, 15};  
    string headers[] = {"STT", "Ghế Ngồi", "Số CMND", "Họ Tên", "Phái"};

    ClearScreen();

    // Tiêu đề
    Gotoxy(0, 1);
    string title1 = string(BOLD) + "DANH SÁCH HÀNH KHÁCH THUỘC CHUYẾN BAY: " + string(thisCB->maCB) + string(RESET);
    SmallBox(title1, false, false, false, false, tWidth, 1, string(WHITE));

    Gotoxy(0, 2);
    string title2 = "Ngày giờ: " + ToStringDate(thisCB->ngayKH) + " - Nơi đến: " + string(thisCB->sbDich);
    SmallBox(title2, false, false, false, false, tWidth, 1, string(WHITE));

    // Ô Tìm kiếm (Bên trái)
    Gotoxy(10, 4);
    cout << GRAY << "Tìm CMND: " << RESET;
    RedrawInput(searchCMND, 20, 4, 15, ' ', false, false);

    // Trạng thái chuyến bay (Bên phải, nằm CÙNG DÒNG Y=4 với thanh tìm kiếm)
    string statusText[] = {"Đã Huỷ", "Còn Vé", "Hết Vé", "Hoàn Tất"};
    string statusStr = TranFormSatus(thisCB->trangThai) + statusText[thisCB->trangThai] + string(RESET);
    Gotoxy(80, 4); 
    cout << GRAY << "Trạng thái chuyến bay: " << statusStr;

    // Header của bảng
    for(int i = 0; i < 5; i++) {
        Gotoxy(Temp[i], 5);
        SmallBox(headers[i], Temp2[i], 1, string(WHITE)); 
    }

    // GỌI HÀM VẼ BẢNG LẦN ĐẦU TIÊN
    RenderHKBySample (displayArr,thisCB,fields, currentPage, CustomerPerPage, displayTotal, tWidth, Temp, Temp2);
    int rowOnScreen = 0;

    while (true) {
        int startIndex = currentPage * CustomerPerPage;
        int itemsOnThisPage = min(CustomerPerPage, displayTotal - startIndex);
        int totalPages = max(1, (displayTotal + CustomerPerPage - 1) / CustomerPerPage);
        // nếu trống thì chỉ cho người dùng bấm esc, đã trống rồi còn bắt người ta tìm kiếm. 
        if (displayTotal == 0) {
            Gotoxy(0, 10);
            SmallBox("Không tìm thấy hành khách nào phù hợp!", false, false, false, false, tWidth, 1, string(RED));
            NavKey o = NAV_DOWN;
            while(o != NAV_ESC)o = GetNavKey();
            goto EXIT_VIEW;
        } 
        else {
            int index = startIndex + rowOnScreen;
            int oldPage = currentPage; 
            int oldRow = rowOnScreen;
            string stt = to_string(index + 1);
            string cmnd = string(displayArr[index]->get_cmnd());
            string hoten = string(displayArr[index]->get_ho()) + " " + string(displayArr[index]->get_ten());
            string phai = displayArr[index]->get_phai() ? "Nữ" : "Nam";
            string soVe = "?";
            for(int v = 0; v < thisCB->socho; v++){
                if(strcmp(thisCB->DSV[v], displayArr[index]->get_cmnd()) == 0){ 
                    soVe = to_string(v + 1);
                    soVe = string(3 - soVe.size(), '0') + soVe;
                    break; 
                }
            }
            string data[] = {stt, soVe, cmnd, hoten, phai};

            // TÔ MÀU SÁNG DÒNG HIỆN TẠI
            for(int i = 0; i < 5; i++){
                Gotoxy(Temp[i], 8 + rowOnScreen);
                SmallBox(data[i], false, false, true, true, Temp2[i], 1, string(HIGHLIGHT));
            }

            NavKey key = PaginationController(displayTotal , CustomerPerPage, currentPage, current_Row);

            // TRẢ LẠI MÀU BÌNH THƯỜNG
            for(int i = 0; i < 5; i++){
                Gotoxy(Temp[i], 8 + rowOnScreen);
                SmallBox(data[i], false, false, true, true, Temp2[i], 1);
            }
            rowOnScreen = current_Row;
            // XỬ LÝ PHÍM ĐIỀU HƯỚNG
            if (key == NAV_UP && oldRow == 0) {
                    Gotoxy(10, 4); cout << YELLOW << "Tìm CMND: " << RESET;
                    while(true) {
                        
                        int inputChar = 0;
                        NavKey searchKey = GetNavKey(inputChar);

                        if (searchKey == NAV_ESC) goto EXIT_VIEW;
                        if (searchKey == NAV_DOWN && displayTotal > 0) {
                            Gotoxy(10, 4); cout << GRAY << "Tìm CMND: " << RESET;
                            RedrawInput(searchCMND, 20, 4, 15, ' ', false, false);
                            break; 
                        }
                        bool  isSearchChanged = UiFindHK(searchCMND, searchKey, inputChar,displayArr , displayTotal,thisCB);
                        
                        if (isSearchChanged) {
                            currentPage = 0;
                            rowOnScreen = 0;
                            current_Row = 0;
                            RenderHKBySample (displayArr,thisCB,fields, currentPage, CustomerPerPage, displayTotal, tWidth, Temp, Temp2);
                        }
                      
                    }
                    rowOnScreen = 0;
                    current_Row = 0;
            } 
            else if (key == NAV_ESC) goto EXIT_VIEW;
            if(currentPage != oldPage){
                RenderHKBySample(displayArr, thisCB, fields, currentPage, CustomerPerPage, displayTotal, tWidth, Temp, Temp2);
            }
        }
    }

EXIT_VIEW:
    if (displayArr != NULL) delete[] displayArr;
}

void View_MB_CB(CB* thisCB){
    int thisMB = Find_MB(dsMB, thisCB->soHieuMB);
    if(thisMB == -1) return ;
    ClearScreen();
    string u = ReadFilePicture(path_file_picture);
    cout << u ; 
    string s = "Thông số kỹ thuật của máy bay có mã : " + string(thisCB->maCB);
    Gotoxy((GetTerminalWidth() - visualLength(s)) / 2 + 33, 1);
    SmallBox(s,visualLength(s) + 4,3);
    Gotoxy((GetTerminalWidth() - visualLength(s)) / 2 + 35, 6);
    string t = "Mã máy bay :";
    SmallBox(t,20,1,string(YELLOW));
    Gotoxy(100 , 6);
    t = string(dsMB.list[thisMB]->soHieuMB);
    SmallBox(t,18,1);
    Gotoxy((GetTerminalWidth() - visualLength(s)) / 2 + 35, 9);
    t =  "Loại máy bay :";
    SmallBox( t,20,1,string(YELLOW));
    Gotoxy(100 , 9);
    t = string(dsMB.list[thisMB]->loaiMB);
    SmallBox(t,18,1);
    Gotoxy((GetTerminalWidth() - visualLength(s)) / 2+35, 12);
    t = "Số Ghế Tối Đa :";
    SmallBox( t ,20,1,string(YELLOW));
    Gotoxy(100, 12);
    t = to_string(dsMB.list[thisMB]->socho);
    SmallBox(t,18,1);
   

    _getch();
}

bool InputSimpleHK(string& res, NavKey key, int& ch,int x,int y) {
    // Xử lý nút xóa (Backspace)
    if (key == NAV_BACK && !res.empty()) {
        res.pop_back();
        Gotoxy(x, y);
        cout << res << "  "; 
        return true;
    } 
    // Xử lý gõ chữ cái / số
    else if ((isalnum(ch) || ch == '-') && key == NAV_UNKNOWN && visualLength(res) < cmnd_max) { 
        res += toupper((char)ch); 
        Gotoxy(x, y);
        cout << res; 
        return true;
    }
    return false;
}

bool UiFindHK(string& res, NavKey key, int& ch, HK**& arrHK, int& totalItems) {
    int X = 20 , Y = 4 ;
    bool isChanged = InputSimpleHK(res, key, ch, X, Y);
    if (isChanged) {
        if (arrHK != NULL) delete[] arrHK; 
        
        if (res.empty()) {
            arrHK = duyet_HK(dsHK);
            totalItems = dsHK.slHK;
        } else {
            arrHK = Find_HK_OnRage(dsHK, const_cast<char*>(res.c_str()), totalItems);
        }
        return true ;
    }
    return false ;
}

bool UiFindHK(string& res, NavKey key, int& ch, HK**& arrHK, int& TotalCustomer,CB* thisCB) {
    int X = 20 , Y = 4 ;
    bool isChanged = InputSimpleHK(res, key, ch, X, Y);
    if (isChanged) {
        HK** OriginHK = Get_DSHKCB(dsCB, dsHK, thisCB->maCB, TotalCustomer);
        if (arrHK != NULL) delete[] arrHK; 
        if (res.empty()) {
           arrHK = OriginHK;
        } else {
            int t = 0 ;
            arrHK = new HK*[TotalCustomer];
            for(int i = 0; i <TotalCustomer; i++) {
                string c = string(OriginHK[i]->get_cmnd());

                if (c.find(res) == 0) {
                    arrHK[t++] = OriginHK[i];
                }
            }
            TotalCustomer = t ;
            delete [] OriginHK;
        }
        return true ;
    }
    return false ;
}


void Render_HK_Page(HK** arr, int totalItems, int currentPage, int itemsPerPage) {
    // Cấu hình tọa độ (Temp) và Độ rộng (Temp2) cho 4 cột: STT, CMND, Họ và Tên, Phái
    int Temp[] = {10, 20, 45, 90};
    int Temp2[] = {10, 25, 45, 15};
    
    int startIndex = currentPage * itemsPerPage;
    int endIndex = startIndex + itemsPerPage;
    if (endIndex > totalItems) endIndex = totalItems;
    
    int itemsOnThisPage = endIndex - startIndex;
    int rowOnScreen = 0;
    int totalPages = (totalItems + itemsPerPage - 1) / itemsPerPage;
    if (totalPages == 0) totalPages = 1;

    // Xóa vùng dữ liệu cũ trước khi vẽ trang mới
    ClearRegion(10, 8, 110, 16); 
    
    string m = "Trang " + to_string(currentPage + 1) + "/" + to_string(totalPages) + "  |  Dùng phím ◄/► để chuyển trang, ▲/▼ đổi lựa chọn, ESC để thoát.";
    Gotoxy(35, 8 + itemsPerPage + 1);
    SmallBox(m, 50, 4, string(GRAY));
    
    if (totalItems == 0 || arr == NULL) {
        Gotoxy(50, 8);
        cout << RED << "Không có dữ liệu hành khách nào!" << RESET;
        return;
    }

    for(int i = startIndex; i < endIndex; i++) {
        rowOnScreen = i - startIndex; 
        string inputs[4] ;
        inputs[0] = to_string(i + 1);
        inputs[1] = string(arr[i]->get_cmnd());
        inputs[2] = string(arr[i]->get_ho()) + " " + string(arr[i]->get_ten());
        inputs[3] = arr[i]->get_phai() ? "Nữ" : "Nam";

        // Vẽ từng ô cho dòng hiện tại
        for(int i = 0 ; i < 4 ;i++){
            Gotoxy(Temp[i], 8 + rowOnScreen); 
            SmallBox(inputs[i], false, false, true, true, Temp2[i], 1);
        }
    }
    
    for(int i = 0 ; i < 4 ; i++){
        Gotoxy(Temp[i], 8 + rowOnScreen + 1);
        SmallBox("...", false, false, true, true, Temp2[i], 1, GRAY);
    }
}
void Get_Input_HK(bool IsHighLight,HK* arrHK[], int rowOnScreen,int index, int Temp[], int Temp2[] ){
    string data[4] = {
        to_string(index + 1),
        string(arrHK[index]->get_cmnd()),
        string(arrHK[index]->get_ho()) + " " + string(arrHK[index]->get_ten()),
        arrHK[index]->get_phai() ? "Nữ" : "Nam"
    };

    // Tô sáng dòng hiện tại
    for(int i = 0; i < 4; i++){
        Gotoxy(Temp[i], 8 + rowOnScreen);
        SmallBox(data[i], false, false, true, true, Temp2[i], 1, IsHighLight ? HIGHLIGHT : WHITE);
    }
}
void It_list_HK(int mod, string res = "") { 
    start:
    realTimeUpdateCB(dsCB, dsHK, dsMB);
    ClearScreen();
    int Temp[] = {10, 20, 45, 90};
    int Temp2[] = {10, 25, 45, 15};
    string typemode[] = {"In danh Sách", "Hủy Vé Theo Hành Khách"};
    string headers[] = {"STT", "CMND", "Họ và Tên", "Phái"};
    
    int totalItems = dsHK.slHK;
    HK** arrHK = duyet_HK(dsHK); 
    
    // In Header (Tìm kiếm CMND)
    PrintHeader(res, mod, "Danh Sách Hành Khách", 4, headers, typemode, Temp, Temp2,50,10,87);
    
    int currentPage = 0;
    int current_Row = 0;
    const int itemsPerPage = 15;

    while (true) {
        int startIndex = currentPage * itemsPerPage;
        int endIndex = min(startIndex + itemsPerPage, totalItems);
        int itemsOnThisPage = max(0, endIndex - startIndex);
        int totalPages = max(1, (totalItems + itemsPerPage - 1) / itemsPerPage);
        
        Render_HK_Page(arrHK, totalItems, currentPage, itemsPerPage);
        
        string m = "Trang " + to_string(currentPage + 1) + "/" + to_string(totalPages) + "  |  Dùng phím ◄/► để chuyển trang, ▲/▼ đổi lựa chọn, ESC để thoát.";
        Gotoxy(35, 8 + itemsPerPage + 1);
        SmallBox(m, 50, 4, string(GRAY));

        int rowOnScreen = current_Row;

        while (true) {
            if (totalItems == 0) {
                _getch();
                goto CLEANUP_AND_EXIT;
            } else {
                int index = rowOnScreen + startIndex;
                int oldPage = currentPage; 
                int oldRow = rowOnScreen;
                Get_Input_HK(true,arrHK, rowOnScreen, index,  Temp,Temp2 );
                NavKey ch =  PaginationController(totalItems, itemsPerPage, currentPage, current_Row);
                // Trả lại màu bình thường khi con trỏ rời đi
                Gotoxy(35, 8 + itemsPerPage + 1);
                SmallBox(m, 50, 4, string(GRAY)); 
                Get_Input_HK(false,arrHK, rowOnScreen, index,  Temp,Temp2 );
                rowOnScreen = current_Row;
                if (ch == NAV_UP && oldRow == 0) {
                    // VÀO THANH TÌM KIẾM
                    while(true) {
                        Gotoxy(20 + res.length(), 4);
                        int inputChar = 0;
                        NavKey key = GetNavKey(inputChar);

                        if(key == NAV_ESC) goto CLEANUP_AND_EXIT;

                        // Nhấn DOWN để rời khỏi thanh tìm kiếm
                        if(key == NAV_DOWN && totalItems > 0) {
                            break; // Chỉ thoát vòng gõ phím
                        }
                        
                        int oldItems = totalItems;
                        bool changed = res.empty() ;
                        UiFindHK(res, key, inputChar, arrHK, totalItems);

                        if (oldItems != totalItems || key == NAV_BACK || (isdigit(inputChar) && key == NAV_UNKNOWN)) {
                            currentPage = 0; 
                            
                            startIndex = currentPage * itemsPerPage;
                            endIndex = min(startIndex + itemsPerPage, totalItems);
                            itemsOnThisPage = max(0, endIndex - startIndex);
                            totalPages = max(1, (totalItems + itemsPerPage - 1) / itemsPerPage);
                            
                            if(res.empty() && !changed) Render_HK_Page(arrHK, totalItems, currentPage, itemsPerPage); // Nếu xóa hết thì về trang đầu, nếu có kết quả mới thì cũng về trang đầu, chỉ khi đang tìm kiếm mà gõ thêm thì mới giữ nguyên trang
                            else if (!res.empty()) Render_HK_Page(arrHK, totalItems, currentPage, itemsPerPage);
                            
                            m = "Trang " + to_string(currentPage + 1) + "/" + to_string(totalPages) + "  |  Dùng phím ◄/► để chuyển trang, ▲/▼ đổi lựa chọn, ESC để thoát.";
                            Gotoxy(35, 8 + itemsPerPage + 1);
                            SmallBox(m, 50, 4, string(GRAY));
                        }
                    }
                    rowOnScreen = 0; 
                    current_Row = 0;
                }else if (ch == NAV_ENTER) {
                    if (mod == 0) {
                        Gotoxy(35, 8 + itemsPerPage + 1);
                        SmallBox("Đây Là Chế Độ In Danh Sách, Bạn Chỉ Có Quyền Xem Ở Chế Độ Này", 50, 4, string(RED));
                    }
                    if (mod == 1) {
                        listCB A;
						CopyMarkList(A, dsHK, arrHK[index]->get_cmnd());
                        It_list_CB(2, A, "", arrHK[index]->get_cmnd());
                        A.Clear();
                        goto start;
                    }
                } 
                else if (ch == NAV_ESC) {
                    goto CLEANUP_AND_EXIT;
                }
                if (currentPage != oldPage) break ;
            }
        }
    }

CLEANUP_AND_EXIT:
    if (arrHK != NULL) delete[] arrHK;
}



// Phần này thống kê đặc biệt không được để các hàm in danh sách thường ở gần đây
void StatiscalMB(){
    realTimeUpdateCB(dsCB, dsHK, dsMB);
    listMB dsMB_Stat = Get_Flight_Stats(dsMB);
    int field = 5 ;
    int Temp[] = {10,20,50,90,100};
    int Temp2[] ={10,30,40,10,15};
    string Typemode[] = {"In danh Sách","Hiệu Chỉnh","Xoá"};
    string headers[] = {"STT", "Số Hiệu Máy Bay", "Loại Máy Bay", "Số Chỗ", "Số Lần Bay"};
    PrintHeader("", 0, "Danh Sách Máy Bay", 5, headers, Typemode, Temp, Temp2,50,-1,97);
    int currentPage = 0;
    int current_Row = 0;
    while(true){
        int itemsPerPage = 15; // Ví dụ: màn hình chỉ chứa được 15 máy bay
        int startIndex = currentPage * itemsPerPage;
        int endIndex = startIndex + itemsPerPage;
        int rowOnScreen = 0 ;
        
        Render_MB_Page(dsMB_Stat, currentPage, itemsPerPage);

        int itemsOnThisPage = min(itemsPerPage, dsMB_Stat.slMB - (currentPage * itemsPerPage));
        if(itemsOnThisPage < 0) itemsOnThisPage = 0;

        //In thông tin điều hướng ở cuối
        Gotoxy(35, 8 + itemsPerPage + 1);
        int totalPages = (dsMB_Stat.slMB + itemsPerPage - 1) / itemsPerPage; // Tính tổng số trang
        if(totalPages == 0) totalPages = 1;
        string m = "Trang " + to_string(currentPage + 1) + "/" + to_string(totalPages) + "  |  Dùng phím ◄/► để chuyển trang, ▲/▼ đổi lựa chọn, ESC để thoát." ;
        if (itemsOnThisPage == 0) {
            NavKey ch = GetNavKey();
            if (ch == NAV_ESC) {
                return;
            }
            continue; 
        }
        SmallBox(m,50,4,string(GRAY));
        rowOnScreen = current_Row ;
        while(true){
            string s ;
            int index = rowOnScreen + startIndex;// rowOnScreen + startIndex -1
            Get_Input_MB(true, dsMB_Stat, rowOnScreen, startIndex, Temp, Temp2);
            Gotoxy(35, 8 + itemsPerPage + 1);
            SmallBox(m,50,4,string(GRAY));
            int oldPage = currentPage; 
            int oldRow = rowOnScreen;
            NavKey ch =  PaginationController(dsMB_Stat.slMB, itemsPerPage, currentPage, current_Row);
            Get_Input_MB(false, dsMB_Stat, rowOnScreen, startIndex, Temp, Temp2);
            rowOnScreen = current_Row;
            if (ch == NAV_ESC){
                return ;
            }
            if (currentPage != oldPage) break;
        }
    }

}

void FindCBOnTimeAndPlace(string res){
    int Y = 5 ; 
    int col = 1 ;
    int StartCalendarX = 64;
    int StartCalendarY = 1;
    int field = 5 ;
    int size = 4 ;
    int S = 6 ;
    int Temp[] = {0,10,40,50,0,0};
    int Temp2[] ={10,30,10,10,0,0};
    int Bin[] = {1,1,0,1,0,1};
    string typemode[] = {"Tìm Chuyến Bay Theo Ngày Khởi Hành/Sân Bay Đích",};
    //string headers[] = {"STT","Mã Chuyến Bay", "Ngày Khởi Hành", "Số Vé","Sân bay đích","Status"};
    string headers[] = {"STT","Mã Chuyến Bay", "Vé Trống","Status"};
    PrintHeader(res, 0, "Danh Sách Chuyến Bay", size, headers,typemode, Temp, Temp2,-1,-1,3,1);
    string q[] = {"","","",""};
    string Type[] = {RED, GREEN, YELLOW, CYAN};
    q[0] = Type[0] + SQ + RESET + ": Chuyến bay bị huỷ " ;
    q[1] = Type[1] + SQ + RESET + ": Chuyến bay còn vé " ;
    q[2] = Type[2] + SQ + RESET + ": Chuyến bay hết vé " ;
    q[3] = Type[3] + SQ + RESET + ": Chuyến bay hoàn tất " ;
    Gotoxy(63,19);
    string u = "  " + q[0] + q[1] +"    " +q[2] + q[3];
    SmallBox(u,50,2);
    DateTime t ;
    string add = "";
    int slCB ;
    realTimeUpdateCB(dsCB, dsHK, dsMB);
    CB** CBSearch = Search_CB(dsCB,t,const_cast<char*>(add.c_str()),slCB);
    string ti = time_now();
    int currentPage = 0;
    int current_Row = 0;
    const int itemsPerPage = 12; // Ví dụ: màn hình chỉ chứa được 12 CB
    UiDrawCalendarWithoutTime(StartCalendarX,StartCalendarY,8,col,time_now());
    Gotoxy(StartCalendarX,  col*4+13);
    string v =  "Tìm Kiếm Sân Bay :";
    cout << v;
    Gotoxy(StartCalendarX+visualLength(v),  col*4+12);
    SmallBox("",25,1);
    Gotoxy(0,21);
    SmallBox("",50,7);
    Gotoxy(0,19);
    SmallBox("Thông Tin Chuyến Bay",50,1,(string)YELLOW);
    Gotoxy(63,23);
    SmallBox("ESC thoát "+ string(DM) +" Nhấn D để chuyển sang chọn ngày " + string(DM) + " Backspace để bỏ qua field chọn ngày",50,2,string(GRAY));
    Gotoxy(65,12);
    cout << GRAY<< "D tháng sau || S tháng trước ||  Dùng phím ◄/► để chọn ngày trong tháng."<< RESET ;
    while(true){
         // Thiết lập thông số phân trang
        
        int startIndex = currentPage * itemsPerPage;
        int endIndex = startIndex + itemsPerPage;
        
        // Đảm bảo không in lố số lượng CB thực tế
        if (endIndex > slCB) {
            endIndex = slCB;
        }
        int itemsOnThisPage = endIndex - startIndex;
        int rowOnScreen = 0;

        int WidthClear = 0;
        //In thông tin điều hướng ở cuối
        int totalPages = (slCB == 0) ? 1 : (slCB + itemsPerPage - 1) / itemsPerPage;
        for (int i = 0; i < size; i++) WidthClear += Temp2[i];
        ClearRegion(0, Y + itemsOnThisPage, WidthClear, 14 - itemsOnThisPage);

        string m = "Trang " + to_string(currentPage + 1) + "/" + to_string(totalPages) + "  |  Dùng phím ◄/► để chuyển trang.";
        Gotoxy(63, 27);
        SmallBox(m, 50, 1, string(GRAY));

        Render_CB_Page(CBSearch,slCB,Y,25,8,currentPage,itemsPerPage,S,Temp,Temp2,Bin);
        if (slCB == 0) {
            Gotoxy(15, Y);
            cout << RED << "Không có dữ liệu chuyến bay nào!" << RESET;
            Gotoxy(5, Y + 1);
            cout << RED << "Nhấn Backspace để bỏ qua ngày hoặc chọn một ngày khác!" << RESET;
        }
        DrawACat(53,24);

        // LƯU CÁC NODE CỦA TRANG NÀY VÀO MẢNG TẠM
        CB* pageNodes[itemsPerPage]; 
        for (int k = 0; k < itemsOnThisPage; k++) {
            // Truy cập thẳng vào mảng kết quả tìm kiếm bằng index
            pageNodes[k] = CBSearch[startIndex + k]; 
        }

        rowOnScreen = current_Row ;

        while(true){
            int key ;
            NavKey ch;
            int oldPage = currentPage; 
            int oldRow = rowOnScreen;
            //PaginationController
            if(slCB > 0){
                string s ;
                int index = rowOnScreen + startIndex;// rowOnScreen + startIndex
                CB* SelectedNode = pageNodes[rowOnScreen]; // Đây là chính là chuyến đang 
                string Box[] = { to_string(index+1),SelectedNode->maCB,to_string(SelectedNode->socho - SelectedNode->sove),TranFormSatus(SelectedNode->trangThai)};
                for(int i = 0; i < size ;i++){
                    Gotoxy(Temp[i],Y + rowOnScreen);
                    s = Box[i];
                    if (i == 3){
                        SmallBox(SQ,false, false,true, true, Temp2[i],1,s);
                    }
                    if(i!= 3)SmallBox(s,false, false,true, true, Temp2[i],1,HIGHLIGHT);
                }
                UiThongTinCB(2,18,47,7,SelectedNode->maCB);
                ch =  PaginationController(slCB, itemsPerPage, currentPage, current_Row,key);

                for(int i = 0; i < size ;i++){
                    Gotoxy(Temp[i],Y + rowOnScreen);
                    s = Box[i];
                    if (i == 3){
                        SmallBox(SQ,false, false,true, true, Temp2[i],1,s);
                    }
                    if(i!= 3)SmallBox(s,false, false,true, true, Temp2[i],1);
                }
            }
            rowOnScreen = current_Row;
            if (ch == NAV_UP && oldRow == 0) {
                if (currentPage > 0) {
                    currentPage--; 
                    current_Row = itemsPerPage - 1;
                }
            }else if (ch == NAV_ESC){
                delete[] CBSearch;
                return ;
            }else if(ch == NAV_UNKNOWN && tolower(key) == 'd' ){
                string old = ti;
                string r = "";
                NavKey k = NAV_LEFT;
                DateTime T ;
                // Vòng lặp do-while giữ Lịch không bị tắt khi đổi tháng HOẶC khi chọn sai ngày
                while (k == NAV_LEFT || k == NAV_RIGHT){
                    ClearRegion(StartCalendarX, StartCalendarY, 8*6 + 7, 13); // Xóa sạch không gian trước khi vẽ
                    k = DrawCalendarWithoutTime(StartCalendarX, StartCalendarY, 8, 1, ti,true);
                    // Nếu người dùng nhấn ENTER để chốt ngày, ta tiến hành kiểm tra
                }
                if(k == NAV_ENTER)  T = StringtoTime(ti);
                if(k == NAV_BACK) ti = old;
                if(k == NAV_ESC){
                    ti = old ;
                    break;
                }
                InputStringSBD(r,StartCalendarX+visualLength(v)+1,  col*4 + 13,23,' ');
                RedrawInput(r,StartCalendarX+visualLength(v)+1,  col*4 + 13,23,' ',false,false);
                delete[] CBSearch;
                realTimeUpdateCB(dsCB, dsHK, dsMB);
                CBSearch = Search_CB(dsCB,T,const_cast<char*>(r.c_str()),slCB);
                currentPage = 0;
                current_Row = 0;
                break;
            }
            if (currentPage != oldPage) break;
        }
    }
    
}

void HuyChuyen() {
    int width = 120;
    int width1 = 80;
    int width2 = 39;
    int height = 28;
    int height2 = 9;
    int startX = (GetTerminalWidth() - width) / 2;
    int formY = 2;
    int startY = 0;
    start:
    ClearScreen();
	SmallBox("", width, height, string(WHITE));
    string t = "HỦY VÉ THEO CHUYẾN BAY";
	string s = "HỦY VÉ THEO HÀNH KHÁCH";
	Gotoxy(2, 1);
	SmallBox(t, 120 - 4, 12, string(YELLOW));
    Gotoxy(2, 15);
    SmallBox(s, 120 - 4, 12, string(WHITE));

    Gotoxy(4, 3);
    cout << string(112, '-');
    Gotoxy(4, 12);
    cout << string(112, '-');

    Gotoxy(4, 3 + 14);
    cout << string(112, '-');
    Gotoxy(4, 12 + 14);
    cout << string(112, '-');
    int select = 1;
    while (true) {
        int k = GetNavKey();
        if (k == NAV_DOWN) {
            select++;
            if (select > 2) select = 1;
        }
        else if (k == NAV_UP) {
            select--;
            if (select < 1) select = 2;
        }
        if (select == 1) {
            Gotoxy(2, 1);
            SmallBox(t, 120 - 4, 12, string(YELLOW));
            Gotoxy(2, 15);
            SmallBox(s, 120 - 4, 12, string(WHITE));
        }
        else {
			Gotoxy(2, 1);
			SmallBox(t, 120 - 4, 12, string(WHITE));
			Gotoxy(2, 15);
			SmallBox(s, 120 - 4, 12, string(YELLOW));
        }
        if (k == NAV_ENTER) {
			if (select == 1) CancelTicketByCB();
			else CancelTicketByHK(1, "");
            goto start;
        }
        else
        if (k == NAV_ESC) {
            ClearScreen();
            return;
        }
        Gotoxy(4, 3);
        cout << string(112, '-');
        Gotoxy(4, 12);
        cout << string(112, '-');

        Gotoxy(4, 3 + 14);
        cout << string(112, '-');
        Gotoxy(4, 12 + 14);
        cout << string(112, '-');
    }
}

NavKey SeatClear(int x, int y, int xt, int yt, CB* SelectedCB) {
    int seatCount = SelectedCB->socho;
    int stage = (seatCount / 60); // Số tầng ghế (mỗi tầng 60 ghế)
    if (seatCount % 60 == 0) stage--; // Nếu số ghế là bội số của 60 thì giảm đi 1 tầng vì tầng cuối sẽ không có ghế nào
    int stageSelected = 0;
    int startXT = xt;
    int startYT = yt;
    int action;
selectStage:
    while (true) {
        int tempX = startXT, tempY = startYT;
        Gotoxy(tempX - 5, tempY);
        SmallBox("IN", (int)6, (int)1, (string)YELLOW);
        Gotoxy(tempX + 64, tempY);
        SmallBox("OUT", (int)7, (int)1, (string)YELLOW);
        for (int i = 0; i < 10; i++) {
            Gotoxy(tempX + 3, tempY);
            SmallBox("", (int)5, (int)1, (string)WHITE);
            Gotoxy(tempX + 5, tempY + 1);
            if (stageSelected == i)  cout << HIGHLIGHT << char('A' + i) << RESET;
            else if (i <= stage) cout << GREEN << char('A' + i) << RESET;
            else cout << RED << char('A' + i) << RESET;
            tempX = tempX + 6;
            int startSeat = stageSelected * 60;
            int seatNumber = min(60, seatCount - (stageSelected * 60));
            int tempX = x;
            int tempY = y - 1;
            int countRow = 0;
            int row = 1;
            ClearRegion(x - 1, y - 1, 77, 9); // Xóa vùng ghế cũ trước khi vẽ lại
            Gotoxy(tempX, tempY + 3);
            cout << WHITE << string(73, '-') << RESET;
            Gotoxy(tempX + 10, tempY + 4);
            cout << WHITE << "  L  Ố  I    Đ  I                 L  Ố  I    Đ  I  " << RESET;
            Gotoxy(tempX, tempY + 5);
            cout << WHITE << string(73, '-') << RESET;
            for (int i = 0; i < seatNumber; i++) {
                if (countRow == 15) {
                    tempX = x;
                    tempY = tempY + 2;
                    countRow = 0;
                    row++;
                    if (row == 3) tempY += 2;
                }
                Gotoxy(tempX + 5 * countRow, tempY);
                if (SelectedCB->DSV[startSeat + i][0] != '\0') cout << (startSeat + i + 1);
                else cout << GRAY << (startSeat + i + 1) << RESET;
                countRow++;
            }
        }
        action = GetNavKey();
        if (action == NAV_ESC) {
            ClearRegion(x - 1, y - 4, 77, 12);
            return NAV_ESC;
        }
        if (action == NAV_LEFT) {
            stageSelected--;
            if (stageSelected < 0) stageSelected = stage;
            continue;
        }
        if (action == NAV_RIGHT) {
            stageSelected++;
            if (stageSelected > stage) stageSelected = 0;
            continue;
        }
        if (action == NAV_ENTER || action == NAV_DOWN) {
            break;
        }
    }
    int startSeat = stageSelected * 60;
    int selectedSeat = stageSelected * 60;
    int seatNumber = min(60, seatCount - (stageSelected * 60));
    while (true) {
        int tempX = x;
        int tempY = y - 1;
        int selectedX;
        int selectedY;
        int countRow = 0;
        int row = 1;
        if (SelectedCB->DSV[selectedSeat][0] != '\0') {
            UiThongTinHK(84, 22, 35, 7, SelectedCB->DSV[selectedSeat]);
        }
		else {
			ClearRegion(84, 22, 35, 7);
		}
        for (int i = 0; i < seatNumber; i++) {
            if (countRow == 15) {
                tempX = x;
                tempY = tempY + 2;
                countRow = 0;
                row++;
                if (row == 3) tempY += 2;
            }
            Gotoxy(tempX + 5 * countRow, tempY);
            if (startSeat + i == selectedSeat && SelectedCB->DSV[startSeat + i][0] != '\0') {
                Gotoxy(tempX + 5 * countRow - 1, tempY);
                cout << "[" << (startSeat + i + 1) << "]";
                selectedX = tempX + 5 * countRow - 1;
                selectedY = tempY;
            }
            else if (startSeat + i == selectedSeat && SelectedCB->DSV[startSeat + i][0] == '\0') {
                Gotoxy(tempX + 5 * countRow - 1, tempY);
                cout << GRAY << "[" << (startSeat + i + 1) << "]" << RESET;
                selectedX = tempX + 5 * countRow - 1;
                selectedY = tempY;
            }
           
            countRow++;
        }
        int action = GetNavKey();
        if (action == NAV_ENTER) {
			if (SelectedCB->DSV[selectedSeat][0] != '\0') {
                ClearRegion(tempX, tempY - 4, 75, 1); 
                if (SelectedCB->trangThai == 0 || SelectedCB->trangThai == 3) {
                    Gotoxy(tempX, tempY - 4);
					string s = "Chuyến bay đã bị hủy hoặc hoàn tất, không thể hủy vé của ghế " + to_string(selectedSeat + 1);
					cout << RED << s << RESET;
                    _getch();
                    ClearRegion(tempX, tempY - 4, 75, 1);
                    Gotoxy(tempX + 10, tempY - 4);
                    cout << WHITE << "  L  Ố  I    Đ  I                 L  Ố  I    Đ  I  " << RESET;
                    continue;
                }
                DateTime now = StringtoTime(time_now());
                if (ss_ngay(SelectedCB->ngayKH, now) <= 60 && isNotPastTime(SelectedCB->ngayKH, now)) {
                    Gotoxy(tempX, tempY - 4);
                    string s = "Không thể hủy vé của ghế " + to_string(selectedSeat + 1) + " vì chuyến bay sắp khởi hành";
                    cout << RED << s << RESET;
                    _getch();
                    ClearRegion(tempX, tempY - 4, 75, 1);
                    Gotoxy(tempX + 10, tempY - 4);
                    cout << WHITE << "  L  Ố  I    Đ  I                 L  Ố  I    Đ  I  " << RESET;
                    continue;
                }
                if (ss_ngay(SelectedCB->ngayKH, now) != 0 && !isNotPastTime(SelectedCB->ngayKH, now)) {
                    Gotoxy(tempX, tempY - 4);
                    string s = "Không thể hủy vé của ghế " + to_string(selectedSeat + 1) + " vì chuyến bay đã qua giờ khởi hành";
                    cout << RED << s << RESET;
                    _getch();
                    ClearRegion(tempX, tempY - 4, 75, 1);
                    Gotoxy(tempX + 10, tempY - 4);
                    cout << WHITE << "  L  Ố  I    Đ  I                 L  Ố  I    Đ  I  " << RESET;
                    continue;
                }
                Gotoxy(tempX + 5, tempY - 4);
				string s = "Bạn có muốn hủy vé của ghế " + to_string(selectedSeat + 1) + " không? ENTER: Đồng ý | ESC: Hủy";
                cout << YELLOW << s << RESET;
				int k = GetNavKey();
                if (k == NAV_ENTER) {
					Cancel_Ticket(dsCB, dsHK, SelectedCB->maCB, selectedSeat + 1);
                    ClearRegion(tempX, tempY - 4, 75, 1);
					string s = "Đã hủy vé của ghế " + to_string(selectedSeat + 1) + " thành công! Nhấn phím bất kỳ để tiếp tục...";
                    Gotoxy(tempX, tempY - 4);
                    cout << GREEN << s << RESET;
                    _getch();
                    ClearRegion(84, 22, 35, 7); // Xoa thong tin hanh khach
                    ClearRegion(tempX, tempY - 4, 75, 1);
                    Gotoxy(tempX + 10, tempY - 4);
                    cout << WHITE << "  L  Ố  I    Đ  I                 L  Ố  I    Đ  I  " << RESET;
                    continue;
                }
                else if (k != NAV_ENTER){
                    ClearRegion(tempX, tempY - 4, 75, 1);
                    Gotoxy(tempX + 10, tempY - 4);
                    cout << WHITE << "  L  Ố  I    Đ  I                 L  Ố  I    Đ  I  " << RESET;
                    continue;
                }
			}
        }
        ClearRegion(selectedX, selectedY, 5, 1);
        Gotoxy(selectedX + 1, selectedY);
        if (SelectedCB->DSV[selectedSeat][0] == '\0') {
            cout << GRAY << (selectedSeat + 1) << RESET;
        }
        else cout << (selectedSeat + 1);
        if (action == NAV_ESC) {
            ClearRegion(84, 22, 35, 7);
            goto selectStage;
        }
        if (action == NAV_RIGHT) {
            selectedSeat++;
            if (selectedSeat > startSeat + seatNumber - 1) selectedSeat = startSeat;
        }
        if (action == NAV_LEFT) {
            selectedSeat--;
            if (selectedSeat < startSeat) selectedSeat = startSeat + seatNumber - 1;
        }
        if (action == NAV_UP) {
            int old = selectedSeat;
            selectedSeat -= 15;
            if (selectedSeat < startSeat) goto selectStage;
        }
        if (action == NAV_DOWN) {
            int old = selectedSeat;
            selectedSeat += 15;
            if (selectedSeat > startSeat + seatNumber - 1) selectedSeat = old;
        }
    }
    return NAV_ENTER;
}
void CancelTicketByCB() {
start:
    realTimeUpdateCB(dsCB, dsHK, dsMB);
    int width = 120;
    int width1 = 80;
    int width2 = 39;
    int height = 28;
    int height2 = 9;
    int startX = (GetTerminalWidth() - width) / 2;
    int formY = 2;
    int startY = 0;
    ClearScreen();


    Gotoxy(startX + width1 + 1, formY - 1);
    SmallBox("THÔNG TIN CHUYẾN BAY", (int)width2, (int)1, (string)YELLOW);

    Gotoxy(startX + width1 + 1, formY + 2);
    SmallBox("", (int)width2, (int)height2 + 1, (string)WHITE);

    Gotoxy(startX + width1 + 1, formY + height2 + 5);
    SmallBox("THÔNG TIN KHÁCH HÀNG", (int)width2, (int)1, (string)YELLOW);

    Gotoxy(startX + width1 + 1, formY + height2 + 8);
    SmallBox("", (int)width2, (int)height2, (string)WHITE);

    Gotoxy(0, formY + height2 + 5);
    SmallBox("", (int)width1, (int)height2 + 3, (string)WHITE);

    Gotoxy(0, formY + height2 + 2);
    SmallBox("◄/► : Sang Trái / Sang Phải | ▲/▼ : Đi Lên / Đi Xuống | ESC : Thoát", (int)width1, (int)1, (string)GRAY);

    int Y = 4;
    int col = 1;
    int StartCalendarX = 64;
    int StartCalendarY = 1;
    int field = 5;
    int size = 4;
    int S = 6;
    int Temp[] = { 0,10,40,0,0,0 };
    int Temp2[] = { 10,30,10,0,0,0 };
    int Bin[] = { 1,1,0,0,0,1 };

    string typemode[] = { "Kiểm Tra Danh Sách Vé Và Hủy Vé", };
    string headers[] = { "STT","Mã Chuyến Bay","Status","" };
    PrintHeader("", 0, "Danh Sách Chuyến Bay", size, headers, typemode, Temp, Temp2, -1, -1, 1, 0);
    string q[] = { "","","","" };
    string Type[] = { RED, GREEN, YELLOW, CYAN };
    q[0] = Type[0] + SQ + RESET + ": Chuyến bay bị huỷ";
    q[1] = Type[1] + SQ + RESET + ": Chuyến bay còn vé";
    q[2] = Type[2] + SQ + RESET + ": Chuyến bay hết vé";
    q[3] = Type[3] + SQ + RESET + ": Chuyến bay hoàn tất";
    ClearRegion(startX + width1 - 30, formY, 10, 8);
    Gotoxy(startX + width1 - 30, formY - 1);
    SmallBox("", 30, 6, string(WHITE));
    Gotoxy(startX + width1 - 26, formY + 1);
    cout << q[0];
    Gotoxy(startX + width1 - 26, formY + 2);
    cout << q[1];
    Gotoxy(startX + width1 - 26, formY + 3);
    cout << q[2];
    Gotoxy(startX + width1 - 26, formY + 4);
    cout << q[3];

    const int itemsPerPage = 6;
    int currentPage = 0;

    int slCB = 0;
    DateTime dateTemp = DateTime();
    char Address[sbDich_max];
    Address[0] = '\0';

    string input;
    int action;
    listCB subListCB;

    CB** subCB = new CB * [dsCB.slCB];
    CB* tempCB = dsCB.head;
    while (tempCB != NULL) {
        subCB[slCB] = tempCB;
        slCB++;
        tempCB = tempCB->next;
    }

    Render_CB_Page(subCB, slCB, Y, 8, 8, currentPage, itemsPerPage, S, Temp, Temp2, Bin);
    delete[] subCB;
    slCB = 0;

    int selectedCBIndex = 0;
    int currentRow = 0;
    while (true) {
    startS:
        slCB = 0;
        char Prefix[maCB_max];
        string input;
        Gotoxy(startX + width1 - 30, formY + 8);
        cout << " Tìm Kiếm: ";
        Gotoxy(startX + width1 - 20, formY + 7);
        SmallBox("", 20, 1, string(WHITE));
        Gotoxy(startX + width1 - 18, formY + 8);
        int curX = whereX();
        int curY = whereY();
    nhap:
        while (true) {
            currentPage = 0;
            selectedCBIndex = 0;
            currentRow = 0;
            Gotoxy(curX, curY);
            int ch = _getch();
            if (ch == 0 || ch == 224) {
                ch = _getch();
            }
            else if (ch == 13) break;
            else if (ch == 27) {
                CB* tempCB = subListCB.head;
                while (tempCB != NULL) {
                    CB* toDelete = tempCB;
                    tempCB = tempCB->next;
                    delete toDelete;
                }
                ClearScreen();
                return;
            }
            else if (input.size() < maCB_max - 1 && (ch >= '0' && ch <= '9' || ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z' || ch == '-' )) {
                if ((input.size() > 0 && input[input.size() - 1] == '-' && ch == '-') || (input.size() == 0 && ch == '-')) continue;
                input += (char)ch;
                Gotoxy(startX + width1 - 18, formY + 8);
                cout << input;
                curX = whereX(); curY = whereY();

                strcpy(Prefix, input.c_str());
                subListCB = Find_CB_OnRage(dsCB, Prefix);
                subCB = new CB * [subListCB.slCB];
                CB* tempCB = subListCB.head;
                while (tempCB != NULL) {
                    subCB[slCB] = tempCB;
                    slCB++;
                    tempCB = tempCB->next;
                }

                ClearRegion(0, Y, 50, 8);
                Render_CB_Page(subCB, slCB, Y, 8, 8, currentPage, itemsPerPage, S, Temp, Temp2, Bin);
                delete[] subCB;
                slCB = 0;
            }
            else if (ch == 8 && !input.empty()) {
                Gotoxy(whereX() - 1, whereY());
                cout << " ";
                input.pop_back();
                Gotoxy(startX + width1 - 18, formY + 8);
                cout << input;
                curX = whereX(); curY = whereY();

                strcpy(Prefix, input.c_str());
                subListCB = Find_CB_OnRage(dsCB, Prefix);
                subCB = new CB * [subListCB.slCB];
                CB* tempCB = subListCB.head;
                while (tempCB != NULL) {
                    subCB[slCB] = tempCB;
                    slCB++;
                    tempCB = tempCB->next;
                }

                ClearRegion(0, Y, 50, 8);
                Render_CB_Page(subCB, slCB, Y, 8, 8, currentPage, itemsPerPage, S, Temp, Temp2, Bin);
                delete[] subCB;
                slCB = 0;
            }
        }

        strcpy(Prefix, input.c_str());
        subListCB = Find_CB_OnRage(dsCB, Prefix);

        subCB = new CB* [subListCB.slCB];
        CB* tempCB = subListCB.head;
        while (tempCB != NULL) {
            subCB[slCB] = tempCB;
            slCB++;
            tempCB = tempCB->next;
        }

        while (true) {
            ClearRegion(0, Y, 50, 8);
            Render_CB_Page(subCB, slCB, Y, 8, 8, currentPage, itemsPerPage, S, Temp, Temp2, Bin);
            Gotoxy(Temp[0], Y + currentRow);
            SmallBox(to_string(selectedCBIndex + 1), false, false, true, true, Temp2[0], 1, HIGHLIGHT);
            Gotoxy(Temp[1], Y + currentRow);
            SmallBox(subCB[selectedCBIndex]->maCB, false, false, true, true, Temp2[1], 1, HIGHLIGHT);

            UiThongTinCB(startX + width1 + 3, formY, 35, 7, subCB[selectedCBIndex]->maCB);
            action = GetNavKey();
            if (action == NAV_UP) {
                currentRow--;
                if (currentRow < 0) {
                    currentRow = 0;
                    continue;
                }
                selectedCBIndex--;
                if (selectedCBIndex < 0) selectedCBIndex = 0;
                continue;
            }
            else if (action == NAV_DOWN) {
                currentRow++;
                selectedCBIndex++;
                if (currentRow >= itemsPerPage || selectedCBIndex >= slCB) {
                    if (selectedCBIndex >= slCB) {
                        selectedCBIndex = 0;
                        currentRow = 0;
                        currentPage = 0;
                        continue;
                    }
                    if (currentPage + 1 < (slCB + itemsPerPage - 1) / itemsPerPage) {
                        currentPage++;
                        currentRow = 0;
                        continue;
                    }
                }
            }
            else if (action == NAV_RIGHT) {
                if (currentPage + 1 < (slCB + itemsPerPage - 1) / itemsPerPage) {
                    currentPage++;
                    currentRow = 0;
                }
                else {
                    currentPage = 0;
                    currentRow = 0;
                }
                selectedCBIndex = currentPage * itemsPerPage + currentRow;
                continue;
            }
            else if (action == NAV_LEFT) {
                if (currentPage - 1 >= 0) {
                    currentPage--;
                    currentRow = 0;
                }
                else {
                    currentPage = (slCB + itemsPerPage - 1) / itemsPerPage - 1;
                    currentRow = 0;
                }
                selectedCBIndex = currentPage * itemsPerPage + currentRow;
                continue;
            }
            else if (action == NAV_ESC) {
                delete[] subCB;
                slCB = 0;
                goto nhap;
            }
            else {
                break;
            }
        }
        CB* targetCB = Find_CB(dsCB, subCB[selectedCBIndex]->maCB);
        SeatClear(3, formY + height2 + 10, 7, formY + height2 + 6, targetCB);
        delete[] subCB;
        slCB = 0;
        goto nhap;
    }

}
void CancelTicketByHK(int mod, string res) {
    It_list_HK(mod, res);
}

NavKey SeatList(int x, int y, int xt, int yt, CB* SelectedCB) {
    int seatCount = SelectedCB->socho;
    int stage = (seatCount / 60); // Số tầng ghế (mỗi tầng 60 ghế)
    if (seatCount % 60 == 0) stage--; // Nếu số ghế là bội số của 60 thì giảm đi 1 tầng vì tầng cuối sẽ không có ghế nào
    int stageSelected = 0;
    int startXT = xt;
    int startYT = yt;
    int action;
selectStage:
    while (true) {
        int tempX = startXT, tempY = startYT;
        Gotoxy(tempX - 5, tempY);
        SmallBox("IN", (int)6, (int)1, (string)YELLOW);
        Gotoxy(tempX + 64, tempY);
        SmallBox("OUT", (int)7, (int)1, (string)YELLOW);
        for (int i = 0; i < 10; i++) {
            Gotoxy(tempX + 3, tempY);
            SmallBox("", (int)5, (int)1, (string)WHITE);
            Gotoxy(tempX + 5, tempY + 1);
            if (stageSelected == i)  cout << HIGHLIGHT << char('A' + i) << RESET;
            else if (i <= stage) cout << GREEN << char('A' + i) << RESET;
            else cout << RED << char('A' + i) << RESET;
            tempX = tempX + 6;
            int startSeat = stageSelected * 60;
            int seatNumber = min(60, seatCount - (stageSelected * 60));
            int tempX = x;
            int tempY = y - 1;
            int countRow = 0;
            int row = 1;
            ClearRegion(x - 1, y - 1, 77, 9); // Xóa vùng ghế cũ trước khi vẽ lại
            Gotoxy(tempX, tempY + 3);
            cout << WHITE << string(73,'-') << RESET;
            Gotoxy(tempX + 10, tempY + 4);
            cout << WHITE << "  L  Ố  I    Đ  I                 L  Ố  I    Đ  I  " << RESET;
            Gotoxy(tempX, tempY + 5);
            cout << WHITE << string(73, '-') << RESET;
            for (int i = 0; i < seatNumber; i++) {
                if (countRow == 15) {
                    tempX = x;
                    tempY = tempY + 2;
                    countRow = 0;
                    row++;
                    if (row == 3) tempY+=2;
                }
                Gotoxy(tempX + 5 * countRow, tempY);
                if (SelectedCB->DSV[startSeat + i][0] != '\0') cout << (startSeat + i + 1);
                else cout << GRAY << (startSeat + i + 1) << RESET;
                countRow++;
            }
        }
        action = GetNavKey();
        if (action == NAV_ESC) {
            ClearRegion(x - 1, y - 4, 77, 12);
            return NAV_ESC;
        }
        if (action == NAV_LEFT) {
            stageSelected--;
            if (stageSelected < 0) stageSelected = stage;
            continue;
        }
        if (action == NAV_RIGHT) {
            stageSelected++;
            if (stageSelected > stage) stageSelected = 0;
            continue;
        }
        if (action == NAV_ENTER || action == NAV_DOWN) {
            break;
        }
    }
    int startSeat = stageSelected * 60;
    int selectedSeat = stageSelected * 60;
    int seatNumber = min(60, seatCount - (stageSelected * 60));
    while (true) {
        int tempX = x;
        int tempY = y - 1;
        int selectedX;
        int selectedY;
        int countRow = 0;
        int row = 1;
        if (SelectedCB->DSV[selectedSeat][0] != '\0') {
            UiThongTinHK(84, 22, 35, 7, SelectedCB->DSV[selectedSeat]);
        }
        else {
			ClearRegion(84, 22, 35, 7);
        }
        // In danh sach ghe
        for (int i = 0; i < seatNumber; i++) {
            if (countRow == 15) {
                tempX = x;
                tempY = tempY + 2;
                countRow = 0;
                row++;
                if (row == 3) tempY+=2;
            }
            Gotoxy(tempX + 5 * countRow, tempY);
            if (startSeat + i == selectedSeat && SelectedCB->DSV[startSeat + i][0] != '\0') {
                Gotoxy(tempX + 5 * countRow - 1, tempY);
                cout << "[" << (startSeat + i + 1) << "]";
                selectedX = tempX + 5 * countRow - 1;
                selectedY = tempY;
            }
            else if (startSeat + i == selectedSeat && SelectedCB->DSV[startSeat + i][0] == '\0') {
                Gotoxy(tempX + 5 * countRow - 1, tempY);
                cout << GRAY << "[" << (startSeat + i + 1) << "]" << RESET;
                selectedX = tempX + 5 * countRow - 1;
                selectedY = tempY;
            }
          
            countRow++;
        }
    seatselect:
        int action = GetNavKey();
        ClearRegion(selectedX, selectedY, 5, 1);
        Gotoxy(selectedX + 1, selectedY);
        if (SelectedCB->DSV[selectedSeat][0] == '\0') {
            cout << GRAY << (selectedSeat + 1) << RESET;
        }
		else cout << (selectedSeat + 1);
        if (action == NAV_ESC) {
            ClearRegion(84, 22, 35, 7);
            goto selectStage;
        }
        if (action == NAV_RIGHT) {
            selectedSeat++;
            if (selectedSeat > startSeat + seatNumber - 1) selectedSeat = startSeat;
        }
        if (action == NAV_LEFT) {
            selectedSeat--;
            if (selectedSeat < startSeat) selectedSeat = startSeat + seatNumber - 1;
        }
        if (action == NAV_UP) {
            int old = selectedSeat;
            selectedSeat -= 15;
            if (selectedSeat < startSeat) goto selectStage;
        }
        if (action == NAV_DOWN) {
            int old = selectedSeat;
            selectedSeat += 15;
            if (selectedSeat > startSeat + seatNumber - 1) selectedSeat = old;
        }
    }
    return NAV_ENTER;
}

// Hàm hiện thị danh sách ghế trống của chuyến bay
void EmptySeatList() {
start:
    int width = 120;
    int width1 = 80;
    int width2 = 39;
    int height = 28;
    int height2 = 9;
    int startX = (GetTerminalWidth() - width) / 2;
    int formY = 2;
    int startY = 0;
    ClearScreen();


    Gotoxy(startX + width1 + 1, formY - 1);
    SmallBox("THÔNG TIN CHUYẾN BAY", (int)width2, (int)1, (string)YELLOW);

    Gotoxy(startX + width1 + 1, formY + 2);
    SmallBox("", (int)width2, (int)height2 + 1, (string)WHITE);

    Gotoxy(startX + width1 + 1, formY + height2 + 5);
    SmallBox("THÔNG TIN KHÁCH HÀNG", (int)width2, (int)1, (string)YELLOW);

    Gotoxy(startX + width1 + 1, formY + height2 + 8);
    SmallBox("", (int)width2, (int)height2, (string)WHITE);

    Gotoxy(0, formY + height2 + 5);
    SmallBox("", (int)width1, (int)height2 + 3, (string)WHITE);

    Gotoxy(0, formY + height2 + 2);
    SmallBox("◄/► : Sang Trái / Sang Phải | ▲/▼ : Đi Lên / Đi Xuống | ESC : Thoát", (int)width1, (int)1, (string)GRAY);

    int Y = 4;
    int col = 1;
    int StartCalendarX = 64;
    int StartCalendarY = 1;
    int field = 5;
    int size = 4;
    int S = 6;
    int Temp[] = { 0,10,40,0,0,0 };
    int Temp2[] = { 10,30,10,0,0,0 };
    int Bin[] = { 1,1,0,0,0,1 };

    string typemode[] = { "Kiểm Tra Danh Sách Vé Trống Của Chuyến Bay", };
    string headers[] = { "STT","Mã Chuyến Bay","Status",""};
    PrintHeader("", 0, "Danh Sách Chuyến Bay", size, headers, typemode, Temp, Temp2, -1, -1, 1, 0);
    string q[] = { "","","","" };
    string Type[] = { RED, GREEN, YELLOW, CYAN };
    q[0] = Type[0] + SQ + RESET + ": Chuyến bay bị huỷ";
    q[1] = Type[1] + SQ + RESET + ": Chuyến bay còn vé";
    q[2] = Type[2] + SQ + RESET + ": Chuyến bay hết vé";
    q[3] = Type[3] + SQ + RESET + ": Chuyến bay hoàn tất";
	ClearRegion(startX + width1 - 30, formY, 10, 8);
    Gotoxy(startX + width1 - 30, formY - 1);
	SmallBox("", 30, 6, string(WHITE));
    Gotoxy(startX + width1 - 26, formY + 1);
    cout << q[0];
    Gotoxy(startX + width1 - 26, formY + 2);
    cout << q[1];
    Gotoxy(startX + width1 - 26, formY + 3);
    cout << q[2];
	Gotoxy(startX + width1 - 26, formY + 4);
    cout << q[3];

    const int itemsPerPage = 6;
    int currentPage = 0;

    int slCB = 0;
    DateTime dateTemp = DateTime();
    char Address[sbDich_max];
    Address[0] = '\0';

    string input;
    int action;
    listCB subListCB;

    CB** subCB = new CB *[dsCB.slCB];
    CB* tempCB = dsCB.head;
    while (tempCB != NULL) {
        subCB[slCB] = tempCB;
        slCB++;
        tempCB = tempCB->next;
    }

    Render_CB_Page(subCB, slCB, Y, 8, 8, currentPage, itemsPerPage, S, Temp, Temp2, Bin);
    delete[] subCB;
    slCB = 0;

    int selectedCBIndex = 0;
    int currentRow = 0;
    while (true) {
    startS:
        slCB = 0;
        char Prefix[maCB_max];
        string input;
        Gotoxy(startX + width1 - 30, formY + 8);
        cout << " Tìm Kiếm: ";
        Gotoxy(startX + width1 - 20, formY + 7);
		SmallBox("", 20, 1, string(WHITE));
        Gotoxy(startX + width1 - 18, formY + 8);
        int curX = whereX();
        int curY = whereY();
    nhap:
        while (true) {
            currentPage = 0;
            selectedCBIndex = 0;
            currentRow = 0;
            Gotoxy(curX, curY);
			int ch = _getch();
            if (ch == 0 || ch == 224) {
                ch = _getch();
            }
            else if (ch == 13) break;
            else if (ch == 27) {
                CB* tempCB = subListCB.head;
                while (tempCB != NULL) {
                    CB* toDelete = tempCB;
                    tempCB = tempCB->next;
                    delete toDelete;
                }
                ClearScreen();
                return;
            }
            else if (input.size() < maCB_max - 1 && ( ch >= '0' && ch <= '9' || ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z' || ch == '-' )) {
                if ((input.size() > 0 && input[input.size() - 1] == '-' && ch == '-') || (input.size() == 0 && ch == '-')) continue;
                input += (char)ch;
                Gotoxy(startX + width1 - 18, formY + 8);
                cout << input;
				curX = whereX(); curY = whereY();

                strcpy(Prefix, input.c_str());
                subListCB = Find_CB_OnRage(dsCB, Prefix);
                subCB = new CB * [subListCB.slCB];
                CB* tempCB = subListCB.head;
                while (tempCB != NULL) {
                    subCB[slCB] = tempCB;
                    slCB++;
                    tempCB = tempCB->next;
                }

                ClearRegion(0, Y, 50, 8);
                Render_CB_Page(subCB, slCB, Y, 8, 8, currentPage, itemsPerPage, S, Temp, Temp2, Bin);
                delete[] subCB;
                slCB = 0;
            }
			else if (ch == 8 && !input.empty()) {
                Gotoxy(whereX() - 1, whereY());
                cout << " ";
                input.pop_back();
                Gotoxy(startX + width1 - 18, formY + 8);
                cout << input;
                curX = whereX(); curY = whereY();

                strcpy(Prefix, input.c_str());
                subListCB = Find_CB_OnRage(dsCB, Prefix);
                subCB = new CB * [subListCB.slCB];
                CB* tempCB = subListCB.head;
                while (tempCB != NULL) {
                    subCB[slCB] = tempCB;
                    slCB++;
                    tempCB = tempCB->next;
                }

                ClearRegion(0, Y, 50, 8);
                Render_CB_Page(subCB, slCB, Y, 8, 8, currentPage, itemsPerPage, S, Temp, Temp2, Bin);
                delete[] subCB;
                slCB = 0;
			}
        }

        strcpy(Prefix, input.c_str());
        subListCB = Find_CB_OnRage(dsCB, Prefix);

        subCB = new CB * [subListCB.slCB];
        CB* tempCB = subListCB.head;
        while (tempCB != NULL) {
            subCB[slCB] = tempCB;
            slCB++;
            tempCB = tempCB->next;
        }

        while (true) {
            ClearRegion(0, Y, 50, 8);
            Render_CB_Page(subCB, slCB, Y, 8, 8, currentPage, itemsPerPage, S, Temp, Temp2, Bin);
            Gotoxy(Temp[0], Y + currentRow);
			SmallBox(to_string(selectedCBIndex + 1), false, false, true, true, Temp2[0], 1, HIGHLIGHT);
            Gotoxy(Temp[1], Y + currentRow);
            SmallBox(subCB[selectedCBIndex]->maCB, false, false, true, true, Temp2[1], 1, HIGHLIGHT);

			UiThongTinCB(startX + width1 + 3, formY, 35, 7, subCB[selectedCBIndex]->maCB);
            action = GetNavKey();
            if (action == NAV_UP) {
                currentRow--;
                if (currentRow < 0) {
                    currentRow = 0;
                    continue;
                }
                selectedCBIndex--;
                if (selectedCBIndex < 0) selectedCBIndex = 0;
                continue;
            }
            else if (action == NAV_DOWN) {
                currentRow++;
                selectedCBIndex++;
                if (currentRow >= itemsPerPage || selectedCBIndex >= slCB) {
                    if (selectedCBIndex >= slCB) {
                        selectedCBIndex = 0;
                        currentRow = 0;
                        currentPage = 0;
                        continue;
                    }
                    if (currentPage + 1 < (slCB + itemsPerPage - 1) / itemsPerPage) {
                        currentPage++;
                        currentRow = 0;
                        continue;
                    }
                }
            }
            else if (action == NAV_RIGHT) {
                if (currentPage + 1 < (slCB + itemsPerPage - 1) / itemsPerPage) {
                    currentPage++;
                    currentRow = 0;
                }
                else {
                    currentPage = 0;
                    currentRow = 0;
                }
				selectedCBIndex = currentPage * itemsPerPage + currentRow;
                continue;
            }
            else if (action == NAV_LEFT) {
                if (currentPage - 1 >= 0) {
                    currentPage--;
                    currentRow = 0;
                }
                else {
                    currentPage = (slCB + itemsPerPage - 1) / itemsPerPage - 1;
                    currentRow = 0;
                }
				selectedCBIndex = currentPage * itemsPerPage + currentRow;
                continue;
            }
            else if (action == NAV_ESC) {
                delete[] subCB;
                slCB = 0;
                goto nhap;
            }
            else {
                break;
            }
        }
        SeatList(3, formY + height2 + 10, 7, formY + height2 + 6, subCB[selectedCBIndex]);
        delete[] subCB;
        slCB = 0;
        goto nhap;
    }

}

void Tutorial(){
    Gotoxy(0,0);
    SmallBox("Hướng dẫn sử dụng",120,3);
    Gotoxy(2,6);
    cout << GRAY << "1. Sử dụng phím mũi tên để di chuyển qua lại giữa các lựa chọn." << RESET;
    Gotoxy(2,8);
    cout << GRAY << "2. Nhấn Enter để chọn một tùy chọn/ Nhấn ESC để quay lại menu trước." << RESET;
    Gotoxy(2,10);
    cout << GRAY << "3. A để chuyển sang tháng trước, D để chuyển sang tháng sau." << RESET;
    Gotoxy(2,12);
    cout << GRAY << "4. F để quay lại phần tìm kiếm, khi đã vào phần tìm kiếm thì có thể nhập mã chuyến bay cần tìm." << RESET;
    Gotoxy(2,14);
    cout << GRAY << "5. Backspace để không chọn ngày giờ" << RESET;
    Gotoxy(2,16);
    cout << GRAY << "6. Chỉ bật Unikey(Tiếng Việt) khi nhập tên(Hành khách,Sân Bay), hệ thống chưa được tối ưu khi sử dụng bàn phím Tiếng Việt" << RESET;
    _getch();
}

// Phần của các Router và Hàm đồ hoạ giao diện nhìn cơ bản ở đây

void ViewQLMB(){
    int startX = 1 ;
    int startY = 22;
    int lenght = MenusSize[3];
    Gotoxy(startX,startY);
    int space = 3 ;
    for(int i = 0 ; i < lenght ;i++){
        SmallBox(subOptions4[i],20,5);
        startX = whereX();
        Gotoxy(startX + 20 + space, startY);
    }  
}

void ViewQLCB(){
    int startX = 1 ;
    int startY = 22;
    int lenght = MenusSize[4];
    Gotoxy(startX,startY);
    int space = 3 ;
    for(int i = 0 ; i < lenght ;i++){
        SmallBox(subOptions5[i],20,5);
        startX = whereX();
        Gotoxy(startX + 20 + space, startY);
    }  
}


void ViewQLHK(){
    int startX = 1 ;
    int startY = 22;
    int lenght = MenusSize[2];
    Gotoxy(startX,startY);
    int space = 3 ;
    for(int i = 0 ; i < lenght ;i++){
        SmallBox(subOptions3[i],20,5);
        startX = whereX();
        Gotoxy(startX + 20 + space, startY);
    }  
}


void Router_B(int mainIdx, function<void()> func_1, 
                  function<void()> func_2, 
                  function<void()> func_3,
                  function<void()> func_4){
    int chosen = 0; // Vị trí lựa chọn trong menu con
    int startX, startY ;
    int space = 3;
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

            SmallBox(s, 20, 5, color);
            startX = whereX();
            Gotoxy(startX + 20 + space, startY);
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
            Gotoxy(startX+5, startY + (i * (3 + space))); 

            SmallBox(s, true, true, true, true, 30, 1, color);
            Gotoxy(whereX(),whereY() + 1);
        }
        if(mainMenuIdx == 0){ 
            if(chosen == 0) ViewQLMB();
            else if(chosen == 1) ViewQLCB() ;
            else ViewQLHK();
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

// Hiển thị giao diện bên tay phải
void ViewOptions(int mainMenuIdx) {
    Gotoxy(71, 6); 
    int startX = whereX();
    int startY = whereY();
    
    // 1. Xóa vùng cũ 
    ClearRegion(startX+5, startY, 30, 20); 
    if( mainMenuIdx >= 2) return ;
    string* subOptions = Menus[mainMenuIdx];
    int count = MenusSize[mainMenuIdx];

    int space = 2;
    // 3. Vẽ nội dung ra
    for(int i = 0 ; i < count ; i++) {
        // con trỏ sẽ nhảy xuống dưới, nên vẫn cần Gotoxy để căn dòng
        SmallBox(subOptions[i], true, true, true, true, 30, 1, WHITE);
        Gotoxy(whereX(),whereY()+space);
    }
}

int MainMenuOptionInBoard(string options[], int length) {
    int chosen = 0;
    int startX = whereX();
    int startY = whereY();
    int A[4] = {0,0,0,0};

    ClearRegion(startX, startY, 70, length * 3);

    auto DrawOption = [&](int index, int currentChosen) {
        
        bool isSelected = (index == currentChosen);
        string s = isSelected ? (string(ARR) + " " + options[index] + " " + ARL) : options[index];
        string color = isSelected ? YELLOW : WHITE;

        SmallBox(s, (index == 0), (index == length - 1), true, true, 70, 3, color);
    };

    Gotoxy(startX, startY );
    for (int i = 0; i < length; i++){
        A[i] = whereY();
        DrawOption(i, chosen);
    }

    while (true) {
        Gotoxy(startX,  A[chosen]);
        DrawOption(chosen, chosen);
        ViewOptions(chosen);

        //if(chosen == 2) //Hướng dẫn sử dụng
        int pre_chosen = chosen;
        chosen = GetKey(chosen, length);

        if (chosen == 999) return pre_chosen;
        if (chosen == 1000) return 3;

       
        if (chosen != pre_chosen) {
            Gotoxy(startX,  A[pre_chosen]);
            DrawOption(pre_chosen, chosen); 
        }
    }
}

void RunInNewTab(void (*func)()) {
    cout << NEWTAB;
    func();
    cout << CLOSETAB;
}

void RunInNewTab(NavKey (*func)()) {
    NavKey ch = NAV_ENTER;
    do {
        cout << NEWTAB;
        ch = func();     
        cout << CLOSETAB; 
        
    } while(ch == NAV_ENTER); 
}
// []() { func(<type> Name);}

void Menu_QuanLyMayBay() {
    Router_B(3, 
        []() { RunInNewTab(CustomerAddMB); }, 
        []() { RunInNewTab([]() { It_list_MB(1); });}, 
        []() { RunInNewTab([]() { It_list_MB(2); });},
        []() { RunInNewTab([]() { It_list_MB(0); });} 
    );
}

void Menu_QuanLyChuyenBay() {
    Router_B(4, 
        []() { RunInNewTab(CustomerAddCB); }, // Thêm
        []() { RunInNewTab([]() { It_list_CB(1, dsCB, ""); });}, // Sửa
        []() { RunInNewTab([]() {FindCBOnTimeAndPlace();}); }, // Xóa
        []() { RunInNewTab([]() { It_list_CB(0, dsCB, ""); });}
    );
}

void Menu_QuanLyHanhKhach() {
    Router_B(2, 
        []() { RunInNewTab(BookTicket); }, // Đặt vé
        []() { RunInNewTab(HuyChuyen); }, // Hủy vé
        []() { RunInNewTab(EmptySeatList); },  // Xóa
        []() { RunInNewTab([]() { It_list_HK(0); }); }  // DS Hành Khách 
    );
}

void Menu_MayBayChuyenBay() {
     RunInNewTab([]() { It_list_CB(3, dsCB, ""); });
}

void Menu_ThongKeLuotBay() {
     RunInNewTab(StatiscalMB);
}

void Menu_InDanhSachHanhKhach() {
     // mod = 0: Chế độ chỉ xem
    RunInNewTab([]() { It_list_HK(0); });
}

void MainScreen(){
    string options[] = {"1. Quản Lý Hệ Thống",  
                        "2. Tra Cứu & Thống Kê", 
                        "3. Hướng Dẫn Sử Dụng",
                        "4. Thoát"};
    while(true){
        ClearScreen();
        SmallBox("Quản lí chuyến Bay Nội Địa",120,3);
        // nhảy xuống dòng do SmallBox k tự động xuống dòng
        Gotoxy(whereX(),whereY()+2);
        int chosen = MainMenuOptionInBoard(options,sizeof(options)/sizeof(string));
        if(chosen >= 0 && chosen <= 1)
        {
            switch (chosen)
            {
            case 0:
                // Gọi Router_Board và truyền tên các hàm menu vào
                Router_Board(chosen, Menu_QuanLyMayBay, Menu_QuanLyChuyenBay, Menu_QuanLyHanhKhach);
                break;
            case 1:
                Router_Board(chosen, Menu_MayBayChuyenBay, Menu_ThongKeLuotBay, Menu_InDanhSachHanhKhach);
                break;
            default:
                break;
            }
        }
        else if(chosen == 2)
        {
            RunInNewTab([]() { Tutorial(); });
        }
        else if(chosen == 3)
        {
            exit(0); // thoát chương trình
        }
        
    }
}