<div align="center">

<img src="https://capsule-render.vercel.app/api?type=waving&color=0:0ea5e9,100:1e3a8a&height=220&section=header&text=QLDSCB&fontSize=72&fontColor=ffffff&animation=fadeIn&fontAlignY=38" alt="QLDSCB banner" width="100%"/>

# ✈️ Quản Lý Chuyến Bay Nội Địa

**Điều phối chuyến bay, đặt vé và quản lý hành khách — ngay trên màn hình Console.**
Giao diện trực quan, thao tác hoàn toàn bằng bàn phím, xây dựng từ cấu trúc dữ liệu thuần túy.

<br/>

![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![Console UI](https://img.shields.io/badge/Console-UI-0ea5e9?style=for-the-badge&logo=windowsterminal&logoColor=white)
![Windows](https://img.shields.io/badge/Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white)
![No STL](https://img.shields.io/badge/No-STL-ef4444?style=for-the-badge)
![Data Structures](https://img.shields.io/badge/CTDL%20%26%20GT-1e3a8a?style=for-the-badge)

<br/>

<img src="./docs/screenshots/ve-trong-so-do-ghe.png" alt="Sơ đồ ghế và thông tin khách hàng" width="90%"/>

<sub>Sơ đồ ghế trực quan — chọn ghế, xem ngay thông tin chuyến bay và hành khách.</sub>

</div>

---

## 🌟 Điểm nổi bật

<table>
<tr>
<td width="33%" valign="top">

### 🎨 Giao diện đẹp ngay trong Console
Khung viền, màu sắc trạng thái, ảnh ASCII art và lịch tháng — một ứng dụng terminal nhưng không hề "khô khan".

</td>
<td width="33%" valign="top">

### ⌨️ Điều khiển bằng phím
Di chuyển bằng mũi tên, xác nhận bằng `ENTER`, thoát bằng `ESC`. Không cần chuột, thao tác nhanh và mượt.

</td>
<td width="33%" valign="top">

### 🧠 Xây dựng từ con số 0
Không dùng STL. Toàn bộ dữ liệu được quản lý bằng mảng con trỏ, danh sách liên kết, cây BST do nhóm tự cài đặt.

</td>
</tr>
</table>

---

## 📸 Giao diện sản phẩm

<table>
<tr>
<td width="50%" align="center">
<img src="./docs/screenshots/lap-chuyen-bay.png" alt="Lập chuyến bay mới"/>
<br/><b>🛫 Lập chuyến bay mới</b>
<br/><sub>Chọn ngày giờ bằng lịch tháng, chọn máy bay từ danh sách.</sub>
</td>
<td width="50%" align="center">
<img src="./docs/screenshots/tim-chuyen-bay.png" alt="Tìm chuyến bay theo ngày và sân bay"/>
<br/><b>🔎 Tra cứu chuyến bay</b>
<br/><sub>Lọc theo ngày khởi hành và sân bay đến, hiển thị số vé trống.</sub>
</td>
</tr>
<tr>
<td width="50%" align="center">
<img src="./docs/screenshots/thong-so-may-bay.png" alt="Thông số kỹ thuật máy bay"/>
<br/><b>🛩️ Quản lý máy bay</b>
<br/><sub>Xem thông số: mã, loại máy bay, số ghế tối đa.</sub>
</td>
<td width="50%" align="center">
<img src="./docs/screenshots/huy-ve.png" alt="Menu hủy vé"/>
<br/><b>🎫 Hủy vé linh hoạt</b>
<br/><sub>Hủy theo chuyến bay hoặc theo hành khách.</sub>
</td>
</tr>
</table>

---

## ✨ Bạn có thể làm gì với QLDSCB?

| | Tính năng | Mô tả |
| :---: | :--- | :--- |
| 🛩️ | **Quản lý máy bay** | Thêm, xóa, hiệu chỉnh thông tin máy bay (tối đa 300 máy bay). |
| 🛫 | **Quản lý chuyến bay** | Lập chuyến bay mới, đổi ngày giờ khởi hành, hủy chuyến. Danh sách luôn được sắp xếp theo mã chuyến bay. |
| 🎟️ | **Đặt vé** | Kiểm tra CMND, tự động thêm hành khách mới, mỗi khách chỉ mua một vé trên mỗi chuyến. |
| ❌ | **Hủy vé** | Hủy vé theo chuyến bay hoặc theo hành khách. |
| 👥 | **Danh sách hành khách** | In danh sách hành khách của một chuyến bay theo mẫu có sẵn. |
| 🔎 | **Tra cứu chuyến bay** | Tìm theo ngày `dd/mm/yyyy` và nơi đến, cho biết giờ bay và số chỗ còn trống. |
| 💺 | **Xem vé trống** | Sơ đồ ghế của chuyến bay, ghế nào còn trống nhìn là thấy. |
| 📊 | **Thống kê lượt bay** | Danh sách máy bay xếp hạng giảm dần theo số lượt thực hiện chuyến bay. |

### 🚦 Trạng thái chuyến bay nhìn là hiểu

| Màu | Ý nghĩa |
| :---: | :--- |
| 🟥 | Chuyến bay bị hủy |
| 🟩 | Chuyến bay còn vé |
| 🟨 | Chuyến bay hết vé |
| 🟦 | Chuyến bay hoàn tất |

---

## 🧩 Bên trong ứng dụng

Mỗi đối tượng được chọn cấu trúc dữ liệu phù hợp nhất với cách nó được sử dụng:

| Đối tượng | Cấu trúc dữ liệu | Vì sao chọn? |
| :--- | :--- | :--- |
| 🛩️ **Máy bay** | Mảng con trỏ | Truy cập nhanh theo chỉ số, tối đa 300 máy bay. |
| 🛫 **Chuyến bay** | Danh sách liên kết đơn | Chèn chuyến mới mà vẫn giữ thứ tự theo mã chuyến bay. |
| 🧑 **Hành khách** | Cây nhị phân tìm kiếm (BST) | Tìm và kiểm tra CMND trùng lặp nhanh. |
| 🎫 **Vé** | Mảng | Mỗi vé tương ứng một ghế, lưu CMND người đặt. |

---

## ⌨️ Phím điều khiển nhanh

| Phím | Chức năng |
| :---: | :--- |
| `←` `→` `↑` `↓` | Di chuyển giữa các mục, ghế hoặc trang |
| `ENTER` | Xác nhận và tiếp tục |
| `ESC` | Hủy hoặc thoát màn hình hiện tại |
| `D` / `A` | Chuyển sang tháng sau / tháng trước khi chọn ngày |
| `Backspace` | Bỏ qua bước chọn ngày |

> 💡 **Lưu ý:** hãy **tắt Unikey** khi sử dụng chương trình để tránh lỗi nhập liệu.

---

## 🚀 Chạy thử

```bash
git clone https://github.com/evilogre1112/FirstProject.git
```

1. Mở project bằng IDE hỗ trợ C++ (Dev-C++, Visual Studio, Code::Blocks, ...).
2. Build và chạy chương trình.
3. Nên dùng **Windows Terminal** và phóng to cửa sổ để hiển thị đầy đủ giao diện.

---

## 👥 Nhóm thực hiện

Đồ án môn **Cấu trúc dữ liệu & Giải thuật**.
Xem chi tiết phần việc của từng thành viên tại [Bảng phân chia khối lượng công việc](./WORK_DISTRIBUTION/WORK_DISTRIBUTION.md).

<div align="center">

<br/>

⭐ Nếu thấy dự án thú vị, hãy tặng nhóm một ngôi sao nhé!

<img src="https://capsule-render.vercel.app/api?type=waving&color=0:0ea5e9,100:1e3a8a&height=100&section=footer" alt="footer" width="100%"/>

</div>
