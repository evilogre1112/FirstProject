#include "Global.h"


// ---- cấu trúc chuyến bay ----//
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

short DateTime::get_hh(){
        return hh;
}
short DateTime::get_mm(){
    return mm;
}
short DateTime::get_dd(){
        return dd;
}
short DateTime::get_mt(){
        return mt;
}
short DateTime::get_yy(){
        return yy;
}
bool DateTime::set_hh(short h){
    if (h >= 0 && h <= 23) {
        hh = h;
        return true;
    }
    return false;
}
bool DateTime::set_mm(short m){
    if (m >= 0 && m <= 59) {
        mm = m;
        return true;
    }
    return false;
}
bool DateTime::set_mt(short t){
    if (t >= 1 && t <= 12) {
        mt = t;
        return true;
    }
    return false;
}
bool DateTime::set_yy(short y){
    if (y >= 1900 && y <= 2100) { // Giới hạn năm hợp lý
        yy = y;
        return true;
    }
    return false;
}
bool DateTime::set_dd(short d){
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
        next = NULL;
        maCB[0] = '\0';
        trangThai = 1;
        soHieuMB[0] = '\0';
        DSV[0] = '\0';
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
        CBdau = NULL;
    }

    // ---- cấu trúc hành khách ----//
    HK::HK(){
        ho = new char[41];
        ten = new char[16];
        cmnd = new char[14];
        ho_ten_full = new char[61];
        trai = phai = NULL;
    }
    
    bool HK::ktc(){
        int n = strlen(ho_ten_full);
        for(int i = 0; i < n; i++)
        if(ho_ten_full[i] >= '0' && ho_ten_full[i] <= '9') return false;
        return true;
    } 

    bool HK::kts(){
        int n = strlen(cmnd);
        for(int i = 0; i < n; i++)
        if(cmnd[i] < '0' || cmnd[i] > '9') return false;
        return true;
    }

    bool HK::set_ho_ten(){
        cout<<"nhap ho va ten : ";
        cin.getline(ho_ten_full,60);
        if(ktc()){                     // có số là false
            // tách tên và họ ra
            int n = strlen(ho_ten_full);
            int pos = -1;
            for(pos=n-1;pos >= 0 && ho_ten_full[pos] != ' ';pos--);
            if(pos < 0){
                strcpy(ho,"");
                strcpy(ten,ho_ten_full);
            }else{
                strcpy(ten, ho_ten_full + pos + 1);     //con tro chuoi
                strncpy(ho, ho_ten_full, pos);
                ho[pos] = '\0';
            }
            return true;
        }     
        return false;                    
    }
    
    bool HK::set_cmnd(){
        cin.getline(cmnd,14);
        return kts();            
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
        delete[] ho_ten_full;
    }


    listHK::listHK(){
        slHK = 0;
        goc = NULL;
    }
