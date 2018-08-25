#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <windows.h>
#include "room.h"

void show_gst_menu();
void gst_menu();

struct Guest
{
    char name[20];
    char sex;
    char age[4];
    char in_time[17];//实际的登记入住时间
    char out_time[17];//实际的退房时间
    char indt[9];//预订的入住时间
    char otdt[9];//预订的离店时间
    char rstat;//存储客人预订的状态 rstat==1表示在预订状态 rstat==0表示不在预订状态
    int room_id;
    char gst_id[50];
    int num;//同住人数
    struct Guest *next;
};

int gotoxy(int x, int y) //定位光标位置
{
    HANDLE handle; //定义句柄变量handle
    COORD coord; //定义结构体coord (坐标系coord)
    coord.X = x; //横坐标x
    coord.Y = y; //纵坐标y
    handle = GetStdHandle(STD_OUTPUT_HANDLE); //获取控制台输出句柄(值为-11)
    SetConsoleCursorPosition(handle, coord); //移动光标
}

void gfree(struct Guest *head)//释放动态存储空间
{
    if (NULL == head)
        return;
    struct Guest * p=head;
    struct Guest * t=p;
    while(NULL != p->next)
    {
        t=p->next;
        free(p);
        p=t;
    }
    free(p);
    head=NULL;
    return;
}

void copy_gst(struct Guest *p1,struct Guest tmp)//将读取的信息复制到指针指向的存储空间
{
    strcpy(p1->name,tmp.name);
    p1->sex=tmp.sex;
    strcpy(p1->age,tmp.age);
    strcpy(p1->in_time,tmp.in_time);
    strcpy(p1->out_time,tmp.out_time);
    strcpy(p1->indt,tmp.indt);
    strcpy(p1->otdt,tmp.otdt);
    p1->room_id=tmp.room_id;
    strcpy(p1->gst_id,tmp.gst_id);
    p1->rstat=tmp.rstat;
    p1->num=tmp.num;
}

struct Guest *creat_gst_link()//创建客人的动态链表
{
    void copy_gst(struct Guest *p1,struct Guest tmp);

    FILE *fp;
    struct Guest *head,*p1,*p2;
    int i=0;//i用来记录链表结点的个数
    struct Guest temp;

    if((fp=fopen("guest.dat","rb"))==NULL)
    {
        printf("\ncan not open guest.dat");
        exit(0);
    }

    p1=p2=(struct Guest *) malloc(sizeof(struct Guest));
    fread(&temp,sizeof(struct Guest),1,fp);
    copy_gst(p1,temp);
    head=NULL;
    while(!feof(fp))
    {
        i++;
        if(i==1)
            head=p1;
        else
            p2->next=p1;
        p2=p1;
        p1=(struct Guest *) malloc(sizeof(struct Guest));

        fread(&temp,sizeof(struct Guest),1,fp);
        copy_gst(p1,temp);
    }

    p2->next=NULL;

    fclose(fp);

    return(head);
}

struct Guest *find_end(struct Guest *head)//寻找链表的尾结点
{
    struct Guest *p;
    p=head;
    while(head!=NULL&&p->next!=NULL)//注意 head!=NULL要放在前面 否则程序出错
    {
        p=p->next;
    }
    return p;
}

void swap_gst(struct Guest *p1,struct Guest *p2)//交换两个结点的值
{
    struct Guest tmp;//防止重名

    strcpy(tmp.name,p1->name);
    tmp.sex=p1->sex;
    strcpy(tmp.age,p1->age);
    strcpy(tmp.in_time,p1->in_time);
    strcpy(tmp.out_time,p1->out_time);
    strcpy(tmp.indt,p1->indt);
    strcpy(tmp.otdt,p1->otdt);
    tmp.room_id=p1->room_id;
    strcpy(tmp.gst_id,p1->gst_id);
    tmp.rstat=p1->rstat;
    tmp.num=p1->num;

    strcpy(p1->name,p2->name);
    p1->sex=p2->sex;
    strcpy(p1->age,p2->age);
    strcpy(p1->in_time,p2->in_time);
    strcpy(p1->out_time,p2->out_time);
    strcpy(p1->indt,p2->indt);
    strcpy(p1->otdt,p2->otdt);
    p1->room_id=p2->room_id;
    strcpy(p1->gst_id,p2->gst_id);
    p1->rstat=p2->rstat;
    p1->num=p2->num;

    strcpy(p2->name,tmp.name);
    p2->sex=tmp.sex;
    strcpy(p2->age,tmp.age);
    strcpy(p2->in_time,tmp.in_time);
    strcpy(p2->out_time,tmp.out_time);
    strcpy(p2->indt,tmp.indt);
    strcpy(p2->otdt,tmp.otdt);
    p2->room_id=tmp.room_id;
    strcpy(p2->gst_id,tmp.gst_id);
    p2->rstat=tmp.rstat;
    p2->num=tmp.num;
}

void gst_sort_indt(struct Guest *pBegin,struct Guest *pEnd)//按照预订入住日期排序
{
    if(NULL == pBegin || NULL == pEnd || pBegin == pEnd)
    {
        return;
    }

    struct Guest *pFirst = pBegin;
    struct Guest *pSecond = pFirst->next;

    long nMidValue = atoi(pFirst->indt);

    while(pSecond != pEnd->next && pSecond != NULL)
    {
        if(atoi(pSecond->indt)<nMidValue)
        {
            pFirst = pFirst->next;
            swap_gst(pFirst,pSecond);
        }
        pSecond = pSecond->next;
    }
    swap_gst(pBegin,pFirst);
    gst_sort_indt(pBegin,pFirst);
    gst_sort_indt(pFirst->next,pEnd);
}

void gst_sort_otdt(struct Guest *pBegin,struct Guest *pEnd)//按照预订离店日期排序
{
    if(NULL == pBegin || NULL == pEnd || pBegin == pEnd)
    {
        return;
    }

    struct Guest *pFirst = pBegin;
    struct Guest *pSecond = pFirst->next;

    int nMidValueindt = atoi(pFirst->indt);
    int nMidValueotdt = atoi(pFirst->otdt);

    while(pSecond != pEnd->next && pSecond != NULL)
    {
        if(atoi(pSecond->otdt)<nMidValueotdt&&atoi(pSecond->indt)<=nMidValueindt)
        {
            pFirst = pFirst->next;
            swap_gst(pFirst,pSecond);
        }
        pSecond = pSecond->next;
    }
    swap_gst(pBegin,pFirst);
    gst_sort_otdt(pBegin,pFirst);
    gst_sort_otdt(pFirst->next,pEnd);
}


char *get_time_book(char tim[11])//获取系统时间 精确到日 格式为20180101
{
    char y[5];
    char m[3];
    char d[3];

    time_t t;
    struct tm *lt;
    time(&t);
    lt = localtime (&t);

    sprintf(y,"%d",lt->tm_year+1900);
    sprintf(m,"%d",lt->tm_mon+1);
    sprintf(d,"%d",lt->tm_mday);

    strcpy(tim,y);
    if(strlen(m)==1)
        strcat(tim,"0");
    strcat(tim,m);
    if(strlen(d)==1)
        strcat(tim,"0");
    strcat(tim,d);

    return tim;
}

char *get_time_std(char tim[17])//获取系统时间 精确到分 格式为2018/01/01/11:30
{
    char y[5];
    char m[3];
    char d[3];
    char h[3];
    char min[3];

    time_t t;
    struct tm *lt;
    time(&t);
    lt = localtime (&t);

    sprintf(y,"%d",lt->tm_year+1900);
    sprintf(m,"%d",lt->tm_mon+1);
    sprintf(d,"%d",lt->tm_mday);
    sprintf(h,"%d",lt->tm_hour);
    sprintf(min,"%d",lt->tm_min);

    strcpy(tim,y);
    strcat(tim,"/");
    if(strlen(m)==1)
        strcat(tim,"0");
    strcat(tim,m);
    strcat(tim,"/");
    if(strlen(d)==1)
        strcat(tim,"0");
    strcat(tim,d);
    strcat(tim,"/");
    if(strlen(h)==1)
        strcat(tim,"0");
    strcat(tim,h);
    strcat(tim,":");
    if(strlen(min)==1)
        strcat(tim,"0");
    strcat(tim,min);
    return tim;
}

int check_input_name(char name[500])//检查输入的名字是否为汉字（不超过五个字）
{
    int i,j,flag=1;
    char punctuation[]={"·！@#￥……（）——【】：“”；‘《》？，。、"};

    if(strlen(name)>10)
        flag = 0;
    for(i=0;i<strlen(name);i++)
    {
        if(name[i]>=0)
            flag=0;

        if(flag==0)
            break;
    }
    for(i=0;i<strlen(name);i+=2)
    {
        if(flag==0)
            break;
        for(j=0;j<strlen(punctuation);j+=2)
        {
            if(punctuation[j]==name[i]&&punctuation[j+1]==name[i+1])
            {
                flag=0;
                break;
            }
        }
    }

    return flag;
}

int check_input_age(char age[500])//检查输入的年龄（是否为数字 18~120）
{
    int i,flag=1;

    if(atoi(age)>120||atoi(age)<18)
        flag = 0;
    for(i=0;i<strlen(age);i++)
    {
        if(!(age[i]>='0'&&age[i]<='9'))
        {
            flag = 0;
            break;
        }
    }
    return flag;
}

int checkCityCode(char gid[500])//检查输入的身份证号是否有省份代码
{
    int flag=1;//flag==1表示检验不合格
    int i;

    const char cityCode[35][3] = {"11","12","13","14","15","21","22","23","31","32","33","34","35","36","37","41","42","43","44","45","46","50","51","52","53","54","61","62","63","64","65","71","81","82","91"};

    for(i=0;i<35;i++)
    {
        if(flag==0)
            break;
        if(memcmp(cityCode[i],gid,2)==0)
            flag=0;
    }

    return flag;
}

int gisdigit(char c)//检查输入的身份证号是否为数字
{
    int flag;

    if(c>='0'&&c<='9')
        flag=1;
    else
        flag=0;
    return flag;
}

int check_input_gid(char gid[500])//检查身份证号格式
{
    int gisdigit(char c);
    int checkCityCode(char gid[500]);

    int i;
    long gsum = 0;
    //加权因子
    int gr[17] ={7,9,10,5,8,4,2,1,6,3,7,9,10,5,8,4,2};
    //校验码
    char schecker[12]={'1','0','X','9','8','7','6','5','4','3','2','\0'};
    //检验长度
    if(strlen(gid)!=18)
        return -1;
    //检验数字
    for(i=0;i<18;i++)
    {
        if(gisdigit(gid[i])==0&&!(gid[i]=='X'&&i==17))
        {
            return -2;
        }
    }

    //检验省份代码
    if(checkCityCode(gid)==1)
        return -3;
    //检验校验码
    for(i=0;i<=16;i++)
    {
        gsum +=  ((gid[i]-48)*gr[i]);
    }
    if(schecker[gsum%11] != gid[17])
    {
        return -5;
    }

    return 0;
}

int check_input_rid(char rck[500])//检查输入的房间格式（全为数字）
{
    int i,flag=1;

    for(i=0;i<strlen(rck);i++)
    {
        if(flag==0)
            break;
        if(rck[i]<'0'||rck[i]>'9')
            flag = 0;
    }

    return flag;
}

void check_in()//登记入住
{
    struct Guest gst={.out_time="未退房"},gst_ck;
    FILE *fp;
    char in_time[17],itm[17],tme[13],ch[500]={"\0"};
    char gid_ck[500]={"\0"};//用于检查用户ID输入是否正确
    int flag=1;

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    strcpy(in_time,get_time_std(itm));
    strcpy(gst.in_time,in_time);

    fflush(stdin);
    system("cls");
    draw_gid();
    GetConsoleScreenBufferInfo(hConsole, &csbi);
	gotoxy(28,csbi.dwCursorPosition.Y-11);
    while(gets(gid_ck),check_input_gid(gid_ck)!=0)
    {
        if(strlen(gid_ck)==1&&gid_ck[0]=='#')
            {
                flag=0;
                break;
            }

        fflush(stdin);
        system("cls");
        draw_gid_error();
        GetConsoleScreenBufferInfo(hConsole, &csbi);
        gotoxy(28,csbi.dwCursorPosition.Y-11);
    }
    if(flag==0)
    {
        gst_menu();
    }
    else
    {
        strcpy(gst.gst_id,gid_ck);

        if(save_gst(gst))
        {
        system("cls");
        draw_check_in_succ();
        }
        else
        {
            system("cls");
            draw_check_in_fail();
        }
        fflush(stdin);
        getch();
        gst_menu();
    }
}

int chk_gst_stat_in(struct Guest *head,struct Guest gst)//检查客人能否入住
{
    int flag,f_id=0,f_stat=0;//flag==0表示客人不可以入住 f_id==0表示此前无该人的入住记录，客人是第一次入住 f_stat==0表示客人未预订
    struct Guest *p;
    char tim_bk[11]={"\0"};

    p = head;

    while(p!=NULL)
    {
        if(strcmp(p->gst_id,gst.gst_id)==0)
        {
            f_id=1;
            if(strcmp(p->in_time,"未入住")==0&&(atoi(p->indt)<=atoi(get_time_book(tim_bk)))&&(atoi(p->otdt)>=atoi(get_time_book(tim_bk))))
            {
                f_stat=1;
            }
        }
        p=p->next;
    }

    gfree(head);
    p = NULL;

    if(f_stat==1)
        flag=1;
    else
        flag=0;

    return flag;
}

int save_gst(struct Guest gst)//保存客人信息
{
    FILE *fp;
    int flag = 1;//flag==1表示客人登记成功
    struct Guest temp;
    char tim[17]={'\0'};
    char tim_bk[11]={'\0'};

    if((fp=fopen("guest.dat","rb+"))==NULL)
    {
        //printf("\n can not open this file");
        exit(0);
    }

    if(chk_gst_stat_in(creat_gst_link(),gst))
    {
        while(!feof(fp))
        {
            fread(&temp,sizeof(struct Guest),1,fp);

            if(strcmp(gst.gst_id,temp.gst_id)==0)
            {
                if(strcmp(temp.in_time,"未入住")==0&&(atoi(temp.indt)<=atoi(get_time_book(tim_bk)))&&(atoi(temp.otdt)>=atoi(get_time_book(tim_bk))))
                {
                    strcpy(temp.in_time,gst.in_time);
                    strcpy(temp.out_time,gst.out_time);
                    fseek(fp,-1*sizeof(struct Guest),1);
                    fwrite(&temp,sizeof(struct Guest),1,fp);

                    break;
                }
            }
        }
    }
    else
        flag = 0;

    fclose(fp);

    return flag;
}

int check_gst_stat_out(struct Guest *head,char gid[50])//检查客人能否退房
{
    int flag,f_id=0,f_stat=0;//flag==0表示客人无法退房 f_id==0表示此前无此人的开房记录 f_stat==0表示无此人最近的未退房记录
    struct Guest *p;

    p = head;

    while(p!=NULL)
    {
        if(strcmp(p->gst_id,gid)==0)
        {
            f_id=1;
            if(strcmp(p->out_time,"未退房")==0)
            {
                f_stat=1;
            }
        }
        p=p->next;
    }

    gfree(head);
    p=NULL;
    if(f_id==0||f_stat==0)
        flag=0;
    else
        flag=1;

    return flag;
}

void check_out()//退房
{
    FILE *fp;
    struct Guest gst={.rstat='0'};
    char tim[17]={'\0'};//存储退房时间
    char tb[11]={'\0'},tbook[11]={'\0'};//存储退房时间 精确到日期
    char gid[50]={"\0"};//存储客户身份id
    char gid_ck[500]={"\0"};//存储键盘输入，用于检查id格式
    int flag=1;
    struct room *pr;

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    fflush(stdin);
    system("cls");
    draw_gid();
    GetConsoleScreenBufferInfo(hConsole, &csbi);
	gotoxy(28,csbi.dwCursorPosition.Y-11);
    while(gets(gid_ck),check_input_gid(gid_ck)!=0)
    {
        if(strcmp(gid_ck,"#")==0)
        {
            flag=0;
            break;
        }
        fflush(stdin);
        system("cls");
        draw_gid_error();
        GetConsoleScreenBufferInfo(hConsole, &csbi);
        gotoxy(28,csbi.dwCursorPosition.Y-11);
    }

    if(flag==0)
    {
        gst_menu();
    }
    else
    {
        strcpy(gid,gid_ck);

        if((fp=fopen("guest.dat","rb+"))==NULL)
        {
            printf("\ncan not open this file\n");
            exit(0);
        }

        if(check_gst_stat_out(creat_gst_link(),gid)==0)
        {
            system("cls");
            draw_check_out_fail();
        }
        else
        {
            while(!feof(fp))
            {
                fread(&gst,sizeof(struct Guest),1,fp);

                if(strcmp(gst.gst_id,gid)==0)
                {
                    if(strcmp(gst.out_time,"未退房")==0)
                    {
                        strcpy(gst.out_time,get_time_std(tim));
                        gst.rstat = '0';
                        fseek(fp,-1*sizeof(struct Guest),1);
                        fwrite(&gst,sizeof(struct Guest),1,fp);
                        strcpy(tb,get_time_book(tbook));
                        if(atoi(tb)<=atoi(gst.otdt))//存疑 判断条件需要改动
                        {
                            pr=creat();
                            if(strcmp(gst.indt,gst.otdt)==0)
                                time_giveup_book_room(pr,gst.room_id,gst.indt);
                            else
                                giveup_book_room(pr,gst.room_id,tb,gst.otdt);
                            save(pr);
                            //Free(pr);
                        }

                        system("cls");
                        draw_check_out_succ();

                    break;
                    }
                }
            }
        }

        fclose(fp);

        fflush(stdin);
        getch();
        gst_menu();
    }
}

void show_gst_name()//按姓名显示客人
{
    int check_input_name(char name[500]);
    struct Guest *creat_gst_link();
    char info[10][20]={{"客人姓名"},{"性别"},{"年龄"},{"身份证号"},{"入住人数"},{"房间号"},{"预订入住时间"},{"预订离店时间"},{"登记时间"},{"退房时间"}};
    int f_menu=1;//f_menu用来控制菜单
    void change_gst_his();

    FILE *fp;
    struct Guest gst;
    struct Guest *pg,*ghead;
    char ch[500];
    int flag=0;//flag=0表示没有此人的历史记录

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    system("cls");
    fflush(stdin);
    draw_name();
    GetConsoleScreenBufferInfo(hConsole, &csbi);
	gotoxy(35,csbi.dwCursorPosition.Y-11);
    while(gets(ch),check_input_name(ch)==0)
    {
        if(strcmp(ch,"#")==0)
        {
            f_menu=0;
            break;
        }
        fflush(stdin);
        system("cls");
        draw_name_error();
        GetConsoleScreenBufferInfo(hConsole, &csbi);
        gotoxy(35,csbi.dwCursorPosition.Y-11);

    }
    if(f_menu==0)
        show_gst_menu();
    else
    {
        system("mode con cols=124 lines=125");
        strcpy(gst.name,ch);

        ghead=creat_gst_link();
        pg=ghead;
        find_end(pg);
        gst_sort_indt(pg,find_end(pg));
        gst_sort_otdt(pg,find_end(pg));
        system("cls");
        printf("\n\n===========================================================================================================================\n");
        printf("||%-10s %-5s%-5s %-18s  %-4s  %-6s %-12s %-12s %-17s %-16s||\n",info[0],info[1],info[2],info[3],info[4],info[5],info[6],info[7],info[8],info[9]);
        while(pg!=NULL)
        {
            if(strcmp(pg->name,gst.name)==0)
            {
                printf("||%-10s  %-4c%-5s %-18s  %-d         %-6d %-12s %-12s %-17s %-16s||\n",pg->name,pg->sex,pg->age,pg->gst_id,pg->num,pg->room_id,pg->indt,pg->otdt,pg->in_time,pg->out_time);
                flag=1;
            }
            pg=pg->next;
        }

        if(flag==0)
        {
            system("cls");
            printf("\n\n===========================================================================================================================\n");
            printf("||                                                                                                                       ||\n");
            printf("||                                             没有此人的历史记录                                                        ||\n");
        }

        printf("||                                                                                                                       ||\n");
        printf("||                                                按任意键退出                                                           ||\n");
        printf("||                                                                                                                       ||\n");
        printf("===========================================================================================================================\n");
        gfree(ghead);
        pg=NULL;
        fflush(stdin);
        getch();
        show_gst_menu();
    }
}

void show_gst_date()//按日期显示客人
{
    int check_input_date(char dck[500]);
    struct Guest *creat_gst_link();
    char info[10][20]={{"客人姓名"},{"性别"},{"年龄"},{"身份证号"},{"入住人数"},{"房间号"},{"预订入住时间"},{"预订离店时间"},{"登记时间"},{"退房时间"}};

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    struct Guest *pg,*ghead;
    char date[20]={"\0"},idt[20]={"\0"},odt[20]={"\0"};//用来存储要查询的日期
    char dck[500];//用来存储键盘输入 检查日期格式是否正确
    int i,flag=0;//flag==0表示没有该日的历史记录
    int f_menu=1;//f_menu用来控制菜单

    fflush(stdin);
    system("cls");
    draw_ck_date();
    GetConsoleScreenBufferInfo(hConsole, &csbi);
	gotoxy(35,csbi.dwCursorPosition.Y-10);

    while(gets(dck),ckdt(dck)==0)
    {
        if(strcmp(dck,"#")==0)
        {
            f_menu=0;
            break;
        }
        fflush(stdin);
        system("cls");
        draw_ck_date_error();
        GetConsoleScreenBufferInfo(hConsole, &csbi);
        gotoxy(35,csbi.dwCursorPosition.Y-10);
    }

    if(f_menu==0)
        show_gst_menu();
    else
    {
        system("mode con cols=124 lines=125");
        strcpy(date,dck);

        ghead = creat_gst_link();
        pg = ghead;
        gst_sort_indt(pg,find_end(pg));
        gst_sort_otdt(pg,find_end(pg));

        system("cls");
        printf("\n\n===========================================================================================================================\n");
        printf("||%-10s %-5s%-5s %-18s  %-4s  %-6s %-12s %-12s %-17s %-16s||\n",info[0],info[1],info[2],info[3],info[4],info[5],info[6],info[7],info[8],info[9]);
        while(pg!=NULL)
        {
            if(strcmp(date,pg->indt)==0||strcmp(date,pg->otdt)==0)
            {
                printf("||%-10s  %-4c%-5s %-18s  %-d         %-6d %-12s %-12s %-17s %-16s||\n",pg->name,pg->sex,pg->age,pg->gst_id,pg->num,pg->room_id,pg->indt,pg->otdt,pg->in_time,pg->out_time);
                flag=1;
            }
            pg=pg->next;
        }

        if(flag==0)
        {
            system("cls");
           printf("\n\n===========================================================================================================================\n");
            printf("||                                                                                                                       ||\n");
            printf("||                                             没有该日的历史记录                                                        ||\n");
        }

        printf("||                                                                                                                       ||\n");
        printf("||                                                按任意键退出                                                           ||\n");
        printf("||                                                                                                                       ||\n");
        printf("===========================================================================================================================\n");

        gfree(ghead);
        pg=NULL;
        fflush(stdin);
        getch();
        show_gst_menu();
    }
}

void show_gst_gid()//按身份证号显示客人
{
    struct Guest *creat_gst_link();
    int check_input_gid(char gid[500]);
    struct Guest *pg,*ghead;
    char gid[50];//用来存储要查找的ID
    char gid_ck[500];//存储键盘输入 检查用户ID格式
    int flag=0;//flag==0表示没有该ID的历史记录
    char info[10][20]={{"客人姓名"},{"性别"},{"年龄"},{"身份证号"},{"入住人数"},{"房间号"},{"预订入住时间"},{"预订离店时间"},{"登记时间"},{"退房时间"}};
    int f_menu=1;//控制菜单

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    fflush(stdin);
    system("cls");
    draw_gid();
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    gotoxy(28,csbi.dwCursorPosition.Y-11);

    while(gets(gid_ck),check_input_gid(gid_ck)!=0)
    {
        if(strcmp(gid_ck,"#")==0)
        {
            f_menu=0;
            break;
        }
        fflush(stdin);
        system("cls");
        draw_gid_error();
        GetConsoleScreenBufferInfo(hConsole, &csbi);
        gotoxy(28,csbi.dwCursorPosition.Y-11);
    }
    if(f_menu==0)
        show_gst_menu();
    else
    {
        system("mode con cols=124 lines=125");
        strcpy(gid,gid_ck);

        ghead=creat_gst_link();
        pg=ghead;
        gst_sort_indt(pg,find_end(pg));
        gst_sort_otdt(pg,find_end(pg));

        system("cls");
        printf("\n\n===========================================================================================================================\n");
        printf("||%-10s %-5s%-5s %-18s  %-4s  %-6s %-12s %-12s %-17s %-16s||\n",info[0],info[1],info[2],info[3],info[4],info[5],info[6],info[7],info[8],info[9]);
        while(pg!=NULL)
        {
            if(strcmp(gid,pg->gst_id)==0)
            {
                flag=1;
                printf("||%-10s  %-4c%-5s %-18s  %-d         %-6d %-12s %-12s %-17s %-16s||\n",pg->name,pg->sex,pg->age,pg->gst_id,pg->num,pg->room_id,pg->indt,pg->otdt,pg->in_time,pg->out_time);
            }
            pg=pg->next;
        }

        if(flag==0)
        {
            system("cls");
            printf("\n\n===========================================================================================================================\n");
            printf("||                                                                                                                       ||\n");
            printf("||                                             没有此人的历史记录                                                        ||\n");
        }

        printf("||                                                                                                                       ||\n");
        printf("||                                                按任意键退出                                                           ||\n");
        printf("||                                                                                                                       ||\n");
        printf("===========================================================================================================================\n");

        gfree(ghead);
        pg=NULL;
        fflush(stdin);
        getch();
        show_gst_menu();
    }

}

void show_gst_rid()//按房间号显示客人
{
    char rid_ck[500]={'\0'};//用于检查键盘输入是否正确
    int rid;//存储要查询的房间ID
    int flag=0;//flag==0表示没有该房间的历史记录
    struct Guest *pg,*ghead;
    char info[10][20]={{"客人姓名"},{"性别"},{"年龄"},{"身份证号"},{"入住人数"},{"房间号"},{"预订入住时间"},{"预订离店时间"},{"登记时间"},{"退房时间"}};
    int f_menu=1;//控制菜单

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    fflush(stdin);
    system("cls");
    draw_show_rid();
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    gotoxy(35,csbi.dwCursorPosition.Y-11);
    while(gets(rid_ck),check_input_rid(rid_ck)==0||atoi(rid_ck)<1)
    {
        if(strcmp(rid_ck,"#")==0)
        {
            f_menu=0;
            break;
        }
        fflush(stdin);
        system("cls");
        draw_show_rid_error();
        GetConsoleScreenBufferInfo(hConsole, &csbi);
        gotoxy(35,csbi.dwCursorPosition.Y-11);
    }
    if(f_menu==0)
        show_gst_menu();
    else
    {
        system("mode con cols=124 lines=125");
        rid=atoi(rid_ck);

        ghead = creat_gst_link();
        pg = ghead;
        gst_sort_indt(pg,find_end(pg));
        gst_sort_otdt(pg,find_end(pg));


        system("cls");
        printf("\n\n===========================================================================================================================\n");
        printf("||%-10s %-5s%-5s %-18s  %-4s  %-6s %-12s %-12s %-17s %-16s||\n",info[0],info[1],info[2],info[3],info[4],info[5],info[6],info[7],info[8],info[9]);
        while(pg != NULL)
        {
            if(pg->room_id == rid)
            {
                flag=1;
                printf("||%-10s  %-4c%-5s %-18s  %-d         %-6d %-12s %-12s %-17s %-16s||\n",pg->name,pg->sex,pg->age,pg->gst_id,pg->num,pg->room_id,pg->indt,pg->otdt,pg->in_time,pg->out_time);
            }
            pg=pg->next;
        }

        if(flag==0)
        {
            system("cls");
            printf("\n\n===========================================================================================================================\n");
            printf("||                                                                                                                       ||\n");
            printf("||                                            没有该房间的历史记录                                                       ||\n");
        }

        printf("||                                                                                                                       ||\n");
        printf("||                                                按任意键退出                                                           ||\n");
        printf("||                                                                                                                       ||\n");
        printf("===========================================================================================================================\n");

        gfree(ghead);
        pg = NULL;
        fflush(stdin);
        getch();
        show_gst_menu();
    }
}

void show_gst_all()//显示所有客人
{
    struct Guest *pg,*ghead;
    int flag=0;
    char info[10][20]={{"客人姓名"},{"性别"},{"年龄"},{"身份证号"},{"入住人数"},{"房间号"},{"预订入住时间"},{"预订离店时间"},{"登记时间"},{"退房时间"}};
    system("mode con cols=124 lines=125");

    ghead = creat_gst_link();
    pg = ghead;
    gst_sort_indt(pg,find_end(pg));
    gst_sort_otdt(pg,find_end(pg));


    system("cls");
    printf("\n\n===========================================================================================================================\n");
    printf("||%-10s %-5s%-5s %-18s  %-4s  %-6s %-12s %-12s %-17s %-16s||\n",info[0],info[1],info[2],info[3],info[4],info[5],info[6],info[7],info[8],info[9]);
    while(pg != NULL)
    {
        flag=1;
        printf("||%-10s  %-4c%-5s %-18s  %-d         %-6d %-12s %-12s %-17s %-16s||\n",pg->name,pg->sex,pg->age,pg->gst_id,pg->num,pg->room_id,pg->indt,pg->otdt,pg->in_time,pg->out_time);
        pg=pg->next;
    }
    if(flag==0)
    {
        system("cls");
        printf("\n\n===========================================================================================================================\n");
        printf("||                                                                                                                       ||\n");
        printf("||                                              没有历史记录                                                             ||\n");
    }

    printf("||                                                                                                                       ||\n");
    printf("||                                                按任意键退出                                                           ||\n");
    printf("||                                                                                                                       ||\n");
    printf("===========================================================================================================================\n");

    gfree(ghead);
    pg = NULL;
    fflush(stdin);
    getch();
    show_gst_menu();
}

int input(struct Guest *p)//预订房间时的客人信息输入
{
    char ch[500];//检查用户输入

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    system("cls");
    system("mode con cols=80 lines=25");
    fflush(stdin);
    draw_name();
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    gotoxy(35,csbi.dwCursorPosition.Y-11);

    while(gets(ch),check_input_name(ch)==0)
    {
        if(strlen(ch)==1&&ch[0]=='#')
        {
            return 0;
        }
        fflush(stdin);
        system("cls");
        draw_name_error();
        GetConsoleScreenBufferInfo(hConsole, &csbi);
        gotoxy(35,csbi.dwCursorPosition.Y-11);

    }
    strcpy(p->name,ch);

        system("cls");
        fflush(stdin);
        draw_sex();
        GetConsoleScreenBufferInfo(hConsole, &csbi);
        gotoxy(35,csbi.dwCursorPosition.Y-11);
        while(gets(ch),(strcmp(ch,"m") !=0 && strcmp(ch,"w") != 0))
        {
            if(strlen(ch)==1&&ch[0]=='#')
            {
                return 0;
            }
            fflush(stdin);
            system("cls");
            draw_sex_error();
            GetConsoleScreenBufferInfo(hConsole, &csbi);
            gotoxy(35,csbi.dwCursorPosition.Y-11);
        }
        p->sex=ch[0];

        system("cls");
        fflush(stdin);
        draw_age();
        GetConsoleScreenBufferInfo(hConsole, &csbi);
        gotoxy(36,csbi.dwCursorPosition.Y-11);
        while(gets(ch),check_input_age(ch)==0)
        {
            if(strlen(ch)==1&&ch[0]=='#')
            {
                return 0;
            }
            fflush(stdin);
            system("cls");
            draw_age_error();
            GetConsoleScreenBufferInfo(hConsole, &csbi);
            gotoxy(36,csbi.dwCursorPosition.Y-11);

        }
        strcpy(p->age,ch);

    fflush(stdin);
    system("cls");
    draw_gid();
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    gotoxy(28,csbi.dwCursorPosition.Y-11);

    while(gets(ch),check_input_gid(ch)!=0)
    {
        if(strlen(ch)==1&&ch[0]=='#')
            {
                return 0;
            }
        fflush(stdin);
        system("cls");
        draw_gid_error();
        GetConsoleScreenBufferInfo(hConsole, &csbi);
        gotoxy(28,csbi.dwCursorPosition.Y-11);
    }
    strcpy(p->gst_id,ch);

    return 1;
}

void reserve_time()//预订钟点房
{
    struct Guest gst={.in_time="未入住",.out_time="\0",.rstat='1'};
    struct room *proom;
    struct room *pr;
    char dtck[500];//检查时间输入
    char tb[11]={'\0'};//获取当前时间 检查预订入住日期是否小于当前
    char rid_ck[500]={'\0'};//检查房间号输入
    char num_ck[500]={'\0'};//检查同行人数输入
    FILE *fp;

    if(input_date_reserve(dtck,0)==0)
        gst_menu();
    else
    {//1
        strcpy(gst.indt,dtck);
        strcpy(gst.otdt,dtck);
        if((atoi(gst.indt)<atoi(get_time_book(tb)))||atoi(gst.indt)>20181231||atoi(gst.indt)<20180101||atoi(gst.otdt)>20181231||atoi(gst.otdt)<20180101)
        {
            system("cls");
            draw_date_error();
            fflush(stdin);
            getch();
            gst_menu();
        }
        if(input_num_reserve(num_ck)==0)
        {
            gst_menu();
        }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        else
        {//2

            gst.num=atoi(&num_ck[0]);
            switch(gst.num)
            {
            case 1:
                pr = choose(find_freeroom(creat(),gst.indt,gst.otdt),10,0,0);
                break;
            case 2:
                pr = choose(find_freeroom(creat(),gst.indt,gst.otdt),10,0,0);
                break;
            case 3:
                pr = choose(find_freeroom(creat(),gst.indt,gst.otdt),10,0,0);
                break;

            }
            system("cls");
            system("mode con cols=110 lines=40");
            serch(pr,1);
            if(pr==NULL)
            {
                printf("\n按任意键退出");
                getch();
                gst_menu();
            }
            else
            {//3
                HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                CONSOLE_SCREEN_BUFFER_INFO csbi;

                fflush(stdin);
                printf("\t\t\t------------------------------------------------------------\n"
                                "\t\t\t************************************************************\n"
                                "\t\t\t||                     输入#退出                          ||\n"
                                "\t\t\t||                                                        ||\n"
                                "\t\t\t||                请输入要预订的房间号                    ||\n"
                                "\t\t\t||                                                        ||\n"
                                "\t\t\t||                                                        ||\n"
                                "\t\t\t************************************************************\n"
                                "\t\t\t------------------------------------------------------------\n");
            GetConsoleScreenBufferInfo(hConsole, &csbi);
                gotoxy(50,csbi.dwCursorPosition.Y-4);

                if(input_rid_reserve(rid_ck,pr)==0)
                {
                    gst_menu();
                    //Free(pr);
                }
                else
                {//4
                    system("mode con cols=80 lines=25");

                    gst.room_id=atoi(rid_ck);

                    if(check_room(gst,pr)==0)//返回0 表示房间不可选
                    {
                        system("cls");
                        draw_room_error();
                        //Free(pr);
                        fflush(stdin);
                        getch();
                        gst_menu();
                    }
                    else
                    {//5
                        //Free(pr);
                        if(input(&gst)==0)
                        {
                            gst_menu();
                            //Ffree(pr);
                        }
                        else
                        {//6
                            if(check_gst_stat_reserve(gst)==1)//返回1 表示可以预订
                            {
                                strcpy(gst.in_time,"未入住");
                                strcpy(gst.out_time,"\0");
                                gst.rstat='1';
                                proom=creat();
                                time_book_room(proom,gst.room_id,gst.indt);
                                save(proom);
                                fp=fopen("guest.dat","ab+");
                                fwrite(&gst,sizeof(struct Guest),1,fp);
                                fclose(fp);
                                system("cls");
                                draw_book_succ();
                                //Free(proom);
                                //free(pr);
                                fflush(stdin);
                                getch();
                                gst_menu();
                            }
                            else
                            {
                                system("cls");
                                draw_book_faul();
                                //free(pr);
                                fflush(stdin);
                                getch();
                                gst_menu();
                            }
                        }//6
                    }//5
                }//4
            }//3
        }//2
    }//1
}

int input_reserve_choose(char ch_ck[500])//检查询问是否预订钟点房的输入（y/n）
{
    int flag=1;

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    system("cls");
    fflush(stdin);
    draw_time_reserve();
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    gotoxy(35,csbi.dwCursorPosition.Y-11);

    while(gets(ch_ck),strcmp(ch_ck,"y")!=0&&strcmp(ch_ck,"n")!=0)
    {
        if(strlen(ch_ck)==1&&ch_ck[0]=='#')
        {
            flag=0;
            break;
        }
        else
        {
            system("cls");
            fflush(stdin);
            draw_time_reserve_error();
            GetConsoleScreenBufferInfo(hConsole, &csbi);
            gotoxy(35,csbi.dwCursorPosition.Y-11);
        }

    }
    return flag;
}

void reserve_choose()//预订 询问是否预订钟点房
{
    void reserve();
    void reserve_time();

    void reserve_time();
    char ch_ck[500];
    char choice;

    if(input_reserve_choose(ch_ck)==0)
        gst_menu();
    else
    {
        choice = ch_ck[0];

        switch(choice)
        {
        case 'y':
            reserve_time();
            break;
        case 'n':
            reserve();
            break;
        }
    }

}

int input_date_reserve(char dtck[500],int a)//输入日期 预订时 a==0表示输入入住时间 a==1表示输入离店时间
{
    int flag=1;

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    system("cls");
    fflush(stdin);
    if(a==0)
    {
        draw_indt();
        GetConsoleScreenBufferInfo(hConsole, &csbi);
        gotoxy(29,csbi.dwCursorPosition.Y-11);
    }
    else if(a==1)
    {
        draw_otdt();
        GetConsoleScreenBufferInfo(hConsole, &csbi);
        gotoxy(29,csbi.dwCursorPosition.Y-11);
    }
    while(gets(dtck),ckdt(dtck)==0)
    {
        if(strlen(dtck)==1&&dtck[0]=='#')
        {
            flag=0;
            break;
        }
        else
        {
            system("cls");
            fflush(stdin);
            if(a==0)
            {
                draw_indt_error();
                GetConsoleScreenBufferInfo(hConsole, &csbi);
                gotoxy(29,csbi.dwCursorPosition.Y-11);
            }
            else if(a==1)
            {
                draw_otdt_error();
                GetConsoleScreenBufferInfo(hConsole, &csbi);
                gotoxy(29,csbi.dwCursorPosition.Y-11);
            }
        }

    }
    return flag;
}

int input_num_reserve(char num_ck[500])//输入入住人数 预订时
{
    int flag=1;

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    system("cls");
    fflush(stdin);
    draw_num_with();
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    gotoxy(34,csbi.dwCursorPosition.Y-11);
    while(gets(num_ck),strlen(num_ck)>1||(num_ck[0]!='3'&&num_ck[0]!='1'&&num_ck[0]!='2'))
    {
        if(num_ck[0]=='#')
        {
            flag=0;
            break;
        }
        system("cls");
        fflush(stdin);
        draw_num_with_error();
        GetConsoleScreenBufferInfo(hConsole, &csbi);
        gotoxy(34,csbi.dwCursorPosition.Y-11);
    }
    return flag;
}

int input_rid_reserve(char rid_ck[500],struct room *pr)//输入入住的房间号 预订时
{
    int flag=1;

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    while(gets(rid_ck),check_input_rid(rid_ck)==0||atoi(rid_ck)<1)
    {
        if(strlen(rid_ck)==1&&rid_ck[0]=='#')
        {
            flag=0;
            break;
        }
        fflush(stdin);
        system("cls");
        serch(pr,1);
        printf("\t\t\t------------------------------------------------------------\n"
                                "\t\t\t************************************************************\n"
                                "\t\t\t||                     输入#退出                          ||\n"
                                "\t\t\t||                     请正确输入！                       ||\n"
                                "\t\t\t||                请输入要预订的房间号                    ||\n"
                                "\t\t\t||                                                        ||\n"
                                "\t\t\t||                                                        ||\n"
                                "\t\t\t************************************************************\n"
                                "\t\t\t------------------------------------------------------------\n");
                        GetConsoleScreenBufferInfo(hConsole, &csbi);
                        gotoxy(50,csbi.dwCursorPosition.Y-4);

    }

    return flag;
}

void reserve()//预订非钟点房
{
    struct Guest gst={.in_time="未入住",.out_time="\0",.rstat='1'};
    struct room *proom;
    struct room *pr;
    char dtck[500];//检查时间输入
    char tb[11]={'\0'};//获取当前时间 检查预订入住日期是否小于当前
    char rid_ck[500]={'\0'};//检查房间号输入
    char num_ck[500]={'\0'};//检查同行人数输入
    FILE *fp;

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    if(input_date_reserve(dtck,0)==0)
        gst_menu();
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    else
    {//1
        strcpy(gst.indt,dtck);

        if(input_date_reserve(dtck,1)==0)
            gst_menu();
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        else
        {//2
            strcpy(gst.otdt,dtck);
            if((atoi(gst.indt)>=atoi(gst.otdt))||(atoi(gst.indt)<atoi(get_time_book(tb)))||atoi(gst.indt)>20181231||atoi(gst.indt)<20180101||atoi(gst.otdt)>20181231||atoi(gst.otdt)<20180101)
            {
                system("cls");
                draw_date_error();
                fflush(stdin);
                getch();
                gst_menu();
            }

            else
            {//3

                if(input_num_reserve(num_ck)==0)
                {
                    gst_menu();
                }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                else
                {//4

                    gst.num=atoi(&num_ck[0]);
                    switch(gst.num)
                    {
                    case 1:
                        pr = choose(find_freeroom(creat(),gst.indt,gst.otdt),15,0,0);
                        break;
                    case 2:
                        pr = choose(find_freeroom(creat(),gst.indt,gst.otdt),14,0,0);
                        break;
                    case 3:
                        pr = choose(find_freeroom(creat(),gst.indt,gst.otdt),9,0,0);
                        break;

                    }
                    system("cls");
                    system("mode con cols=110 lines=40");
                    serch(pr,1);
                    if(pr==NULL)
                    {
                        printf("\n按任意键退出");
                        getch();
                        gst_menu();
                    }
                    else
                    {//5
                        //free(pr);

                        GetConsoleScreenBufferInfo(hConsole, &csbi);
                        fflush(stdin);
                        printf("\t\t\t------------------------------------------------------------\n"
                                "\t\t\t************************************************************\n"
                                "\t\t\t||                     输入#退出                          ||\n"
                                "\t\t\t||                                                        ||\n"
                                "\t\t\t||                请输入要预订的房间号                    ||\n"
                                "\t\t\t||                                                        ||\n"
                                "\t\t\t||                                                        ||\n"
                                "\t\t\t************************************************************\n"
                                "\t\t\t------------------------------------------------------------\n");
                        GetConsoleScreenBufferInfo(hConsole, &csbi);
                        gotoxy(50,csbi.dwCursorPosition.Y-4);

                        if(input_rid_reserve(rid_ck,pr)==0)
                        {
                            gst_menu();
                            //Free(pr);
                        }
                        else
                        {//6

                            gst.room_id=atoi(rid_ck);
                            system("mode con cols=80 lines=25");

                            if(check_room(gst,pr)==0)//返回0 表示房间不可选
                            {
                                system("cls");
                                draw_room_error();
                                //Free(pr);
                                fflush(stdin);
                                getch();
                                gst_menu();
                            }
                            else
                            {//7
                               // Free(pr);
                                if(input(&gst)==0)
                                    gst_menu();
                                else
                                {//8
                                    if(check_gst_stat_reserve(gst)==1)//返回1 表示可以预订
                                    {
                                        strcpy(gst.in_time,"未入住");
                                        strcpy(gst.out_time,"\0");
                                        gst.rstat='1';
                                        proom=creat();
                                        book_room(proom,gst.room_id,gst.indt,gst.otdt);
                                        save(proom);
                                        fp=fopen("guest.dat","ab+");
                                        fwrite(&gst,sizeof(struct Guest),1,fp);
                                        fclose(fp);
                                        system("cls");
                                        draw_book_succ();
                                       // Free(proom);
                                        fflush(stdin);
                                        getch();
                                        gst_menu();
                                    }
                                    else
                                    {
                                        system("cls");
                                        draw_book_faul();
                                        fflush(stdin);
                                        getch();
                                        gst_menu();
                                    }
                                }//8
                            }//7
                        }//6
                    }//5
                }//4
            }//3
        }//2
    }//1
}

int ckdt(char dtck[500])//检查预订时的时间输入
{
    int i,flag=1,d,m;//flag==0表示输入不符合格式 d存储日期 m存储月份
    char mck[3]={"\0"},dck[3]={"\0"};//mck检查月份是否正常 dck检查日期是否正常

    if(strlen(dtck)>8)
        flag=0;
    for(i=0;i<8;i++)
    {
        if(flag==0)
            break;
        if(dtck[i]<'0' || dtck[i]>'9')
        {
            flag=0;
        }
    }

    if(flag==1)
    {
        mck[0]=dtck[4];
        mck[1]=dtck[5];
        dck[0]=dtck[6];
        dck[1]=dtck[7];

        m = atoi(mck);
        d = atoi(dck);
        switch(m)
        {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            if(d>31)
                flag=0;
            break;
        case 4:
        case 6:
        case 9:
        case 11:
            if(d>30)
                flag=0;
            break;
        case 2:
            if(d>28)
                flag=0;
            break;
        default:
            flag=0;
            break;

        }
    }

    return flag;
}

int check_gst_stat_reserve(struct Guest gst)//检查客人是否可以预订
{
    int flag,f_id=0,f_stat=1;//flag==0表示客人不可以预订 f_id==0表示此前无该人的预订记录，客人是第一次预订 f_stat==0表示客人不在预订状态
    struct Guest *pg,*ghead;

    ghead = creat_gst_link();
    pg = ghead;

    while(pg!=NULL)
    {
        if(strcmp(pg->gst_id,gst.gst_id)==0)
        {
            f_id=1;
            if(pg->rstat == '0')
            {
                f_stat=0;
            }
            else
                f_stat=1;
        }
        pg=pg->next;
    }

    if(f_id==0||f_stat==0)
        flag=1;
    else
        flag=0;

    gfree(ghead);
    pg = NULL;

    return flag;
}

int check_gst_stat_giveuprsv(struct Guest gst)//检查客人是否可以取消预订
{
    int flag=0;//flag==0表示客人不可以取消预订
    struct Guest *pg,*ghead;
    char tim[11];

    ghead = creat_gst_link();
    pg=ghead;

    while(pg!=NULL)
    {
        if(strcmp(pg->gst_id,gst.gst_id)==0)
        {
            if(pg->rstat == '1'&&(strcmp(pg->in_time,"未入住")==0)&&atoi(pg->otdt)>=atoi(get_time_book(tim)))
            {
                flag=1;
                break;
            }
        }
        pg=pg->next;
    }

    gfree(ghead);
    pg=NULL;
    return flag;
}

int check_room(struct Guest gst,struct room *pr)//检查客人所选房间能否被预订
{
    struct room *proom;
    int flag=0;//flag=0表示输入的房间不在可选范围内

    proom=pr;

    while(proom!=NULL)
    {
        if(proom->id==gst.room_id)
        {
            flag=1;
            break;
        }
        proom=proom->next;
    }

    //free(p);
    return flag;
}

void giveup_reserve()//取消预订
{
    FILE *fp;
    struct Guest gst={.rstat='0'};
    struct room *pr;
    char gid[50];//存储客户身份id
    char gid_ck[500]={"\0"};//存储键盘输入，用于检查id格式
    int flag=1;

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    system("cls");
    fflush(stdin);
    draw_gid();
    GetConsoleScreenBufferInfo(hConsole, &csbi);
	gotoxy(28,csbi.dwCursorPosition.Y-11);
    while(gets(gid_ck),check_input_gid(gid_ck)!=0)
    {
        if(strcmp(gid_ck,"#")==0)
        {
            flag=0;
            break;
        }
        fflush(stdin);
        system("cls");
        draw_gid_error();
        GetConsoleScreenBufferInfo(hConsole, &csbi);
        gotoxy(28,csbi.dwCursorPosition.Y-11);
    }

    if(flag==0)
        gst_menu();
    else
    {
        strcpy(gid,gid_ck);
        strcpy(gst.gst_id,gid_ck);

        if(check_gst_stat_giveuprsv(gst)==1)
        {
            if((fp=fopen("guest.dat","rb+"))==NULL)
            {
                printf("\ncan not open this file\n");
                exit(0);
            }

            while(!feof(fp))
            {
                fread(&gst,sizeof(struct Guest),1,fp);

                if(strcmp(gst.gst_id,gid)==0)
                {
                    if(gst.rstat=='1')
                    {
                        pr=creat();
                        strcpy(gst.in_time,"已取消");
                        strcpy(gst.out_time,"已取消");
                        gst.rstat = '0';
                        fseek(fp,-1*sizeof(struct Guest),1);
                        fwrite(&gst,sizeof(struct Guest),1,fp);
                        if(strcmp(gst.indt,gst.otdt)==0)
                            time_giveup_book_room(pr,gst.room_id,gst.indt);
                        else
                            giveup_book_room(pr,gst.room_id,gst.indt,gst.otdt);
                        save(pr);
                        system("cls");
                        draw_giveup_reserve_succ();

                       // Free(pr);
                        break;
                    }
                }
            }
            fclose(fp);
        }
        else
        {
            system("cls");
            draw_giveup_reserve_fail();
        }

        fflush(stdin);
        getch();
        gst_menu();
    }
}

int cmp_mon(int mon,struct Guest gst)//统计某月有几人入住
{
    char dateFirst[10]={"20180100"};
    char dateSecond[10]={"20180200"};
    int flag;//flag==1表示入住时间在这个月内

    if(mon<10)
    {
        dateFirst[5]=mon+48;
        if(mon==9)
        {
            dateSecond[4]='1';
            dateSecond[5]='0';
        }
        else
            dateSecond[5]=mon+49;
    }

    else if(mon>=10)
    {
        dateFirst[4]='1';
        dateFirst[5]=mon-10+48;
        dateSecond[4]='1';
        dateSecond[5]=mon-10+49;
    }
    if(atoi(dateFirst)<atoi(gst.indt)&&atoi(gst.indt)<atoi(dateSecond))
        flag=1;
    else
        flag=0;
    return flag;
}

void counting_gst(struct room *rhead)//客人信息统计
{
    int cmp_mon(int mon,struct Guest gst);
    struct Guest *creat_gst_link();

    struct Guest *ghead,*pg;
    int population,mon;

    ghead=creat_gst_link();
    pg=ghead;
    population=0;

    system("mode con cols=80 lines=25");
    printf("\n\n\n\n\t*******=========================================*******\n");

    while(pg!=NULL)
    {
        if(strcmp(pg->in_time,"未入住")!=0&&strcmp(pg->in_time,"已取消")!=0)
        {
            population += pg->num;
        }
        pg=pg->next;
    }

    printf("\t*  *  *||           总共有%-3d人入住          ||*  *  *\n",population);

    population=0;
    pg=ghead;
    while(pg!=NULL)
    {
        if(pg->sex=='m'&&strcmp(pg->in_time,"未入住")!=0&&strcmp(pg->in_time,"已取消")!=0)
            population += pg->num;
        pg = pg->next;
    }
    printf("\t*  *  *||男性有%-3d人\t",population);

    population=0;
    pg=ghead;
    while(pg!=NULL)
    {
        if(pg->sex=='w'&&strcmp(pg->in_time,"未入住")!=0&&strcmp(pg->in_time,"已取消")!=0)
            population += pg->num;
        pg = pg->next;
    }
    gotoxy(42,6);
    printf("女性有%-3d人||*  *  *\n",population);

    for(mon=1;mon<=6;mon++)
    {
        population=0;
        pg=ghead;
        while(pg!=NULL)
        {
            if(cmp_mon(mon,*pg)==1&&strcmp(pg->in_time,"未入住")!=0&&strcmp(pg->in_time,"已取消")!=0)
                population+=pg->num;
            pg=pg->next;
        }
        printf("\t*  *  *||%-2d月有%-3d人入住\t\n",mon,population);
    }

    for(mon=7;mon<=12;mon++)
    {
        population=0;
        pg=ghead;
        while(pg!=NULL)
        {
            if(cmp_mon(mon,*pg)==1&&strcmp(pg->in_time,"未入住")!=0&&strcmp(pg->in_time,"已取消")!=0)
                population+=pg->num;
            pg=pg->next;
        }
        gotoxy(38,mon);
        printf("%-2d月有%-3d人入住||*  *  *\n",mon,population);
    }

    population=0;
    pg=ghead;
    while(pg!=NULL)
    {
        if(atoi(pg->age)>=18&&atoi(pg->age)<=30&&strcmp(pg->in_time,"未入住")!=0&&strcmp(pg->in_time,"已取消")!=0)
            population += pg->num;
        pg = pg->next;
    }
    printf("\t*  *  *||18岁到30岁%-3d人\t",population);

    population=0;
    pg=ghead;
    while(pg!=NULL)
    {
        if(atoi(pg->age)>=31&&atoi(pg->age)<=45&&strcmp(pg->in_time,"未入住")!=0&&strcmp(pg->in_time,"已取消")!=0)
            population += pg->num;
        pg = pg->next;
    }
    gotoxy(38,13);
    printf("31岁到45岁%-3d人||*  *  *\n",population);

    population=0;
    pg=ghead;
    while(pg!=NULL)
    {
        if(atoi(pg->age)>=46&&atoi(pg->age)<=60&&strcmp(pg->in_time,"未入住")!=0&&strcmp(pg->in_time,"已取消")!=0)
            population += pg->num;
        pg = pg->next;
    }
    printf("\t*  *  *||46岁到60岁%-3d人\t",population);

    population=0;
    pg=ghead;
    while(pg!=NULL)
    {
        if(atoi(pg->age)>=61&&atoi(pg->age)<=120&&strcmp(pg->in_time,"未入住")!=0&&strcmp(pg->in_time,"已取消")!=0)
            population += pg->num;
        pg = pg->next;
    }
    gotoxy(38,14);
    printf("60岁以上有%-3d人||*  *  *\n",population);

    population = 0;
    pg=ghead;
    while(pg!=NULL)
    {
        if(pg->num==1&&strcmp(pg->in_time,"未入住")!=0&&strcmp(pg->in_time,"已取消")!=0)
            population++;
        pg=pg->next;
    }
    printf("\t*  *  *||一人入住有%-3d次\t",population);

    population = 0;
    pg=ghead;

    while(pg!=NULL)
    {
        if(pg->num==2&&strcmp(pg->in_time,"未入住")!=0&&strcmp(pg->in_time,"已取消")!=0)
            population++;
        pg=pg->next;
    }
    gotoxy(38,15);
    printf("两人入住有%-3d次||*  *  *\n",population);

    population = 0;
    pg=ghead;

    while(pg!=NULL)
    {
        if(pg->num==3&&strcmp(pg->in_time,"未入住")!=0&&strcmp(pg->in_time,"已取消")!=0)
            population++;
        pg=pg->next;
    }
    printf("\t*  *  *||三人入住有%-3d次\t\t     ||*  *  *\n",population);

    population=0;
    pg=ghead;
    while(pg!=NULL)
    {
        if(strcmp(pg->out_time,"未退房")==0)
            population += pg->num;
        pg = pg->next;
    }
    printf("\t*  *  *||           当前有%-3d人在住          ||*  *  *\n",population);

    gfree(ghead);
    pg=NULL;
    fflush(stdin);
    printf("\t*  *  *||            按任意键返回            ||*  *  *\n");

    printf("\t*******=========================================*******\n");

    getch();
    Roomstat(rhead);
}

