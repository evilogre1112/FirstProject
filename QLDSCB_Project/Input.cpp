#include "UI.h"
#include "Draw.h"
#include "Extentions.h"
#include "Input.h"

using namespace std;

static listMB* pDsMB = nullptr;
static listCB* pDsCB = nullptr;
static listHK* pDsHK = nullptr;
static listSBD* pDsSBD = nullptr;

#define dsMB (*pDsMB)
#define dsCB (*pDsCB)
#define dsHK (*pDsHK)
#define dsSBD (*pDsSBD)

void _TruyenThamSo(listMB &A, listCB& B , listHK &C, listSBD& D){
    pDsMB = &A ;
    pDsCB = &B ;
    pDsHK = &C ; 
    pDsSBD = &D;
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
int InputString(string &result, int x, int y, int maxLength, char placeholder, bool onlyNumbers, bool isCenter, bool isHighlight, bool isUppercase,bool isonlychar, bool canspace ) {
    // Vẽ lần đầu tiên
    RedrawInput(result, x, y, maxLength, placeholder, isCenter, isHighlight);

    while (true) {
        int ch;
        NavKey key = GetNavKey(ch); 

        if (key != NAV_UNKNOWN && key != NAV_BACK) return key;

        if (key == NAV_BACK) {
            if (!result.empty()) {
                unsigned char c = (unsigned char)result.back();
                // Từ 0 đến 127 là mã ASCII chuẩn
                // 128 đến 255 là mã ASCII có chứa tiếng việt có dấu 
                // 0x80 là 128 
                if (c >= 0x80) { // Nếu xoá chữ tiếng việt thì vào vòng này để xoá các bit cuối trước
                    // unsigned char thực hiện không dấu ( từ 0 đến 255) bit k dấu
                    // char có bit dấu từ (-128 đến 127) nếu 1 ký tự có bit 1 đầu tiên -> đây là bit dấu âm 
                    // 0xc0 (hex) -> bin 11000000 AND 1 
                    // result.back() && 0xc0 lấy byte cuối , để 6 bit cuối là 0 , giữ lại 2 bit đầu
                    // 0x80 (hex) -> bin 10|000000
                    // "â" trong tiếng việt có 2 byte , result.pop_back() chỉ xoá đi 1 byte ở byte đuôi, byte đầu còn nằm trong chuỗi
                    // 2 loại byte , byte đầu luôn là 0, 01, 011,...
                    // byte cuối luôn bắt đầu bằng 10...
                    // Dòng while ở dưới để xoá byte đuôi 
                    while (!result.empty() && ((unsigned char)result.back() & 0xc0) == 0x80) {
                        result.pop_back();
                    }
                }
                if (!result.empty()) result.pop_back(); // Sau khi xoá byte đuôi thì xoá byte đầu 
                
                RedrawInput(result, x, y, maxLength, placeholder, isCenter, isHighlight);
            }
        } 
        else if (visualLength(result) < maxLength) {
            if (result.empty() && ch == ' ') continue;
            if (!canspace && ch == ' ') continue ; 
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
                if (!canspace && ch == ' ') continue ; 
                if (onlyNumbers) continue; // Nếu chỉ nhập số thì chặn luôn các ký tự UTF-8
                
                result += (char)ch;
            }

            RedrawInput(result, x, y, maxLength, placeholder, isCenter, isHighlight);
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
            if (visualLength(result) < 16) {
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
        
        if (visualLength(result) < 16) {
            if (!isdigit(ch)) continue; 

            int len = visualLength(result);
            
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

            len = visualLength(result); 
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
        else if (visualLength(result) < maxLength) {
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
                    string match = tmp.list[0].sbDich; // match, u chuỗi màu xám gợi ý
                    // t -> t|ân sơn nhất   
                    if (visualLength(match) >= visualLength(result)) {
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

bool InputSimpleHK(string& res, NavKey key, int& ch,int x,int y) {
    // Xử lý nút xóa (Backspace)
    if (key == NAV_BACK && !res.empty()) {
        if (!res.empty()) {
                unsigned char c = (unsigned char)res.back();
                if (c >= 0x80) {
                    while (!res.empty() && ((unsigned char)res.back() & 0xc0) == 0x80) {
                        res.pop_back();
                    }
                }
                if (!res.empty()) res.pop_back();
        }
        RedrawInput(res,x,y,ho_max + ten_max,' ',false,false);
        return true;
    } 
    else if(visualLength(res) < ho_max + ten_max && key == NAV_UNKNOWN){
        if(!res.empty() && res.back() == ' ' && ch == ' ') return false;
        if((unsigned char)ch < 128){ 
            if(ch != ' ' && ch != '-' && !isalnum(ch)) return false ;
            res += (char)ch; 
        }else{
            res += (char)ch;
        }
        RedrawInput(res,x,y,ho_max + ten_max,' ',false,false);
        return true ;
    }
    // Xử lý gõ chữ cái / số
    
    return false;
}

// Khối hàm thực hiện In danh sách MB và các thao tác sữa xoá ở đây 
bool SimpleInput(string& res, NavKey key, int& ch, int x, int y, int width, bool canspace, bool onlyNums) {
    bool isChanged = false; 
    if (key == NAV_BACK && !res.empty()) {
        res.pop_back();
        Gotoxy(x, y);
        cout << res << "  ";
        isChanged = true;
    } 
    else if (visualLength(res) < width && (( (!res.empty() && res.back() != ' ' && ch == ' ') || ((isalnum(ch) || ch == '-') && key == NAV_UNKNOWN) ) )) { 
        if ((char)ch == ' ' && !canspace) return false ; 
        if (((char)ch < '0' || (char)ch > '9') && onlyNums) return false;
        if (((char)ch == '-' && res.size() == 0) || ((char)ch == '-' && res.size() > 0 && res[res.size() - 1] == '-')) return false;
        res += toupper((char)ch); 
        Gotoxy(x, y);
        cout << res; 
        isChanged = true;
    }
    return isChanged;
}
