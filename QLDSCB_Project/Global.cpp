#include "Global.h"
#include "Logic.h"
#include <fstream>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <string>


using namespace std;

// ---- cấu trúc máy bay ----//
MB::MB() {
    strcpy(soHieuMB,"");    // hàm này copy từ phía phải sang trái
    strcpy(loaiMB,"");      // có cáh tương đương đó là cho phần [0]='\0';
    socho = 0;
    SLB = 0;
}

bool MB::set_soHieuMB(char *shmb) {
    if (shmb == NULL || strlen(shmb) == 0) return false;
    strncpy(this->soHieuMB, shmb, soHieuMB_max);
    this->soHieuMB[soHieuMB_max - 1] = '\0';
    return true;
}

bool MB::set_loaiMB(char *loai) {
    if (loai == NULL || strlen(loai) == 0) return false;
    strncpy(this->loaiMB, loai, loaiMB_max);
    this->loaiMB[loaiMB_max - 1] = '\0';
    return true;
}

bool MB::set_socho(int c) {
    if (c <= 0) return false;
    this->socho = c;
    return true;
}


listMB::listMB() {
    slMB = 0;
    for (int i = 0; i < slMB_max; i++)
        list[i] = NULL;
}

DateTime::DateTime (){
    hh=12;
    mm=00;
    dd=1;
    mt=1;
    yy=2026;
}

int DateTime::get_hh(){
        return hh;
}

int DateTime::get_mm(){
    return mm;
}

int DateTime::get_dd(){
        return dd;
}

int DateTime::get_mt(){
        return mt;
}

int DateTime::get_yy(){
        return yy;
}

bool DateTime::set_hh(int h){
    if (h >= 0 && h <= 23) {
        hh = h;
        return true;
    }
    return false;
}

bool DateTime::set_mm(int m){
    if (m >= 0 && m <= 59) {
        mm = m;
        return true;
    }
    return false;
}

bool DateTime::set_mt(int t){
    if (t >= 1 && t <= 12) {
        mt = t;
        return true;
    }
    return false;
}

bool DateTime::set_yy(int y){
    if (y >= 1900 && y <= 2100) { // Giới hạn năm hợp lý
        yy = y;
        return true;
    }
    return false;
}

bool DateTime::set_dd(int d){
    short thang[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (yy % 4==0 && yy % 100 != 0|| yy % 400==0) thang[2]=29;
    if (d >= 1 && d <= thang[mt]) {
        dd = d;
        return true;
    }
    return false;
}

    // ---- cấu trúc chuyến bay ----

CB::CB(int sc) {                      // truyền tham số socho 
        maCB[0] = '\0';
        trangThai = 1;
        soHieuMB[0] = '\0';
        socho = sc;
        sbDich = new char[50];        // độ dài tên 50 kí tự
        sbDich[0] = '\0';
        if(sc>0) {
            DSV= new char*[socho];
            for(int i=0;i<sc;i++)
            {
                DSV[i] = new char[cmnd_max];
                DSV[i][0] = '\0';
            }
        } else { DSV = NULL;}
    }

CB::CB(){
    maCB[0] = '\0';
    trangThai = 1;
    soHieuMB[0] = '\0';
    sbDich = NULL;
    DSV = NULL;
    socho = 0;
    next = NULL;
}    

    CB::~CB(){
        if(sbDich != NULL) delete[] sbDich;
        if (DSV !=NULL) 
        {
            for(int i=0; i<socho; i++){
                delete[] DSV[i];        //giải phóng chuỗi Cmnd
            }
            delete[] DSV;
        }
    }
    
    bool CB::set_maCB(char ma[maCB_max])
    {
        if(ma==NULL || strlen(ma)==0) return false; //kiểm tra rỗng
        strncpy(this->maCB, ma, maCB_max);
        this->maCB[maCB_max-1] ='\0';
        //bảo đảm chuỗi luôn có điểm kết thuc
        return true;
    }

    bool CB::set_ngayHK(int h, int m, int y, int mmt, int d){
        bool check = true;  
        check &= this->ngayKH.set_yy(y);
        check &= this->ngayKH.set_mt(mmt);
        check &= this->ngayKH.set_dd(d);
        check &= this->ngayKH.set_hh(h);
        check &= this->ngayKH.set_mm(m);
        return check; // Trả về true nếu tất cả các giá trị đều hợp lệ

    }

    bool CB::set_sbDich(char *sbd){
        int n=strlen(sbd);
        if(sbd==NULL|| n==0) return false;

        // giải phóng bộ nhớ nếu còn
        if( this->sbDich != NULL ) delete[] this->sbDich;
        this->sbDich = new char[n+1];
        strncpy(this->sbDich,sbd,n);
        this->sbDich[n] = '\0';
        return true;
    }

    bool CB::set_trangThai(int i){
        // Kiểm tra i có nằm trong phạm vi 0-3 không
        if (i >= 0 && i <= 3) {
            this->trangThai = i;
            return true;
        }
        return false;
    }

    bool CB::set_soHieuMB(char *shmb){
        int n=strlen(shmb);
        if(shmb==NULL|| n==0) return false;

        strncpy(this->soHieuMB,shmb,n);
        this->soHieuMB[n] = '\0';
        return true;
    }

    bool CB::set_socho(int c) {
    if (c <= 0) return false;

    // 1. Giải phóng bộ nhớ cũ (nếu có) để tránh memory leak
    if (this->DSV != nullptr) {
        for (int i = 0; i < this->socho; i++) {
            delete[] this->DSV[i]; // Giải phóng từng chuỗi cmnd
        }
        delete[] this->DSV; // Giải phóng mảng con trỏ
    }

    // 2. Cập nhật số chỗ và cấp phát mới
    this->socho = c;
    this->DSV = new char*[c];
    for (int i = 0; i < c; i++) {
        this->DSV[i] = new char[cmnd_max];
        strcpy(this->DSV[i], "0");
    }

    return true;

}
    listCB::listCB(){
        slCB = 0;
        head = NULL;
    }

    // ---- cấu trúc hành khách ----//
    HK::HK(){
        ho =NULL;
        ten =NULL;
        cmnd = NULL;
        phai = false;
        left = right = NULL;
    }

    bool HK::set_cmnd(char *new_cmnd){
        if(new_cmnd ==NULL) return false;
        int n = strlen(new_cmnd);
        for(int i = 0; i < n; i++)
            if(new_cmnd[i] < '0' || new_cmnd[i] > '9') return false;
        //giải phóng bộ nhớ nếu có
        if(this->cmnd!= NULL)  delete[] this->cmnd;
        // cấp lại bộ nhớ mới'
        this->cmnd= new char[n+1];
        //dùng strncpy để sao chép an toàn
        strncpy(this->cmnd, new_cmnd,n);
        this->cmnd[n]='\0';
        return true;           
    }

    bool HK::set_ho(char *new_ho){
        if(new_ho ==NULL) return false;
        int n = strlen(new_ho);
        for(int i = 0; i < n; i++)
            if(new_ho[i] >= '0' && new_ho[i] <= '9') return false;
        //giải phóng bộ nhớ nếu có
        if(this->ho!= NULL)  delete[] this->ho;
        // cấp lại bộ nhớ mới'
        this->ho= new char[n+1];
        //dùng strncpy để sao chép an toàn
        strncpy(this->ho, new_ho,n);
        this->ho[n]='\0';
        return true;
    }

    bool HK::set_ten(char *new_ten){
       if(new_ten ==NULL) return false;
        int n = strlen(new_ten);
        for(int i = 0; i < n; i++)
            if(new_ten[i] >= '0' && new_ten[i] <= '9') return false;
        //giải phóng bộ nhớ nếu có
        if(this->ten!= NULL)  delete[] this->ten;
        // cấp lại bộ nhớ mới'
        this->ten= new char[n+1];
        //dùng strncpy để sao chép an toàn
        strncpy(this->ten, new_ten,n);
        this->ten[n]='\0';
        return true;
    }

    char* HK::get_cmnd(){
        return cmnd;
    }

    char* HK::get_ho()
    {
        return ho;
    }

    char* HK::get_ten(){
        return ten;
    }
//--------------------------------------------------
    bool HK::set_phai(bool gt){
        phai = gt;
        return true;
    }

    bool HK::get_phai(){
        return phai;
    }

    HK::~HK(){
        delete[] ho;
        delete[] ten;
        delete[] cmnd;
    }


    listHK::listHK(){
        slHK = 0;
        goc = NULL;
    }

    // ---- đọc file ----


    bool Get_Data_CB(listCB &dsCB, listMB &dsMB, const char *path_file_CB){
        ifstream f(path_file_CB);                               // mở file cb
        if( !f.is_open() ) return false;
        dsCB.slCB=0;
        dsCB.head = NULL;
    
        while(true){
            CB* tmp =new CB();

            if (!(f.getline(tmp->maCB, maCB_max, '|'))) {
                delete tmp;                                     // Xóa vùng nhớ vừa cấp phát vì không có dữ liệu để lưu
                break;
            }
            //---doc ngay thang---
            f >> tmp->ngayKH.hh;f.ignore(1,':');
            f >> tmp->ngayKH.mm;f.ignore(1,'|');
            f >> tmp->ngayKH.dd;f.ignore(1,'/');
            f >> tmp->ngayKH.mt;f.ignore(1,'/');
            f >> tmp->ngayKH.yy;f.ignore(1,'|');
            //---sân bay đích---
            tmp->sbDich = new char[sbDich_max];
            f.getline(tmp->sbDich, sbDich_max ,'|');
            //---trang thai---
            f>>tmp->trangThai;f.ignore(1,'|');
            //---số hiệu máy bay---
            f.getline(tmp->soHieuMB, soHieuMB_max,'|');
            //---số chỗ---
            f>>tmp->socho;            
            f.ignore(100,'\n');// nhảy xuống dòng mới sau khi đọc số chỗ
            //---mảng danh sách cmnd----
            // 2. Cấp phát mảng DSV dựa trên SỐ CHỖ TỐI ĐA (Khung máy bay)
            if (tmp->socho > 0 && tmp->socho <= socho_max) {
                tmp->DSV = new char*[tmp->socho];
                for (int i = 0; i < tmp->socho; i++) {
                    tmp->DSV[i] = new char[cmnd_max];
                    strcpy(tmp->DSV[i], "0"); // Khởi tạo tất cả ghế là trống
                }
                // 3. Đọc danh sách CMND đã bán dựa trên SỐ CHỖ ĐÃ BÁN
                char line[50];
            while (true) {
                streampos pos_truoc = f.tellg(); // Lưu vị trí hiện tại
                
                if (!f.getline(line, 50)) break;
                if (strlen(line) == 0) continue;

                // Đếm số dấu '|' để phân biệt dòng khách và dòng CB mới
                int countSep = 0;
                for (int i = 0; line[i] != '\0'; i++) {
                    if (line[i] == '|') countSep++;
                }

                if (countSep == 1) {
                    // Tách CMND và Số vé bằng hàm tìm ký tự strchr
                    char *ptr = strchr(line, '|'); 
                    if (ptr != NULL) {
                        *ptr = '\0'; // Tạm thời cắt chuỗi tại dấu '|'
                        char *s_cmnd = line;       // Phần trước '|'
                        int vi_tri = atoi(ptr + 1); // Phần sau '|' chuyển thành số

                        if (vi_tri >= 1 && vi_tri <= tmp->socho) {
                            strncpy(tmp->DSV[vi_tri - 1], s_cmnd, cmnd_max - 1);
                            tmp->DSV[vi_tri - 1][cmnd_max - 1] = '\0';
                        }
                    }
                } else {
                    // Nếu là mã CB mới (nhiều dấu |), quay lại vị trí cũ và dừng đọc vé
                    f.seekg(pos_truoc);
                    break;
                }
                }
            }
            Add_CB(dsCB, dsMB, tmp);

        f >> ws; // Bỏ qua khoảng trắng thừa cuối file
    }
    f.close();
    return true;
}
    bool Get_Data_MB(listMB &dsMB, const char *path_file_MB){
        ifstream f(path_file_MB);
        if(!f.is_open()) return false;
        dsMB.slMB=0;
        while(dsMB.slMB < slMB_max&& !f.eof() ){        //đọc đến cuối file
            MB* tmp =new MB();                          //tạo ô nhớ chưaas MB
            f.getline(tmp->soHieuMB,soHieuMB_max,'|');
            // --- số hiệu máy bay----
            if(strlen(tmp->soHieuMB)== 0||f.eof()){
                delete tmp;
                break;
            }
            //---loai mb----
            f.getline(tmp->loaiMB, loaiMB_max,'|');
            //---so cho---
            if(!(f>> tmp->socho)) {
                delete tmp;
                break;
            }f.ignore(1, '|'); // Bỏ qua dấu gạch đứng sau số chỗ

            // 4. Đọc Số lần bay (SLB)
            if (!(f >> tmp->SLB)) {
            // Nếu không đọc được SLB, có thể để mặc định là 0
            tmp->SLB = 0;}
            // xóa dáu cách còn lại dấu chống dòng
            f.ignore(100,'\n');
            //dùng hàm Add_MB để thêm vào danh sách máy bay
           if (!Add_MB(dsMB, tmp)) {
            // Nếu Add_MB trả về false (do trùng mã hoặc danh sách đầy 300)
            delete tmp; 
            // Không cần break ở đây nếu bạn muốn tiếp tục đọc các máy bay khác hợp lệ
        }

        if (f.eof()) break;
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

        // TẬN DỤNG HÀM CỦA BẠN MÌNH
        // Hàm này đã dùng vòng lặp while để chèn vào cây nên ko sợ đệ quy
        Add_HK(dsHK, b_ho, b_ten, b_cmnd, b_phai);
    }

    f.close();
    return true;
}
