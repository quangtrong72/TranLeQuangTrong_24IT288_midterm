# Implementing a Simplified ls(1)

## 1. Thông tin sinh viên

- Họ và tên: Trần Lê Quang Trọng
- Mã sinh viên: 24IT288
- Lớp: 24SE2
- Tên đề tài: Implementing a Simplified ls(1)
- Ngôn ngữ lập trình: C
- Môi trường phát triển: Ubuntu Linux
- Công cụ biên dịch: GCC
- Công cụ quản lý mã nguồn: Git và GitHub

## 2. Giới thiệu đề tài

Đề tài xây dựng chương trình `myls` bằng ngôn ngữ C nhằm mô phỏng
một số chức năng của lệnh `ls(1)` trên hệ điều hành Linux.

Chương trình được tổ chức thành nhiều file mã nguồn để dễ quản lý,
kiểm tra, bảo trì và phát triển.

## 3. Mục tiêu

- Tìm hiểu cách liệt kê tệp và thư mục trên Linux.
- Làm quen với lập trình hệ thống bằng ngôn ngữ C.
- Sử dụng các thư viện và lời gọi hệ thống để truy xuất thông tin tệp.
- Tổ chức chương trình thành nhiều file `.c` và `.h`.
- Sử dụng Makefile để tự động hóa quá trình biên dịch.
- Sử dụng Git và GitHub để quản lý mã nguồn.

## 4. Công nghệ sử dụng

- C: Ngôn ngữ lập trình chính.
- GCC: Biên dịch mã nguồn C.
- Make: Tự động hóa quá trình biên dịch.
- Ubuntu Linux: Môi trường phát triển và kiểm thử.
- Git: Quản lý phiên bản mã nguồn.
- GitHub: Lưu trữ và chia sẻ dự án.

## 5. Cấu trúc dự án

    TranLeQuangTrong_24IT288_midterm/
    ├── include/
    │   ├── list.h
    │   └── options.h
    ├── src/
    │   ├── main.c
    │   ├── options.c
    │   └── list.c
    ├── Makefile
    ├── README.md
    └── .gitignore

### Chức năng các thành phần

- `src/main.c`: Điểm bắt đầu của chương trình.
- `src/options.c`: Xử lý các tùy chọn của chương trình.
- `src/list.c`: Xử lý việc liệt kê tệp và thư mục.
- `include/options.h`: Khai báo các thành phần liên quan đến tùy chọn.
- `include/list.h`: Khai báo các hàm liệt kê tệp và thư mục.
- `Makefile`: Tự động hóa quá trình biên dịch.
- `.gitignore`: Loại trừ các file không cần thiết khỏi Git.

## 6. Biên dịch chương trình

Mở Terminal tại thư mục dự án và chạy:

    make

Để xóa các file biên dịch cũ:

    make clean

Sau khi biên dịch thành công, chương trình thực thi có tên `myls`
được tạo trong thư mục dự án.

## 7. Cách sử dụng

Chạy chương trình trong thư mục hiện tại:

    ./myls

Liệt kê các mục bao gồm tệp ẩn:

    ./myls -a

Hiển thị thông tin chi tiết:

    ./myls -l

Hiển thị số inode:

    ./myls -i

Hiển thị ký hiệu loại tệp:

    ./myls -F

Liệt kê nội dung thư mục `/tmp`:

    ./myls /tmp

Kiểm tra đường dẫn không tồn tại:

    ./myls /abcxyz

Lưu ý: Kết quả thực tế của từng tùy chọn phụ thuộc vào phần mã nguồn
đã triển khai.

## 8. Kiểm thử

Quá trình kiểm thử được thực hiện trên Ubuntu Linux.

Các nội dung kiểm thử bao gồm:

- Biên dịch chương trình bằng Makefile.
- Liệt kê tệp và thư mục.
- Kiểm tra tùy chọn hiển thị tệp ẩn.
- Kiểm tra chế độ hiển thị chi tiết.
- Kiểm tra hiển thị số inode và ký hiệu loại tệp.
- Liệt kê một thư mục được chỉ định.
- Kiểm tra trường hợp đường dẫn không tồn tại.

Kết quả: Chương trình biên dịch thành công và các lệnh kiểm tra cơ bản
đã được thực hiện.

## 9. Kết luận

Qua đề tài, sinh viên có cơ hội tìm hiểu cách làm việc với tệp và thư mục
trong Linux, thực hành lập trình C, tổ chức mã nguồn thành nhiều thành phần
và sử dụng Makefile để biên dịch chương trình.

Đề tài cũng giúp rèn luyện kỹ năng kiểm thử chương trình và quản lý
mã nguồn bằng Git và GitHub.

## 10. GitHub

Repository:
https://github.com/quangtrong72/TranLeQuangTrong_24IT288_midterm
