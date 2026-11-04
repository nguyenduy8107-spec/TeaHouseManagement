# KẾT QUẢ KIỂM THỬ THỰC TẾ

Phiên bản kiểm thử: Tea House Management System – C++17 (bản hoàn thiện theo đề bài)

Các kiểm tra đã chạy trong môi trường biên dịch G++:

1. Biên dịch toàn bộ `main.cpp` và `TeaHouse.cpp` với `-std=c++17 -Wall -Wextra -pedantic`: thành công, không có cảnh báo từ compiler.
2. Khởi động chương trình, vào Guest và xem danh sách trà: hoạt động; hiển thị các dòng trà theo ấm và các quy cách 50g, 100g, 200g, 357g, 1kg.
3. Tạo đơn mua 2 ấm Ô Long: tổng tiền 300.000 VND; tồn kho P001 giảm từ 30 xuống 28.
4. Mở hóa đơn: hiển thị đúng tên sản phẩm, số lượng, đơn giá tại thời điểm bán và tổng tiền.
5. Thay đổi tên/giá sản phẩm sau khi bán: hóa đơn cũ vẫn giữ tên và đơn giá đã lưu trong đơn hàng.
6. Đặt phòng R001 trong khung 09:00–10:30: chương trình tạo booking; tính tròn lên 2 giờ, tiền dự kiến 1.000.000 VND.
7. Thử đặt trùng phòng R001 trong khung 10:00–11:00 cùng ngày: chương trình từ chối vì trùng lịch.

Các trường hợp khác trong `TEST_CASES.md` vẫn cần nhóm chạy trên máy cá nhân trước khi nộp.
