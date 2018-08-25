#ifndef ROOM_H_INCLUDED
#define ROOM_H_INCLUDED

#define len sizeof(struct room)
#define MAX 10000

enum R_type{    //����
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
    int id;  //������
    int price;  //�۸�
    enum R_type type; //��������
    int money;  //�ۼ�����
    int time;      //�ۼ�Ԥ������
    int q_time; //�ۼ�ȡ��Ԥ������
    int wifi;   //wifi����
    int breakfast;  //�������
    int wc; //��������
    char occupy[370]; //Ԥ������
    int floor;  //λ��
    struct room *next;
};

/*int n=0,num,back,ppp=0;
int f[1000]={0};
char password[50];*/

#endif // ROOM_H_INCLUDED
