NavKey DrawCalendar(int x , int y,int rowpadding , int colpadding,string& DateString){
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
    int max_rows = (FirstDayOfMonth + Days_In_Month - 1) / 7;
    
    // --- BẮT ĐẦU VẼ KHUNG GIỜ/PHÚT CO GIÃN ĐỘNG ---
    int calWidth = rowpadding * 7; 
    int timeY = y + colpadding * max_rows + 7; 
    
    // Bóp nhỏ cụm Giờ/Phút lại (Tổng rộng khoảng 28 ký tự)
    int timeStartX = x ; 

    Gotoxy(timeStartX, timeY); cout << "Giờ" ;
    Gotoxy(timeStartX + 4, timeY - 1); SmallBox(string(3,' ') + AD, 8, 1); // Rộng 8
    
    Gotoxy(timeStartX + 14, timeY); cout << "Phút" ;
    Gotoxy(timeStartX + 19, timeY - 1); SmallBox(string(3,' ') + AD, 8, 1); // Rộng 8 

    //Gotoxy(x, timeY + 3);
    //SmallBox("Arrows:Chọn | A/D:Tháng | ESC:Thoát",rowpadding * 6 , 1, string(YELLOW));
    // ----------------------------------------------
    
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
   
    DateTime ChossenDay = StringtoTime(DateString);
    
    // Nếu đang ở tháng hiện tại, mốc nhỏ nhất là realToday. Nếu là tháng tương lai, mốc nhỏ nhất là 1.
    int minDay = (month == realMonth && year == realYear) ? realToday : 1;

    while(true){
        int D = ChossenDay.get_dd() ;
        while(true){
            int ch ;
            NavKey key = GetNavKey(ch);
            int old_cx = x + rowpadding * ((FirstDayOfMonth + D - 1) % 7);
            int old_cy = y + ((FirstDayOfMonth + D - 1) / 7) * colpadding + 5;
          
            Gotoxy(old_cx - 3, old_cy); cout << " "; 
            Gotoxy(old_cx + 3, old_cy); cout << " ";
            Gotoxy(old_cx, old_cy);
            
            // Fix màu sắc: Chỉ tô xám/xanh nếu đúng là tháng hiện tại
            if (month == realMonth && year == realYear) {
                if (D < realToday) cout << GRAY << D << RESET; 
                else if (D == realToday) cout << GREEN << D << RESET;
                else cout << D;
            } else {
                cout << D; // Tháng tương lai thì in màu trắng bình thường
            }
        
            if (key == NAV_RIGHT) {
                if (D < Days_In_Month) D++;
            } 
            else if (key == NAV_LEFT) {
                // Đã fix: Lùi thoải mái nếu ở tháng tương lai, chặn ở realToday nếu ở tháng hiện tại
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
            else if(key == NAV_UNKNOWN && (tolower(ch) == 'h' || tolower(ch) == 'm')){
                ChossenDay.set_dd(D); 
                Gotoxy(old_cx - 3, old_cy); cout << "["; 
                Gotoxy(old_cx + 3, old_cy); cout << "]";
                Gotoxy(old_cx, old_cy);
                cout << CYAN << D << RESET;
                timeFocus = (tolower(ch) == 'h') ? 1 : 2; 
                break;
            }
            else if (key == NAV_UNKNOWN && tolower(ch) == 'd') {
                ChossenDay.set_dd(Days_In_Month); 
                DateString = NextDay(ChossenDay); 
                return NAV_ENTER; 
            }
            else if (key == NAV_UNKNOWN && tolower(ch) == 'a') {
                string prevMonthStr = LastDay(DateString); 
                
                // Chỉ cho lùi nếu tháng trước KHÔNG phải là quá khứ
                if (IsGreaterTime(time_now(), prevMonthStr) != 1) { 
                    DateString = prevMonthStr;
                    return NAV_ENTER;
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
        while(true){
            // 1. In Giờ
            Gotoxy(hourX, textY);
            if (timeFocus == 1) cout << YELLOW; else cout << RESET;
            cout << (H < 10 ? "0" : "") << H << RESET;
            
            // 2. In Phút
            Gotoxy(minX, textY);
            if (timeFocus == 2) cout << YELLOW; else cout << RESET;
            cout << (M < 10 ? "0" : "") << M << RESET;

            // 3. Bắt phím
            int ch;
            NavKey key = GetNavKey(ch);
            
            if (key == NAV_ESC) {
                DateString = "";
                return NAV_ESC;
            }
            else if (key == NAV_ENTER) {
                // Chốt thời gian, cập nhật DateString và thoát Picker
                ChossenDay.set_hh(H);
                ChossenDay.set_mm(M);
                DateString = ToStringDate(ChossenDay);
                return NAV_ENTER;
            }
            
            // Quay về Lịch (phím X)
            else if (key == NAV_UNKNOWN && tolower(ch) == 'x') {
                // Trả lại màu sáng bình thường cho Giờ/Phút trước khi thoát lên trên
                Gotoxy(hourX, textY); cout << (H < 10 ? "0" : "") << H;
                Gotoxy(minX, textY); cout << (M < 10 ? "0" : "") << M;
                break; // Break vòng lặp này -> Vòng lặp tổng sẽ quay ngược lên vòng chọn Ngày
            }
            
            // Đảo Focus giữa Giờ và Phút
            else if (key == NAV_LEFT || key == NAV_RIGHT) {
                timeFocus = (timeFocus == 1) ? 2 : 1;
            }
            else if (key == NAV_UNKNOWN && (char(ch) == 'h' || char(ch) == 'H')) timeFocus = 1;
            else if (key == NAV_UNKNOWN && (char(ch) == 'm' || char(ch) == 'M')) timeFocus = 2;
            
            // Tăng giảm giá trị
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