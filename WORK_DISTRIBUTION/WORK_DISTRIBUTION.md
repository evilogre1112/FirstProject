## 📑KHỐI LƯỢNG CÔNG VIỆC

$\color{Yellow}{\text{I. STRUCT VÀ DATA}}$

* **Nhiệm vụ trọng tâm:** Xây dựng nền móng dữ liệu vững chắc cho toàn bộ hệ thống quản lý bay.
* **Chi tiết công việc:**
    * - 💠 **Định nghĩa Struct:** Thiết lập toàn bộ các cấu trúc dữ liệu: **Mảng con trỏ** (Máy bay), **Danh sách liên kết đơn** (Chuyến bay/Vé), và **Cây Nhị phân Tìm kiếm - BST** (Hành khách) theo đúng yêu cầu bảng dữ liệu tại `README.md`.
        > *Lưu ý: Chỉ thiết lập khai báo cấu trúc dữ liệu, không bao gồm các hàm xử lý logic.*
    * - 💾 **File I/O:** Viết các hàm nạp (Load) và lưu (Save) dữ liệu máy bay, chuyến bay và hành khách từ file `.txt` hoặc `.dat` để đảm bảo dữ liệu không bị mất khi thoát chương trình.
    * - 🛠️ **Hàm tiện ích:** Xử lý chuỗi, tính toán độ dài tiếng Việt có dấu (quan trọng để kẻ bảng danh sách hành khách và chuyến bay không bị vỡ giao diện). Cung cấp các hàm bổ trợ khi thành viên làm Logic hoặc Giao diện yêu cầu.
    * - 📊 **Data Collector:** Thu thập hoặc tạo dữ liệu mẫu (Mock Data) cho danh sách 1-300 máy bay, các chuyến bay nội địa và thông tin hành khách để phục vụ quá trình kiểm thử (Testing).
 ---

$\color{Yellow}{\text{II. LOGIC:}}$

- **Nhiệm vụ trọng tâm:** Xử lý thuật toán, quản lý bộ nhớ con trỏ và các thao tác dữ liệu phức tạp của hệ thống hàng không.
- **Chi tiết công việc:**
    - $\color{Cyan}{\text{CRUD:}}$ Cài đặt các chức năng Thêm/Xóa/Hiệu chỉnh (Create/Read/Update/Delete) cho:
        - **Máy bay:** Quản lý trong mảng con trỏ (300 phần tử).
        - **Chuyến bay:** Quản lý trong danh sách liên kết đơn (luôn đảm bảo thứ tự tăng dần theo mã).
        - **Hành khách:** Quản lý trên cây nhị phân tìm kiếm (BST) theo số CMND.
    - $\color{Cyan}{\text{Dữ liệu thô (Raw Data):}}$ Xử lý và cung cấp kết quả cho các yêu cầu nghiệp vụ dưới dạng mảng con trỏ/mảng tĩnh để UI chỉ việc hiển thị (a-h).
       
    - $\color{Cyan}{\text{Cấu trúc đặc biệt:}}$ Xử lý logic liên kết phức tạp giữa các cấu trúc:
        - **Chuyến bay chứa Danh sách vé:** Quản lý việc đặt/hủy vé dựa trên số CMND và tình trạng chỗ ngồi.
        - **Liên kết BST:** Truy xuất thông tin Họ tên/Phái từ cây BST hành khách thông qua số CMND lưu tại danh sách vé.

---
$\color{Yellow}{\text{III. GIAO DIỆN VÀ QUY ƯỚC (UI AND CONTRACT)}}$

- **Nhiệm vụ trọng tâm:** Thiết kế trải nghiệm người dùng chuyên nghiệp và điều phối luồng xử lý toàn diện của hệ thống quản lý bay.
- **Chi tiết công việc:**
    - $\color{Cyan}{\text{Hợp đồng dữ liệu (Convention):}}$ Quy định chuẩn tên hàm (ví dụ: `datVe`, `huyChuyen`, `searchHanhKhach`), kiểu trả về và bộ mã lỗi (Error Codes) để người làm Logic và Struct tuân theo. Contract này phải được thống nhất trước khi triển khai để đảm bảo tính tương thích khi ghép nối mã nguồn giữa các thành viên.
    - $\color{Cyan}{\text{Triển khai UI:}}$ Xây dựng giao diện điều khiển cho toàn bộ 8 chức năng từ **a đến h** (từ quản lý Máy bay đến Thống kê lượt bay) dựa trên các cấu trúc dữ liệu thô nhận được từ phần Logic.
    - $\color{Cyan}{\text{Xử lý sự kiện:}}$ 
        - Thiết lập cơ chế bắt phím điều hướng (Up/Down/Enter/ESC) để di chuyển giữa các danh sách Máy bay/Chuyến bay thay vì nhập số thủ công.
        - Thực hiện thuật toán phân trang (Pagination) khi hiển thị Danh sách hành khách và Danh sách vé để giao diện luôn gọn gàng và dễ theo dõi.
        - Kẻ bảng biểu hiển thị thông tin chuyến bay và sơ đồ chỗ ngồi, đảm bảo căn lề chuẩn theo các mẫu kết xuất đã quy định tại `README.md`.

---

