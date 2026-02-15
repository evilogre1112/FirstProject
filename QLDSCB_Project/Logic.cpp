//
//  Logic.cpp
//  Nơi chứa code của các hàm Logic
//  By Tran Thanh Thien || RimceEL
//
//
#include "Logic.h"

// Một số tiện ích
int ss_str(char* a, char* b) {
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

// Danh sách máy bay kiểu con trỏ phải luôn đảm bảo khi đưa vào đã được sắp xếp
// Rút ngắn thời gian thêm sửa và xoá

// --- CÂU A: QUẢN LÝ MÁY BAY ---
// Thao tác trên DSMB
// MB = type của cấu trúc 1 máy bay

// Tiện ích sắp xếp
void Merge_MB(MB* dsMB[], int l,int m, int r) {
    int x_size = m - l; int y_size = r - m + 1;
    MB* x[x_size]; MB* y[y_size];
    int nx = l; int ny = m;
    for (int i = 0; i < x_size; i++) {
        x[i] = dsMB[nx];
        nx++;
    }
    for (int j = 0; j < y_size; j++) {
        y[j] = dsMB[ny];
        ny++;
    }
    int i = j = 0;
    while (i < x_size && j < y_size) {
        if (x[i]->soHieuMB <= y[j]->soHieuMB) {
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
}
// Merge sort MB
void Sort_MB(MB* dsMB[], int l, int r) {
    if (l >= r) return;
    int m = (r + l) / 2;
    Sort_MB(dsMB, l, m - 1);
    Sort_MB(dsMB, m, r);
    Merge_MB(dsMB, l, m, r);
}

// FindMB
int Find_MB(MB* dsMB[],int n,char* soHieuMB) {
    int l = 0, r = n - 1;
    int m = (r + l) / 2;
    while (l <= r) {
        int result = ss_str(dsMB[m]->soHieuMB, soHieuMB);
        if (result == 0) return true;
        else if (result == 1) r = m - 1;
        else l = m + 1;
        m = (r + l) / 2;
    }
    return false;
}

bool Add_MB(MB *dsMB[], int &slMB, MB *newMB)
{
    return false;
}

bool Del_MB(MB *dsMB[], int &slMB, const string& soHieuMB)
{
    return false;
}

bool Edit_MB(MB *dsMB[], int slMB, const string& soHieuMB, MB *infoUpdate)
{
    return false;
}

bool Add_CB(CB *&dsCB, CB *newCB)
{
    return false;
}

bool Update_Time_CB(CB *dsCB, const string& maCB, const DateTime &newTime)
{
    return false;
}

bool Cancel_CB(CB *&dsCB, const string& maCB)
{
    return false;
}

int Status_CB(CB *dsCB, const string& maCB)
{
    return 0;
}

void Init_Tickets(CB *newCB, int soCho)
{
    
}

CB *Find_CB(CB *const dsCB, const string& maCB)
{
    return nullptr;
}

HK *Find_HK(CB *const dsCB, HK *const dsHK, const string& maCB, const string& cmnd)
{
    return nullptr;
}

bool Add_HK(HK *&dsHK, const string& ho, const string& ten, const string& cmnd, const string& phai)
{
    return false;
}

bool Is_Ticket_Booked(CB *const dsCB, const string& maCB, const string& CMND)
{
    return false;
}

bool Book_Ticket(CB *dsCB, const string& maCB, const string& CMND, int seatNumber)
{
    return false;
}

bool Cancel_Ticket(CB *&dsCB, const string& maCB, int seatNumber)
{
    return false;
}

HK **Get_DSHKCB(CB *const dsCB, HK *const dsHK, const string& maCB, int &sldsHK)
{
    return nullptr;
}

CB **Search_CB(CB *const dsCB, const DateTime &date, const string& address, int &sldsCB)
{
    return nullptr;
}

int *Get_Empty_Seats(CB *const dsCB, const string& maCB, int &sldsVT)
{
    return nullptr;
}

MB_Stat *Get_Flight_Statistics(CB *const dsCB, int &count)
{
    return nullptr;
}

