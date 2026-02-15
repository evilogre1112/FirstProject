#ifndef LOGIC_H
#define LOGIC_H

#include "Global.h"
#include <string>
// Các biến sau là biến toàn cục(extern) trong "Global.h"
/**
 *  slMB số lượng máy bay
 *  sldsHK số lượng danh sách hành khách
 *  sldsCB số lượng danh sách chuyến bay
 *  sldsVT số lượng danh sách vé trống
 */

// Logic quy định cho struct viết cấu trúc của ngày giờ có tên là DateTime (dd/mm/yyyy)

// --- CÂU A: QUẢN LÝ MÁY BAY ---
// Thao tác trên DSMB
// MB = type của cấu trúc 1 máy bay 

/**
 * @brief       Thêm 1 máy bay mới vào mảng con trỏ
 * @param dsMB  Mảng các con trỏ trỏ đến đối tượng máy bay
 * @param slMB  Số lượng máy bay hiện có (truyền tham chiếu để cập nhật)
 * @param newMB Con trỏ trỏ đến máy bay mới đã được cấp phát vùng nhớ
 * @return      true nếu thêm thành công, false nếu mảng đầy (n >= 300) hoặc trùng mã
 */
bool Add_MB(MB* dsMB[], int &slMB, MB* newMB);

/**
 * @brief           Xóa máy bay dựa trên Số hiệu máy bay (Mã duy nhất)
 * @param dsMB      Mảng các con trỏ
 * @param slMB      Số lượng máy bay hiện có (cập nhật lại n sau khi xóa)
 * @param soHieuMB  Chuỗi C15 chứa mã máy bay cần xóa
 * @return          true nếu tìm thấy và xóa thành công, false nếu không tìm thấy
 */
bool Del_MB(MB* dsMB[], int &slMB, const string& soHieuMB);

/**
 * @brief               Hiệu chỉnh thông tin máy bay
 * @param dsMB          Mảng các con trỏ
 * @param slMB          Số lượng máy bay hiện có
 * @param soHieuMB      Chuỗi C15 chứa mã máy bay cần hiệu chỉnh
 * @param infoUpdate    thông tin cập nhật cho soHieuMB trong mảng
 * @return              true nếu hiệu chỉnh thành công
 */
bool Edit_MB(MB* dsMB[], int slMB, const string& soHieuMB , MB* infoUpdate);

/**
 * @brief           tìm máy bay X trong danh sách máy bay
 * @param dsMB      mảng các con trỏ
 * @param slMB      Số lượng máy bay hiện có
 * @param soHieuMB  chuỗi C15 chứa mã máy bay cần tìm
 * @return          vị trí máy bay trong danh sách
 */
int Find_MB(MB* const dsMB[],int slMB,const string& soHieuMB);

// --- CÂU B: QUẢN LÝ CHUYẾN BAY ---
// Thao tác trên DSCB
// CB = type của 1 chuyến bay

/**
 * @brief           Thêm 1 chuyến bay mới vào danh sách liên kết đơn
 * @param dsCB      Danh sách chuyến bay
 * @param newCB     Con trỏ trỏ đến node chuyến bay mới đã được cấp phát
 * @return          true nếu thêm thành công, false nếu trùng mã chuyến bay
 */
bool Add_CB(CB* &dsCB, CB* newCB);

/**
 * @brief           Hiệu chỉnh ngày giờ khởi hành của một chuyến bay
 * @param dsCB      danh sách chuyến bay
 * @param maCB      Mã hiệu chuyến bay cần tìm để sửa
 * @param newTime   Cấu trúc chứa ngày giờ mới
 * @return          true nếu tìm thấy và sửa thành công
 */
bool Update_Time_CB(CB* dsCB, const string& maCB, const DateTime& newTime);

/**
 * @brief       Huỷ 1 chuyến bay trong danh sách liên kết đơn
 * @param dsCB  Danh sách chuyến bay
 * @param maCB  Chuỗi C15 chứa mã hiệu chuyến bay cần tìm để hủy
 * @return      True nếu huỷ  được và thành công
*/
bool Cancel_CB(CB* &dsCB, const string& maCB);

/**
 * @brief       Cho biết trạng thái của chuyến bay trong DSCB.
 * @param dsCb  Danh sách chuyến bay
 * @param maCB  Chuỗi C15 chứa mã hiệu chuyến bay cần tìm để trả về trạng thái
 * @return      Trạng thái chuyến bay:
 *                  1: Còn vé
 *                  2: Hết vé
 *                  3: Hoàn tất (đã bay)
 *                  0: Đã hủy
 *                  -1: Không tìm thấy mã chuyến bay này trong hệ thống.
 */
 int Status_CB(CB* dsCB, const string& maCB);

/**
 * @brief           Khởi tạo danh sách vé trống cho chuyến bay vừa lập
 * @param newCB     Node chuyến bay mới
 * @param soCho     Số chỗ lấy từ thông tin máy bay thực hiện chuyến đó
 */
void Init_Tickets(CB* newCB, int soCho);

/**
 * @brief       Tìm chuyến bay trong DSCB.
 * @param dsCB  Danh sách chuyến bay
 * @param maCB  Chuỗi C15 chứa mã hiệu chuyến bay cần tìm.
 * @return      Địa chỉ của Node chuyến bay nếu thấy, ngược lại trả về NULL
*/
CB* Find_CB(CB* const dsCB,const string& maCB);

// --- CÂU C: ĐẶT VÉ ---
// Tương tác giữa Cây BST (Hành khách) và Danh sách liên kết (Chuyến bay)

/**
 * @brief       Tìm kiếm và lấy thông tin hành khách trên một chuyến bay cụ thể dựa trên CMND
 * @param dsCB  danh sách chuyến bay
 * @param dsHK  Danh sách hành khách tổng quát (Cây BST)
 * @param CMND  Số chứng minh nhân dân của khách cần tìm
 * @return      Địa chỉ của hành khách trong BST nếu họ có đặt vé trên chuyến này, ngược lại NULL
 */
HK* Find_HK(CB* const dsCB, HK* const dsHK,const string& maCB, const string& cmnd);

/**
 * @brief       Thêm một hành khách mới vào cây BST
 * @param dsHK  danh sách hành khách (cây BST)
 * @param ho    Họ
 * @param ten   Tên
 * @param cmnd  Chứng minh nhân dân
 * @param phai  Phái Nam/Nu
 * @return      true nếu thêm thành công, false nếu trùng mã CMND
 */
bool Add_HK(HK* &dsHK,const string&  ho,const string& ten ,const string& cmnd,const string& phai );

/**
 * @brief       Kiểm tra hành khách đã mua vé trên chuyến bay này chưa (1 vé/chuyến)
 * @param dsCB  Danh sách chuyến bay
 * @param maCB  Mã chuyến bay
 * @param CMND  Số CMND cần kiểm tra trong danh sách vé
 * @return      True nếu đã có tên trong danh sách vé, false nếu chưa
 */
bool Is_Ticket_Booked(CB* const dsCB,const string& maCB,const string& CMND);

/**
 * @brief               Thực hiện đặt vé (ghi CMND vào chỗ ngồi) trên một chuyến bay cụ thể
 * @param dsCB          Danh sách chuyến bay
 * @param maCB          Mã chuyến bay
 * @param CMND          Số chứng minh nhân dân của hành khách
 * @param seatNumber    Số thứ tự ghế khách chọn (từ 1 đến Số chỗ)
 * @return              True nếu đặt thành công, false nếu ghế đã có người hoặc chuyến bay không còn cho đặt hoặc khách hàng mới chưa có trong dsKH
 */
bool Book_Ticket(CB* dsCB,const string& maCB,const string& CMND, int seatNumber);

// --- CÂU D: HỦY VÉ ---

/**
 * @brief               Thực hiện hủy vé của hành khách trên một chuyến bay
 * @param dsCB          Danh sách chuyến bay
 * @param seatNumber    Số thứ tự ghế cần hủy (tính từ 1 đến n)
 * @return              True nếu hủy thành công, false nếu ghế đó vốn dĩ đang trống hoặc chuyến bay không còn cho phép hủy
 */
bool Cancel_Ticket(CB* &dsCB, const string& maCB , int seatNumber);

// -- CÂU E: IN DANH SÁCH HÀNH KHÁCH THEO MÃ CB --

/**
 * @brief           Thu thập thông tin cá nhân của toàn bộ hành khách trên 1 chuyến bay
 * @param dsCB      Danh sách chuyến bay
 * @param dsHK      Gốc cây BST hành khách (danh sách hành khách)
 * @param maCB      Mã chuyến bay cần lấy danh sách
 * @param sldsHK    Tham chiếu để trả về số lượng hành khách thực tế
 * @return          Trả về mảng con trỏ hành khách
 * @note            Trả về NULL nếu không thấy chuyến bay hoặc không có khách. UI phải delete[] sau khi dùng.
 */
HK** Get_DSHKCB(CB* const dsCB, HK* const dsHK,const string& maCB, int &sldsHK);

// -- CÂU F: TRA CỨU CÁC CHUYẾN BAY DỰA VÀO NGÀY VÀ NƠI ĐẾN --

/**
 * @brief           Tìm thông tin chuyến bay theo ngày và địa chỉ
 * @param dsCB      Sách liên kết đơn của các chuyến bay 
 * @param date      Định dạng dd/mm/yyyy
 * @param address   Nơi đến XXXXX
 * @param sldsCB    Logic sẽ đếm và ghi số lượng danh sách chuyến bay vào đây trước khi return.
 * @return          Danh sách chuyến bay theo ngày và địa chỉ (mảng con trỏ)
 * @note            UI có trách nhiệm delete[] mảng này sau khi in xong.
 */
CB** Search_CB(CB* const dsCB ,const DateTime& date,const string& address, int& sldsCB);

// -- CÂU G: IN DANH SÁCH VÉ TRỐNG --

/**
 * @brief           Lấy danh sách các số ghế còn trống của MỘT chuyến bay X 
 * @param dsCB      sách liên kết đơn của các chuyến bay 
 * @param maCB      Logic sẽ đếm và ghi số lượng danh sách vé trống vào đây trước khi return.
 * @param sldsVT    Logic sẽ đếm và ghi số lượng danh sách chuyến bay vào đây trước khi return.
 * @return          Danh sách vé trống
 * @note            UI có trách nhiệm delete[] mảng này sau khi in xong để tránh rò rỉ bộ nhớ.
 */
int* Get_Empty_Seats(CB* const dsCB ,const string& maCB, int &sldsVT);

// -- CÂU H: THỐNG KÊ LƯỢT BAY --

// Tạo struct lưu trữ hai giá trị là Máy Bay và Số Lần Bay như sau.
/*
    struct MB_Stat {
        string SHMB[15];  // Lấy từ cấu trúc Máy bay
        int SLB;        // Biến đếm số lần xuất hiện trong DSCB
    };
*/

/**
 * @brief       Thống kê số lần thực hiện chuyến bay của từng máy bay có trong hệ thống
 * @param dsCB  Đầu danh sách liên kết đơn các chuyến bay
 * @param count Tham chiếu để Logic trả về số lượng máy bay tìm thấy (lượt bay > 0)
 * @return      Trả về mảng MB_Stat chứa Số hiệu máy bay và Số lượt bay
 * @note        Thứ tự mảng: Sắp xếp giảm dần theo số lượt thực hiện chuyến bay.
 * @note        UI có trách nhiệm delete[] mảng này sau khi in xong.
 */
MB_Stat* Get_Flight_Statistics(CB* const dsCB, int& count);


#endif