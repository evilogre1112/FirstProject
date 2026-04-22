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


// May Bay
// Hàm Đọc và Ghi file
bool Save_MB(ofstream& save, listMB& dsMB);

void swap_MB(MB* &a, MB* &b) {
    MB temp = *a;
    *a = *b;
    *b = temp;
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
    bool check = false; // kiem tra xem co hanh khach tren chuyen bay do hay chua
    if (temp != NULL) {
        for (int i = 0; i < temp->socho; i++) {
            if (temp->DSV[i][0] != '\0') {
                check = true;
                break;
            }
        }
    }
    if (check == true) return false; // Khong the xoa mot may bay ma da co chuyen bay dang su dung da co khach
    int index = Find_MB(dsMB, soHieuMB);
    if (index == -1) return false; // Khong the xoa mot may bay khong ton tai
    delete dsMB.list[index];
    for (int i = index; i < dsMB.slMB - 1; i++) {
        dsMB.list[i] = dsMB.list[i + 1];
    }
    dsMB.list[dsMB.slMB - 1] = NULL;
    dsMB.slMB--;
    if (temp != NULL )
        Cancel_CB(dsCB, temp->maCB);
    return true;
}

bool Edit_MB(listMB& dsMB, listCB& dsCB, char* const soHieuMB, MB *infoUpdate) {
    int index = Find_MB(dsMB, soHieuMB);
    if (index == -1) return false;
    if (infoUpdate->socho < dsMB.list[index]->socho) return false;
    // Trường hợp cập nhật trùng mã máy bay
    if (ss_str(dsMB.list[index]->soHieuMB, infoUpdate->soHieuMB) == 0) {
        int SLBcu = dsMB.list[index]->SLB;
        *dsMB.list[index] = *infoUpdate;
        dsMB.list[index]->SLB = SLBcu;
        CB* temp = Find_Active_MB(dsCB, soHieuMB);
        if (temp != NULL) {
            char **newDSV = new char* [infoUpdate->socho];
            for (int i = 0; i < temp->socho; i++)
                newDSV[i] = temp->DSV[i];
            for (int i = temp->socho; i < infoUpdate->socho; i++) {
                newDSV[i] = new char[cmnd_max];
                newDSV[i][0] = '\0';
            }
            temp->socho = infoUpdate->socho;
            strcpy(temp->soHieuMB, infoUpdate->soHieuMB);
            delete[] temp->DSV;
            temp->DSV = newDSV;
        }
        return true;
    }
    // Trường hợp cập nhật khác mã máy bay
    int exist = Find_MB(dsMB, infoUpdate->soHieuMB);
    if (exist != -1) return false;
    int SLBcu = dsMB.list[index]->SLB;
    *dsMB.list[index] = *infoUpdate;
    dsMB.list[index]->SLB = SLBcu;
    CB* temp = Find_Active_MB(dsCB, soHieuMB);
    if (temp != NULL) {
        char **newDSV = new char* [infoUpdate->socho];
        for (int i = 0; i < temp->socho; i++)
            newDSV[i] = temp->DSV[i];
        for (int i = temp->socho; i < infoUpdate->socho; i++) {
            newDSV[i] = new char[cmnd_max];
            newDSV[i][0] = '\0';
        }
        temp->socho = infoUpdate->socho;
        strcpy(temp->soHieuMB, infoUpdate->soHieuMB);
        delete[] temp->DSV;
        temp->DSV = newDSV;
    }
    Sort_MB(dsMB, 0, dsMB.slMB - 1);
    return true;
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
    
    for (int i = 0; i < CB1->socho; i++) {
        delete[] CB1->DSV[i];
    }
  
    CB1->socho = CB2->socho;
    
    char** newDSV = new char* [CB2->socho];
    for (int i = 0; i < CB2->socho; i++) {
        newDSV[i] = new char[cmnd_max];
        strcpy(newDSV[i], CB2->DSV[i]);
    }
    delete[] CB1->DSV;
    CB1->DSV = newDSV;
}

void Swap_CB(CB* &CB1, CB* &CB2) {
    
    char tmp_maCB[maCB_max];
    strcpy(tmp_maCB, CB1->maCB);
    strcpy(CB1->maCB, CB2->maCB);
    strcpy(CB2->maCB, tmp_maCB);

    DateTime tmpDate = CB1->ngayKH;
    CB1->ngayKH = CB2->ngayKH;
    CB2->ngayKH = tmpDate;
   
    char* tmpSB = CB1->sbDich;
    CB1->sbDich = CB2->sbDich;
    CB2->sbDich = tmpSB;
  
    int tmpTrangThai = CB1->trangThai;
    CB1->trangThai = CB2->trangThai;
    CB2->trangThai = tmpTrangThai;
   
    char tmp_soHieu[soHieuMB_max];
    strcpy(tmp_soHieu, CB1->soHieuMB);
    strcpy(CB1->soHieuMB, CB2->soHieuMB);
    strcpy(CB2->soHieuMB, tmp_soHieu);
  
    int tmpSoCho = CB1->socho;
    CB1->socho = CB2->socho;
    CB2->socho = tmpSoCho;

    char** tmpDSV = CB1->DSV;
    CB1->DSV = CB2->DSV;
    CB2->DSV = tmpDSV;
}

void Sort_CB(listCB &dsCB) {
    CB* temp1 = dsCB.head;
    if (temp1 == NULL) return;
    CB* temp2;
    while (temp1 != NULL) {
        temp2 = temp1->next;
        while (temp2 != NULL) {
            if (ss_str(temp1->maCB, temp2->maCB) == 1) {
                Swap_CB(temp1, temp2);
            }
            temp2 = temp2->next;
        }
        temp1 = temp1->next;
    }
}

CB *Find_CB(listCB &dsCB, char* const maCB) {
    CB* temp = dsCB.head;
    if (temp == NULL) return NULL;
    while(temp != NULL) {
        if (ss_str(temp->maCB, maCB) == 0) return temp;
        if (ss_str(temp->maCB, maCB) == 1) return NULL;
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

CB* Find_Active_MB(listCB &dsCB, char* const soHieuMB) {
    CB* temp = dsCB.head;
    while(temp != NULL) {
        if (ss_str(temp->soHieuMB, soHieuMB) == 0) {
            if (temp->trangThai == 1 || temp->trangThai == 2) return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

bool Add_CB(listCB &dsCB, listMB &dsMB, CB *newCB) {
    if (Find_Active_MB(dsCB, newCB->soHieuMB) != NULL) return false;
    int index = Find_MB(dsMB, newCB->soHieuMB);
    if (index == -1) return false;
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
    if (ss_str(temp->maCB, newCB->maCB) == 0) return false;
    CB* temp2 = temp->next;
    temp->next = newCB;
    newCB->next = temp2;
    dsCB.slCB++;
    if (newCB->trangThai == 3) dsMB.list[index]->SLB++;
    return true;
}

bool Update_Time_CB(listCB &dsCB, char* const maCB, const DateTime &newTime) {
    CB* temp = Find_CB(dsCB, maCB);
    if (temp == NULL) return false;
    if (temp->trangThai == 3 || temp->trangThai == 0) return false;
    for (int i = 0; i < temp->socho; i++) {
        if (temp->DSV[i][0] != '\0') return false; // khon cho phep thay doi thoi gian khoi hanh neu CB da co hanh khach dat
    }
    temp->ngayKH = newTime;
    return true;
}

int Update_CB(listCB &dsCB, listMB &dsMB, char* const maCB, CB* infor) {
    CB* temp = Find_Active_MB(dsCB, infor->soHieuMB);
    if (temp != NULL && ss_str(temp->maCB, maCB) != 0) return -2;
    temp = Find_CB(dsCB, infor->maCB);
    if (temp != NULL && ss_str(maCB, infor->maCB) != 0) return -1;
    if (Find_MB(dsMB, infor->soHieuMB) == -1) return -3;
    temp = Find_CB(dsCB, maCB);
    if (temp == NULL) return -4;
    if (temp->socho > infor->socho) return -5;
    bool check = false;
    for (int i = 0; i < temp->socho; i++) {
        if (temp->DSV[i][0] != '\0') {
            check = true;
            break;
        }
    }
    
    if (check) { // Trường hợp đã có khách
        if (ss_str(temp->sbDich, infor->sbDich) != 0) return -6;
        if (ss_ngay(temp->ngayKH, infor->ngayKH) != 0) return -6;
        if (temp->trangThai != infor->trangThai) return -6;
        
    }
    else { // Trường hợp chưa có khách
        strcpy(temp->sbDich, infor->sbDich);
        temp->ngayKH = infor->ngayKH;
        temp->trangThai = infor->trangThai;
    }
    strcpy(temp->maCB, infor->maCB);
    // Cap nhat so cho moi cho CB
    char **newDSV = new char* [infor->socho];
    for (int i = 0; i < temp->socho; i++)
        newDSV[i] = temp->DSV[i];
    for (int i = temp->socho; i < infor->socho; i++) {
        newDSV[i] = new char[cmnd_max];
        newDSV[i][0] = '\0';
    }
    temp->socho = infor->socho;
    strcpy(temp->soHieuMB, infor->soHieuMB);
    delete[] temp->DSV;
    temp->DSV = newDSV;
    Sort_CB(dsCB);
    return true;
}

bool Cancel_CB(listCB &dsCB, char* const maCB) {
    CB* temp = Find_CB(dsCB, maCB);
    if (temp == NULL) return false;
    if (temp->trangThai == 3 || temp->trangThai == 0) return false;
    bool check = false; // kiem tra xem co hanh khach dat chuyen bay do hay chua
    for (int i = 0; i < temp->socho; i++) {
        if (temp->DSV[i][0] != '\0') {
            check = true;
            break;
        }
    }
    if (check == true) return false; // Khong the huy mot chuyen bay da co khach
    temp->trangThai = 0;
    for (int i = 0; i < temp->socho; i++)
        delete[] temp->DSV[i];
    delete[] temp->DSV;
    temp->DSV = NULL;
    return true;
}

int Status_CB(listCB &dsCB, char* const maCB) {
    CB* temp = Find_CB(dsCB, maCB);
    if (temp == NULL) return -1;
    return temp->trangThai;
}

void Init_Tickets(CB *newCB, int soCho) {
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

bool Is_Ticket_Booked(CB *const dsCB, const char* maCB, const char* CMND)
{
    return false;
}

bool Book_Ticket(CB *dsCB, const char* maCB, const char* CMND, int seatNumber)
{
    return false;
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
        while (ss_SLB(pivot, dsMB.list[j]) == 2) j++;
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
    listMB flight_Statics;
    flight_Statics.slMB = dsMB.slMB;
    for (int i = 0; i < dsMB.slMB; i++) {
        flight_Statics.list[i] = new MB();
        *flight_Statics.list[i] = *dsMB.list[i];
    }
    Sort_SLB(flight_Statics, 0, flight_Statics.slMB - 1);
    return flight_Statics;
}

listMB Find_MB_OnRage(listMB& dsMB, char* const query) {
    listMB A;
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
listHK Find_HK_OnRage(listHK &dsHK, char* const query);
