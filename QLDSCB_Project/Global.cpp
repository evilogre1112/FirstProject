#include "Global.h"
#include "Logic.h"
#include <fstream>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <string>
#include <ctime>


using namespace std;

// ---- cấu trúc máy bay ----//
MB::MB() {
    strcpy(soHieuMB,"");    // hàm này copy từ phía phải sang trái
    strcpy(loaiMB,"");      // có cáh tương đương đó là cho phần [0]='\0';
    socho = 0;
    SLB = 0;
    dsHD.head = NULL;
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

void listMB::Clear() {
    for (int i = 0; i < slMB; i++) {
        if (list[i] != NULL) {
            delete list[i];
            list[i] = NULL;
        }
    }
    slMB = 0;
}

DateTime::DateTime (){
    hh=12;
    mm=00;
    dd=1;
    mt=1;
    yy=2026;
}

int DateTime::get_hh()const{
        return hh;
}

int DateTime::get_mm()const{
    return mm;
}

int DateTime::get_dd()const{
        return dd;
}

int DateTime::get_mt()const{
        return mt;
}

int DateTime::get_yy()const{
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

void DateTime::time_now() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    this->hh = ltm->tm_hour;
    this->mm = ltm->tm_min;
    this->dd = ltm->tm_mday;
    this->mt = ltm->tm_mon + 1;
    this->yy = ltm->tm_year + 1900;
}

    // ---- cấu trúc chuyến bay ----

CB::CB(int sc) {                      // truyền tham số socho 
    maCB[0] = '\0';
    trangThai = 1;
    soHieuMB[0] = '\0';
    socho = sc;
    sove = 0;
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
    next = NULL;
}

CB::CB(){
    maCB[0] = '\0';
    trangThai = 1;
    soHieuMB[0] = '\0';
    sbDich = NULL;
    DSV = NULL;
    socho = 0;
    sove = 0;
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

void listCB::Clear() {
    CB* temp = head;
    while (temp != NULL) {
        CB* toDelete = temp;
        temp = temp->next;
        delete toDelete;
    }
    head = NULL;
    slCB = 0;
}

// -- Cấu trúc đánh dấu CB --
markCB::markCB() {
    mark = NULL;
    next = NULL;
}

markList::markList() {
    head = NULL;
}

    // ---- cấu trúc hành khách ----//
    HK::HK(){
        ho =NULL;
        ten =NULL;
        cmnd = NULL;
        phai = false;
        left = right = NULL;
        dsDatVe.head = NULL;
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


