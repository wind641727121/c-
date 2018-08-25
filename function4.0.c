#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>
#include <string.h>
#include "room.h"
//#include "quanjv.h"
//#define len sizeof(struct room)
//#define MAX 10000
struct room * sort(struct room *head,int op);
/*enum R_type{
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
    int id;
    int price;
    enum R_type type;
    int money;
    int time;
    int q_time;
    int wifi;
    int breakfast;
    int wc;
    char occupy[370];
    int floor;
    struct room *next;
};*/
int n=0,num,back,ppp=0;     //n为房间数，num为房间编号,back为背景色，ppp为验证密码
int f[1000]={0};    //房牌号
char password[50];  //密码
int judge(int min,int max) //判断输入 正确返回输入值，错误返回-1 只对数字有效
{
    char c[100];
    gets(c);
    int i,sum=-1;
    for (i=0;c[i]!='\0';i++)
    {
        if (c[i]>'9'||c[i]<'0') return -1;
        if (i==0) sum =0;
        sum = sum*10+c[i]-48;
    }
    if (sum<min||sum>max) return -1;
    return sum;
}
struct room * creat()         //创建链表 记录ID，背景色，密码
{
    int j;
    for (j=0;j<1000;j++)f[j]=0;
    FILE *fb;
    if((fb = fopen("date.dat","r"))==NULL)
    {
        printf("打开文件错误\n");
       exit(0);
    }

    struct room * head;
    struct room * p1,* p2;
    n=0;
    p1=p2=(struct room*)malloc(len);
    fscanf(fb,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%s\n",&p1->id,&p1->price,&p1->type,&p1->money,&p1->time,&p1->wifi,&p1->breakfast,&p1->wc,&p1->floor,&p1->q_time,&p1->occupy);
    head= NULL;
    while (p1->id!=0)
    {
        n++;
        f[p1->floor]=1;
        if (n==1)head = p1;
        else p2->next=p1;
        p2=p1;
        //fprintf(fb,"%d,%d\n",p1->id,p1->time);
        p1=(struct room *)malloc(len);
        fscanf(fb,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%s\n",&p1->id,&p1->price,&p1->type,&p1->money,&p1->time,&p1->wifi,&p1->breakfast,&p1->wc,&p1->floor,&p1->q_time,&p1->occupy);
    }
    p2->next=NULL;
    fclose(fb);
    //////////////////
    FILE *fa;
    if((fa = fopen("id.dat","r+"))==NULL)
    {
        printf("打开文件错误\n");
        exit(0);
    }

    fscanf(fa,"%d\n",&num);
    fscanf(fa,"%s\n",password);
    fscanf(fa,"%d\n",&back);
    fclose(fa);

    return (head);
}
int serch (struct room *head,int choice)     //输出链表值
{
    struct room *pt = head;
    if (head==NULL||n==0){
             system("mode con cols=80 lines=25");
         printf("\n\n\n\n\t************************************************************\n"
            "\t *     *    *     *    *     *    *     *     *     *     *\n"
            "\t    *    *     *     *     *    *    *     *     *     *    \n"
            "\t *     *    *     *    *     *    *     *     *     *     *\n"
            "\t************************************************************\n"
            "\t *     *      *            对不起            *       *    *  \n"
            "\t    *      *     *       当前无房间       *     *      *     \n"
            "\t *     * *         *   请按任意键返回   *          * *    *   \n"
            "\t************************************************************\n"
            "\t *     *    *     *    *     *    *     *    *     *     *\n"
            "\t    *    *     *     *     *    *    *    *     *     *    \n"
            "\t *     *    *     *    *     *    *     *    *     *     *\n"
            "\t************************************************************\n");
            getch();

            if(choice==0)
                RoomMessage(head);
            else if(choice==1)
                gst_menu();

            }
    else{printf("\t***********************************************************************************************\n");
        printf("\t||当前有%3d个房间：                                                                             ||\n",n);
        int i;
        //printf("\t************************************************************");
        for (i=0;i<n;i++)
        {
            printf("\t||房间编号:%-3d房价：%5d元  房牌号:%3d 房型：",pt->id,pt->price,pt->floor);
            switch (pt->type)
            {
                case 0 :printf("大床房       ");break;
                case 1 :printf("单人房       ");break;
                case 2 :printf("双人房       ");break;
                case 3 :printf("豪华大床房   ");break;
                case 4 :printf("豪华双人房   ");break;
                case 5 :printf("多床房       ");break;
                case 6 :printf("钟点房(大床) ");break;

            }
            printf("备注：");
            switch (pt->wifi)
            {
                case 0 :printf("有无线网络和宽带 ");break;
                case 1 :printf("无无线网络和宽带 ");break;
            }
            switch (pt->breakfast)
            {
                case 0 :printf("有早餐 ");break;
                case 1 :printf("无早餐 ");break;
            }
            switch (pt->wc)
            {
                case 0 :printf("有独卫||\n");break;
                case 1 :printf("无独卫||\n");break;
            }
             pt = pt->next;
        }printf("\t***********************************************************************************************\n");
    }
    return 0;
}
struct room * find_tail (struct room *head)  //寻找链表的最后一位
    {
    struct room *p1=head;
    while (p1->next!=NULL)
    {
        p1=p1->next;
    }
    return p1;
}
struct room * creat_room(struct room *head,int type,int price,int wifi ,int breakfast,int wc,int floor)  //新建房间 type代表房型  price代表价格，价格范围（1-MAX） 返回头指针
    {
        if (f[floor]==1) { printf("\n\n\n\n\t************************************************************\n"
            "\t *     *    *     *    *     *    *     *     *     *     *\n"
            "\t    *    *     *     *     *    *    *     *     *     *    \n"
            "\t *     *    *     *    *     *    *     *     *     *     *\n"
            "\t************************************************************\n"
            "\t *     *      *            对不起            *       *    *  \n"
            "\t    *      *     *    该门牌号已被占用    *     *      *     \n"
            "\t *     * *         *   请按任意键返回   *          * *    *   \n"
            "\t************************************************************\n"
            "\t *     *    *     *    *     *    *     *    *     *     *\n"
            "\t    *    *     *     *     *    *    *    *     *     *    \n"
            "\t *     *    *     *    *     *    *     *    *     *     *\n"
            "\t************************************************************\n");return head;}
        if (price<1||price>MAX)  {
                printf("\n\n\n\n\t************************************************************\n"
            "\t *     *    *     *    *     *    *     *     *     *     *\n"
            "\t    *    *     *     *     *    *    *     *     *     *    \n"
            "\t *     *    *     *    *     *    *     *     *     *     *\n"
            "\t************************************************************\n"
            "\t *     *      *       请输入合理的价格       *       *    *  \n"
            "\t    *      *     *                        *     *      *     \n"
            "\t *     * *         *   请按任意键返回   *          * *    *   \n"
            "\t************************************************************\n"
            "\t *     *    *     *    *     *    *     *    *     *     *\n"
            "\t    *    *     *     *     *    *    *    *     *     *    \n"
            "\t *     *    *     *    *     *    *     *    *     *     *\n"
            "\t************************************************************\n");return head;}
        struct room *tail=malloc(len);
        if (head!=NULL) tail =  find_tail (head);
        struct room * p1;
        p1 = malloc(len);
        if (head!=NULL)
        tail->next=p1;
        p1->id = ++num;
        n++;
        p1->price=price;
        p1->type = type;
        p1->money = 0;
        p1->time = 0;
        p1->q_time = 0;
        p1->floor = floor;
        p1->wifi = wifi;
        p1->breakfast = breakfast;
        p1->wc = wc;
        int i;
        for (i=0;i<=366;i++)
        {
            p1->occupy[i]=48;
        }
        p1->occupy[367]='\0';
        p1->next=NULL;
        if (head==NULL) head = p1;
         printf("\n\n\n\n\t************************************************************\n"
            "\t *     *    *     *    *     *    *     *     *     *     *\n"
            "\t    *    *     *     *     *    *    *     *     *     *    \n"
            "\t *     *    *     *    *     *    *     *     *     *     *\n"
            "\t************************************************************\n"
            "\t *     *      *            恭喜您            *       *    *  \n"
            "\t    *      *     *        新建成功        *     *      *     \n"
            "\t *     * *         *   请按任意键返回   *          * *    *   \n"
            "\t************************************************************\n"
            "\t *     *    *     *    *     *    *     *    *     *     *\n"
            "\t    *    *     *     *     *    *    *    *     *     *    \n"
            "\t *     *    *     *    *     *    *     *    *     *     *\n"
            "\t************************************************************\n");
        //printf("新建成功\n");
        return  head;
    }
int save(struct room * head) //保存链表和信息 保存成功返回1 失败返回0
{
    head = sort(head,2);
    save_his(head);
     FILE *fb;
    if((fb = fopen("date.dat","w+"))==NULL)
    {
        printf("无法打开文件\n");
        exit(0);
    }
     struct room * p1=head;
     int i;
     for (i=0;i<n;i++)
     {
         fprintf(fb,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%s\n",p1->id,p1->price,p1->type,p1->money,p1->time,p1->wifi,p1->breakfast,p1->wc,p1->floor,p1->q_time,p1->occupy);
         p1=p1->next;
     }
     char a[10];
     a[0]='\0';
     fprintf(fb,"0,0,0\n",a);
     fclose(fb);
     ///////////////
     if((fb = fopen("id.dat","w+"))==NULL)
    {
        printf("打开文件错误\n");
        return 0;
    }

        fprintf(fb,"%d\n",num);
        fprintf(fb,"%s\n",password);
        fprintf(fb,"%d\n",back);
        fclose(fb);
     return 1;
}
struct room * del_room (struct room * head,int id)     //删除房间，输入编号 删除失败返回NULL 返回NULL有两种情况，一种是删除的房间不存在，一种是没房间了，删完了
{
    struct room *p1,*p2;
    p1=head;
    p2=head;
    int flag = 0;
    int i;
    for (i=0;i<n;i++)
    {
        if (p1->id == id) {flag++;break;}
        p2=p1;
        p1=p1->next;
    }
    if (flag==0)
    {
        //printf("删除房间不存在\n");
        printf("\n\n\n\n\t************************************************************\n"
            "\t *     *    *     *    *     *    *     *     *     *     *\n"
            "\t    *    *     *     *     *    *    *     *     *     *    \n"
            "\t *     *    *     *    *     *    *     *     *     *     *\n"
            "\t************************************************************\n"
            "\t *     *      *            对不起            *       *    *  \n"
            "\t    *      *     *     删除房间不存在     *     *      *     \n"
            "\t *     * *         *   请按任意键返回   *          * *    *   \n"
            "\t************************************************************\n"
            "\t *     *    *     *    *     *    *     *    *     *     *\n"
            "\t    *    *     *     *     *    *    *    *     *     *    \n"
            "\t *     *    *     *    *     *    *     *    *     *     *\n"
            "\t************************************************************\n");
        return NULL;
    }
    else
        {
            if (p1==head)
                {
                    head =p1->next;
                }
            else
                {
                    p2->next = p1->next;
                }

                n--;
            f[p1->floor]=0;
            return head;
        }
}


int All_chage(struct room *head,int rate) //修改所有房间的价格（超出价格范围的价格不会变化） 成功返回0 失败返回1 失败原因rate超出范围 rate 代表%数 例如50代表*50% 范围 （1-999）
    {

        struct room * p1=head;
        int i;
        for (i=0;i<n;i++)
            {
                if (p1->price*rate>100*MAX||p1->price*rate<100)
                    {
                            p1=p1->next;
                            continue;
                    }
                            p1->price=p1->price*rate;
                            p1->price=(int)(p1->price/100);
                            p1=p1->next;
            }
            return 0;

    }
int Type_chage(struct room *head,int type,int rate) //修改特定房型的价格（超出价格范围的价格不会变化）  type 代表房型
    {

        struct room * p1=head;
        int i;
        for (i=0;i<n;i++)
            {
                if (p1->type==type)
                {


                if (p1->price*rate>100*MAX||p1->price*rate<100)
                    {
                            p1=p1->next;
                            continue;
                    }
                            p1->price=p1->price*rate;
                            p1->price=(int)(p1->price/100);

                }
                 p1=p1->next;
            }
            return 0;
    }
int  chage(struct room *head,int id,int op,int price,int type) //修改特定房间（超出价格范围的价格不会变化）
      {
            struct room * p1=head;
             int i;
            switch (op)
            {
                case 0:

                    for (i=0;i<n;i++)
                        {
                            if (p1->id==id)
                            {
                            if (price>MAX||price<1)
                                {

                                    return 0;
                                }
                                p1->price= price;
                                return 0;

                            }
                            p1 = p1->next;
                        }
                        break;
                case 1:
                         p1=head;
                        for (i=0;i<n;i++)
                            {
                                if (p1->id==id)
                                    {
                                        p1->type=type;
                                        return 0;
                                    }
                                        p1=p1->next;
                            }break;
                case 2: for (i=0;i<n;i++)
                            {
                                if (p1->id==id)
                                {
                                    p1->wifi=type;
                                    return 0;
                                }
                                    p1=p1->next;
                            }break;
                case 3: for (i=0;i<n;i++)
                            {
                                if (p1->id==id)
                                {
                                    p1->breakfast=type;
                                    return 0;
                                }
                                    p1=p1->next;
                            }break;
                case 4: for (i=0;i<n;i++)
                            {
                                if (p1->id==id)
                                {
                                    p1->wc=type;
                                    return 0;
                                }
                                    p1=p1->next;
                            }break;
            }
            return 1;
        }



struct room * swtich (struct room *head,struct room *j1,struct room *j2) //交换链表位置
    {

        struct room *p1,*p2,*p3,*p4,*flag=head;
         p1=p2=p3=p4=NULL;
        if (j1!=head&&j2!=head)
        {
            if (j1==j2) return head;
            int i ;
            for (i=0;i<n;i++)
            {
                if (flag->next==j1) p1=flag ;

                if (flag->next==j2) p3=flag ;
                flag = flag->next;
            }
            if (j1->next==j2)
            {
                j1->next=j2->next;
                j2->next=j1;
                p1->next=j2;
                return head;
            }
            else if (j2->next==j1)
            {
                j2->next=j1->next;
                j1->next=j2;
                p3->next=j1;
                return head;
            }
            p4=j2->next;
            p1->next=j2;
            j2->next=j1->next;
            p3->next=j1;
            j1->next=p4;
            return head;
        }
        else if (j1==head)
        {

            int i ;
            for (i=0;i<n;i++)
            {
                if (flag->next==j2) p3=flag;
                flag = flag->next;
            }
            if (j1->next==j2)
            {
                j1->next=j2->next;
                j2->next=j1;
                return j2;
            }
            p4=j2->next;
            j2->next=j1->next;
            p3->next=j1;
            j1->next=p4;
            return j2;
        }
        else  if (j2==head)
            {
                int i ;
                for (i=0;i<n;i++)
                {
                    if (flag->next==j1) p1=flag ;
                    flag = flag->next;
                }

             if (j2->next==j1)
                {
                    j2->next=j1->next;
                    j1->next=j2;
                    return j1;
                }
            p4=j2->next;
            p1->next=j2;
            j2->next=j1->next;
            j1->next=p4;
            return j1;
            }
            return head;

    }
/*int r_print (struct room * pt) //输出链表内容 菜单不用

{
            printf("房间编号:%-3d房价：%5d元  房牌号:%3d ",pt->id,pt->price,pt->floor);
            switch (pt->type)
            {
                case 0 :printf("大床房     ");break;
                case 1 :printf("单人房     ");break;
                case 2 :printf("双人房     ");break;
                case 3 :printf("豪华大床房 ");break;
                case 4 :printf("豪华双人房 ");break;
                case 5 :printf("多床房     ");break;

            }
            switch (pt->wifi)
            {
                case 0 :printf("有无线网络和宽带 ");break;
                case 1 :printf("无无线网络和宽带 ");break;
            }
            switch (pt->breakfast)
            {
                case 0 :printf("有早餐 ");break;
                case 1 :printf("无早餐 ");break;
            }
            switch (pt->wc)
            {
                case 0 :printf("有独卫\n");break;
                case 1 :printf("无独卫\n");break;
            }
            return 0;

}*/
struct room * choose(struct room * head,int op,int min,int max) //筛选房间 返回链表头 如果为空则无结果
{
        int i,j;
        struct room * p1=head,*p2;
        int result=0;int t = n;
        switch (op)
        {

            case 2: for (i=0;i<t;i++)//从大到小
                    {
                        //printf("1:%d\n",p1->id);
                        if (p1->price<min||p1->price>max)
                            {p2=p1;p1=p1->next;head=del_room(head,p2->id),result++;continue;}
                        p1=p1->next;
                    }break;
            case 4:for (i=0;i<t;i++)//大床
                    {
                        if (p1->type!=0)
                            {p2=p1;p1=p1->next;head=del_room(head,p2->id),result++;continue;}
                        p1=p1->next;
                    }break;
            case 5:for (i=0;i<t;i++)//单人
                    {
                        if (p1->type!=1)
                            {p2=p1;p1=p1->next;head=del_room(head,p2->id),result++;continue;}
                        p1=p1->next;
                    }break;
            case 6:for (i=0;i<t;i++)//双人
                    {
                        if (p1->type!=2)
                            {p2=p1;p1=p1->next;head=del_room(head,p2->id),result++;continue;}
                        p1=p1->next;
                    }break;
             case 7:for (i=0;i<t;i++)//v_s
                    {
                        if (p1->type!=3)
                            {p2=p1;p1=p1->next;head=del_room(head,p2->id),result++;continue;}
                        p1=p1->next;
                    }break;
             case 8:for (i=0;i<t;i++)//v_d
                    {
                        if (p1->type!=4)
                            {p2=p1;p1=p1->next;head=del_room(head,p2->id),result++;continue;}
                        p1=p1->next;
                    }break;
             case 9:for (i=0;i<t;i++)//multi
                    {
                        if (p1->type!=5)
                            {p2=p1;p1=p1->next;head=del_room(head,p2->id),result++;continue;}
                        p1=p1->next;
                    }break;
            case 10:for (i=0;i<t;i++)//time_room
                    {
                        if (p1->type!=6)
                            {p2=p1;p1=p1->next;head=del_room(head,p2->id),result++;continue;}
                        p1=p1->next;
                    }break;
            case 11:for (i=0;i<t;i++)//wifi 0
                    {
                        if (p1->wifi==1)
                            {p2=p1;p1=p1->next;head=del_room(head,p2->id),result++;continue;}
                        p1=p1->next;
                    }break;
            case 12:for (i=0;i<t;i++)//breakfast
                    {
                        if (p1->breakfast==1)
                            {p2=p1;p1=p1->next;head=del_room(head,p2->id),result++;continue;}
                        p1=p1->next;
                    }break;
             case 13:for (i=0;i<t;i++)//wc
                    {
                        if (p1->wc==1)
                            {p2=p1;p1=p1->next;head=del_room(head,p2->id),result++;continue;}
                        p1=p1->next;
                    }break;

            case 14:for (i=0;i<t;i++)//双人 大床 multi
                    {
                        if (p1->type!=2&&p1->type!=0&&p1->type!=5)
                            {p2=p1;p1=p1->next;head=del_room(head,p2->id),result++;continue;}
                        p1=p1->next;
                    }break;
            case 15:for (i=0;i<t;i++)//非time_room
                    {
                        if (p1->type==6)
                            {p2=p1;p1=p1->next;head=del_room(head,p2->id),result++;continue;}
                        p1=p1->next;
                    }break;
        }

             return head;

}

int c_day(int mon ,int day) //根据月份算天数
{
    //printf("%d %d\n",mon,day);
    int sum = 0;
    switch (mon)
    {
        case 12 :
        case 11 : if (mon!=11) sum+=30;
        case 10 : if (mon!=10) sum+=31;
        case 9 : if (mon!=9) sum+=30;
        case 8 : if (mon!=8) sum+=31;
        case 7 : if (mon!=7) sum+=31;
        case 6 : if (mon!=6) sum+=30;
        case 5 : if (mon!=5) sum+=31;
        case 4 : if (mon!=4) sum+=30;
        case 3 : if (mon!=3) sum+=31;
        case 2 : if (mon!=2) sum+=28;
        case 1 : if (mon!=1) sum+=31;break;
    }
    return (sum +day);
}
int c_date (int date,char* cday) //根据天数算月份
{

    int mon,day;
    if (date<32) mon = 1,day = date;
    else if (date<60) mon = 2,day = date-31;
    else if (date<91) mon = 3,day = date-59;
    else if (date<121) mon = 4,day = date-90;
    else if (date<152) mon = 5,day = date -120;
    else if (date<182) mon = 6,day = date -151;
    else if (date<213) mon = 7,day = date-181;
    else if (date<244) mon = 8,day = date-212;
    else if (date<274) mon = 9,day = date-243;
    else if (date<305) mon = 10,day = date-273;
    else if (date<335) mon = 11,day = date-304;
    else mon = 12,day = date -334;
    cday[0]='0';
    switch (mon)
    {
        case 1:cday[1]='1';break;
        case 2:cday[1]='2';break;
        case 3:cday[1]='3';break;
        case 4:cday[1]='4';break;
        case 5:cday[1]='5';break;
        case 6:cday[1]='6';break;
        case 7:cday[1]='7';break;
        case 8:cday[1]='8';break;
        case 9:cday[1]='9';break;
        case 10:cday[1]='0';cday[0]='1';break;
        case 11:cday[1]='1';cday[0]='1';break;
        case 12:cday[1]='2';cday[0]='1';break;

    }
    if (day<10) cday[2]='0',cday[3]=day+48;
    else
        {
            cday[2]=(int)(day/10)+48;cday[3]=day%10+48;
        }
        cday[4]='\0';
    return 0;

}
struct room * find_pointer(struct room *head,int id)
{
    int i;
    for (i=0;i<n;i++)
     {
        if (head->id==id)
            return head;
        if (head->next==NULL)
            return NULL;
         head=head->next;
     }
     return NULL;

}
int giveup_book_room(struct room *head,int id,char in [],char out []) //取消预订
{
    int in_day,out_day, days;
    in_day = c_day((in[4]-48)*10+in[5]-48,in[6]*10-480+in[7]-48);
    out_day=c_day(out[4]*10+out[5]-480-48,out[6]*10+out[7]-480-48);
    //printf("%d %d\n",in_day,out_day);
    days=out_day-in_day;
    struct room *p1=find_pointer(head,id);
    int i;
    if (p1!=NULL)
    {
        for (i=in_day;i<=out_day;i++)
        {
            p1->occupy[i]='0';
        }
    }
    p1->q_time++;
    p1->money-=days*p1->price;
    return 0;

}
int book_room(struct room *head,int id,char in [],char out []) //预订房间
{
    int in_day,out_day, days;
    in_day = c_day((in[4]-48)*10+in[5]-48,in[6]*10-480+in[7]-48);
    out_day=c_day(out[4]*10+out[5]-480-48,out[6]*10+out[7]-480-48);
    //printf("%d %d\n",in_day,out_day);
    days=out_day-in_day;
    struct room *p1=find_pointer(head,id);
    int i;
    if (p1!=NULL)
    {
        for (i=in_day;i<=out_day;i++)
        {
            p1->occupy[i]='1';
        }
    }
    p1->time++;
    p1->money+=days*p1->price;
    return 0;

}
int time_book_room(struct room *head,int id,char in []) //钟点房的预订
{
    int in_day,out_day, days;
    in_day = c_day((in[4]-48)*10+in[5]-48,in[6]*10-480+in[7]-48);
    struct room *p1=find_pointer(head,id);
    int i;
    if (p1!=NULL)
    {

            p1->occupy[in_day]='1';
    }
    p1->time++;
    p1->money+=p1->price;
    return 0;
}

int time_giveup_book_room(struct room *head,int id,char in []) //钟点房的取消预订
{
    int in_day;
    in_day = c_day((in[4]-48)*10+in[5]-48,in[6]*10-480+in[7]-48);
    struct room *p1=find_pointer(head,id);
    if (p1!=NULL)
    {

            p1->occupy[in_day]='0';

    }
    p1->q_time++;
    p1->money-=p1->price;
    return 0;
}


struct room * sort(struct room *head,int op)  //排序 0-1 价格 0小到大 1大到小
{
     int i,j;
     if (n==1||n==0) return head;
     struct room *p1=head;
     switch (op)
     {
        case 0:
                for (i=0;i<n-1;i++)
                    {
                        head = p1; int flag = 0;
                        for (j=0;j<n-1-i;j++)
                        {
                            if (head->price>head->next->price)
                            {p1=swtich(p1,head,head->next);flag++;continue;}
                            head = head->next;
                        }
                        if (flag==0) break;
                    }break;
        case 1:
                for (i=0;i<n-1;i++)
                    {
                            head = p1;int flag = 0;
                        for (j=0;j<n-1-i;j++)
                        {
                            if (head->price<head->next->price)
                             {p1=swtich(p1,head,head->next);flag++;continue;}
                            head = head->next;
                        }
                         if (flag==0) break;
                    }break;

        case 2:
                for (i=0;i<n-1;i++)
                    {
                        head = p1;int flag = 0;
                        for (j=0;j<n-1-i;j++)
                        {
                            if (head->id>head->next->id)
                             {p1=swtich(p1,head,head->next);flag++;continue;}
                            head = head->next;
                        }
                         if (flag==0) break;
                    }break;
         case 3:
                for (i=0;i<n-1;i++)
                    {
                        head = p1;int flag = 0;
                        for (j=0;j<n-1-i;j++)
                        {
                            if (head->id<head->next->id)
                             {p1=swtich(p1,head,head->next);flag++;continue;}
                            head = head->next;
                        }
                         if (flag==0) break;
                    }break;
         case 4:
                for (i=0;i<n-1;i++)
                    {
                        head = p1;int flag = 0;
                        for (j=0;j<n-1-i;j++)
                        {
                            if (head->money<head->next->money)
                             {p1=swtich(p1,head,head->next);continue;}
                            head = head->next;
                        }
                         if (flag==0) break;
                    }break;
     }
     return p1;


}
struct room * find_freeroom (struct room * head,char in[],char out[])  //寻找空余房间
{
    int in_day,out_day;
    in_day = c_day((in[4]-48)*10+in[5]-48,in[6]*10-480+in[7]-48);
    out_day=c_day(out[4]*10+out[5]-480-48,out[6]*10+out[7]-480-48);
    struct room *p1=head,*p2=head;
    int i,j;
    int t  = n;
    for (i=0;i<t;i++)
    {
        int flag = 0;
        for (j=in_day;j<=out_day;j++)
        {
            if (p1->occupy[j]=='1'){flag = 1;break;}
        }
        if (flag==1){p2 = p1;p1 = p1->next;head=del_room(head,p2->id);}
        else{p1 = p1->next;}
    }
    return head;
}
int numtoc(int n,char a[])  //数字变成字符串
{

//01234567
    a[8]='\0';
   int i;
   for (i=7;i>=0;i--)
   {
       a[i]=n%10+48;
       n/=10;
   }

   return 0;
}
//int find_history()
int search_his(char a[50][50]);
extern char *get_time(char tim[20])//获取系统当前时间 返回字符串格式为
{
    char y[5];
    char m[3];
    char d[3];
    char h[3];
    char min[3];
    char sec[3];

    time_t t;
    struct tm *lt;
    time(&t);
    lt = localtime (&t);

    sprintf(y,"%d",lt->tm_year+1900);
    sprintf(m,"%d",lt->tm_mon+1);
    sprintf(d,"%d",lt->tm_mday);
    sprintf(h,"%d",lt->tm_hour);
    sprintf(min,"%d",lt->tm_min);
    sprintf(sec,"%d",lt->tm_sec);

    strcpy(tim,y);
    if(strlen(m)==1)
        strcat(tim,"0");
    strcat(tim,m);
    if(strlen(d)==1)
        strcat(tim,"0");
    strcat(tim,d);
    if(strlen(h)==1)
        strcat(tim,"0");
    strcat(tim,h);
    if(strlen(min)==1)
        strcat(tim,"0");
    strcat(tim,min);
    if(strlen(sec)==1)
        strcat(tim,"0");
    strcat(tim,sec);
    return tim;
}
int save_his( struct room *head)//创建文件名为当前系统时间的.dat文件并保存链表
{
    char filename[20];
    char tim[20];
    char *get_time(char tim[20]);
    FILE *fp;
    char path[50];
    strcpy(filename,get_time(tim));
    strcat(filename,".dat");
    sprintf(path,"history\\%s",filename);
    if((fp=fopen(path,"w+")) == NULL)
       {
           printf("can not open this file1\n");
           exit(0);
       }
    struct room * p1=head;
     int i;
     for (i=0;i<n;i++)
     {
         fprintf(fp,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%s\n",p1->id,p1->price,p1->type,p1->money,p1->time,p1->wifi,p1->breakfast,p1->wc,p1->floor,p1->q_time,p1->occupy);
         p1=p1->next;
     }
     char a[20];
     a[0]='\0';
    fprintf(fp,"0,0,0\n",a);
    fclose(fp);
    if((fp=fopen("time.dat","a+")) == NULL)
       {
           printf("can not open this file2\n");
           exit(0);
       }
    fprintf(fp,"%s\n",filename);
    fclose(fp);
    return 0;
}
int search_his(char a[50][50]) //搜索历史记录 返回条数
{
    FILE *fp;
    if((fp=fopen("time.dat","r+")) == NULL)
       {
           printf("can not open this file3\n");
           exit(0);
       }
       int i=0;
       //printf("%d",feof(fp));
    while (feof(fp)==0)
    {
        fscanf(fp,"%s\n",a[i]);
        i++;
    }
    int j = i-1;
    printf("\t      *************************************************\n");
    for (j=i-1;j>=0&&j>=i-30;j--)
    {
        put_data(j,a[j]);
    } printf("\t      *************************************************\n");
    fclose(fp);
    return i;
}
int put_data(int i,char a[])
{
    printf("\t      ||编号：%2d 修改时间：%c%c%c%c年%c%c月%c%c日%c%c时%c%c分%c%c秒||\n",i,a[0],a[1],a[2],a[3],a[4],a[5],a[6],a[7],a[8],a[9],a[10],a[11],a[12],a[13]);
    return 0;
}
struct room *creat_his(char filename[]) //历史记录链表
{
    FILE *fp;
    char path[100];
    sprintf(path,"history\\%s",filename);
    if((fp=fopen(path,"r+")) == NULL)
       {
           printf("can not open this file4\n");
           exit(0);
       }
    struct room * head;
    struct room * p1,* p2;
    n=0;
    p1=p2=(struct room*)malloc(len);
    fscanf(fp,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%s\n",&p1->id,&p1->price,&p1->type,&p1->money,&p1->time,&p1->wifi,&p1->breakfast,&p1->wc,&p1->floor,&p1->q_time,&p1->occupy);
    head= NULL;
    while (p1->id!=0)
    {
        n++; f[p1->floor]=1;
        if (n==1)head = p1;
        else p2->next=p1;
        p2=p1;
        //fprintf(fb,"%d,%d\n",p1->id,p1->time);
        p1=(struct room *)malloc(len);
        fscanf(fp,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%s\n",&p1->id,&p1->price,&p1->type,&p1->money,&p1->time,&p1->wifi,&p1->breakfast,&p1->wc,&p1->floor,&p1->q_time,&p1->occupy);
    }
    p2->next=NULL;
    fclose(fp);
    return head;
}
int pass() //验证密码 正确返回1 错误为0
{
    if (ppp==1) return 1;
        char a[50];
        system("mode con cols=80 lines=25");
       printf("\t************************************************************\n"
                "\t *     *    *     *    *     *    *     *    *     *     *\n"
                "\t    *    *     *     *     *    *    *    *     *     *    \n"
                "\t *     *    *     *    *     *    *     *    *     *     *\n"
                "\t************************************************************\n"
                "\t------------------------------------------------------------\n"
                "\t************************************************************\n"
                "\t||                                                        ||\n"
                "\t||                                                        ||\n"
                "\t||                   请输入管理员密码                     ||\n"
                "\t||                                                        ||\n"
                "\t||                                                        ||\n"
                "\t||                                                        ||\n"
                "\t||                                                        ||\n"
                "\t||                                                        ||\n"

                "\t************************************************************\n"
                "\t------------------------------------------------------------\n"
                "\t************************************************************\n"
                "\t************************************************************\n"
                "\t *     *    *     *    *     *    *     *    *     *     *\n"
                "\t    *    *     *     *     *    *    *    *     *     *    \n"
                "\t *     *    *     *    *     *    *     *    *     *     *\n"
                "\t************************************************************\n");
        gotoxy(33,12);
        gets(a);
        system("cls");
        if (strcmp(a,password)==0) return 1;
        else return -1;
}
int new_pass () //更改密码
{
        if (pass()<0) return -1;
        char a[50];
        //printf("请输入新的管理员密码：");
        printf("\t************************************************************\n"
                "\t *     *    *     *    *     *    *     *    *     *     *\n"
                "\t    *    *     *     *     *    *    *    *     *     *    \n"
                "\t *     *    *     *    *     *    *     *    *     *     *\n"
                "\t************************************************************\n"
                "\t------------------------------------------------------------\n"
                "\t************************************************************\n"
                "\t||                                                        ||\n"
                "\t||                                                        ||\n"
                "\t||                 请输入新的管理员密码                   ||\n"
                "\t||                                                        ||\n"
                "\t||                                                        ||\n"
                "\t||                                                        ||\n"
                "\t||                                                        ||\n"
                "\t||                                                        ||\n"

                "\t************************************************************\n"
                "\t------------------------------------------------------------\n"
                "\t************************************************************\n"
                "\t************************************************************\n"
                "\t *     *    *     *    *     *    *     *    *     *     *\n"
                "\t    *    *     *     *     *    *    *    *     *     *    \n"
                "\t *     *    *     *    *     *    *     *    *     *     *\n"
                "\t************************************************************\n");
        gotoxy(33,12);
        gets(a);
        system("cls");
        printf("\t------------------------------------------------------------\n"
                "\t************************************************************\n"
                "\t||                                                        ||\n"
                "\t||                                                        ||\n");
         printf("\t          确定以 %s 作为新密码吗?（0-是 其他-否）     \n",a);
         printf("\t||                                                        ||\n"
                "\t||                                                        ||\n"
                "\t||                                                        ||\n"
                "\t||                                                        ||\n"
                "\t||                                                        ||\n"
                "\t************************************************************\n"
                "\t------------------------------------------------------------\n");
        int i=getch();
        if (i!='0') return 0;
        else
            {
                strcpy(password,a);
                FILE *fb;
                 if((fb = fopen("id.dat","w+"))==NULL)
                    {
                        printf("打开文件错误\n");
                        exit(0);
                    }

                    fprintf(fb,"%d\n",num);
                    fprintf(fb,"%s\n",password);
                    fprintf(fb,"%d",back);
                    fclose(fb);
                    return 1;
            }


}
void set_back(int op)  //更改颜色 color 0=黑色  1蓝色 2 绿色 3湖蓝色 4红色 5紫色 6黄色 7白色 8灰色 9淡蓝色
                        // A淡绿色 B淡浅绿色 C淡红色 D淡紫色 E淡黄色 F亮白色</span>
{
    switch(op)
    {
        case 0 :system("color 0A");break;case 32 :system("color 2D");break;
        case 1 :system("color 1A");break;case 33 :system("color 3D");break;
        case 2 :system("color 2A");break;case 34 :system("color 4D");break;
        case 3 :system("color 3A");break;case 35 :system("color 5D");break;
        case 4 :system("color 4A");break;case 36 :system("color 6D");break;
        case 5 :system("color 5A");
        case 6 :system("color 6A");break;case 37 :system("color 7D");break;
        case 7 :system("color 7A");break;case 38 :system("color 8D");break;
        case 8 :system("color 8A");break;case 39 :system("color 9D");break;
        case 9 :system("color 9A");break;case 40 :system("color 0E");break;
        case 10:system("color 0B");break;case 41 :system("color 1E");break;
        case 11:system("color 1B");break;case 42 :system("color 2E");break;
        case 12 :system("color 2B");break;case 43 :system("color 3E");break;
        case 13 :system("color 3B");break;case 44:system("color 4E");break;
        case 14 :system("color 4B");break;case 45 :system("color 5E");break;
        case 15 :system("color 5B");break;case 46 :system("color 6E");break;
        case 16 :system("color 6B");break;case 47:system("color 7E");break;
        case 17 :system("color 7B");break;case 48 :system("color 8E");break;
        case 18 :system("color 8B");break;case 49:system("color 9E");break;
        case 19 :system("color 9B");break;case 50 :system("color 0F");break;
        case 20 :system("color 0C");break;case 51:system("color 1F");break;
        case 21 :system("color 1C");break;case 52 :system("color 2F");break;
        case 22 :system("color 2C");break;case 53:system("color 3F");break;
        case 23 :system("color 3C");break;case 54 :system("color 4F");break;
        case 24 :system("color 4C");break;case 55 :system("color 5F");break;
        case 25 :system("color 5C");break;case 56 :system("color 6F");break;
        case 26 :system("color 6C");break;case 57:system("color 7F");break;
        case 27 :system("color 7C");break;case 58 :system("color 8F");break;
        case 28 :system("color 8C");break;case 59:system("color 9F");break;
        case 29 :system("color 9C");break;
        case 30 :system("color 0D");break;
        case 31 :system("color 1D");break;

    }
}
void quicksort(char *a,int left,int right) //快速排序
{
    int i,j,t,temp;
    if(left>right)
       return;

    temp=a[left];
    i=left;
    j=right;
    while(i!=j)
    {

                   while(a[j]<=temp && i<j)
                            j--;
                   while(a[i]>=temp && i<j)
                            i++;

                   if(i<j)
                   {
                            t=a[i];
                            a[i]=a[j];
                            a[j]=t;
                   }
    }

    a[left]=a[i];
    a[i]=temp;

    quicksort(a,left,i-1);
    quicksort(a,i+1,right);
}
struct room * stat_mon(struct room *head) //统计
{system("mode con cols=130 lines=50");
    int i,sum =0,time=0,q=0;
    struct room *p1=sort(head,4);
    int m[10]={0};
printf("\n\n\n\n\n\n\t*******************************************************************************************************************\n");
    for (i=0;i<n;i++)
    {
        int day = 0;
        int j;
        for (j=1;j<=365;j++)
        {
            if (p1->occupy[j]=='1') day++;
        }
        m[p1->type]+=p1->money;
        q+=p1->q_time;
        sum +=p1->money;
        time+=p1->time;
        printf("\t||房间编号：%d \t预计收入：%-7d元\t预定次数：%3d \t取消预定次数：%3d\t累计预定天数: %3d\t\t||\n",p1->id,p1->money,p1->time,p1->q_time,day);
        p1=p1->next;
    }
    printf("\t||目前为止今年总收入：%-7d元\t\t\t\t\t\t\t\t\t\t\t        ||   "
            "\n\t||其中大床房%5d元 单人床%5d元 双人房%5d元 豪华大床%5d元 豪华双人%5d元 多床房%5d元 钟点房%5d元\t||\n"
            "\t||总预定次数：%3d \t\t\t总取消次数：%3d\t\t\t\t\t\t\t\t        ||\n",sum,m[0],m[1],m[2],m[3],m[4],m[5],m[6],time,q);
    printf("\t||输入任意键返回\t\t\t\t\t\t\t\t\t\t\t\t        ||\n");
printf("\t*******************************************************************************************************************\n");getch();Roomstat(head);
    return 0;
}

void FREE(struct room *head) //释放链表

{
    struct room *p1=head;
    while (head!=NULL)
    {
        free(head);
        head = p1->next;
        p1=p1->next;
    }
    return ;
}






/*int main()
{
    struct room*head;
    while (1){
    char a[50][50];
    int i;
    search_his(a);
    scanf("%d",&i);
    head=creat_his(a[i]);
    serch(head);}
}

*/
