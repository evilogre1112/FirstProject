#include "Global.h"

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

    HK::HK(){
        ho = new char[40];
        ten = new char[15];
        cmnd = new char[13];
        ho_ten_full = new char[60];
        trai = phai = NULL;
    }
    void HK::ho_ten(){
        cout<<"nhap ho va ten : ";
        int i=0;
        while(true){
            char tmp = getch();
            if (tmp == '\n' || tmp == 13 || tmp == '\r')    //các kí tự cso thể của enter
            {
                ho_ten_full[i]='\0';
                break;
            }else if (tmp == 8){                            // phòng nút xóa khi viết sai
                if (i > 0){
                    i--;
                    cout<<"\b \b";
                }
                // chặn cách kí tự số và kí tự lạ
            }else if ((tmp<='A'&& tmp>='Z') || (tmp <= 'a'&& tmp >='z')|| tmp == ' '){
                ho_ten_full[i++] = tmp;
                cout << tmp;
            }
        }
        cout<<endl;
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
    }
    void HK::nhap_cmnd(){
        cout<<"nhap CMND : ";
        int i=0;
        while(true){
            char tmp = getch();
            if (tmp == '\n' || tmp == 13 || tmp == '\r')    //các kí tự cso thể của enter
            {
                cmnd[i]='\0';
                break;
            }else if (tmp == 8){                            // phòng nút xóa khi viết sai
                if (i > 0){
                    i--;
                    cout<<"\b \b";
                }
                // chặn cách kí tự số và kí tự lạ
            }else if (tmp>= '0' && tmp <= '9'){
               if(i<12) 
               { 
                cmnd[i++] = tmp;
                cout << tmp;
               }
            }
        }              
    }

    listHK::listHK(){
        slHK = 0;
        goc = NULL;
    }
