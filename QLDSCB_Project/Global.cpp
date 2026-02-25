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
int DateTime::get_yy(){
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
bool DateTime::set_yy(int y){
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
        phai = false;
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


    bool set_CB(listCB &dsCB, const char *cb){
        ifstream f(cb);                         // mở file cb
        if( !f.is_open() ) return false;
        dsCB.slCB=0;
        dsCB.head = NULL;
        CB* last = NULL;    // con trỏ lưu trữ địa chỉ cuối trước đó
        
        while(!f.eof()){
            CB* tmp =new CB();
            f.getline(tmp->maCB,maCB_max,' ');
            //---doc ngay thang---
            f >> tmp->ngayKH.hh;
            f.ignore();
            f >> tmp->ngayKH.mm;
            f.ignore();
            f >> tmp->ngayKH.dd;
            f.ignore();
            f >> tmp->ngayKH.mt;
            f.ignore();
            f >> tmp->ngayKH.yy;
            f.ignore();
            //---sân bay đích---
            tmp->sbDich = new char[50];
            f.getline(tmp->sbDich,50 ,' ');
            //---trang thai---
            f>>tmp->trangThai;
            //---số hiệu máy bay---
            f.getline(tmp->soHieuMB, soHieuMB_max,' ');
            //---số chỗ ----
            f >> tmp->socho;
            //---mảng danh sách cmnd----
            tmp->DSV = new char*[tmp->socho];
            for(int i=0;i< tmp->socho;i++){
                tmp->DSV[i] = new char[cmnd_max];
                f >> tmp->DSV[i];
            }
            f.ignore();
            //---con tro next---
            tmp->next=NULL;
            if(dsCB.head == NULL)
            {
                dsCB.head=tmp;
                //nút dầu tiên
            }else {
                last->next=tmp;
                // last này là nút trước đó đang nối với nút mới
            }
            last=tmp;
            // cập nhật nut nới
        }
        f.close();
        return true;
    }

    bool set_MB(listMB &dsMB, const char *mb){
        ifstream f(mb);
        if(!f.is_open()) return false;
        dsMB.slMB=0;
        while(dsMB.slMB < slMB_max&& !f.eof() ){//đọc đến cuối file
            MB* tmp =new MB();                  //tạo ô nhớ chưaas MB
            f.getline(tmp->soHieuMB,soHieuMB_max,' ');
            // --- số hiệu máy bay----
            if(strlen(tmp->soHieuMB)== 0||f.eof()){
                delete tmp;
                break;
            }
            //---loai mb----
            f.getline(tmp->loaiMB, loaiMB_max,' ');
            f>> tmp->socho;
            // xóa dáu cách còn lại dấu chống dòng
            f.ignore();
            dsMB.list[dsMB.slMB]=tmp;
            dsMB.slMB++;
        }
        f.close();
        return true;
    }

    bool set_KH_tree(HK* & goc, HK * moi){

    }
    bool set_HK(listHK &dsHK, const char *hk){
        ifstream f(hk);
        if(!f.is_open()) return false;
        dsHK.goc =NULL;         // cây rỗng
        dsHK.slHK=0;
        while(!f.eof()){
            HK* tmp = new HK();
            
        }
        return true;
    }

    bool set_file(listMB &dsMB, listCB &dsCB, listHK &dsHK, const char *mb, const char *cb, const char *hk ){
        bool ok1 = set_MB(dsMB , mb);
        bool ok2= set_CB(dsCB , cb);
        bool ok3 = set_HK(dsHK , hk);

        return ok1&&ok2&&ok3;
    }
