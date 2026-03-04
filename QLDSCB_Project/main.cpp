//Hạn chế sử dụng bits/stdc++.h, tốn bộ nhớ
#include <iostream>
#include "Global.h"
#include "Logic.h"

using namespace std ;

// Hàm hỗ trợ in cây nhị phân hành khách (Duyệt trung thứ tự - Inorder)
void print_HK_test(HK* goc) {
    if (goc != NULL) {
        print_HK_test(goc->left);
        cout << left << setw(15) << goc->cmnd 
             << setw(25) << goc->ho 
             << setw(15) << goc->ten 
             << (goc->phai ? "Nu" : "Nam") << endl;
        print_HK_test(goc->right);
    }
}

int main(){
    listCB dsCB;
    listHK dsHK;
    listMB dsMB;
    cout << "--- DANG NAP DU LIEU TU FILE ---" << endl;

    // 1. Test nạp Máy Bay
    if (set_MB(dsMB, path_file_MB)) {
        cout << "[OK] Nap file May Bay thanh cong. So luong: " << dsMB.slMB << endl;
        for(int i=0; i < dsMB.slMB; i++) {
            cout << "   + " << dsMB.list[i]->soHieuMB << " (" << dsMB.list[i]->loaiMB << ")" << endl;
        }
    } else {
        cout << "[Loi] Khong mo duoc file May Bay tai: " << path_file_MB << endl;
    }

    // 2. Test nạp Hành Khách
    if (set_HK(dsHK, path_file_HK)) {
        cout << "[OK] Nap file Hanh Khach thanh cong. So luong: " << dsHK.slHK << endl;
        cout << left << setw(15) << "CMND" << setw(25) << "HO" << setw(15) << "TEN" << "PHAI" << endl;
        print_HK_test(dsHK.goc);
    } else {
        cout << "[Loi] Khong mo duoc file Hanh Khach tai: " << path_file_HK << endl;
    }

    // 3. Test nạp Chuyến Bay
    if (set_CB(dsCB, path_file_CB)) {
        cout << "[OK] Nap file Chuyen Bay thanh cong. So luong: " << dsCB.slCB << endl;
        CB* p = dsCB.head;
        while (p != NULL) {
            cout << "   + Ma CB: " << p->maCB << " | Dich: " << p->sbDich 
                 << " | Ngay: " << p->ngayKH.dd << "/" << p->ngayKH.mt << "/" << p->ngayKH.yy << endl;
            p = p->next;
        }
    } else {
        cout << "[Loi] Khong mo duoc file Chuyen Bay tai: " << path_file_CB << endl;
    }

    cout << "--- KET THUC KIEM TRA ---" << endl;
    
    // Lưu ý: Trong thực tế bạn nên gọi các hàm Clear/Delete để giải phóng bộ nhớ trước khi return 0
    return 0;
}
