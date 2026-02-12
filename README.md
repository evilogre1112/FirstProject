Chào bạn, một sự chuyển đổi từ "Quản lý sinh viên" sang "Quản lý chuyến bay" rất thú vị! Đây là một bài toán kinh điển trong môn CTDL & GT tại PTIT, yêu cầu khả năng xử lý con trỏ và cây nhị phân thực thụ hơn.

Dưới đây là nội dung đồ án đã được cập nhật và trình bày lại theo đúng "văn phong" chuyên nghiệp để bạn đưa vào file `README.md` hoặc tài liệu nhóm:

---

# ✈️ Đồ án CTDL & GT: Quản lý các chuyến bay nội địa

Dự án quản lý vận hành hàng không, tập trung vào tối ưu hóa việc lưu trữ thông tin máy bay, điều phối chuyến bay và quản lý đặt vé của hành khách thông qua các cấu trúc dữ liệu thuần túy.

---

### 🛠 Cấu trúc dữ liệu sử dụng

| Đối tượng | Cấu trúc dữ liệu | Chi tiết thông tin |
| --- | --- | --- |
| **Máy bay** | `Mảng con trỏ` | Tối đa 300 máy bay. Số hiệu MB (C15 - Duy nhất), Loại MB (C40), Số chỗ (≥ 20). |
| **Chuyến bay** | `Danh sách liên kết đơn` | Mã CB (C15 - Duy nhất), Ngày giờ, Sân bay đến, Trạng thái (0-3), Số hiệu MB, Danh sách vé. (Luôn tăng dần theo Mã CB). |
| **Hành khách** | `Cây Nhị phân Tìm kiếm (BST)` | Số CMND (Duy nhất), Họ, Tên, Phái. |
| **Danh sách Vé** | `Mảng/Dãy số` | Số vé (1 đến Số chỗ), Số CMND hành khách đã đặt. |

> **Trạng thái chuyến bay:** 0: Hủy chuyến | 1: Còn vé | 2: Hết vé | 3: Hoàn tất.

---

### 🚀 Danh sách chức năng chính

* [ ] **a. Quản lý Máy bay:** Cập nhật  danh sách máy bay.
* [ ] **b. Quản lý Chuyến bay:** * Lập chuyến bay mới.
* Hiệu chỉnh ngày giờ khởi hành.
* Hủy chuyến.


* [ ] **c. Đặt vé:** * Kiểm tra CMND: Nếu mới thì cập nhật vào **Cây BST**, nếu có rồi thì in thông tin kiểm tra.
* Mỗi khách chỉ được mua 1 vé/chuyến.


* [ ] **d. Hủy vé:** Cho phép hành khách hủy vé đã đặt trên chuyến bay.
* [ ] **e. In danh sách hành khách:** Dựa vào mã chuyến bay, in danh sách theo mẫu kết xuất.
* [ ] **f. Tra cứu chuyến bay:** Tìm chuyến bay theo ngày và nơi đến còn vé. In rõ số lượng slot trống và giờ bay.
* [ ] **g. In danh sách vé trống:** Liệt kê các số vé chưa có người đặt của một chuyến bay X.
* [ ] **h. Thống kê lượt bay:** In danh sách máy bay và số lượt thực hiện chuyến bay, sắp xếp **giảm dần** theo số lượt.

---

### 📄 Mẫu Kết xuất Dữ liệu (Reports)

#### **1. Danh sách hành khách thuộc chuyến bay**

<h3 align="center">
DANH SÁCH HÀNH KHÁCH THUỘC CHUYẾN BAY: [Mã CB] 

<i>Ngày giờ: dd/mm/yyyy hh:mm - Nơi đến: [Sân bay]</i>
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