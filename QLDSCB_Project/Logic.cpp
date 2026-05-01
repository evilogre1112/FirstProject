#include "Logic.h"
#include "Global.h"
#include <iostream>
#include <fstream>


// Một số tiện ích
int ss_str(char* const a, char* const b) {
    unsigned long len_a = strlen(a);
    unsigned long len_b = strlen(b);
    unsigned long n = min (len_a, len_b);
    for (unsigned long i = 0; i < n; i++) {
        if (a[i] > b[i]) return 1;
        if (a[i] < b[i]) return 2;
    }
    if (len_a > len_b) return 1;
    if (len_a < len_b) return 2;
    return 0;
} // a == b return 0, a > b return 1, a < b return 2

void to_lower(char* str) {
    int i = 0;
    while (str[i] != '\0') {
        if (str[i] >= 'A' && str[i] <= 'Z') str[i] = tolower(str[i]);
        i++;
    }
}

bool cmp_prefix(char* str, char* prefix) {
    int i = 0;
    while (str[i] != '\0' && prefix[i] != '\0') {
        if (str[i] != prefix[i]) return false;
        i++;
    }
    if (str[i] == '\0' && prefix[i] != '\0') return false;
    return true;
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

// Các hàm phục vụ cho việc đánh dấu các CB cho MB và HK
void clearMarkMB(MB* MB) {
    markCB* tnode = MB->dsHD.head;
    while (tnode != NULL) {
        markCB* temp = tnode;
        tnode = tnode->next;
        delete temp;
    }
    MB->dsHD.head = NULL;
}

bool addMarkMB(MB* MB, markCB* newMark) {
    markCB* tnode = MB->dsHD.head;
    if (newMark == NULL) return false;
    if (tnode == NULL) {
        MB->dsHD.head = newMark;
        return true;
    }
    while (tnode->next != NULL) {
        if (ss_ngay(tnode->mark->ngayKH, newMark->mark->ngayKH) < 1440) return false;
        tnode = tnode->next;
    }
    if (ss_ngay(tnode->mark->ngayKH, newMark->mark->ngayKH) < 1440) return false;
    tnode->next = newMark;
    return true;
}

bool delMarkMB(MB* MB, char* const maCB) {
    markCB* tnode = MB->dsHD.head;
    if (tnode == NULL) return false;
    if (ss_str(tnode->mark->maCB, maCB) == 0) {
        markCB* tmpDel = tnode;
        tnode = tnode->next;
        MB->dsHD.head = tnode;
        delete tmpDel;
        return true;
    }
    markCB* temp = tnode;
    tnode = tnode->next;
    while (tnode != NULL) {
        if (ss_str(tnode->mark->maCB, maCB) == 0) {
            markCB* tmpDel = temp->next;
            temp->next = tnode->next;
            delete tmpDel;
            return true;
        }
        tnode = tnode->next;
        temp = temp->next;
    }
    return false;
}

void clearMarkHK(HK* HK) {
    markCB* tnode = HK->dsDatVe.head;
    while (tnode != NULL) {
        markCB* temp = tnode;
        tnode = tnode->next;
        delete temp;
    }
    HK->dsDatVe.head = NULL;
}

bool addMarkHK(HK* HK, markCB* newMark) {
    markCB* tnode = HK->dsDatVe.head;
    if (newMark == NULL) return false;
    if (tnode == NULL) {
        HK->dsDatVe.head = newMark;
        return true;
    }
    while (tnode->next != NULL) {
        if (ss_ngay(tnode->mark->ngayKH, newMark->mark->ngayKH) < 1440) return false;
        tnode = tnode->next;
    }
    if (ss_ngay(tnode->mark->ngayKH, newMark->mark->ngayKH) < 1440) return false;
    tnode->next = newMark;
    return true;
}

bool delMarkHK(HK* HK, char* const maCB) {
    markCB* tnode = HK->dsDatVe.head;
    if (tnode == NULL) return false;
    if (ss_str(tnode->mark->maCB, maCB) == 0) {
        markCB* tmpDel = tnode;
        tnode = tnode->next;
        HK->dsDatVe.head = tnode;
        delete tmpDel;
        return true;
    }
    markCB* temp = tnode;
    tnode = tnode->next;
    while (tnode != NULL) {
        if (ss_str(tnode->mark->maCB, maCB) == 0) {
            markCB* tmpDel = temp->next;
            temp->next = tnode->next;
            delete tmpDel;
            return true;
        }
        tnode = tnode->next;
        temp = temp->next;
    }
    return false;
}

// May Bay

void swap_MB(MB* &a, MB* &b) {
    MB* temp = a;
    a = b;
    b = temp;
}

// Tiện ích sắp xếp
int Hoare_Partion(listMB& dsMB, int l,int r) {
    int i = l, j = r;
    char pivot[soHieuMB_max];
    strcpy(pivot, dsMB.list[l]->soHieuMB);
    while (true) {
        while (ss_str(dsMB.list[i]->soHieuMB, pivot) == 2) i++;
        while (ss_str(dsMB.list[j]->soHieuMB, pivot) == 1) j--;
        if (i < j) {
            swap_MB(dsMB.list[i], dsMB.list[j]);
            i++;
            j--;
        }
        else return j;
    }
    return j;
}
// Quick sort MB
void Sort_MB(listMB& dsMB, int l, int r) {
    if (l >= r) return;
    int m = Hoare_Partion(dsMB, l, r);
    Sort_MB(dsMB, l, m);
    Sort_MB(dsMB, m + 1, r);
}

// Tìm vị trí chèn
int find_insert_posMB(listMB& dsMB, char* const soHieuMB) {
    int l = 0, r = dsMB.slMB - 1;
    int m = (l + r) / 2;
    while (l <= r) {
        int result = ss_str(dsMB.list[m]->soHieuMB, soHieuMB);
        if (result == 0) return m;
        else if (result == 1) r = m - 1;
        else l = m + 1;
        m = (l + r) / 2;
    }
    return l; // tra ve vi tri can chen
}

int Find_MB(listMB& dsMB, char* const soHieuMB) {
    int l = 0, r = dsMB.slMB - 1;
    int m = (l + r) / 2;
    while (l <= r) {
        int result = ss_str(dsMB.list[m]->soHieuMB, soHieuMB);
        if (result == 0) return m;
        else if (result == 1) r = m - 1;
        else l = m + 1;
        m = (l + r) / 2;
    }
    return -1; // not found
}

bool Add_MB(listMB& dsMB, MB *newMB) {
    if (dsMB.slMB >= 300) {
        return false;
    }
    int index = find_insert_posMB(dsMB, newMB->soHieuMB);
    if (index == dsMB.slMB) {
        dsMB.list[index] = newMB;
        dsMB.slMB++;
        return true;
    }
    if (ss_str(dsMB.list[index]->soHieuMB, newMB->soHieuMB) == 0) {
        return false;
    }
    for (int i = dsMB.slMB; i > index; i--) dsMB.list[i] = dsMB.list[i - 1];
    dsMB.list[index] = newMB;
    dsMB.slMB++;
    return true;
}

bool Del_MB(listMB& dsMB, listCB &dsCB, char* const soHieuMB) {
    CB* temp = Find_Active_MB(dsCB, soHieuMB);
    if (temp != NULL) return false; // Khong the xoa mot may bay ma da co chuyen bay dang su dung
    int index = Find_MB(dsMB, soHieuMB);
    if (index == -1) return false; // Khong the xoa mot may bay khong ton tai
    markCB* tnode = dsMB.list[index]->dsHD.head;
    while (tnode != NULL) { // Huy nhung chuyen bay dang su dung may bay bi xoa
        tnode->mark->trangThai = 0;
        for (int i = 0; i < tnode->mark->socho; i++)
            delete[] tnode->mark->DSV[i];
        delete[] tnode->mark->DSV;
        tnode->mark->DSV = NULL;
        tnode->mark->socho = 0;
        tnode = tnode->next;
    }
    clearMarkMB(dsMB.list[index]);
    delete dsMB.list[index];
    for (int i = index; i < dsMB.slMB - 1; i++) {
        dsMB.list[i] = dsMB.list[i + 1];
    }
    dsMB.list[dsMB.slMB - 1] = NULL;
    dsMB.slMB--;
    return true;
}

bool Edit_MB(listMB& dsMB, listCB& dsCB, char* const soHieuMB, MB *infoUpdate) {
    int index = Find_MB(dsMB, soHieuMB);
    if (index == -1) return false;
    CB* temp = Find_Active_MB(dsCB, soHieuMB);
    markCB* tnode = dsMB.list[index]->dsHD.head;
    if (temp != NULL) { // Có chuyến bay đang sử dụng máy bay này
        while (tnode != NULL) {
            for (int i = tnode->mark->socho - 1; i >= 0; i--) {
                if (tnode->mark->DSV[i][0] != '\0' && infoUpdate->socho <= i) return false;
                else if (tnode->mark->DSV[i][0] != '\0') break;
            }
            tnode = tnode->next;
        }
    }
    int old_soCho = dsMB.list[index]->socho;
    // Trường hợp cập nhật trùng mã máy bay
    if (ss_str(dsMB.list[index]->soHieuMB, infoUpdate->soHieuMB) == 0) {
        int SLBcu = dsMB.list[index]->SLB;
        markCB* tnode = dsMB.list[index]->dsHD.head;
        *dsMB.list[index] = *infoUpdate;
        dsMB.list[index]->SLB = SLBcu; // Thông tin này không được thay đổi
        dsMB.list[index]->dsHD.head = tnode; // Thông tin này không được thay đổi
        CB* temp = dsCB.head;
        while (temp != NULL) {
            if (ss_str(temp->soHieuMB, soHieuMB) == 0) { // Tìm những chuyến bay đang sử dụng máy bay này
                char **newDSV = new char* [infoUpdate->socho];
                if (infoUpdate->socho > old_soCho) { // Nếu số chỗ Update lớn hơn số chỗ hiện tại
                    for (int i = 0; i < temp->socho; i++)
                        newDSV[i] = temp->DSV[i];
                    for (int i = temp->socho; i < infoUpdate->socho; i++) {
                        newDSV[i] = new char[cmnd_max];
                        newDSV[i][0] = '\0';
                    }
                }
                else if (infoUpdate->socho < old_soCho){ // Nếu số chỗ update nhỏ hơn
                    for (int i = 0; i < infoUpdate->socho; i++) {
                        newDSV[i] = temp->DSV[i];
                    }
                    for (int i = infoUpdate->socho; i < old_soCho; i++)
                        delete[] temp->DSV[i];
                }
                else { // Nếu số chỗ update là bằng nhau
                    for (int i = 0; i < infoUpdate->socho; i++)
                        newDSV[i] = temp->DSV[i];
                }
                temp->socho = infoUpdate->socho;
                delete[] temp->DSV;
                temp->DSV = newDSV;
            }
            temp = temp->next;
        }
        return true;
    }
    return false;
}

// Chuyen Bay
void copy_CB(CB* CB1, CB* CB2) {
    
    strcpy(CB1->maCB, CB2->maCB);

    CB1->ngayKH = CB2->ngayKH;
   
    if (CB1->sbDich == NULL) {
        char* newsb = new char[sbDich_max];
        strcpy(newsb, CB2->sbDich);
        CB1->sbDich = newsb;
    }
    else {
        strcpy(CB1->sbDich, CB2->sbDich);
    }
  
    CB1->trangThai = CB2->trangThai;
   
    strcpy(CB1->soHieuMB, CB2->soHieuMB);
    
    CB1->sove = CB2->sove;
    int sochocu = CB1->socho;
    CB1->socho = CB2->socho;
    
    if (CB2->DSV != NULL) {
        if (CB1->DSV != NULL)
            for (int i = 0; i < sochocu; i++) {
                delete[] CB1->DSV[i];
            }
        char** newDSV = new char* [CB2->socho];
        for (int i = 0; i < CB2->socho; i++) {
            newDSV[i] = new char[cmnd_max];
            strcpy(newDSV[i], CB2->DSV[i]);
        }
        delete[] CB1->DSV;
        CB1->DSV = newDSV;
    }
    else {
        if (CB1->DSV != NULL)
            for (int i = 0; i < sochocu; i++) {
                delete[] CB1->DSV[i];
            }
        delete[] CB1->DSV;
        CB1->DSV = NULL;
    }
}

void Swap_CB(CB* &CB1, CB* &CB2) {
    CB* temp = CB1;
    CB1 = CB2;
    CB2 = temp;
}

int HP_CB(CB** dsCB, int l, int r) {
    int i = l;
    int j = r;
    char pivot[maCB_max];
    strcpy(pivot, dsCB[l]->maCB);
    while (true) {
        while (ss_str(pivot, dsCB[i]->maCB) == 1) i++;
        while (ss_str(pivot, dsCB[j]->maCB) == 2) j--;
        if (i < j) {
            Swap_CB(dsCB[i], dsCB[j]);
            i++;
            j--;
        }
        else return j;
    }
}

void QuickSort_CB(CB** dsCB, int l, int r) {
    if (l >= r) return;
    int m = HP_CB(dsCB, l, r);
    QuickSort_CB(dsCB, l, m);
    QuickSort_CB(dsCB, m + 1, r);
}

void Sort_CB(listCB &dsCB) {
    if (dsCB.slCB <= 1) return;
    CB** dsMangCB = new CB*[dsCB.slCB];
    CB* temp = dsCB.head;
    int i = 0;
    while (temp != NULL) {
        dsMangCB[i] = temp;
        i++;
        temp = temp->next;
    }
    QuickSort_CB(dsMangCB, 0, dsCB.slCB - 1);
    for (int i = 0; i < dsCB.slCB; i++) {
        if (i == dsCB.slCB - 1) dsMangCB[i]->next = NULL;
        else dsMangCB[i]->next = dsMangCB[i + 1];
    }
    dsCB.head = dsMangCB[0];
    delete[] dsMangCB;
}

CB *Find_CB(listCB &dsCB, char* const maCB) {
    CB* temp = dsCB.head;
    if (temp == NULL) return NULL;
    while(temp != NULL) {
        int result = ss_str(temp->maCB, maCB);
        if (result == 0) return temp;
        if (result == 1) return NULL;
        temp = temp->next;
    }
    return NULL;
}

CB* find_insert_posCB(listCB &dsCB, char* const maCB) {
    if (dsCB.head == NULL) return NULL;
    if (ss_str(dsCB.head->maCB, maCB) == 1) return NULL;
    if (ss_str(dsCB.head->maCB, maCB) == 0) return dsCB.head;
    CB* temp1 = dsCB.head;
    CB* temp2 = dsCB.head->next;
    while (temp1 != NULL) {
        if (temp2 == NULL) return temp1;
        if (ss_str(temp2->maCB, maCB) == 0) return temp2;
        if (ss_str(temp2->maCB, maCB) == 1) return temp1;
        temp1 = temp1->next;
        temp2 = temp2->next;
    }
    return NULL;
}

CB* Find_Active_MB(listCB &dsCB, char* const soHieuMB) { // Tìm xem có chuyến bay nào đang hoạt động (Đang có chuyến bay sử dụng)
    CB* temp = dsCB.head;
    while(temp != NULL) {
        if (ss_str(temp->soHieuMB, soHieuMB) == 0) {
            if ((temp->trangThai == 1 || temp->trangThai == 2)) return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

bool Add_CB(listCB &dsCB, listMB &dsMB, CB *newCB) {
    int index = Find_MB(dsMB, newCB->soHieuMB);
    if (index == -1) return false; // Khong the them mot chuyen bay voi may bay khong ton tai
    markCB* newMark = new markCB();
    newMark->mark = newCB;
    if (newCB->trangThai == 1 || newCB->trangThai == 2) {
        if (addMarkMB(dsMB.list[index], newMark) == false) {
            delete newMark;
            return false; // Khong the them trung MB neu MB do dang hoat dong tren cac chuyen bay khac ma khong cach nhau it nhat 1 ngay
        }
    }
    CB* temp = find_insert_posCB(dsCB, newCB->maCB);
    if (temp == NULL && dsCB.head == NULL) {
        dsCB.head = newCB;
        dsCB.slCB++;
        if (newCB->trangThai == 3) dsMB.list[index]->SLB++;
        return true;
    }
    if (temp == NULL && dsCB.head != NULL) {
        newCB->next = dsCB.head;
        dsCB.head = newCB;
        dsCB.slCB++;
        if (newCB->trangThai == 3) dsMB.list[index]->SLB++;
        return true;
    }
    if (ss_str(temp->maCB, newCB->maCB) == 0) {
        delMarkMB(dsMB.list[index], newCB->maCB);
        return false; // Khong the them mot chuyen bay neu bi trung maCB
    }
    CB* temp2 = temp->next;
    temp->next = newCB;
    newCB->next = temp2;
    dsCB.slCB++;
    if (newCB->trangThai == 3) dsMB.list[index]->SLB++;
    return true;
}

bool Update_Time_CB(listCB &dsCB, listMB &dsMB, char* const maCB, const DateTime &newTime) {
    CB* temp = Find_CB(dsCB, maCB);
    if (temp == NULL) return false;
    if (temp->trangThai == 3 || temp->trangThai == 0) return false;
    if (temp->sove > 0) return false; // Khong the thay doi thoi gian neu da co khach dat ve
    int index = Find_MB(dsMB, temp->soHieuMB);
    markCB* tnode = dsMB.list[index]->dsHD.head;
    while (tnode != NULL) {
        if (ss_str(tnode->mark->maCB, maCB) != 0) {
            if (ss_ngay(tnode->mark->ngayKH, newTime) < 1440) return false; // Khong the thay doi thoi gian neu nhu thoi gian do bi xung dot voi thoi gian cua cac chuyen bay su dung cung mot may bay
        }
        tnode = tnode->next;
    }
    temp->ngayKH = newTime;
    return true;
}

int Update_CB(listCB &dsCB, listMB &dsMB, char* const maCB, CB* infor) {
    CB* temp = Find_CB(dsCB, maCB);
    if (temp == NULL) return -1; // Chuyen bay khong ton tai
    if (ss_str(infor->maCB, maCB) != 0) return -2; // Khong duoc sua maCB
    if (temp->trangThai == 0 || temp->trangThai == 3) return -3; // Khong duoc sua cac chuyen bay da huy hoac hoan tat
    for (int i = temp->socho - 1; i >= 0; i--) {
        if (temp->DSV[i][0] != '\0' && infor->socho <= i) return -4; // Không thể đổi socho mà bé hơn sô chố lớn nhất đang có khách
        else if (temp->DSV[i][0] != '\0') break;
    }
    
    if (ss_str(temp->soHieuMB, infor->soHieuMB) == 0) { // Truong hop cap nhat trung so hieu MB
        // Truong hop da co khach
        if (temp->sove > 0) {
            if (ss_str(temp->sbDich, infor->sbDich) != 0) return -5; // Nhung thong tin khong duoc thay doi khi da co khach
            if (ss_ngay(temp->ngayKH, infor->ngayKH) != 0) return -5; // Nhung thong tin khong duoc thay doi khi da co khach
            if (temp->trangThai != infor->trangThai) return -5; // Nhung thong tin khong duoc thay doi khi da co khach
        }
        else { // Truong hop khong co khach
            if (Update_Time_CB(dsCB, dsMB, maCB, infor->ngayKH) == false) return -6; // Xung dot thoi gian
            strcpy(temp->sbDich, infor->sbDich);
            temp->trangThai = infor->trangThai;
        }
    }
    else { // Truong hop khac so hieu MB
        // Truong hop da co khach
        if (temp->sove > 0) {
            if (ss_str(temp->sbDich, infor->sbDich) != 0) return -5; // Nhung thong tin khong duoc thay doi khi da co khach
            if (ss_ngay(temp->ngayKH, infor->ngayKH) != 0) return -5; // Nhung thong tin khong duoc thay doi khi da co khach
            if (temp->trangThai != infor->trangThai) return -5; // Nhung thong tin khong duoc thay doi khi da co khach
        }
        // Truong hop khong co khach
        // Thay doi mark moi cho MB moi
        int index = Find_MB(dsMB, infor->soHieuMB);
        if (index == -1) return -7; // MB duoc cap nhat khong ton tai
        markCB* tnode = dsMB.list[index]->dsHD.head;
        while (tnode != NULL) { // Kiem tra xung dot thoi gian
            if (ss_ngay(tnode->mark->ngayKH, infor->ngayKH) < 1440) return -6; // Xung dot thoi gian
            tnode = tnode->next;
        }
        int index2 = Find_MB(dsMB, temp->soHieuMB);
        markCB* newMark = new markCB();
        newMark->mark = temp;
        delMarkMB(dsMB.list[index2], temp->maCB); // Xoa mark cua MB cu
        temp->ngayKH = infor->ngayKH;
        addMarkMB(dsMB.list[index], newMark); // Them mark CB hien tai cho MB moi
        strcpy(temp->sbDich, infor->sbDich);
        temp->trangThai = infor->trangThai;
        strcpy(temp->soHieuMB, infor->soHieuMB);
    }
    char **newDSV = new char* [infor->socho];
    int cpysocho = min(infor->socho, temp->socho);
    for (int i = 0; i < cpysocho; i++)
        newDSV[i] = temp->DSV[i];
    for (int i = temp->socho; i < infor->socho; i++) {
        newDSV[i] = new char[cmnd_max];
        newDSV[i][0] = '\0';
    }
    if (infor->socho < temp->socho) { // Don dep cac so cho bi thua
        for (int i = infor->socho; i < temp->socho; i++)
            delete[] temp->DSV[i];
    }
    temp->socho = infor->socho;
    delete[] temp->DSV;
    temp->DSV = newDSV;
    return true;
}

bool Cancel_CB(listCB &dsCB, listMB &dsMB, char* const maCB) {
    CB* temp = Find_CB(dsCB,  maCB);
    if (temp == NULL) return false; // Chuyen bay khong ton tai
    if (temp->trangThai == 3 || temp->trangThai == 0) return false; // Da hoat tat hoac bi huy -> khong huy
    if (temp->sove > 0) return false; // Da co khach -> khong huy
    temp->trangThai = 0;
    int index = Find_MB(dsMB, temp->soHieuMB);
    delMarkMB(dsMB.list[index], maCB);
    for (int i = 0; i < temp->socho; i++)
        delete[] temp->DSV[i];
    delete[] temp->DSV;
    temp->DSV = NULL;
    temp->socho = 0;
    return true;
}

int Status_CB(listCB &dsCB, char* const maCB) {
    CB* temp = Find_CB(dsCB, maCB);
    if (temp == NULL) return -1;
    return temp->trangThai;
}

void Init_Tickets(CB *newCB, int soCho) {
    if (newCB->DSV != NULL) return;
    newCB->DSV = new char*[soCho];
    for (int i = 0; i < soCho; i++) {
        newCB->DSV[i] = new char[cmnd_max];
        newCB->DSV[i][0] = '\0';
    }
}

HK* Find_HK_At_List(listHK &dsHK, char* const cmnd) {
    HK* temp = dsHK.goc;
    while(temp != NULL) {
        int result = ss_str(temp->cmnd, cmnd);
        if (result == 0) return temp;
        if (result == 1) temp = temp->left;
        else temp = temp->right;
    }
    return NULL;
}

HK* Find_HK(listCB &dsCB, listHK &dsHK, char* const maCB, char* const cmnd) {
    CB* cb = Find_CB(dsCB, maCB);
    if (cb == NULL) return NULL;
    if (cb->DSV == NULL) return NULL;
    HK* hk = Find_HK_At_List(dsHK, cmnd);
    if (hk == NULL) return NULL;
    for (int i = 0; i < cb->socho; i++) {
        if (cb->DSV[i][0] == '\0') continue;
        int result = ss_str(cb->DSV[i], cmnd);
        if (result == 0) return hk;
    }
    return NULL;
}

bool Add_HK(listHK &dsHK, char* const ho, char* const ten, char* const cmnd, bool phai) {
    HK* newHK = new HK();
    newHK->set_ho(ho);
    newHK->set_ten(ten);
    newHK->set_cmnd(cmnd);
    newHK->set_phai(phai);
    
    if (dsHK.goc == NULL) {
        dsHK.goc = newHK;
        dsHK.slHK++;
        return true;
    }
    
    HK* temp = dsHK.goc;
    
    while (temp != NULL) {
        int result = ss_str(newHK->cmnd, temp->cmnd);
        if (result == 1) {
            if (temp->right == NULL) {
                temp->right = newHK;
                break;
            }
            else temp = temp->right;
        }
        else if (result == 2) {
            if (temp->left == NULL) {
                temp->left = newHK;
                break;
            }
            else temp = temp->left;
        }
        else {
            delete newHK;
            return false;
        }
    }
    dsHK.slHK++;
    return true;
}

string Is_Ticket_Booked(listCB &dsCB, listHK &dsHK, char* const maCB, char* const CMND) {
    CB* tempCB = Find_CB(dsCB, maCB);
    if (tempCB == NULL) return "1"; // ma chuyen bay khong ton tai trong he thong
    HK* tempHK = Find_HK_At_List(dsHK, CMND);
    if (tempHK == NULL) return "2"; // hanh khach khong ton tai trong he thong
    markCB* tnode = tempHK->dsDatVe.head;
    while (tnode != NULL) {
        if (ss_str(tnode->mark->maCB, maCB) == 0) return "0"; // hanh khach da dat ve tren chuyen bay nay
        tnode = tnode->next;
    }
    return "3"; // hanh khach chua dat ve tren CB nay
}

string Book_Ticket(listCB &dsCB, listHK &dsHK, char* const maCB, char* const CMND, int seatNumber) {
    string res = Is_Ticket_Booked(dsCB, dsHK, maCB, CMND);
    if (res != "3") return res;
    CB* tempCB = Find_CB(dsCB, maCB);
    if (tempCB->trangThai != 1) return "3"; // Chuyen bay da het ve hoac khong con hoat dong
    if (tempCB->DSV[seatNumber - 1][0] != '\0') return "4"; // da co khach dat ghe nay;
    HK* tempHK = Find_HK_At_List(dsHK, CMND);
    markCB* newMark = new markCB();
    newMark->mark = tempCB;
    if (addMarkHK(tempHK, newMark) == false) {
        delete newMark;
        return "5"; // dang ky chuyen bay sai thoi gian quy dinh;
    }
    strcpy(tempCB->DSV[seatNumber - 1], CMND);
    tempCB->sove++;
    if (tempCB->sove == tempCB->socho) tempCB->trangThai = 2; // CB da het ve;
    return "0";
}

bool Cancel_Ticket(CB *&dsCB, const char* maCB, int seatNumber)
{
    return false;
}

HK **Get_DSHKCB(CB *const dsCB, HK *const dsHK, const char* maCB, int &sldsHK)
{
    return nullptr;
}

CB **Search_CB(CB *const dsCB, const DateTime &date, const char* address, int &sldsCB)
{
    return nullptr;
}

int *Get_Empty_Seats(CB *const dsCB, const char* maCB, int &sldsVT)
{
    return nullptr;
}

int ss_SLB(MB* a, MB* b) {
    if (a->SLB > b->SLB) return 2;
    if (a->SLB < b->SLB) return 1;
    if (ss_str(a->soHieuMB, b->soHieuMB) == 1) return 1;
    if (ss_str(a->soHieuMB, b->soHieuMB) == 2) return 2;
    return 0;
}

int HP_SLB(listMB& dsMB, int l, int r) {
    MB* pivot = new MB();
    *pivot = *dsMB.list[l];
    int i = l;
    int j = r;
    while (true) {
        while (ss_SLB(pivot, dsMB.list[i]) == 1) i++;
        while (ss_SLB(pivot, dsMB.list[j]) == 2) j--;
        if (i < j) {
            swap_MB(dsMB.list[i], dsMB.list[j]);
            i++;
            j--;
        }
        else {
            delete pivot;
            return j;
        }
    }
}

void Sort_SLB(listMB& dsMB, int l, int r) {
    if (l >= r) return;
    int m = HP_SLB(dsMB, l, r);
    Sort_SLB(dsMB, l, m);
    Sort_SLB(dsMB, m + 1, r);
}

listMB Get_Flight_Stats (listMB &dsMB) {
    listMB flight_Statics = listMB();
    flight_Statics.slMB = dsMB.slMB;
    for (int i = 0; i < dsMB.slMB; i++) {
        flight_Statics.list[i] = new MB();
        *flight_Statics.list[i] = *dsMB.list[i];
        flight_Statics.list[i]->dsHD.head = NULL; // Khong copy dsMark de dam bao dsMark cua MB an toan
    }
    Sort_SLB(flight_Statics, 0, flight_Statics.slMB - 1);
    return flight_Statics;
}

listMB Find_MB_OnRage(listMB& dsMB, char* const query) {
    listMB A = listMB();
    A.slMB = 0;
    char temp[soHieuMB_max];
    char prefix[soHieuMB_max];
    strcpy(prefix, query);
    to_lower(prefix);
    for (int i = 0; i < dsMB.slMB; i++) {
        strcpy(temp, dsMB.list[i]->soHieuMB);
        to_lower(temp);
        if (cmp_prefix(temp, prefix)) {
            A.list[A.slMB] = new MB();
            *A.list[A.slMB] = *dsMB.list[i];
            A.list[A.slMB]->dsHD.head = NULL; // Khong copy dsMark de dam bao dsMark cua MB an toan
            A.slMB++;
        }
    }
    return A;
}

listCB Find_CB_OnRage(listCB &dsCB, char* const query) {
    listCB A;
    A.slCB = 0;
    A.head = NULL;
    CB* atemp = A.head;
    char temp[maCB_max];
    char prefix[maCB_max];
    strcpy(prefix, query);
    to_lower(prefix);
    CB* tnode = dsCB.head;
    while (tnode != NULL) {
        strcpy(temp, tnode->maCB);
        to_lower(temp);
        if (cmp_prefix(temp, prefix)) {
            if (A.head == NULL) {
                A.head = new CB();
                copy_CB(A.head, tnode);
                atemp = A.head;
                A.slCB++;
            }
            else {
                CB* newCB = new CB();
                copy_CB(newCB, tnode);
                atemp->next = newCB;
                atemp = atemp->next;
                A.slCB++;
            }
        }
        tnode = tnode->next;
    }
    return A;
}

listHK Find_HK_OnRage(listHK &dsHK, char* const query) {
    listHK A;
    return A;
}

bool Can_DeL_MB(listMB &dsMB,listCB &dsCB,char* const soHieuMB){
    CB* temp = Find_Active_MB(dsCB, soHieuMB);
    if (temp != NULL) return false; // Khong the xoa mot may bay ma da co chuyen bay dang su dung da co khach
    int index = Find_MB(dsMB, soHieuMB);
    if (index == -1) return false; // Khong the xoa mot may bay khong ton tai
    return true;
}

void Del_SubDsMB(listMB& SubDsMB, char* const soHieuMB) {
    int index = Find_MB(SubDsMB, soHieuMB);
    delete SubDsMB.list[index];
    for (int i = index; i < SubDsMB.slMB - 1; i++) {
        SubDsMB.list[i] = SubDsMB.list[i + 1];
    }
    SubDsMB.list[SubDsMB.slMB - 1] = NULL;
    SubDsMB.slMB--;
}

string Can_Edit_MB(listMB &dsMB,listCB &dsCB, char* const soHieuMB, MB* infoUpdate){
    int index = Find_MB(dsMB, soHieuMB);
    CB* temp = Find_Active_MB(dsCB, soHieuMB);
    markCB* tnode = dsMB.list[index]->dsHD.head;
    if (temp != NULL) { // Có chuyến bay đang sử dụng máy bay này
        while (tnode != NULL) {
            for (int i = tnode->mark->socho - 1; i >= 0; i--) {
                if (tnode->mark->DSV[i][0] != '\0' && infoUpdate->socho <= i) {
                    return "1" + to_string(i + 1);
                }
                else if (tnode->mark->DSV[i][0] != '\0') break;
            }
            tnode = tnode->next;
        }
    }
    return "0";
}

void Edit_SubDsMB(listMB& SubDsMB, char* const soHieuMB, MB* infoUpdate) {
    int index = Find_MB(SubDsMB, soHieuMB);
    // Trường hợp cập nhật trùng mã máy bay
    if (ss_str(SubDsMB.list[index]->soHieuMB, infoUpdate->soHieuMB) == 0) {
        int SLBcu = SubDsMB.list[index]->SLB;
        markCB* tnode = SubDsMB.list[index]->dsHD.head;
        *SubDsMB.list[index] = *infoUpdate;
        SubDsMB.list[index]->SLB = SLBcu; // Thông tin này không được thay đổi
        SubDsMB.list[index]->dsHD.head = tnode; // Thông tin này không được thay đổi
        return;
    }
}
