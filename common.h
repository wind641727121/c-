#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

void gst_menu();
void show_gst_menu();
void select4_2();
void select4_1();
void setting();
void Rgrade();
void Rsucceed();
void Rpass();
void RoomMessage(struct room *head);
struct room * select1_5(struct room *head);
struct room * select1_4(struct room *head);
struct room * select1_4_1(struct room *head);
struct room * select1_4_2(struct room *head);
struct room * select1_4_3(struct room *head);
struct room * select1_3(struct room *head);
struct room * select1_2(struct room *head);
struct room * select1_1(struct room *head);
struct room * select1_1_2(struct room *head);
struct room * select1_1_1(struct room *head);
int judge(int min,int max);
struct room * creat();
int serch (struct room *head,int choice);
struct room * find_tail (struct room *head);
struct room * creat_room(struct room *head,int type,int price,int wifi ,int breakfast,int wc,int floor);
int save(struct room * head);
struct room * del_room (struct room * head,int id);
int All_chage(struct room *head,int rate);
int Type_chage(struct room *head,int type,int rate);
int  chage(struct room *head,int id,int op,int price,int type);
struct room * swtich (struct room *head,struct room *j1,struct room *j2);
struct room * choose(struct room * head,int op,int min,int max);
int c_day(int mon ,int day);
int c_date (int date,char* cday);
struct room * find_pointer(struct room *head,int id);
int giveup_book_room(struct room *head,int id,char in [],char out []);
int book_room(struct room *head,int id,char in [],char out []);
int time_book_room(struct room *head,int id,char in []);
int time_giveup_book_room(struct room *head,int id,char in []);
struct room * sort(struct room *head,int op);
struct room * find_freeroom (struct room * head,char in[],char out[]);
int numtoc(int n,char a[]);
extern char *get_time(char tim[20]);
int save_his( struct room *head);
int search_his(char a[50][50]);

#endif // COMMON_H_INCLUDED
