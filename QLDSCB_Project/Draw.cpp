#include "UI.h"
#include "Draw.h"
#include "Global.h"
#include "Extentions.h"

using namespace std;

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

void SmallBox(string text, int Width, int Height, string color) {
    SmallBox(text, true, true, true, true, Width, Height, color); // Gọi hàm gốc với các giá trị mặc định
}

// Hàm này dùng để vẽ một hộp nhỏ với nhiều tùy chọn hơn như có/không cạnh trên/dưới/trái/phải, chiều cao, chiều rộng và màu sắc của text
void SmallBox(string text, bool ABOVE, bool UNDER, bool LEFT, bool RIGHT,int Width, int Height, string color ) {
    int startX = whereX();
    int startY = whereY();
    int innerWidth = Width - 2;
    if (innerWidth <= 0) return;

    int totalLinesNeeded = countLinesNeeded(text, innerWidth); // số ký tự tối đa hiển thị trên 1 hàng 
    int actualLinesToPrint = (totalLinesNeeded > Height) ? Height : totalLinesNeeded;
    int startTextRow = (Height - actualLinesToPrint) / 2; // canh giữa 

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
            int currentBytePos = 0; // byte hiện tại 
            int tempPos = 0; // vị trị
            for(int j = 0; j < i - startTextRow; j++) {
                tempPos += getLineInfo(text, tempPos, innerWidth).byteCount; // Tính vị trí 1 chữ hoàn chỉnh dính liền để xuống 
                while (tempPos < text.length() && text[tempPos] == ' ') tempPos++; // vị trí cắt
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

void DrawACat(int x  ,int y){
    Gotoxy(x, y);
    cout << " /\\_/\\";
    Gotoxy(x, y+1);
    cout <<"( o.o )";
    Gotoxy(x, y+2);
    cout << " > ^ <";
}


void PrintMultiLineText(int startX, int startY, const string& text) {
    stringstream ss(text);
    string line;
    int currentY = startY;

    // Tách chuỗi theo từng ký tự '\n'
    while (getline(ss, line, '\n')) {
        Gotoxy(startX, currentY); // Di chuyển đến đúng tọa độ X, và Y hiện tại
        cout << line;
        currentY++;               // Tăng Y lên cho dòng tiếp theo
    }
}

// Lấy tọa độ Y hiện tại của con trỏ

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
//vẽ
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

        if(kt == NAV_RIGHT || kt == NAV_LEFT || kt == NAV_ESC || kt == NAV_BACK || kt == NAV_UP) return kt;

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
            
            if (key == NAV_BACK || key == NAV_ESC || (key == NAV_UNKNOWN && tolower(ch) == 'w')) {
                DateString = "";
                return key;
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
// vẽ lịch và thao tác nhưng k bao gồm giờ và phút 
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

        if (key == NAV_BACK || key == NAV_ESC || (key == NAV_UNKNOWN && tolower(ch) == 'w')) {
            DateString = "";
            return key == NAV_UNKNOWN ? NAV_UP : key;
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
        }
        
        int new_cx = x + rowpadding * ((FirstDayOfMonth + D - 1) % 7);
        int new_cy = y + ((FirstDayOfMonth + D - 1) / 7) * colpadding + 5;
        
        Gotoxy(new_cx - 3, new_cy); cout << "[";
        Gotoxy(new_cx + 3, new_cy); cout << "]";
    }
    return NAV_UNKNOWN;   
}

void DrawNote2(int startX , int formY, int width1){
    string q[] = {"","","",""};
    string Type[] = { RED, GREEN, YELLOW, CYAN };
    q[0] = Type[0] + SQ + RESET + ": Chuyến bay bị huỷ";
    q[1] = Type[1] + SQ + RESET + ": Chuyến bay còn vé";
    q[2] = Type[2] + SQ + RESET + ": Chuyến bay hết vé";
    q[3] = Type[3] + SQ + RESET + ": Chuyến bay hoàn tất";
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
}

void DrawNote1(int x , int y , int width , int height){
    string q[] = {"","","",""};
    string Type[] = { RED, GREEN, YELLOW, CYAN };
    q[0] = Type[0] + SQ + RESET + ": Chuyến bay bị huỷ ";
    q[1] = Type[1] + SQ + RESET + ": Chuyến bay còn vé ";
    q[2] = Type[2] + SQ + RESET + ": Chuyến bay hết vé ";
    q[3] = Type[3] + SQ + RESET + ": Chuyến bay hoàn tất ";
    Gotoxy(x,y);
    string u = "  " + q[0] + q[1] +"    " +q[2] + q[3];
    SmallBox(u,width,height);
}
