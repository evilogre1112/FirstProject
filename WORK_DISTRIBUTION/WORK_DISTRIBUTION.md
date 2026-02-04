## 📑KHỐI LƯỢNG CÔNG VIỆC

$\color{Yellow}{\text{CẤU TRÚC (STRUCTURE VÀ DATA)}}$
- **Nhiệm vụ trọng tâm:** Xây dựng nền móng dữ liệu cho toàn bộ dự án.
- **Chi tiết công việc:**
    -	$\color{Cyan}{\text{Định nghĩa Struct:}}$ Thiết lập toàn bộ các cấu trúc dữ liệu: Cây AVL, Danh sách liên kết đơn, Mảng con trỏ theo đúng yêu cầu bảng dữ liệu.
    $\color{Gray}{\text{(Bảng ở README.md)}}$
    - $\color{Cyan}{\text{File I/O:}}$ Viết các hàm nạp và lưu dữ liệu từ file để đảm bảo dữ liệu không bị mất khi thoát chương trình.
    - $\color{Cyan}{\text{Hàm tiện ích:}}$ Xử lý chuỗi, tính độ dài tiếng Việt có dấu $\color{Red}{\text{(quan trọng để kẻ bảng không bị vỡ)}}$. các hàm phục vụ cho dự án khi người làm Logic hoặc Giao diện yêu cầu.
    - $\color{Cyan}{\text{Data Collector:}}$ Thu thập dữ liệu môn học/sinh viên thật từ website PTIT để làm dữ liệu mẫu.
 ---
$\color{Yellow}{\text{II.LOGIC:}}$
- **Nhiệm vụ trọng tâm:** Xử lý thuật toán và các thao tác dữ liệu phức tạp.
- **Chi tiết công việc:**
    - $\color{Cyan}{\text{CRUD:}}$ Cài đặt các chức năng Thêm/Xóa/Hiệu chỉnh cho tất cả các loại danh sách.
        - $\color{Gray}{\text{Ví dụ:}}$ các cấu trúc lớp tín chỉ , lớp sv v..v..
    - $\color{Cyan}{\text{Dữ liệu thô (Raw Data):}}$ Cung cấp kết quả tính toán/sắp xếp cho các câu a, f, g, h, i dưới dạng mảng con trỏ để UI chỉ việc hiển thị.
    - $\color{Cyan}{\text{Cấu trúc đặc biệt:}}$ Xử lý logic "Danh sách trong Danh sách" (Lớp tín chỉ chứa con trỏ đến danh sách đăng ký).
 ---
$\color{Yellow}{\text{III. GIAO DIỆN VÀ QUY ƯỚC (UI AND CONTRACT)}}$
- **Nhiệm vụ trọng tâm:** Thiết kế trải nghiệm người dùng và điều phối dự án.
- **Chi tiết công việc:**
    - $\color{Cyan}{\text{Hợp đồng dữ liệu (Convention):}}$ Quy định tên hàm, kiểu trả về và mã lỗi để Người làm Logic và Struct tuân theo.
    - $\color{Cyan}{\text{Triển khai UI:}}$ Làm giao diện cho toàn bộ 11 chức năng từ a đến k dựa trên dữ liệu thô nhận được.
    - $\color{Cyan}{\text{Xử lý sự kiện:}}$ Bắt phím điều hướng, phân trang danh sách, kẻ bảng biểu, menu động v..v..
