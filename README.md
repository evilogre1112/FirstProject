# 🎓 Đồ án CTDL & GT: Quản lý điểm sinh viên 

Dự án quản lý dữ liệu học vụ sử dụng các cấu trúc dữ liệu thuần túy trong C++, đảm bảo tối ưu hóa bộ nhớ và tốc độ truy xuất.

---

### 🛠 Cấu trúc dữ liệu sử dụng
| Đối tượng | Cấu trúc dữ liệu | Chi tiết thông tin |
| :--- | :--- | :--- |
| **Danh sách môn học** | `Cây AVL` | MAMH (C10), TENMH (C50), STCLT, STCTH |
| **Danh sách lớp sinh viên** | `Mảng con trỏ` | Tối đa 10.000 lớp, quản lý danh sách sinh viên, mỗi lớp có có thông tin: MALOP(C15), TENLOP, con trỏ. |
| **Danh sách sinh viên** | `Danh sách liên kết đơn` | MASV (C15), HO, TEN, PHAI(Nam\Nu), SODT, Email |
| **Danh sách lớp Tín chỉ** | `Danh sách liên kết đơn` | MALOPTC (`Số nguyên tự động tăng`), MAMH, Niên khóa, Học kỳ, Nhóm, số sv min, số sv max, huỷ lớp (`Boolean`), con trỏ dssvdk |
| **danh sách đăng ký** | `Danh sách liên kết đơn` | MASV, DIEM (số thực), huỷ đăng ký (`bool`) |

---

### 🚀 Danh sách chức năng chính
- [ ] **a.** Mở lớp tín chỉ : cho phép cập nhật $\color{cyan}{\text{(Thêm/Xóa/Hiệu chỉnh)}}$ thông tin của lớp tính chỉ.
- [ ] **b.** In danh sách SV đã đăng ký lớp tín chỉ theo tham số: 
    + $\color{Gray}{\text{Niên khoá}}$
    + $\color{Gray}{\text{Học kỳ}}$
    + $\color{Gray}{\text{Nhóm}}$
    + $\color{Gray}{\text{Mã môn học}}$
- [ ] **c.** Nhập lớp và sinh viên:
    + $\color{Gray}{\text{Cập nhật sinh viên của 1 lớp}}$
       + $\color{Gray}{\text{Nhập mã lớp trước, sau đó nhập các sinh viên vào lớp đó}}$ <br>
    + $\color{Yellow}{\text{Lưu ý:}}$ Khi nhập phải đủ 3 chức năng $\color{cyan}{\text{(Thêm/Xóa/Hiệu chỉnh)}}$. Quá trình nhập sinh viên sẽ dừng khi ta nhập vào mã sv là chuỗi rỗng. 
- [ ] **d.** In DSSV của một lớp (`Dựa vào mã lớp`) theo`Thứ tự từ điển` $\color{green}{\text{(Tên + Họ)}}$
- [ ] **e.** Nhập môn học : cho phép `cập nhật` $\color{cyan}{\text{(Thêm/Xóa/Hiệu chỉnh)}}$ thông tin môn học.
- [ ] **f.** In danh môn học: `tăng dần theo tên`.
- [ ] **g.** Đăng ký lớp tín chỉ cho sinh viên:
    + $\color{Gray}{\text{User nhập vào mã sinh viên của mình, chương trình tự động in ra thông tin}}$ <br> $\color{Gray}{\text{sinh viên}}$
    + $\color{Gray}{\text{Kế tiếp, user nhập vào niên khoá, học kỳ, chương trình sẽ tự động lọc ra các}}$ <br> $\color{Gray}{\text{lớp tính chỉ đã mở trong niên khoá, học kỳ đó sẽ được sinh viên đăng kí}}$ 
    + $\color{Gray}{\text{Dữ liệu in ra gồm:}}$ 
      + $\color{Gray}{\text{MAMH}}$
      + $\color{Gray}{\text{TENMH}}$
      + $\color{Gray}{\text{NHOM}}$
      + $\color{Gray}{\text{Số sinh viên đã đăng ký}}$
      + $\color{Gray}{\text{Số slot còn trống}}$ 
- [ ] **h.** Huỷ lớp tính chỉ: Tự động hủy lớp tín chỉ nếu không đủ số SV tối thiểu trong 1 niên khoá học kì.
   + $\color{Yellow}{\text{Lưu ý:}}$ Trước khi huỷ cần sự xác nhận của user.
- [ ] **i.** Nhập điểm :
   + $\color{Gray}{\text{User nhập vào niên khoá, học kỳ, nhóm, môn học}}$ <br> $\color{Cyan}{\text{Căn cứ vào giá trị này, chương trình sẽ tự động lọc ra danh sách sinh viên đã}}$ <br> $\color{Cyan}{\text{đăng ký, và in ra màn hình danh sách theo dạng sau:}}$
   $\color{Red}{\text{Chưa kịp chụp cái này hehe :)) }}$
- [ ] **j.** In bảng điểm môn học của 1 lớp tín chỉ:
   + $\color{Gray}{\text{User nhập vào niên khoá, học kỳ, nhóm, môn học, chương trình sẽ in ra bản điểm đã thi.}}$ 
   + $\color{Gray}{\text{Kết quả trên màn hình:}}$ <br>

<h3 align="center">
  BẢNG ĐIỂM MÔN HỌC &lt;tenmon&gt; <br>
  <i>Niên khoá: ... &nbsp; Học kỳ: ... &nbsp; Nhóm: ...</i>
</h3>
<div align="center">

| STT | MASV | HO | TEN | DIEM |
| :---: | :---: | :---: | :---: | :---:|
| 99 | XXXX | XXXXXXXXXXX | XXXXX | 99 |
| 99 | XXXX | XXXXXXXXXXX | XXXXX | 99 |

</div>

- [ ] **k.** In bảng điểm trung bình kết thúc khóa học của 1 lớp theo số tín chỉ. Kết xuất.

<h3 align="center">
  BẢNG THỐNG KÊ ĐIỂM TRUNG BÌNH KHOÁ HỌC <br>
  <i>Lớp &nbsp; &nbsp; : &lt;malop&gt;</i>
</h3>
<div align="center">

| STT | MASV | HO | TEN | DIEMTB |
| :---: | :---: | :---: | :---: | :---:|
| 99 | XXXX | XXXXXXXXXXX | XXXXX | 99.9 |
| 99 | XXXX | XXXXXXXXXXX | XXXXX | 99.9 |

</div>

---
*Ghi chú: Toàn bộ dữ liệu được lưu trữ và nạp tự động qua file văn bản.*

Hình ảnh minh hoạ ở trong folder picture.
[Bảng phân chia Khối lượng Công việc](./WORK_DISTRIBUTION.md)
