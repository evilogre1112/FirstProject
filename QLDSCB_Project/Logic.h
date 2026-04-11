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

// Một số tiện ích
int ss_str(char* const a, char* const b); // so sánh 2 chuỗi



// --- CÂU A: QUẢN LÝ MÁY BAY ---
// Thao tác trên DSMB
// MB = type của cấu trúc 1 máy bay
// LƯU Ý :: CÁC HÀM MÁY BAY ĐÃ ĐƯỢC TỐI ƯU SẴN, HÃY ĐẢM BẢO KHI SỬ DỤNG, DANH SÁCH MÁY BAY ĐÃ ĐƯỢC SẮP XẾP. DANH SÁCH MÁY BAY CHƯA ĐƯỢC SẮP XẾP SẼ CHO RA KẾT QUẢ SAI Ở MỌI HÀM.

/**
 * @brief       Lưu dsMB vào file văn bản txt
 * @param dsMB  Mảng các con trỏ trỏ đến đối tượng máy bay
 * @param save  Tham chiếu biến ghi nội dung trong dsMB vào file txt
 * @return      true nếu ghi file thành công. false nếu ngược lại
 */
bool Save_MB(ofstream& save, listMB& dsMB);

void swap_MB(MB* &a, MB* &b); // tien ich swap noi dung cua 2 may bay

/**
 * @brief       sắp xếp danh sách máy bay theo mã máy bay, sử dụng quick_sort
 * @param dsMB  Mảng các con trỏ trỏ đến đối tượng máy bay
 * @param l     vị trí bắt đầu trong mảng muốn sắp xếp
 * @param r     vị trí kết thúc trong mảng muốn sắp xếp ( Sắp xếp từ vị trí l đên r )
 */
int Hoare_Partion(listMB& dsMB, int l,int r); // Hàm tiện ích, phục vụ cho quick_sort
void Sort_MB(listMB& dsMB, int l, int r);

/**
 * @brief           Tìm vị trí chính xác để chèn MB theo soHieuMB tăng dần
 * @param dsMB      Mảng các con trỏ trỏ đến đối tượng máy bay
 * @param soHieuMB  số hiệu máy bay cần tìm vị trí chèn
 * @return          trả về vị trí chính xác cần chèn để chèn MB có soHieuMB
 */
int find_insert_posMB(listMB& dsMB, char* const soHieuMB);

/**
 * @brief           tìm máy bay X trong danh sách máy bay
 * @param dsMB      mảng các con trỏ
 * @param soHieuMB  chuỗi C15 chứa mã máy bay cần tìm
 * @return          vị trí máy bay trong danh sách, trar về -1 nếu không tìm thấy
 */
int Find_MB(listMB& dsMB, char* const soHieuMB);

/**
 * @brief       Thêm 1 máy bay mới vào mảng con trỏ
 * @param dsMB  Mảng các con trỏ trỏ đến đối tượng máy bay
 * @param newMB Con trỏ trỏ đến máy bay mới đã được cấp phát vùng nhớ
 * @return      true nếu thêm thành công, false nếu mảng đầy (n >= 300) hoặc trùng mã
 */
bool Add_MB(listMB& dsMB, MB* newMB);

/**
 * @brief           Xóa máy bay dựa trên Số hiệu máy bay (Mã duy nhất)
 * @param dsMB      Mảng các con trỏ
 * @param dsCB      DS liên kết đơn CB
 * @param soHieuMB  Chuỗi C15 chứa mã máy bay cần xóa
 * @return          true nếu tìm thấy và xóa thành công, false nếu không tìm thấy
 */
// LUư Ý: Khi truyền soHieuMB khồng được truyền dạng con trỏ của dsMB, ví dụ dsMB.list[i]->soHieuMB
bool Del_MB(listMB& dsMB, listCB& dsCB, char* const soHieuMB);

/**
 * @brief               Hiệu chỉnh thông tin máy bay
 * @param dsMB          Mảng các con trỏ
 * @param soHieuMB      Chuỗi C15 chứa mã máy bay cần hiệu chỉnh
 * @param infoUpdate    thông tin cập nhật cho soHieuMB trong mảng
 * @return              true nếu hiệu chỉnh thành công
 */
// LUƯ Ý: infoUpdate SẼ KHÔNG TỰ ĐÔNG ĐƯỢC GIẢI PHÓNG BỘ NHỚ TRONG HÀM
// LUư Ý: Khi truyền soHieuMB khồng được truyền dạng con trỏ của dsMB, ví dụ dsMB.list[i]->soHieuMB
bool Edit_MB(listMB& dsMB, listCB& dsCB, char* const soHieuMB, MB* infoUpdate);

// --- CÂU B: QUẢN LÝ CHUYẾN BAY ---
// Thao tác trên DSCB
// CB = type của 1 chuyến bay

// LƯU Ý :: CÁC HÀM CHUYẾN BAY ĐÃ ĐƯỢC TỐI ƯU SẴN, HÃY ĐẢM BẢO KHI SỬ DỤNG, DANH SÁCH CHUYẾN BAY ĐÃ ĐƯỢC SẮP XẾP THEO maCB. DANH SÁCH CHUYẾN BAY CHƯA ĐƯỢC SẮP XẾP SẼ CHO RA KẾT QUẢ SAI Ở MỌI HÀM.

/**
 * @brief         Swap data của 2 CB
 * @param CB1     CB thứ 1
 * @param CB2     CB thứ 2
*/
void Swap_CB(CB* &CB1, CB* &CB2);

/**
 * @brief         Sắp xếp listCB theo thứ tự maCB tăng dần
 * @param dsCB         Danh sách chuyến bay
*/
void Sort_CB(listCB &dsCB);

/**
 * @brief       Tìm chuyến bay trong DSCB.
 * @param dsCB  Danh sách chuyến bay
 * @param maCB  Chuỗi C15 chứa mã hiệu chuyến bay cần tìm.
 * @return      Địa chỉ của Node chuyến bay nếu thấy, ngược lại trả về NULL
*/
CB* Find_CB(listCB &dsCB, char* const maCB);

/**
 * @brief       Tìm vị trí chèn chuyến bay trong DSCB đã được sắp xếp theo mã CB.
 * @param dsCB  Danh sách chuyến bay
 * @param maCB  Chuỗi C15 chứa mã hiệu chuyến bay cần tìm.
 * @return      Địa chỉ của CB trước vị trí chèn cần chèn
*/
CB* find_insert_posCB(listCB &dsCB, char* const maCB);

/**
 * @brief           Tìm soHieuMB trong DSCB để kiểm tra xem MB do có đang còn hoạt động hay không.
 * @param dsCB      Danh sách chuyến bay
 * @param soHieuMB  Chuỗi C15 chứa mã hiệu máy bay cần tìm.
 * @return          Địa chỉ của Node chuyến bay nếu thấy, ngược lại trả về NULL
*/
CB* Find_Active_MB(listCB &dsCB, char* const soHieuMB);

/**
 * @brief           Thêm 1 chuyến bay mới vào danh sách liên kết đơn
 * @param dsCB      Danh sách chuyến bay
 * @param newCB     Con trỏ trỏ đến node chuyến bay mới đã được cấp phát
 * @return          true nếu thêm thành công, false nếu trùng mã chuyến bay
 */
// LƯU Ý: Hàm Add_CB không hề tự động khởi tạo DSV, hãy khởi tạo DSV bằng hàm Init_Tickets
bool Add_CB(listCB &dsCB, listMB &dsMB, CB* newCB);

/**
 * @brief           Hiệu chỉnh ngày giờ khởi hành của một chuyến bay
 * @param dsCB      danh sách chuyến bay
 * @param maCB      Mã hiệu chuyến bay cần tìm để sửa
 * @param newTime   Cấu trúc chứa ngày giờ mới
 * @return          true nếu tìm thấy và sửa thành công
 */
bool Update_Time_CB(listCB &dsCB, char* const maCB, const DateTime& newTime);

/**
 * @brief       Huỷ 1 chuyến bay trong danh sách liên kết đơn
 * @param dsCB  Danh sách chuyến bay
 * @param maCB  Chuỗi C15 chứa mã hiệu chuyến bay cần tìm để hủy
 * @return      True nếu huỷ  được và thành công
*/
bool Cancel_CB(listCB &dsCB, char* const maCB);

/**
 * @brief       Cho biết trạng thái của chuyến bay trong DSCB.
 * @param dsCB  Danh sách chuyến bay
 * @param maCB  Chuỗi C15 chứa mã hiệu chuyến bay cần tìm để trả về trạng thái
 * @return      Trạng thái chuyến bay:
 *                  1: Còn vé
 *                  2: Hết vé
 *                  3: Hoàn tất (đã bay)
 *                  0: Đã hủy
 *                  -1: Không tìm thấy mã chuyến bay này trong hệ thống.
 */
 int Status_CB(listCB &dsCB, char* const maCB);

/**
 * @brief           Khởi tạo danh sách vé trống cho chuyến bay vừa lập
 * @param newCB     Node chuyến bay mới
 * @param soCho     Số chỗ lấy từ thông tin máy bay thực hiện chuyến đó
 */
void Init_Tickets(CB* newCB, int soCho);

// --- CÂU C: ĐẶT VÉ ---
// Tương tác giữa Cây BST (Hành khách) và Danh sách liên kết (Chuyến bay)

/**
 * @brief       Tìm kiếm và lấy thông tin hành khách trong dsHK dựa trên CMND
 * @param   root    Gốc của dsHK
 * @param   cmnd    Số chứng minh nhân dân của khách cần tìm
 * @return      Địa chỉ của hành khách trong BST nếu họ có đặt vé trên chuyến này, ngược lại NULL
 */
HK* Find_HK_At_List(HK* root, char* const cmnd);

/**
 * @brief       Tìm kiếm và lấy thông tin hành khách trên một chuyến bay cụ thể dựa trên CMND
 * @param dsCB  danh sách chuyến bay
 * @param root  Nút gốc của cây HK (Cây BST)
 * @param cmnd  Số chứng minh nhân dân của khách cần tìm
 * @return      Địa chỉ của hành khách trong BST nếu họ có đặt vé trên chuyến này, ngược lại NULL
 */
HK* Find_HK(listCB &dsCB, HK* root, char* const maCB, char* const cmnd);

/**
 * @brief       Thêm một hành khách mới vào cây BST
 * @param root  Nút gốc ủa cây hành khách (cây BST)
 * @param ho    Họ
 * @param ten   Tên
 * @param cmnd  Chứng minh nhân dân
 * @param phai  Phái Nam/Nu
 * @return      true nếu thêm thành công, false nếu trùng mã CMND
 */
bool Add_HK(HK* &root, char* const ho, char* const ten, char* const cmnd, bool phai);

/**
 * @brief       Kiểm tra hành khách đã mua vé trên chuyến bay này chưa (1 vé/chuyến)
 * @param dsCB  Danh sách chuyến bay
 * @param maCB  Mã chuyến bay
 * @param CMND  Số CMND cần kiểm tra trong danh sách vé
 * @return      True nếu đã có tên trong danh sách vé, false nếu chưa
 */
bool Is_Ticket_Booked(CB* const dsCB,const char* maCB,const char* CMND);

/**
 * @brief               Thực hiện đặt vé (ghi CMND vào chỗ ngồi) trên một chuyến bay cụ thể
 * @param dsCB          Danh sách chuyến bay
 * @param maCB          Mã chuyến bay
 * @param CMND          Số chứng minh nhân dân của hành khách
 * @param seatNumber    Số thứ tự ghế khách chọn (từ 1 đến Số chỗ)
 * @return              True nếu đặt thành công, false nếu ghế đã có người hoặc chuyến bay không còn cho đặt hoặc khách hàng mới chưa có trong dsKH
 */
bool Book_Ticket(CB* dsCB,const char* maCB,const char* CMND, int seatNumber);

// --- CÂU D: HỦY VÉ ---

/**
 * @brief               Thực hiện hủy vé của hành khách trên một chuyến bay
 * @param dsCB          Danh sách chuyến bay
 * @param seatNumber    Số thứ tự ghế cần hủy (tính từ 1 đến n)
 * @return              True nếu hủy thành công, false nếu ghế đó vốn dĩ đang trống hoặc chuyến bay không còn cho phép hủy
 */
bool Cancel_Ticket(CB* &dsCB, const char* maCB , int seatNumber);

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
HK** Get_DSHKCB(CB* const dsCB, HK* const dsHK,const char* maCB, int &sldsHK);

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
CB** Search_CB(CB* const dsCB ,const DateTime& date,const char* address, int& sldsCB);

// -- CÂU G: IN DANH SÁCH VÉ TRỐNG --

/**
 * @brief           Lấy danh sách các số ghế còn trống của MỘT chuyến bay X 
 * @param dsCB      sách liên kết đơn của các chuyến bay 
 * @param maCB      Logic sẽ đếm và ghi số lượng danh sách vé trống vào đây trước khi return.
 * @param sldsVT    Logic sẽ đếm và ghi số lượng danh sách chuyến bay vào đây trước khi return.
 * @return          Danh sách vé trống
 * @note            UI có trách nhiệm delete[] mảng này sau khi in xong để tránh rò rỉ bộ nhớ.
 */
int* Get_Empty_Seats(CB* const dsCB ,const char* maCB, int &sldsVT);

// -- CÂU H: THỐNG KÊ LƯỢT BAY --

// các tiện ích hỗ trợ cho get_Flight_Stactics (Merge_Sort)
int ss_SLB(MB* a, MB* b);
void Merge_SLB(listMB& dsMB, int l,int m, int r);

/**
 * @brief       sắp xếp danh sách máy bay theo mã SLB giảm dần và soHieuMB tăng dần, sử dụng merge_sort
 * @param dsMB  Mảng các con trỏ trỏ đến đối tượng máy bay
 * @param l     vị trí bắt đầu trong mảng muốn sắp xếp
 * @param r     vị trí kết thúc trong mảng muốn sắp xếp ( Sắp xếp từ vị trí l đên r )
 */
void Sort_SLB(listMB& dsMB, int l, int r);

/**
 * @brief       Thống kê số lần thực hiện chuyến bay của từng máy bay có trong hệ thống
 * @return      Trả về mảng MB chứa Số lượng máy bay giảm dần và Mã hiệu máy bay tăng dần
 * @note        UI có trách nhiệm delete[] mảng này sau khi in xong.
 */
listMB Get_Flight_Stats (listMB &dsMB);


#endif
