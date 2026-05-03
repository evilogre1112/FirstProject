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
    char buffer[20]; 
    DateTime now;
    now.time_now(); // Cập nhật giờ hiện tại vào đối tượng DateTime
    snprintf(buffer, sizeof(buffer), "%02d/%02d/%04d %02d:%02d", 
             now.get_dd(), now.get_mt(), now.get_yy(), 
             now.get_hh(), now.get_mm());
             
    return string(buffer); 
}
DateTime time_now(string time_str){
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

//=========== các hàm đọc dữ liệu ===========//


bool Get_Data_CB(listCB &dsCB, listMB &dsMB, const char *path_file_CB) {
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
            
            
        // --- 2. Lấy Số Chỗ từ Danh Sách Máy Bay ---
        int indexMB = Find_MB(dsMB, tmp->soHieuMB);
        
        if (indexMB == -1) {
            tmp->socho = 0 ; // Không tìm thấy máy bay thì cho số chỗ = 0
        } else {
            // Lấy trực tiếp số chỗ thực tế của chiếc máy bay đó
            tmp->socho = dsMB.list[indexMB]->socho;
        }

        // --- 3. Cấp phát mảng vé ---
        tmp->DSV = new char*[tmp->socho];
        for (int i = 0; i < tmp->socho; i++) {
            tmp->DSV[i] = new char[cmnd_max];
            strcpy(tmp->DSV[i], "0"); // Mặc định tất cả ghế đều trống ("0")
        }

        // --- 4. Đọc Số lượng vé đã bán ---
        int soVeDaBan = 0;
        if (f >> soVeDaBan) {
            tmp->sove = soVeDaBan;
            f.ignore(100, '\n'); // Xóa bộ đệm sau khi đọc xong con số
        }
        // --- 5. Đọc danh sách vé bằng vòng FOR (Vị trí | CMND) ---
        int vt;
        char cmnd[cmnd_max];
        for (int i = 0; i < soVeDaBan; i++) {
            if (!(f >> vt)) break; // Kiểm tra nếu không đọc được số vị trí
            f.ignore(1, '|');      // Bỏ qua đúng 1 ký tự '|'

    // Đọc CMND cho đến khi gặp dấu '|' hoặc xuống dòng
            f >> cmnd;
            // Kiểm tra vị trí hợp lệ rồi mới gán CMND vào ghế
            if (vt >= 1 && vt <= tmp->socho) {
                strcpy(tmp->DSV[vt - 1], cmnd);
            }
        }
    // --- 6. Thêm vào Danh sách liên kết ---
    if (!Add_CB(dsCB, dsMB, tmp)) {
        delete[] tmp->sbDich;
        for (int i = 0; i < tmp->socho; i++) {
            delete[] tmp->DSV[i];
        }
        delete[] tmp->DSV;
        delete tmp; 
    }
    f >> ws;    
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
        // Đếm số vé đã bán
        f << temp->sove << '\n';
        // Ghi danh sách vé (vị trí | CMND)
        if(temp->sove > 0) {    
            int d=0;
            for (int i = 0; i < temp->socho; i++) {
                if (strcmp(temp->DSV[i], "0") != 0) {
                    f << (i + 1) << '|' << temp->DSV[i];
                    d++;
                    if(d<temp->sove||temp->next!=NULL) f << '\n';
                }
            }
        }else{
            if(temp->next!=NULL) f << '\n';
        }
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
    f << '\n' <<CB.maCB << '|'
        << setfill('0') << setw(2) << CB.ngayKH.hh << ':'
        << setfill('0') << setw(2) << CB.ngayKH.mm << '|'
        << setfill('0') << setw(2) << CB.ngayKH.dd << '/'
        << setfill('0') << setw(2) << CB.ngayKH.mt << '/'
        << setfill('0') << setw(4) << CB.ngayKH.yy << '|'
        << CB.sbDich << '|'
        << CB.trangThai << '|'
        << CB.soHieuMB << '|';
    // Đếm số vé đã bán
    f << CB.sove << '\n';
    // Ghi danh sách vé (vị trí | CMND)
    if(CB.sove > 0) {    
        int d=0;
        for (int i = 0; i < CB.socho; i++) {
            if (strcmp(CB.DSV[i], "0") != 0) {
                f << (i + 1) << '|' << CB.DSV[i];
                d++;
                if(d<CB.sove) f << '\n';
            }
        }
    }
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