#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// 1. KHAI BÁO CẤU TRÚC DỮ LIỆU 

// 1.1. Cấu trúc Sách
typedef struct Book_info {
    char id_book[20];       // Mã sách
    char name_book[100];    // Tên sách
    char author[50];        // Tác giả
    char publisher[50];     // Nhà xuất bản
    int year_publish;       // Năm xuất bản
    int quantity;           // Số lượng còn trong kho (>0: Còn, 0: Hết)
    int borrow_count;       // Tổng số lượt sách được mượn
} Book_info;

typedef struct Book {
    Book_info data;
    struct Book *next;
} Book;

// 1.2. Cấu trúc Độc giả
typedef struct Reader_info {
    char id_reader[20];     // Mã độc giả
    char name_reader[50];   // Họ tên độc giả
    char class_name[20];    // Lớp
    char phone[15];         // Số điện thoại
    int current_borrowed;   // Số sách đang mượn hiện tại
} Reader_info;

typedef struct Reader {
    Reader_info data;
    struct Reader *next;
} Reader;

// 1.3. Cấu trúc Phiếu Mượn/Trả (Lịch sử mượn)
typedef struct BorrowData {
    char id_reader[20];     // Mã độc giả mượn
    char id_book[20];       // Mã sách mượn
    char borrow_date[15];   // Ngày mượn
    char return_date[15];   // Ngày trả
    int status;             // 1 = Đang mượn, 0 = Đã trả
} BorrowData;

typedef struct Borrow {
    BorrowData data;
    struct Borrow *next;
} Borrow;

// 2. CÁC HÀM THỰC HIỆN CHỨC NĂNG CHƯƠNG TRÌNH 

// 2.1 Các hàm khởi tạo mới 

// 2.1.1 hàm tạo một sách mới 
Book *makeBook(Book_info x){
    Book *newbook = (Book*)malloc(sizeof(Book));
    if(newbook != NULL){
        x.borrow_count = 0; // mới thêm vào thì lượt mượn = 0
        newbook->data = x;
        newbook->next = NULL;
    }
    return newbook;
}

// 2.1.2 hàm tạo một độc giả mới 
Reader *makeReader(Reader_info x){
    Reader *newreader = (Reader*)malloc(sizeof(Reader));
    if(newreader != NULL){
        x.current_borrowed = 0; // mới tạo chưa mượn cuốn nào
        newreader->data = x;
        newreader->next = NULL;
    }
    return newreader;
}

// 2.1.3 hàm tạo phiếu mượn mới 
Borrow *makeBorrow(BorrowData x){
    Borrow *newborrow = (Borrow*)malloc(sizeof(Borrow));
    if(newborrow != NULL){
        x.status = 1; // 1 là trạng thái đang mượn
        strcpy(x.return_date, "Dang muon");
        newborrow->data = x;
        newborrow->next = NULL;
    }
    return newborrow;
}

// 2.2 Các hàm quản lý sách 

// 2.2.1 hàm tìm Sách theo mã sách 
Book *findBook(Book *head, char id_book[]){
    while(head != NULL){
        if(strcmp(head->data.id_book, id_book) == 0) return head; // tìm thấy trả về node sách đó
        head = head->next;
    }
    return NULL; // không tìm thấy
}

// 2.2.2 hàm thêm sách 
int addBook(Book **head, Book_info new_data){
    Book *prev = NULL, *curr = *head;
    while(curr != NULL){
        if(strcmp(curr->data.id_book, new_data.id_book) == 0) return 0; // trùng mã sách 
        prev = curr;
        curr = curr->next;
    }
    Book *newbook = makeBook(new_data);
    if(newbook == NULL) return -1; // lỗi cấp phát bộ nhớ
    if(prev == NULL) *head = newbook; // nếu danh sách trống k có phần tử nào thêm node đầu
    else prev->next = newbook; // còn lại gán node->next của node cuối vào sách mới
    return 1; // đã thêm vào sách thành công 
}

// 2.2.3 hàm xóa sách 
int deleteBook(Book **head, char id_book[]){
    if(*head == NULL) return 0; // danh sách rỗng 
    Book *curr = *head;
    Book *prev = NULL;
    while(curr != NULL && strcmp(curr->data.id_book, id_book) != 0){
        prev = curr;
        curr = curr->next;
        // duyệt tìm cuốn sách có mã trùng với id_book
    }
    if(curr == NULL) return 0; // không tìm thấy cuốn sách đó
    if(prev == NULL) *head = curr->next; // sách cần xóa nằm ở đầu danh sách 
    else prev->next = curr->next; // trường hợp sách cần xóa nằm ở giữa hoặc cuối
    free(curr); // giải phóng bộ nhớ 
    return 1; // xóa thành công
}

// 2.2.4 hàm sửa thông tin sách
int updateBook(Book *head, char id_book[], Book_info updated_data){
    Book *target = findBook(head, id_book);
    if(target == NULL) return 0; // không tìm thấy sách để sửa

    strcpy(updated_data.id_book, target->data.id_book); // giữ nguyên mã sách cũ
    updated_data.borrow_count = target->data.borrow_count; // giữ nguyên số lượt đã mượn
    target->data = updated_data; // cập nhật thông tin mới
    return 1; // sửa thành công
}

// 2.2.5 hàm kiểm tra sách có trong kho không 
int checkBookStock(Book *head, char id_book[]){
    Book *target = findBook(head, id_book);
    if(target == NULL) return -1; // không tìm thấy sách 
    else return target->data.quantity; // nếu tìm thấy trả về số lượng sách 
}

// 2.3 Các hàm quản lý độc giả

// 2.3.1 tìm độc giả 
Reader *findReader(Reader *head, char id_reader[]){
    while(head != NULL){
        if(strcmp(head->data.id_reader, id_reader) == 0) return head; // tìm thấy độc giả
        head = head->next;
    }
    return NULL; // không tìm thấy
}

// 2.3.2 thêm độc giả mới 
int addReader(Reader **head, Reader_info new_data){
    Reader *prev = NULL, *curr = *head;
    while(curr != NULL){
        if(strcmp(curr->data.id_reader, new_data.id_reader) == 0) return 0; // trùng mã độc giả
        prev = curr;
        curr = curr->next;
    }
    Reader *newreader = makeReader(new_data);
    if(newreader == NULL) return -1; // lỗi bộ nhớ 
    if(prev == NULL) *head = newreader; // danh sách rỗng thì thêm vào đầu
    else prev->next = newreader; // nối vào cuối danh sách
    return 1; // thêm thành công
}

// 2.3.3 xóa độc giả
int deleteReader(Reader **head, char id_reader[]){
    if(*head == NULL) return 0; // danh sách rỗng 
    Reader *curr = *head;
    Reader *prev = NULL;
    while(curr != NULL && strcmp(curr->data.id_reader, id_reader) != 0){
        prev = curr;
        curr = curr->next;
        // duyệt tìm độc giả có mã trùng với id_reader
    }
    if(curr == NULL) return 0; // không tìm thấy 
    if(curr->data.current_borrowed > 0) return -1; // độc giả đang mượn sách không được xóa
    if(prev == NULL) *head = curr->next; // độc giả cần xóa nằm ở đầu danh sách 
    else prev->next = curr->next; // trường hợp độc giả cần xóa nằm ở giữa hoặc cuối
    free(curr); // giải phóng bộ nhớ 
    return 1; // xóa thành công
}

// 2.3.4 cập nhật độc giả
int updateReader(Reader *head, char id_reader[], Reader_info updated_data){
    Reader *target = findReader(head, id_reader);
    if(target == NULL) return 0; // không tìm thấy độc giả

    strcpy(updated_data.id_reader, target->data.id_reader); // giữ nguyên mã độc giả
    updated_data.current_borrowed = target->data.current_borrowed; // giữ nguyên số sách đang mượn
    target->data = updated_data; // gán dữ liệu mới
    return 1; // cập nhật thành công
}

// ---2.4. CÁC HÀM MƯỢN VÀ TRẢ SÁCH---

// 2.4.1 Hàm mượn sách
int BorrowBook(Book *Book_head, Reader *reader_head, Borrow **borrow_head, BorrowData b_info){
    Reader *r = findReader(reader_head, b_info.id_reader); // tìm độc giả mượn sách
    if(r == NULL) return -1; // không có độc giả này 
    
    Book *b = findBook(Book_head, b_info.id_book); // tìm cuốn sách cần mượn
    if(b == NULL) return -2; // không có cuốn sách này
    if(b->data.quantity <= 0) return -3; // sách trong kho đã hết

    Borrow *newborrow = makeBorrow(b_info); // tạo phiếu mượn mới
    if(newborrow == NULL) return 0; // lỗi bộ nhớ
    newborrow->next = *borrow_head; // chèn phiếu mượn vào đầu danh sách
    *borrow_head = newborrow;

    b->data.quantity--; // giảm số lượng sách trong kho đi 1
    b->data.borrow_count++; // tăng số lượt mượn của sách lên 1
    r->data.current_borrowed++; // tăng số sách độc giả đang mượn lên 1
    return 1; // mượn sách thành công
}

// 2.4.2 Hàm trả sách
int ReturnBook(Book *Book_head, Reader *reader_head, Borrow *borrow_head, char id_reader[], char id_book[], char return_date[]){
    while(borrow_head != NULL){
        // tìm phiếu mượn khớp mã độc giả, mã sách và đang ở trạng thái mượn (status == 1)
        if(strcmp(borrow_head->data.id_reader, id_reader) == 0 &&
           strcmp(borrow_head->data.id_book, id_book) == 0 &&
           borrow_head->data.status == 1){
            
            borrow_head->data.status = 0; // đổi trạng thái sang đã trả (0)
            strcpy(borrow_head->data.return_date, return_date); // lưu lại ngày trả

            Book *b = findBook(Book_head, id_book);
            if(b != NULL) b->data.quantity++; // trả lại 1 cuốn vào kho sách

            Reader *r = findReader(reader_head, id_reader);
            if(r != NULL && r->data.current_borrowed > 0) r->data.current_borrowed--; // giảm số sách đang mượn đi 1
            return 1; // trả sách thành công
        }
        borrow_head = borrow_head->next;
    }
    return 0; // không tìm thấy phiếu mượn
}