#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// 1. KHAI BÁO CẤU TRÚC DỮ LIỆU 

// 1.1 khởi tạo danh sách liên kết đơn sách
// 1.1.a thông tin sách 
struct Book_info{
    char id_book[20];       // Mã sách
    char name_book[100];    // Tên sách
    char author[50];        // Tác giả
    char publisher[50];     // Nhà xuất bản
    int year_publish;       // Năm xuất bản
    int quantity;           // Số lượng còn trong kho 
    int borrow_count;       // Tổng số lượt được mượn 
};
//1.1.b danh sách liên kết đơn Sách
struct Book{
    Book_info data;
    struct Book *next;
};

// 1.2 khởi tạo danh sách liên kết đơn độc giả
// 1.2.a thông tin độc giả 
struct Reader_info{
    char id_reader[20];     // Mã độc giả 
    char name_reader[50];   // Họ tên độc giả
    char class_name[20];    // Lớp 
    char phone[15];         // Số điện thoại 
    int current_borrowed;   // Số sách đang mượn 
};
//1.2.b danh sách liên kết đơn độc giả
struct Reader{
    Reader_info data;
    struct Reader *next;
};
typedef struct Book_info Book_info;
typedef struct Reader_info Reader_info;
typedef struct Book Book;
typedef struct Reader Reader;

// 1.3 khợi tạo cấu trúc mượn trả
struct BorrowData{
    char id_reader[20];     // Mã độc giả mượn sách
    char id_book[20];       // Mã sách được mượn 
    char borrow_date[15];   // Ngày mượn 
    char return_date[15];   // Ngày trả
    int status;             // Trạng thái: 1 = Đang mượn, 0 = Đã trả
};
struct Borrow{
    BorrowData data;
    struct Borrow *next;
};
typedef struct BorrowData BorrowData;
typedef struct Borrow Borrow;

