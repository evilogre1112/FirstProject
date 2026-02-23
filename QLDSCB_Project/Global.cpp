#include "Global.h"


// ---- cấu trúc máy bay ----//
 MB::MB() {
    strcpy(soHieuMB,"");    // hàm này copy từ phía phải sang trái
    strcpy(loaiMB,"");      // có cáh tương đương đó là cho phần [0]='\0';
    socho = 0;
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
    if (yy % 4==0 && yy % 100 == 0) thang[2]=29;
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
        DSV[0] = nullptr;
        socho = sc;
        sbDich = new char[50];        // độ dài tên 50 kí tự
        sbDich[0] = '\0';
        if(sc>0) {
            DSV= new char*[socho];
            for(int i=0;i<sc;i++)
            {
                DSV[i] = new char[13];
                DSV[i][0] = '\0';
            }
        } else { DSV=NULL;}
    }
CB::CB(){
    next = NULL;
    socho = 0;
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
    listCB::listCB(){
        slCB = 0;
        head = NULL;
    }

    // ---- cấu trúc hành khách ----//
    HK::HK(){
        ho = new char[41];
        ten = new char[16];
        cmnd = new char[14];
        left = right = NULL;
    }

    
    bool HK::set_cmnd(){
        cin.getline(cmnd,14);
        int n = strlen(cmnd);
        for(int i = 0; i < n; i++)
        if(cmnd[i] < '0' || cmnd[i] > '9') return false;
        return true;           
    }

    bool HK::set_ho(char *new_ho){
        int n = strlen(new_ho);
        for(int i = 0; i < n; i++)
        if(new_ho[i] >= '0' && new_ho[i] <= '9') return false;
        ho = new_ho;
        return true;
    }
    bool HK::set_ten(char *new_ten){
        int n = strlen(new_ten);
        for(int i = 0; i < n; i++)
        if(new_ten[i] >= '0' && new_ten[i] <= '9') return false;
        ten = new_ten;
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
    HK::~HK(){
        delete[] ho;
        delete[] ten;
        delete[] cmnd;
    }


    listHK::listHK(){
        slHK = 0;
        goc = NULL;
    }
