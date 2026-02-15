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

int Find_MB(MB *const dsMB[], int slMB, const char* soHieuMB)
{
    return 0;
}

bool Add_MB(MB *dsMB[], int &slMB, MB *newMB)
{
    return false;
}

bool Del_MB(MB *dsMB[], int &slMB, const char* soHieuMB)
{
    return false;
}

bool Edit_MB(MB *dsMB[], int slMB, const char* soHieuMB, MB *infoUpdate)
{
    return false;
}

bool Add_CB(CB *&dsCB, CB *newCB)
{
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
