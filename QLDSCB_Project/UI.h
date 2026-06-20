#ifndef UI_H
#define UI_H

#include <iostream>
#include <string>
#include "Global.h"
#include "Logic.h"
#include <functional>

using namespace std;

extern int TotalWidth;
extern int TotalHeight;

#define boxWidth 60

#define RESET       "\033[0m"
#define YELLOW      "\033[33m"  
#define GREEN       "\033[32m"
#define GRAY        "\033[90m"
#define WHITE       "\033[37m" 
#define RED         "\033[31m"
#define MAGENTA     "\033[35m"
#define CYAN        "\033[36m"
#define BLUE        "\033[34m"
#define BOLD        "\033[1m"
#define ORANGE      "\033[38;5;208m"

// Các màu Bold (In đậm/Sáng hơn)
#define BOLDRED     "\033[1;31m"
#define BOLDGREEN   "\033[1;32m"
#define BOLDYELLOW  "\033[1;33m"
#define BOLDBLUE    "\033[1;34m"
#define BOLDMAGENTA "\033[1;35m"
#define BOLDCYAN    "\033[1;36m"
#define BOLDWHITE   "\033[1;37m"
#define BOLDUNDERLINE "\033[1;4m"

// Màu Bg
#define BG_YELLOW        "\033[43m"
#define BG_BRIGHT_YELLOW "\033[103m"
#define HIGHLIGHT        "\033[7m"
#define TEXT_BLACK       "\033[30m"

// Các Lệnh "Ảo Thuật" =)))
#define NEWTAB   "\033[?1049h" // Tạo màn hình phụ
#define CLOSETAB "\033[?1049l" // Đóng màn hình phụ
// "\033[%d;%dH" lệnh di chuyển con trỏ trên window
// \033 : Đây là mã ASCII của phím ESC (Escape), hệ bát phân (octal) là 033 (hệ thập phân là 27).rong C++, 
// khi bạn đặt nó trong dấu nháy đơn '', trình biên dịch hiểu đây là một ký tự duy nhất (char) có mã ASCII là 27.
// [ : Ký tự bắt buộc luôn đi liền sau ESC để mở đầu một lệnh điều khiển (gọi là CSI - Control Sequence Introducer).
// %d;%d : Đây là chỗ của hàm printf để bạn nhét hai con số vào. Theo chuẩn ANSI, số đầu tiên là Dòng (Tọa độ Y), số thứ hai là Cột (Tọa độ X).
// H: Chữ cái chốt sổ. Nó là mã lệnh của hành động Cursor Position (Đặt vị trí con trỏ).
// ?1049: Đây là mã ID của một tính năng cụ thể trong chuẩn Terminal. Mã 1049 đại diện cho hành động kép: Lưu lại vị trí con trỏ hiện tại + Mở ra một bộ đệm màn hình mới tinh.
// h: Viết tắt của High (hoặc Set/Enable). Nghĩa là "Bật tính năng này lên".
// l: Viết tắt của low (hoặc Reset/Disable). Nghĩa là "tắt tính năng này đi".
// "\033[6n" là Device Status Report - Cursor Position Report (Yêu cầu báo cáo vị trí con trỏ).
// 6: Mã hiệu cụ thể của câu hỏi: "Con trỏ hiện tại đang đứng ở dòng mấy, cột mấy?"
// n: đại diện cho lệnh yêu cầu báo cáo trạng thái (Report).
// sử dụng lệnh cout hoặc prinf để sử dụng những hàm này 

// Ký tự vẽ khung
#define AL "╔"
#define AR "╗"
#define BL "╚"
#define BR "╝"
#define HL "═"
#define VL "║"
#define CL "╠"
#define CRR "╣"
#define AP "▲"
#define AD "▼"
#define ARL "◄"  // Hoặc "◀"
#define ARR "►"  // Hoặc "▶"
#define SAP "↑"
#define SAD "↓"
#define SQ "■"
#define NSQ "□"
#define CI "○"
#define PT "◉"
#define SB "█"
#define TLC "╭"
#define TRC "╮"
#define VT  "│"
#define BRC "╯"
#define BLC "╰"
#define HZ  "─"
#define DM "◈"


void TruyenThamSo(listMB &A, listCB& B , listHK &C, listSBD &D);

/**
 * @brief trã về mã màu tương ứng với trạng thái st
 * @param st mã trạng thái 
 */
string TranFormSatus(int st);

int GetKey(int chosen,int count);

NavKey CustomerAddMB();
NavKey CustomerAddCB();
NavKey CustomerAddHK(string cmnd);

// Chọn 1 danh sách nhỏ theo shMB, khối hàm hiển thị danh sách nhỏ của MB
void UiThongTinMB(int x,int y , int width, int height,string s);
int  SmallListShMB( string& res, int &index, int width , int x, int y, int items, listMB& A,int X, int Y, int w , int h);
void UiSmallListShMB(int width, int max_items, int x, int y, listMB& A);

// Chọn 1 danh sách nhỏ theo mã CB, khối hàm hiển thị danh sách nhỏ của CB
int SmallListMCB( string& res, CB* &thisCB, int width , int x, int y, int items, listCB &A,int X, int Y, int w , int h);
void UiSmallListMCB(int width, int max_items, int x, int y, listCB& A);
void UiThongTinCB(int x,int y , int width, int height,string MCB);

// Chọn 1 danh sách nhỏ theo mã só chỗ của chuyến bay, khối hàm hiển thị danh sách nhỏ của số chỗ
int SmallListSoCho(string& res, listCB &dsSubCB, CB*& thisCB, int width, int x, int y, int items);
void UiThongTinHK(int x,int y , int width, int height,string s);

void PrintHeader(string res, int mod, string s, int size, string headers[], string typemode[], int Temp[], int Temp2[],int ResX = 50 , int FindX = 10,int ModeX =90, int ResY = 4);

bool MODE2_MB(listMB &SubList,bool IsMainList, int &index, int &current_Row, int &currentPage, int rowOnScreen,int &itemsPerPage,int Temp[], int Temp2[]);
bool MODE3_MB(listMB &SubList,bool IsMainList, int &index, int &current_Row, int &currentPage, int rowOnScreen,int &itemsPerPage,int Temp[], int Temp2[]);
bool MODE2_CB(CB* pageNodes[], int &index, int& startIndex, int current_Row, int currentPage, int rowOnScreen, int itemsPerPage, int Temp[], int Temp2[],int size);

void UiFindMB(string& res, NavKey key, int& ch, listMB& A,int x, int y);
void Render_MB_Page(listMB& danhSach, int currentPage, int itemsPerPage);
void OnlyPrint_List_MB(listMB& dsMBm,int currentPage);
void Get_Input_MB(bool IsHighLight, listMB &tmp, int rowOnScreen,int startIndex, int Temp[], int Temp2[] );
void It_list_MB(int mode, string res = "");
void StatiscalMB();

void OnlyPrint_List_CB(listCB& dsCB,int size,int CurrentPage, int itemsPerPage, int Temp[],int Temp2[],int Bin[] );
void UiFindCB(string& res, NavKey key, int& ch, listCB& A, listCB& mainList);
void Render_CB_Page(listCB& danhSach, int currentPage, int itemsPerPage,int size, int Temp[], int Temp2[],int Bin[]);
void Render_CB_Page(CB** CBSearch, int slCB,int Y,int errX,int errY, int currentPage, int itemsPerPage, int size, int Temp[], int Temp2[], int Bin[]);
void Get_Input_CB(bool IsHighLight,CB* pageNodes[], int rowOnScreen,int startIndex,int size, int Temp[], int Temp2[] );
void It_list_CB(int mod, listCB& A, string res = "", string cmnd = "");
void View_DSHK_CB(CB* thisCB);
void View_MB_CB(CB* thisCB);
void FindCBOnTimeAndPlace(string res="");

bool UiFindHK(string& res, NavKey key, int& ch, HK**& arrHK, int& totalItems);
bool UiFindHK(string& res, NavKey key, int& ch, HK**& arrHK, int& TotalCustomer,CB* thisCB);
void Render_HK_Page(HK** arr, int totalItems, int currentPage, int itemsPerPage);
void RenderHKBySample (HK** displayArr, CB* thisCB,int fields, int currentPage, int CustomerPerPage, int displayTotal,int tWidth,int Temp[], int Temp2[]);
void Get_Input_HK(bool IsHighLight,HK* arrHK[], int rowOnScreen,int index, int Temp[], int Temp2[] );
void It_list_HK(int mod, string res);

void    BookTicket();
void    HuyChuyen();
NavKey  SeatClear(int x, int y, int xt, int yt, CB* SelectedCB);
void    CancelTicketByCB();
void    CancelTicketByHK(int mod, string res = "");
NavKey  SeatList(int x, int y, int xt, int yt, CB* SelectedCB);
void    EmptySeatList();


void RunInNewTab(void (*func)() = [](){});
void RunInNewTab(NavKey (*func)());

void Menu_QuanLyMayBay();
void Menu_QuanLyChuyenBay();
void Menu_QuanLyHanhKhach();
void Menu_MayBayChuyenBay();
void Menu_ThongKeLuotBay();
void Menu_InDanhSachHanhKhach();

// Các hàm Ui hiển thị trước
void ViewQL(int idx);

void Router_B(int mainMenuIdx, function<void()> func_1 = [](){}, 
                  function<void()> func_2 = [](){}, 
                  function<void()> func_3 = [](){},
                  function<void()> func_4 = [](){});

void Router_Board(int mainMenuIdx, 
                  function<void()> func_1 = [](){}, 
                  function<void()> func_2 = [](){}, 
                  function<void()> func_3 = [](){});
                  
int MainMenuOptionInBoard(string options[], int length);
// Menu chính
void MainScreen();


#endif