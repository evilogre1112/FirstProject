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


// May Bay
// Hàm Đọc và Ghi file
bool Save_MB(ofstream& save, listMB& dsMB);

// Tiện ích sắp xếp
void Merge_MB(listMB& dsMB, int l,int m, int r) {
    int x_size = m - l + 1; int y_size = r - m;
    MB** x = new MB*[x_size]; MB** y = new MB*[y_size];
    int nx = l; int ny = m + 1;
    for (int i = 0; i < x_size; i++) {
        x[i] = dsMB.list[nx];
        nx++;
    }
    for (int j = 0; j < y_size; j++) {
        y[j] = dsMB.list[ny];
        ny++;
    }
    int i = 0; int j = 0;
    while (i < x_size && j < y_size) {
        if (ss_str(x[i]->soHieuMB, y[j]->soHieuMB) != 1) {
            dsMB.list[l] = x[i];
            i++;
        }
        else {
            dsMB.list[l] = y[j];
            j++;
        }
        l++;
    }
    if (i == x_size) {
        for (int s = j; s < y_size; s++) {
            dsMB.list[l] = y[s];
            l++;
        }
    }
    else {
        for (int s = i; s < x_size; s++) {
            dsMB.list[l] = x[s];
            l++;
        }
    }
    delete[] x;
    delete[] y;
}
// Merge sort MB
void Sort_MB(listMB& dsMB, int l, int r) {
    if (l >= r) return;
    int m = (r + l) / 2;
    Sort_MB(dsMB, l, m);
    Sort_MB(dsMB, m + 1, r);
    Merge_MB(dsMB, l, m, r);
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
    int index = Find_MB(dsMB, soHieuMB);
    if (index == -1) return false;
    delete dsMB.list[index];
    for (int i = index; i < dsMB.slMB - 1; i++) {
        dsMB.list[i] = dsMB.list[i + 1];
    }
    dsMB.list[dsMB.slMB - 1] = NULL;
    dsMB.slMB--;
    CB* temp = Find_Active_MB(dsCB, soHieuMB);
    if (temp != NULL) Cancel_CB(dsCB, temp->maCB);
    return true;
}

bool Edit_MB(listMB& dsMB, listCB& dsCB, char* const soHieuMB, MB *infoUpdate) {
    int index = Find_MB(dsMB, soHieuMB);
    if (index == -1) return false;
    if (infoUpdate->socho < dsMB.list[index]->socho) return false;
    // Trường hợp cập nhật trùng mã máy bay
    if (ss_str(dsMB.list[index]->soHieuMB, infoUpdate->soHieuMB) == 0) {
        *dsMB.list[index] = *infoUpdate;
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
    *dsMB.list[index] = *infoUpdate;
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
    if (Find_MB(dsMB, newCB->soHieuMB) == -1) return false;
    CB* temp = find_insert_posCB(dsCB, newCB->maCB);
    if (temp == NULL && dsCB.head == NULL) {
        dsCB.head = newCB;
        dsCB.slCB++;
        return true;
    }
    if (temp == NULL && dsCB.head != NULL) {
        newCB->next = dsCB.head;
        dsCB.head = newCB;
        dsCB.slCB++;
        return true;
    }
    if (ss_str(temp->maCB, newCB->maCB) == 0) return false;
    CB* temp2 = temp->next;
    temp->next = newCB;
    newCB->next = temp2;
    dsCB.slCB++;
    return true;
}

bool Update_Time_CB(listCB &dsCB, char* const maCB, const DateTime &newTime) {
    CB* temp = Find_CB(dsCB, maCB);
    if (temp == NULL) return false;
    temp->ngayKH = newTime;
    return true;
}

bool Cancel_CB(listCB &dsCB, char* const maCB) {
    CB* temp = Find_CB(dsCB, maCB);
    if (temp == NULL) return false;
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

HK* Find_HK(HK* root, char* const cmnd) {
    if (root == NULL) return NULL;
    if (ss_str(root->cmnd, cmnd) == 0) return root;
    if (ss_str(root->cmnd, cmnd) == 1) return Find_HK(root->left, cmnd);
    return Find_HK(root->right, cmnd);
}

bool Add_HK(HK *&dsHK, const char* ho, const char* ten, const char* cmnd, const char* phai) {
    return false;
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

MB_Stat *Get_Flight_Statistics(CB *const dsCB, int &count)
{
    return nullptr;
}
