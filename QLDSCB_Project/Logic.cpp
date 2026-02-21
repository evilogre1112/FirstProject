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
int find_insert_pos(listMB& dsMB, char* const soHieuMB) {
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
    int index = find_insert_pos(dsMB, newMB->soHieuMB);
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

bool Del_MB(listMB& dsMB, char* const soHieuMB) {
    int index = Find_MB(dsMB, soHieuMB);
    if (index == -1) return false;
    delete dsMB.list[index];
    for (int i = index; i < dsMB.slMB - 1; i++) {
        dsMB.list[i] = dsMB.list[i + 1];
    }
    dsMB.list[dsMB.slMB - 1] = NULL;
    dsMB.slMB--;
    return true;
}

bool Edit_MB(listMB& dsMB, char* const soHieuMB, MB *infoUpdate) {
    int index = Find_MB(dsMB, soHieuMB);
    if (index == -1) return false;
    if (ss_str(dsMB.list[index]->soHieuMB, infoUpdate->soHieuMB) == 0) {
        *dsMB.list[index] = *infoUpdate;
        return true;
    }
    int exist = Find_MB(dsMB, infoUpdate->soHieuMB);
    if (exist != -1) return false;
    MB* newMB = new MB();
    *newMB = *infoUpdate;
    Del_MB(dsMB, soHieuMB);
    Add_MB(dsMB, newMB);
    return true;
}

// Chuyen Bay
bool Add_CB(listCB &dsCB, CB *newCB) {
    return false;
}

bool Update_Time_CB(CB *dsCB, const char* maCB, const DateTime &newTime)
{
    return false;
}

bool Cancel_CB(CB *&dsCB, const char* maCB)
{
    return false;
}

int Status_CB(CB *dsCB, const char* maCB)
{
    return 0;
}

void Init_Tickets(CB *newCB, int soCho)
{
}

CB *Find_CB(CB *const dsCB, const char* maCB)
{
    return nullptr;
}

HK *Find_HK(CB *const dsCB, HK *const dsHK, const char* maCB, const char* cmnd)
{
    return nullptr;
}

bool Add_HK(HK *&dsHK, const char* ho, const char* ten, const char* cmnd, const char* phai)
{
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
