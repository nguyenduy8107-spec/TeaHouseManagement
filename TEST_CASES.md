# TEST CASES – TEA HOUSE MANAGEMENT SYSTEM

## Môi trường
- Compiler: G++ hỗ trợ C++17
- Giao diện: Terminal/PowerShell
- Dữ liệu: các file `.txt` trong thư mục chạy chương trình

## Kiểm thử đã thực hiện
| ID | Chức năng | Dữ liệu/thao tác | Kết quả mong đợi | Kết quả kiểm tra |
|---|---|---|---|---|
| TC01 | Biên dịch | `g++ main.cpp TeaHouse.cpp -std=c++17 -Wall -Wextra -pedantic -o TeaHouse` | Biên dịch thành công | PASS |
| TC02 | Khởi động | Chọn 0 ở menu chính | Thông báo kết thúc, không crash | PASS |
| TC03 | Guest xem danh mục | Guest → View tea products | Hiển thị bốn dòng trà và các quy cách đóng gói | PASS |
| TC04 | Tạo đơn hàng | Customer `customer01`; mua 2 đơn vị P001 | Tạo hóa đơn 300.000 VND | PASS |
| TC05 | Trừ tồn kho | Mua 2 đơn vị P001, tồn kho ban đầu 30 | Tồn kho còn 28 | PASS |
| TC06 | Lưu giá tại thời điểm bán | Tạo đơn P001 với giá 150.000 | File order lưu giá bán trong chi tiết mặt hàng | PASS |
| TC07 | Đặt phòng | R001, ngày 2026-11-05, 09:00–10:30 | Tạo booking; 2 giờ tính phí, 1.000.000 VND dự kiến | PASS |
| TC08 | Chống đặt trùng | R001 cùng ngày, 10:00–11:00 | Từ chối do trùng lịch | PASS |

## Kiểm thử nhóm nên thực hiện thêm trên máy của nhóm
| ID | Chức năng | Thao tác | Kết quả cần xác nhận |
|---|---|---|---|
| TC09 | Đăng nhập sai | Nhập sai mật khẩu | Không cho vào menu vai trò |
| TC10 | Phân quyền Customer | Thử truy cập menu Owner | Customer không có lựa chọn quản trị |
| TC11 | Phòng bảo trì | Đặt phòng có trạng thái Maintenance | Bị từ chối |
| TC12 | Hủy booking | Customer hủy booking của mình | Trạng thái chuyển Cancelled |
| TC13 | Hủy booking người khác | Customer nhập ID booking của tài khoản khác | Bị từ chối |
| TC14 | Tồn kho không đủ | Đặt số lượng lớn hơn tồn kho | Không tạo đơn và không trừ kho |
| TC15 | Dữ liệu sau khởi động lại | Tạo booking/order, thoát rồi mở lại | Dữ liệu vẫn còn trong file |
| TC16 | Báo cáo doanh thu | Hoàn thành booking và tạo order Paid | Tổng hợp đúng hai nguồn doanh thu |
| TC17 | Artist | Artist thêm và xóa lịch của mình | Lịch được lưu/xóa đúng |
| TC18 | Guest chỉ xem | Guest thử mua hàng/đặt phòng | Không có chức năng giao dịch |

## Lưu ý
Các dòng PASS ở trên là những kiểm tra đã chạy bằng phiên bản đóng gói trong môi trường kiểm thử. Các trường hợp TC09–TC18 là checklist để nhóm tự chạy thêm trên máy Windows/VS Code trước khi nộp. Không nên ghi tất cả là PASS nếu nhóm chưa thực sự kiểm tra.
| TC19 | Hóa đơn giữ dữ liệu cũ | Tạo đơn, sau đó đổi tên/giá sản phẩm trong danh mục rồi mở hóa đơn cũ | Hóa đơn vẫn hiển thị tên và đơn giá tại thời điểm bán | PASS |
