#ifndef ROOM_H_INCLUDED
#define ROOM_H_INCLUDED

#define len sizeof(struct room)
#define MAX 10000

enum R_type{    //房型
    Big = 0,
    Single = 1,
    Double = 2,
    vip_s = 3,
    vip_d = 4,
    multi = 5,
    time_room = 6
};
struct room
{
    int id;  //房间编号
    int price;  //价格
    enum R_type type; //房间类型
    int money;  //累计收入
    int time;      //累计预订次数
    int q_time; //累计取消预订次数
    int wifi;   //wifi有无
    int breakfast;  //早餐有无
    int wc; //独卫有无
    char occupy[370]; //预订天数
    int floor;  //位置
    struct room *next;
};

/*int n=0,num,back,ppp=0;
int f[1000]={0};
char password[50];*/

#endif // ROOM_H_INCLUDED
