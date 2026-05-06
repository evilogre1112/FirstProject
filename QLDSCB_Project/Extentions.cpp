#include "Extentions.h"
#include "Global.h"
#include "Logic.h"
#include "UI.h"
#include <fstream>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <string>
#include <ctime>

using namespace std;


string time_now() { 
    DateTime now;
    now.time_now(); // Cập nhật giờ hiện tại vào đối tượng DateTime
    return ToStringDate(now); 
}
DateTime StringtoTime(string time_str){
    DateTime dt;
    if(time_str.length() != 16) return dt; // Kiểm tra định dạng cơ bản
    int d = GetDayFromStr(time_str);
    int m = GetMonthFromStr(time_str);
    int y = GetYearFromStr(time_str);
    int hh = GetHourFromStr(time_str);
    int mm = GetMinuteFromStr(time_str);
    dt.set_dd(d);
    dt.set_mt(m);
    dt.set_yy(y);
    dt.set_hh(hh);
    dt.set_mm(mm);
    return dt;
}
int Get_Weekday(DateTime dt){
    int d = dt.dd;
    int m = dt.mt;
    int y = dt.yy;
    // Quy tắc Zeller: Tháng 1 và 2 được coi là tháng 13 và 14 của năm trước
    if(m<3) {
        m+=12;
        y--;
    }
    int K = y % 100; // Năm trong thế kỷ
    int J = y / 100; // Thế kỷ

    // Công thức Zeller: h là thứ tính từ Thứ 7 (0=Sat, 1=Sun, ...)
    int h = (d + (13 * (m + 1)) / 5 + K + K / 4 + J / 4 - 2 * J) % 7;

    // Chuyển đổi để h = 0 là Chủ Nhật, h = 6 là Thứ 7
    // Do kết quả phép chia dư % trong C++ với số âm có thể khác toán học, 
    // ta thêm +7 trước khi chia dư lần nữa cho chắc chắn.
    return (h + 6) % 7;
}
string NextDay(DateTime dt){
    dt.dd++;
    // Kiểm tra nếu ngày vượt quá số ngày của tháng
    int daysInCurrentMonth = daysInMonth(dt.mt, dt.yy);
    if (dt.dd > daysInCurrentMonth) {
        dt.dd = 1;
        dt.mt++;
        // Kiểm tra nếu tháng vượt quá 12
        if (dt.mt > 12) {
            dt.mt = 1;
            dt.yy++;
        }
    }
    return ToStringDate(dt);
}
string NextDay(string time_str){
    DateTime dt = StringtoTime(time_str);
    return NextDay(dt);
}
DateTime nextDay(DateTime dt){
    return StringtoTime(NextDay(dt));
}
DateTime nextDay(string time_str){
    return StringtoTime(NextDay(time_str));
}
int Get_Weekday(string time_str){
    DateTime dt = StringtoTime(time_str);
    return Get_Weekday(dt);
}
int IsGreaterTime(string Time1, string Time2){
    DateTime dt1 = StringtoTime(Time1); 
    DateTime dt2 = StringtoTime(Time2); 
    if (dt1.yy != dt2.yy) return dt1.yy > dt2.yy ? 1 : -1;   
    if (dt1.mt != dt2.mt) return dt1.mt > dt2.mt ? 1 : -1;
    if (dt1.dd != dt2.dd) return dt1.dd > dt2.dd ? 1 : -1;
    if (dt1.hh != dt2.hh) return dt1.hh > dt2.hh ? 1 : -1;
    if (dt1.mm != dt2.mm) return dt1.mm > dt2.mm ? 1 : -1;
    return 0; // Nếu tất cả các trường đều bằng nhau
}
string LastDay(DateTime dt){
    dt.dd--;
    if (dt.dd < 1) {
        dt.mt--;
        if (dt.mt < 1) {
            dt.mt = 12;
            dt.yy--;
        }
        dt.dd = daysInMonth(dt.mt, dt.yy);
    }
    return ToStringDate(dt);
}
string LastDay(string time_str){
    DateTime dt = StringtoTime(time_str);
    return LastDay(dt);
}
DateTime lastDay(DateTime dt){
    return StringtoTime(LastDay(dt));
}
DateTime lastDay(string time_str){
    return StringtoTime(LastDay(time_str));
}

//=========== các hàm đọc dữ liệu ===========//


bool Get_Data_CB(listCB &dsCB, listMB &dsMB,listHK &dsHK, const char *path_file_CB) {
// Mở file ở chế độ binary để tránh triệt để lỗi dư ký tự \r trên Windows
    ifstream f(path_file_CB);
    if (!f.is_open()) {
        return false;
    }

    dsCB.slCB = 0;
    dsCB.head = NULL;

    char ma[maCB_max];
// Vòng lặp lớn: Đọc từng chuyến bay
    while (f.getline(ma, maCB_max, '|')) {
        // Dọn dẹp ký tự xuống dòng bị dính từ vòng lặp trước (nếu có)
        int len_ma = strlen(ma);
        if (len_ma > 0 && ma[0] == '\n') { 
            memmove(ma, ma + 1, len_ma); 
            len_ma--; 
        }
        if (len_ma > 0 && ma[len_ma - 1] == '\r') {
            ma[len_ma - 1] = '\0'; 
        }

        if (strlen(ma) < 2) continue;

        CB* tmp = new CB();
        strcpy(tmp->maCB, ma);
        tmp->next = NULL;

        // --- 1. Đọc thông tin cơ bản ---
        f >> tmp->ngayKH.hh; f.ignore(); // bỏ qua ':'
        f >> tmp->ngayKH.mm; f.ignore(); // bỏ qua '|'
        f >> tmp->ngayKH.dd; f.ignore(); // bỏ qua '/'
        f >> tmp->ngayKH.mt; f.ignore(); // bỏ qua '/'
        f >> tmp->ngayKH.yy; f.ignore(); // bỏ qua '|'

        tmp->sbDich = new char[sbDich_max];
        f.getline(tmp->sbDich, sbDich_max, '|');

        f >> tmp->trangThai; f.ignore(); // bỏ qua '|'
        f.getline(tmp->soHieuMB, soHieuMB_max, '|');
            
            
        //--- 2 lấy số chỗ mở trên chuyến bay đó phải nhỏ hơn hoặc bằng so ghế
        f >> tmp->socho; f.ignore();
        
        // --- 3. Cấp phát mảng vé ---
        tmp->DSV = new char*[tmp->socho];
        for (int i = 0; i < tmp->socho; i++) {
            tmp->DSV[i] = new char[cmnd_max];
            strcpy(tmp->DSV[i], "\0"); // Mặc định tất cả ghế đều trống
        }
        // ---- 4. thêm HK vào DSV
        tmp->sove = 0;
        if (!Add_CB(dsCB, dsMB, tmp)) {
        delete[] tmp->sbDich;
        for (int i = 0; i < tmp->socho; i++) {
            delete[] tmp->DSV[i];
        }
        delete[] tmp->DSV;
        delete tmp; 
        return false;
        }
        // --- 5. Đọc danh sách vé (nếu có)---
        while (true) {
            int seatNumber;
            // Dọn dẹp khoảng trắng để peek đúng ký tự tiếp theo
            f >> ws; 

            // Kiểm tra xem có phải là số không trước khi đọc
            if (!isdigit(f.peek())) break; 

            if (!(f >> seatNumber)) {
                f.clear(); // Xóa lỗi nếu lỡ đọc nhầm
                break;
            }

            if (seatNumber <= 0) break; 

            f.ignore(); // bỏ qua '|'
            char CMND[cmnd_max];
            f.getline(CMND, cmnd_max); 

            int len = strlen(CMND);
            if(len > 0 && CMND[len-1] == '\r') CMND[len-1] = '\0';

            Book_Ticket(dsCB, dsHK, tmp->maCB, CMND, seatNumber);
        }
        f.clear(); // Đảm bảo trạng thái file ổn định
        f >> ws;   // Nhảy đến ký tự đầu tiên của chuyến bay tiếp theo
    }
    f.close();
    return true;
}
bool Get_Data_MB(listMB &dsMB, const char *path_file_MB){
    ifstream f(path_file_MB);
    if(!f.is_open()) return false;
    dsMB.slMB=0;
    char tempSoHieu[soHieuMB_max];
    while(dsMB.slMB < slMB_max&&  f.getline(tempSoHieu, soHieuMB_max, '|')){        
        //đọc đến cuối file
        if (strlen(tempSoHieu) == 0) continue;
            // Nếu dòng rỗng, bỏ qua
        MB* tmp =new MB();                          //tạo ô nhớ chứa MB
        strcpy(tmp->soHieuMB, tempSoHieu);
        // --- số hiệu máy bay----
        if(strlen(tmp->soHieuMB)== 0){
            delete tmp;
            break;
        }
        //---loai mb----
        f.getline(tmp->loaiMB, loaiMB_max,'|');
        //---so cho---
        if(!(f>> tmp->socho)) {
            delete tmp;
            break;
        }
        tmp->SLB = 0;
        // xóa dáu cách còn lại dấu chống dòng
        f.ignore(100,'\n');
        //dùng hàm Add_MB để thêm vào danh sách máy bay
        if (!Add_MB(dsMB, tmp)) {
        // Nếu Add_MB trả về false (do trùng mã hoặc danh sách đầy 300)
        delete tmp; 
        }
    }

    f.close();
    return true;
}
// chèn  vào cây nhị phân so sánh cmnd
/*HK*: Khai báo rằng goc là một con trỏ, nó lưu trữ địa chỉ của một đối tượng HK.
    &: Khai báo rằng goc được truyền vào theo dạng tham chiếu. 
Điều này có nghĩa là mọi thay đổi đối với goc bên trong hàm sẽ
    thay đổi trực tiếp biến gốc ở ngoài hàm.
*/
bool Get_Data_HK(listHK &dsHK, const char *path_file_HK) {
    ifstream f(path_file_HK);
    if (!f.is_open()) return false;

// Khởi tạo cây rỗng
    dsHK.goc = NULL;
    dsHK.slHK = 0;

// Biến tạm để đọc dữ liệu từ file
    char b_cmnd[cmnd_max], b_ten[ten_max], b_ho[ho_max];
    bool b_phai;

    while (f.getline(b_cmnd, cmnd_max, '|')) {
    // Nếu dòng rỗng thì bỏ qua
        if (strlen(b_cmnd) == 0) continue;

        // Đọc tiếp Họ, Tên, Phái
        f.getline(b_ho, ho_max, '|');
        f.getline(b_ten, ten_max, '|');
        f >> b_phai;
        f.ignore(1000, '\n'); // Nhảy xuống dòng tiếp theo

        Add_HK(dsHK, b_ho, b_ten, b_cmnd, b_phai);
    }

    f.close();
    return true;
}
// ============== các hàm ghi hết dữ liệu ==============
bool Set_Data_CB(listCB &dsCB, const char *path_file_CB){
    ofstream f(path_file_CB);
    if(!f.is_open()) return false;
    CB* temp = dsCB.head;
    while(temp != NULL){
        f << temp->maCB << '|'
            << setfill('0') << setw(2) << temp->ngayKH.hh << ':'
            << setfill('0') << setw(2) << temp->ngayKH.mm << '|'
            << setfill('0') << setw(2) << temp->ngayKH.dd << '/'
            << setfill('0') << setw(2) << temp->ngayKH.mt << '/'
            << setfill('0') << setw(4) << temp->ngayKH.yy << '|'
            << temp->sbDich << '|'
            << temp->trangThai << '|'
            << temp->soHieuMB << '|';
        // Đếm số vé có thể bán
        f << temp->socho << '\n';
        // Ghi danh sách vé (vị trí | CMND)
        for (int i = 0; i < temp->socho; i++) {
            if (strcmp(temp->DSV[i], "\0") != 0 && strcmp(temp->DSV[i], "") != 0) {
                f << (i + 1) << '|' << temp->DSV[i] << '\n';
            }
        }

        // --- 3. Luôn ghi số 0 để làm "điểm dừng" cho mỗi chuyến bay ---
        f << 0 << '\n';

        temp = temp->next;
    }

    f.close();
    return true;
}

bool Set_Data_MB(listMB &dsMB, const char *path_file_MB){
    ofstream f(path_file_MB);
    if(!f.is_open()) return false;
    if(dsMB.slMB == 0) {
        f.close();
        return true; // Nếu không có máy bay nào, tạo file trống
    }
    for (int i = 0; i < dsMB.slMB; i++) {
        MB* mb = dsMB.list[i];
        f << mb->soHieuMB << '|' << mb->loaiMB << '|' << mb->socho;
        if (i < dsMB.slMB - 1) {
            f << endl;
        }
    }
    f.close();
    return true;
}

bool Set_Data_HK(listHK &dsHK, const char *path_file_HK){
    ofstream f(path_file_HK);
    if(!f.is_open()) return false;
    if (dsHK.goc == NULL) { f.close(); return true; }
    // Ham duyet luu theo thu tu NLR
    Stack<HK*> s;
    s.push(dsHK.goc);
    while(!s.isEmpty()){
        HK* temp =s.pop();
        f << temp->cmnd << '|' << temp->ho << '|' << temp->ten << '|' << temp->phai << '\n';
        if(temp->right != NULL) s.push(temp->right);
        if(temp->left != NULL) s.push(temp->left);
    }
    f.close();
    return true;
}

//=========== các hàm ghi 1 dữ liệu ===========//
bool Set_Data_CB(CB &CB, const char *path_file_CB){
    // mở file với chế dộ thêm cuói file
    ofstream f(path_file_CB, ios::app);
    if(!f.is_open()) return false;
    f << CB.maCB << '|'
        << setfill('0') << setw(2) << CB.ngayKH.hh << ':'
        << setfill('0') << setw(2) << CB.ngayKH.mm << '|'
        << setfill('0') << setw(2) << CB.ngayKH.dd << '/'
        << setfill('0') << setw(2) << CB.ngayKH.mt << '/'
        << setfill('0') << setw(4) << CB.ngayKH.yy << '|'
        << CB.sbDich << '|'
        << CB.trangThai << '|'
        << CB.soHieuMB << '|'
        << CB.socho << '|';;
    // Ghi danh sách vé (vị trí | CMND)
    if(CB.sove > 0) {    
        for (int i = 0; i < CB.socho; i++) {
            if (strcmp(CB.DSV[i], "\0") != 0) {
                f << (i + 1) << '|' << CB.DSV[i]<< '\n';
            }
        }
    }
    f << 0 << '\n'; // Điểm dừng cho chuyến bay này
    f.close();
    return true;
}
bool Set_Data_MB(MB& mb, const char *path_file_MB){
    // mở file với chế dộ thêm cuối file
    ofstream f(path_file_MB, ios::app);
    if(!f.is_open()) return false;
    
    // In \n trước để ngắt dòng với data cũ, sau đó mới ghi data máy bay mới
    f << '\n' << mb.soHieuMB << '|' << mb.loaiMB << '|' << mb.socho;
    
    f.close();
    return true;
}
bool Set_Data_HK(HK& hk, const char *path_file_HK){
    // mở file với chế dộ thêm cuói file
    ofstream f(path_file_HK, ios::app);
    if(!f.is_open()) return false;
    
    // Do code lưu toàn bộ HK của bạn đã có \n ở cuối dòng, nên ở đây ghi bình thường
    f << hk.cmnd << '|' << hk.ho << '|' << hk.ten << '|' << hk.phai << '\n';
    
    f.close();
    return true;
}

int getFirstDayOfWeek(int month, int year) {
    static int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
    year -= month < 3;
    return (year + year / 4 - year / 100 + year / 400 + t[month - 1] + 1) % 7;
}

bool isNotPastTime(const DateTime& t, const DateTime& cur) {
    if (t.yy != cur.yy) return t.yy > cur.yy;
    if (t.mt != cur.mt) return t.mt > cur.mt;
    if (t.dd != cur.dd) return t.dd > cur.dd;
    if (t.hh != cur.hh) return t.hh > cur.hh;
    return t.mm >= cur.mm;
}

// Tiện ích tính số phút chênh lệch của 2 ngày bất kì từ 1900 về sau
bool isLeap(int y) {
    return (y % 400 == 0) || (y % 4 == 0 && y % 100 != 0);
}

int daysInMonth(int m, int y) {
    int days[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
    if (m == 2 && isLeap(y)) return 29;
    return days[m];
}


long long toMinutes(DateTime dt) {
    long long totalDays = 0;

    // năm
    for (int y = 1900; y < dt.get_yy(); y++) {
        totalDays += isLeap(y) ? 366 : 365;
    }

    // tháng
    for (int m = 1; m < dt.get_mt(); m++) {
        totalDays += daysInMonth(m, dt.get_yy());
    }

    // ngày
    totalDays += (dt.get_dd() - 1);

    // phút
    long long totalMinutes = totalDays * 24 * 60;
    totalMinutes += dt.get_hh() * 60 + dt.get_mm();

    return totalMinutes;
}

long long ss_ngay(DateTime const &a, DateTime const &b) {
    return llabs(toMinutes(a) - toMinutes(b));
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