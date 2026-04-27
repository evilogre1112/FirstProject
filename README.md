# ✈️ Đồ án CTDL & GT: Quản lý các chuyến bay nội địa

Dự án quản lý vận hành hàng không, tập trung vào tối ưu hóa việc lưu trữ thông tin máy bay, điều phối chuyến bay và quản lý đặt vé của hành khách thông qua các cấu trúc dữ liệu thuần túy.

---

### 🛠 Cấu trúc dữ liệu sử dụng

| Đối tượng | Cấu trúc dữ liệu | Chi tiết thông tin |
| --- | --- | --- |
| **Máy bay (MB)** | `Mảng con trỏ` | Tối đa 300 máy bay. Số hiệu MB (C15 - Duy nhất), Loại MB (C40), Số chỗ (≥ 20). |
| **Chuyến bay (CB)** | `Danh sách liên kết đơn` | Mã CB (C15 - Duy nhất), Ngày giờ khởi hành, Sân bay đến, Trạng thái (0-3), Số hiệu MB, Danh sách vé cho biết thông tin vé trên chuyến bay. (Luôn tăng dần theo Mã CB). |
| **Hành khách (HK)** | `Cây Nhị phân Tìm kiếm (BST)` | Số CMND (Duy nhất), Họ, Tên, Phái. |
| **Danh sách Vé (DSV)** | `Mảng/Dãy số` | Số vé (1 đến Số chỗ), Số CMND hành khách đã đặt. |

> **Trạng thái chuyến bay:** 0: Hủy chuyến | 1: Còn vé | 2: Hết vé | 3: Hoàn tất.

---

### 🚀 Danh sách chức năng chính

* [ ] **a. Quản lý Máy bay:** Cập nhật  danh sách máy bay.  $\color{cyan}{\text{(Thêm/Xóa/Hiệu chỉnh)}}$ (95%)
* [ ] **b. Quản lý Chuyến bay:** 
    * Lập chuyến bay mới. (Đã Hoàn Thành)
    * Hiệu chỉnh ngày giờ khởi hành.
    * Hủy chuyến.
    * $\color{Red}{\text{DSCB luôn có thứ tự tăng dần theo mã chuyến bay}}$ ( Đã Hoàn Thành)


* [ ] **c. Đặt vé:** 
    * Kiểm tra CMND: Nếu mới thì cập nhật vào **Cây BST**(Danh sách khách hàng), nếu có rồi thì in thông tin kiểm tra.
    * Mỗi khách chỉ được mua 1 vé/chuyến.
    * nếu hành khách chưa có số CMND thì yêu cầu nhập thông tin hành khách trước


* [ ] **d. Hủy vé:** Cho phép hành khách hủy vé đã đặt trên chuyến bay.
* [ ] **e. In danh sách hành khách:** Dựa vào mã chuyến bay, in danh sách theo mẫu kết xuất.
* [ ] **f. Tra cứu chuyến bay:** Tìm chuyến bay theo ngày `dd/mm/yyyy`  và nơi đến `XXXXX` mà còn vé. `cho biết cụ thể số lượng slot trống và giờ bay.`
* [ ] **g. In danh sách vé trống:** Liệt kê các số vé chưa có người đặt của một chuyến bay X.
* [ ] **h. Thống kê lượt bay:** In danh sách máy bay và số lượt thực hiện chuyến bay, sắp xếp **giảm dần** theo số lượt.(Đã hoàn thành)

---

### 📄 Mẫu Kết xuất Dữ liệu (Reports)

#### **1. Danh sách hành khách thuộc chuyến bay**

<h3 align="center">
DANH SÁCH HÀNH KHÁCH THUỘC CHUYẾN BAY:  ###### [Mã CB] 

<i>Ngày giờ: dd/mm/yyyy hh:mm - Nơi đến: xxxxxxxxxxx </i>
</h3>

<div align = "center">

| STT | SỐ VÉ | SỐ CMND | HỌ TÊN | PHÁI |
| --- | --- | --- | --- | --- |
| 1 | 15 | 001099XXXXXX | Nguyễn Văn An | Nam |

</div>

#### **2. Thống kê lượt thực hiện chuyến bay**

<div align = "center">

| Số hiệu máy bay | Số lượt thực hiện |
| --- | --- |
| VN-A123 | 50 |
| VN-A456 | 42 |

</div>

---

### 📖 Bảng chú thích thuật ngữ

Để đảm bảo tính thống nhất trong quá trình phát triển giữa các thành viên, toàn bộ dự án tuân thủ các ký hiệu và quy định sau:

| Ký hiệu | Ý nghĩa đầy đủ | Ghi chú kỹ thuật |
| :--- | :--- | :--- |
| **CTDL & GT** | Cấu trúc dữ liệu và Giải thuật | Nền tảng chính của đồ án (tuyệt đối không dùng thư viện STL). |
| **BST** | Binary Search Tree | Cây nhị phân tìm kiếm, dùng để quản lý danh sách hành khách. |
| **DSV** | Danh sách vé | Gồm các vé, mỗi vé có số ghế và cmnd |
| **MB** | Máy bay | Đối tượng lưu trữ trong mảng con trỏ (tối đa 300). |
| **CB** | Chuyến bay | Đối tượng lưu trữ trong danh sách liên kết đơn. |
| **DSCB** | Danh sách chuyến bay | Luôn được sắp xếp tăng dần theo mã khi thêm mới (Insert). |
| **CMND** | Chứng minh nhân dân | Khóa chính (Key) kiểu số hoặc chuỗi để định danh hành khách. |
| **C15, C40** | Chuỗi ký tự (String/Char) | Ví dụ: `C15` tương đương `char[16]` (đã bao gồm ký tự `\0`). |
| **STT** | Số thứ tự | Chỉ số hiển thị trên giao diện (không lưu trong file). |
| **Trạng thái** | 0, 1, 2, 3 | Các mã số đại diện cho tình trạng vận hành của chuyến bay. |
| **dd/mm/yyyy** | Ngày/Tháng/Năm | Định dạng chuẩn khi nhập liệu và hiển thị ngày khởi hành. |
| **hh:mm** | Giờ:Phút | Định dạng chuẩn cho thời gian cất cánh. |

---

[Bảng phân chia Khối lượng Công việc](./WORK_DISTRIBUTION/WORK_DISTRIBUTION.md)
