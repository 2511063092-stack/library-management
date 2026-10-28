#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// 1. KHAI BÁO CẤU TRÚC DỮ LIỆU 

// 1.1 khởi tạo danh sách liên kết đơn sách
struct Book{
    char id_book[50], name_book[50], author[50], publisher[50];
    int year_publish, quantity;
    struct Book *next;
};

// 1. 2 khởi tạo danh sách liên kết đơn độc giả
struct Reader{
    char id_reader[50], name_reader[50], class[20];
    int sdt;
    struct Reader *next;
};
typedef struct Book Book;
typedef struct Reader Reader;

// 2. tạo hàm các hàm quản lý quá trình mượn trả
// 2.1 hàm thêm/xóa/sửa sách
// 2.1.1  hàm thêm sách
Book *makeBook(char id_book){
      Book *newbook = (Book*)malloc(sizeof(Book));
      strcpy(newbook->id_book, id_book);
      newbook->next = NULL;
      return newbook;
};
// 2.1.2 hàm xóa sách
// 2.1.2.1 xóa đầu sách 
void popfront(Book **head){
    if(*head == NULL) return;
    Book *tmp = *head;
    *head = (*head)->next;
    free(tmp);
}
// 2.1.2.2 xóa cuối danh sách 
void popback(Book **head){
    if(*head == NULL) return;
    Book *tmp = *head;
    if(tmp->next == NULL){
        *head = NULL;
        free(tmp);
        return;
    }
    while(tmp->next->next == NULL){
        tmp = tmp->next;
    }
    Book *last = tmp->next;
    tmp->next = NULL;
    free(last);
}
// 2.1.2.3 xóa vị trí thứ k 
// a, số phần tử trong danh sách 
int size(Book *head){
    int cnt = 0;
    while(head != NULL){
        cnt++;
        head = head->next;
    }
    return cnt;
}
// b, xóa vị trí thứ k trong danh sách
void erase(Book **head, int k){
    if(k < 1 || k > size(*head)) return;
    if(k == 1) popfront(*head);
    else{
    Book *tmp = *head;
    for(int i = 1; i < k - 1; i++){
        tmp = tmp->next; // tmp = k - 1
    }
    Book *index = tmp->next;
    tmp->next = index->next;
    free(index);
    }
}
