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
    int timeY = y + colpadding * 6 + 7;
    
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
                if (IsGreaterTime(time_now(), prevMonthStr) == 1) { 
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
            SmallBox(string(22,' ') + AD, 25,1);
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
            action = InputString(inputs[0], inputX, formY + 2, maCB_max, '_', false);//
        } else if (currentField == 1) {
          string old = inputs[1];
            NavKey k;
            bool dateError = false; //Kiểm tra lỗi ngày quá khứ
            
            // Vòng lặp do-while giữ Lịch không bị tắt khi đổi tháng HOẶC khi chọn sai ngày
            do {
                ClearRegion(startX + width + 3, 1, TotalWidth - width-3, 13); // Xóa sạch không gian trước khi vẽ
                k = DrawCalendar(startX + width + 3, 1, 5, 1, inputs[1]);
                
                dateError = false; // Reset cờ mỗi lần vẽ lại lịch
                
                // Nếu người dùng nhấn ENTER để chốt ngày, ta tiến hành kiểm tra
                if (k == NAV_ENTER) {
                    // Nếu thời gian hiện tại LỚN HƠN thời gian vừa chọn -> Lỗi quá khứ
                    if (IsGreaterTime(time_now(), inputs[1]) == 1) { 
                        Gotoxy(startX+1, formY + 20);
                        string u = "Lỗi: Thời gian khởi hành không được ở trong quá khứ! Bấm phím bất kỳ..." ;
                        SmallBox(u,false,true,false,false,width-2,3,RED);
                        _getch();
                        ClearRegion(startX+1, formY + 20, width-2, 3);
                        dateError = true; // Bật cờ lỗi để vòng lặp while quay lại mở Lịch
                    }
                }
            } while (k == NAV_LEFT || k == NAV_RIGHT || dateError); 
            
            if (k == NAV_ESC) {
                inputs[1] = old;      // Phục hồi ngày cũ
                action = NAV_UNKNOWN; // Ép giữ con trỏ lại ô này
            } else {
                Gotoxy(inputX + 1, formY + 2 + (currentField * 3));
                cout << inputs[1] ;
                action = NAV_DOWN;    // Tự động nhảy xuống ô Sân bay đến
            }
        } else if (currentField == 2) {
            // Tham số TRUE cuối cùng sẽ chặn toàn bộ các ký tự không phải là số
            action = InputString(inputs[2], inputX, formY + 8, sbDich_max, '_', false); 
        }else if (currentField == 3){
            int items = 5 ;
            listMB tmp ;
            tmp = Find_MB_OnRage(dsMB, const_cast<char*>(s.c_str()));
            UiSmallListShMB(15, items, inputX+40, formY + 2 + currentField*3, tmp);
            while(true){
                Gotoxy(inputX+26+s.size(), formY + 2 + currentField*3+1);
                int ch = 0 ;
                NavKey key = GetNavKey(ch);
                ClearRegion(inputX+1, formY + 2 + currentField*3+3, 30, 3);
                if(key == NAV_UP || key  == NAV_RIGHT){
                    ClearRegion(inputX+40, formY + 2 + currentField*3+3, 12, items*2-1);
                    action = key ;
                    break;
                }

                if(key == NAV_ESC){
                    return NAV_ESC  ;
                } 
                
                if(key == NAV_DOWN && tmp.slMB > 0) {
                    int idx = 0 ;
                    action = SmallListShMB(s,idx,15,1,inputX+40,formY + 2 + currentField*3,items,tmp,startX ,formY +18,width,7); // Hàm này hiển thị thông tin MB
                    if(action == NAV_ESC){
                        ClearRegion(inputX+1, formY + 2 + currentField*3+3, 30, 3);
                        return NAV_ESC;
                    }else if(action == NAV_ENTER){
                        inputs[3] = s ;
                        inputs[4] = to_string(tmp.list[idx]->socho);
                        RedrawInput(inputs[4],inputX, formY + 3 + (size - 1) * 3,3,' ',true,false)  ; 
                        ClearRegion(inputX+40, formY + 2 + currentField*3+3, 12, items*2-1);
                        break;
                    }
                }
                UiFindMB(s, key, ch, tmp, inputX+1, formY + 2 + currentField*3+1);
                UiSmallListShMB(15, items, inputX+40, formY + 2 + currentField*3, tmp);
            }
            
        }else if(currentField == 4){
            action = InputString(inputs[4], inputX, formY + 3 + (size - 1) * 3, 3, ' ', true, true); 
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
                NewCB->trangThai = 1;
            
                NewCB->set_socho(stoi(inputs[4]));
                string error_code =Can_Add_CB(dsCB, dsMB, NewCB);
                if(error_code != "0") {
                    Gotoxy(startX+1, formY + 20);
                    if(error_code == "1") {
                        string u = "Lỗi: Số hiệu [" + inputs[3] + "] không tồn tại!";
                        SmallBox(u,false,true,true,false,width-2,3,RED); // tính cả hai cái viền của box xung quanh
                    }
                    else if(error_code == "2") {
                        string u = "Lỗi:Không thể đặt chuyến bay có số hiệu [" + inputs[3] + "] trong 24h gần nhất khi đang thực hiện chuyến bay khác!";
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
                        Gotoxy(startX + width/2 - 20, formY + 15);
                        SmallBox(text, width + width/2, 3, (string)WHITE);
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
