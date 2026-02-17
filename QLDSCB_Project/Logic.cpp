#include "Logic.h"

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
bool Save_MB(ofstream& save, MB* dsMB[], int slMB){
    return true ;
}

// Tiện ích sắp xếp
void Merge_MB(MB* dsMB[], int l,int m, int r) {
    int x_size = m - l + 1; int y_size = r - m;
    MB** x = new MB*[x_size]; MB** y = new MB*[y_size];
    int nx = l; int ny = m + 1;
    for (int i = 0; i < x_size; i++) {
        x[i] = dsMB[nx];
        nx++;
    }
    for (int j = 0; j < y_size; j++) {
        y[j] = dsMB[ny];
        ny++;
    }
    int i = 0; int j = 0;
    while (i < x_size && j < y_size) {
        if (ss_str(x[i]->soHieuMB, y[j]->soHieuMB) != 1) {
            dsMB[l] = x[i];
            i++;
        }
        else {
            dsMB[l] = y[j];
            j++;
        }
        l++;
    }
    if (i == x_size) {
        for (int s = j; s < y_size; s++) {
            dsMB[l] = y[s];
            l++;
        }
    }
    else {
        for (int s = i; s < x_size; s++) {
            dsMB[l] = x[s];
            l++;
        }
    }
    delete[] x;
    delete[] y;
}
// Merge sort MB
void Sort_MB(MB* dsMB[], int l, int r) {
    if (l >= r) return;
    int m = (r + l) / 2;
    Sort_MB(dsMB, l, m);
    Sort_MB(dsMB, m + 1, r);
    Merge_MB(dsMB, l, m, r);
}

// Tìm vị trí chèn
int find_insert_pos(MB *const dsMB[], int slMB, char* const soHieuMB) {
    int l = 0, r = slMB - 1;
    int m = (l + r) / 2;
    while (l <= r) {
        int result = ss_str(dsMB[m]->soHieuMB, soHieuMB);
        if (result == 0) return m;
        else if (result == 1) r = m - 1;
        else l = m + 1;
        m = (l + r) / 2;
    }
    return l; // tra ve vi tri can chen
}

int Find_MB(MB *const dsMB[], int slMB, char* const soHieuMB) {
    int l = 0, r = slMB - 1;
    int m = (l + r) / 2;
    while (l <= r) {
        int result = ss_str(dsMB[m]->soHieuMB, soHieuMB);
        if (result == 0) return m;
        else if (result == 1) r = m - 1;
        else l = m + 1;
        m = (l + r) / 2;
    }
    return -1; // not found
}

bool Add_MB(MB *dsMB[], int &slMB, MB *newMB) {
    if (slMB >= 300) {
        return false;
    }
    int index = find_insert_pos(dsMB, slMB, newMB->soHieuMB);
    if (index == slMB) {
        dsMB[index] = newMB;
        slMB++;
        return true;
    }
    if (ss_str(dsMB[index]->soHieuMB, newMB->soHieuMB) == 0) {
        return false;
    }
    for (int i = slMB; i > index; i--) dsMB[i] = dsMB[i - 1];
    dsMB[index] = newMB;
    slMB++;
    return true;
}

bool Del_MB(MB *dsMB[], int &slMB, char* const soHieuMB) {
    int index = Find_MB(dsMB, slMB, soHieuMB);
    if (index == -1) return false;
    delete dsMB[index];
    for (int i = index; i < slMB - 1; i++) {
        dsMB[i] = dsMB[i + 1];
    }
    dsMB[slMB - 1] = NULL;
    slMB--;
    return true;
}

bool Edit_MB(MB *dsMB[], int slMB, char* const soHieuMB, MB *infoUpdate) {
    int index = Find_MB(dsMB, slMB, soHieuMB);
    if (index == -1) return false;
    if (ss_str(dsMB[index]->soHieuMB, infoUpdate->soHieuMB) == 0) {
        *dsMB[index] = *infoUpdate;
        return true;
    }
    int exist = Find_MB(dsMB, slMB, infoUpdate->soHieuMB);
    if (exist != -1) return false;
    MB* newMB = new MB();
    *newMB = *infoUpdate;
    Del_MB(dsMB, slMB, soHieuMB);
    Add_MB(dsMB, slMB, newMB);
    return true;
}

// Chuyen Bay
bool Add_CB(CB *&headCB, CB *newCB)
{
    return false;
}

bool Update_Time_CB(CB *headCB, const char* maCB, const DateTime &newTime)
{
    return false;
}

bool Cancel_CB(CB *&headCB, const char* maCB)
{
    return false;
}

int Status_CB(CB *headCB, const char* maCB)
{
    return 0;
}

void Init_Tickets(CB *newCB, int soCho)
{
}

CB *Find_CB(CB *const headCB, const char* maCB)
{
    return nullptr;
}

HK *Find_HK(CB *const headCB, HK *const dsHK, const char* maCB, const char* cmnd)
{
    return nullptr;
}

bool Add_HK(HK *&dsHK, const char* ho, const char* ten, const char* cmnd, const char* phai)
{
    return false;
}

bool Is_Ticket_Booked(CB *const headCB, const char* maCB, const char* CMND)
{
    return false;
}

bool Book_Ticket(CB *headCB, const char* maCB, const char* CMND, int seatNumber)
{
    return false;
}

bool Cancel_Ticket(CB *&headCB, const char* maCB, int seatNumber)
{
    return false;
}

HK **Get_DSHKCB(CB *const headCB, HK *const dsHK, const char* maCB, int &sldsHK)
{
    return nullptr;
}

CB **Search_CB(CB *const headCB, const DateTime &date, const char* address, int &sldsCB)
{
    return nullptr;
}

int *Get_Empty_Seats(CB *const headCB, const char* maCB, int &sldsVT)
{
    return nullptr;
}

MB_Stat *Get_Flight_Statistics(CB *const headCB, int &count)
{
    return nullptr;
}
