#include "Logic.h"

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

int Find_MB(MB *const dsMB[], int slMB, const char* soHieuMB)
{
    return 0;
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
