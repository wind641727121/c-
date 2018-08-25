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
    char in_time[17];//ʵ�ʵĵǼ���סʱ��
    char out_time[17];//ʵ�ʵ��˷�ʱ��
    char indt[9];//Ԥ������סʱ��
    char otdt[9];//Ԥ�������ʱ��
    char rstat;//�洢����Ԥ����״̬ rstat==1��ʾ��Ԥ��״̬ rstat==0��ʾ����Ԥ��״̬
    int room_id;
    char gst_id[50];
    int num;//ͬס����
    struct Guest *next;
};

int gotoxy(int x, int y) //��λ���λ��
{
    HANDLE handle; //����������handle
    COORD coord; //����ṹ��coord (����ϵcoord)
    coord.X = x; //������x
    coord.Y = y; //������y
    handle = GetStdHandle(STD_OUTPUT_HANDLE); //��ȡ����̨������(ֵΪ-11)
    SetConsoleCursorPosition(handle, coord); //�ƶ����
}

void gfree(struct Guest *head)//�ͷŶ�̬�洢�ռ�
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

void copy_gst(struct Guest *p1,struct Guest tmp)//����ȡ����Ϣ���Ƶ�ָ��ָ��Ĵ洢�ռ�
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

struct Guest *creat_gst_link()//�������˵Ķ�̬����
{
    void copy_gst(struct Guest *p1,struct Guest tmp);

    FILE *fp;
    struct Guest *head,*p1,*p2;
    int i=0;//i������¼������ĸ���
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

struct Guest *find_end(struct Guest *head)//Ѱ�������β���
{
    struct Guest *p;
    p=head;
    while(head!=NULL&&p->next!=NULL)//ע�� head!=NULLҪ����ǰ�� ����������
    {
        p=p->next;
    }
    return p;
}

void swap_gst(struct Guest *p1,struct Guest *p2)//������������ֵ
{
    struct Guest tmp;//��ֹ����

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

void gst_sort_indt(struct Guest *pBegin,struct Guest *pEnd)//����Ԥ����ס��������
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

void gst_sort_otdt(struct Guest *pBegin,struct Guest *pEnd)//����Ԥ�������������
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


char *get_time_book(char tim[11])//��ȡϵͳʱ�� ��ȷ���� ��ʽΪ20180101
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

char *get_time_std(char tim[17])//��ȡϵͳʱ�� ��ȷ���� ��ʽΪ2018/01/01/11:30
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

int check_input_name(char name[500])//�������������Ƿ�Ϊ���֣�����������֣�
{
    int i,j,flag=1;
    char punctuation[]={"����@#����������������������������������������"};

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

int check_input_age(char age[500])//�����������䣨�Ƿ�Ϊ���� 18~120��
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

int checkCityCode(char gid[500])//�����������֤���Ƿ���ʡ�ݴ���
{
    int flag=1;//flag==1��ʾ���鲻�ϸ�
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

int gisdigit(char c)//�����������֤���Ƿ�Ϊ����
{
    int flag;

    if(c>='0'&&c<='9')
        flag=1;
    else
        flag=0;
    return flag;
}

int check_input_gid(char gid[500])//������֤�Ÿ�ʽ
{
    int gisdigit(char c);
    int checkCityCode(char gid[500]);

    int i;
    long gsum = 0;
    //��Ȩ����
    int gr[17] ={7,9,10,5,8,4,2,1,6,3,7,9,10,5,8,4,2};
    //У����
    char schecker[12]={'1','0','X','9','8','7','6','5','4','3','2','\0'};
    //���鳤��
    if(strlen(gid)!=18)
        return -1;
    //��������
    for(i=0;i<18;i++)
    {
        if(gisdigit(gid[i])==0&&!(gid[i]=='X'&&i==17))
        {
            return -2;
        }
    }

    //����ʡ�ݴ���
    if(checkCityCode(gid)==1)
        return -3;
    //����У����
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

int check_input_rid(char rck[500])//�������ķ����ʽ��ȫΪ���֣�
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

void check_in()//�Ǽ���ס
{
    struct Guest gst={.out_time="δ�˷�"},gst_ck;
    FILE *fp;
    char in_time[17],itm[17],tme[13],ch[500]={"\0"};
    char gid_ck[500]={"\0"};//���ڼ���û�ID�����Ƿ���ȷ
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

int chk_gst_stat_in(struct Guest *head,struct Guest gst)//�������ܷ���ס
{
    int flag,f_id=0,f_stat=0;//flag==0��ʾ���˲�������ס f_id==0��ʾ��ǰ�޸��˵���ס��¼�������ǵ�һ����ס f_stat==0��ʾ����δԤ��
    struct Guest *p;
    char tim_bk[11]={"\0"};

    p = head;

    while(p!=NULL)
    {
        if(strcmp(p->gst_id,gst.gst_id)==0)
        {
            f_id=1;
            if(strcmp(p->in_time,"δ��ס")==0&&(atoi(p->indt)<=atoi(get_time_book(tim_bk)))&&(atoi(p->otdt)>=atoi(get_time_book(tim_bk))))
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

int save_gst(struct Guest gst)//���������Ϣ
{
    FILE *fp;
    int flag = 1;//flag==1��ʾ���˵Ǽǳɹ�
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
                if(strcmp(temp.in_time,"δ��ס")==0&&(atoi(temp.indt)<=atoi(get_time_book(tim_bk)))&&(atoi(temp.otdt)>=atoi(get_time_book(tim_bk))))
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

int check_gst_stat_out(struct Guest *head,char gid[50])//�������ܷ��˷�
{
    int flag,f_id=0,f_stat=0;//flag==0��ʾ�����޷��˷� f_id==0��ʾ��ǰ�޴��˵Ŀ�����¼ f_stat==0��ʾ�޴��������δ�˷���¼
    struct Guest *p;

    p = head;

    while(p!=NULL)
    {
        if(strcmp(p->gst_id,gid)==0)
        {
            f_id=1;
            if(strcmp(p->out_time,"δ�˷�")==0)
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

void check_out()//�˷�
{
    FILE *fp;
    struct Guest gst={.rstat='0'};
    char tim[17]={'\0'};//�洢�˷�ʱ��
    char tb[11]={'\0'},tbook[11]={'\0'};//�洢�˷�ʱ�� ��ȷ������
    char gid[50]={"\0"};//�洢�ͻ����id
    char gid_ck[500]={"\0"};//�洢�������룬���ڼ��id��ʽ
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
                    if(strcmp(gst.out_time,"δ�˷�")==0)
                    {
                        strcpy(gst.out_time,get_time_std(tim));
                        gst.rstat = '0';
                        fseek(fp,-1*sizeof(struct Guest),1);
                        fwrite(&gst,sizeof(struct Guest),1,fp);
                        strcpy(tb,get_time_book(tbook));
                        if(atoi(tb)<=atoi(gst.otdt))//���� �ж�������Ҫ�Ķ�
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

void show_gst_name()//��������ʾ����
{
    int check_input_name(char name[500]);
    struct Guest *creat_gst_link();
    char info[10][20]={{"��������"},{"�Ա�"},{"����"},{"���֤��"},{"��ס����"},{"�����"},{"Ԥ����סʱ��"},{"Ԥ�����ʱ��"},{"�Ǽ�ʱ��"},{"�˷�ʱ��"}};
    int f_menu=1;//f_menu�������Ʋ˵�
    void change_gst_his();

    FILE *fp;
    struct Guest gst;
    struct Guest *pg,*ghead;
    char ch[500];
    int flag=0;//flag=0��ʾû�д��˵���ʷ��¼

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
            printf("||                                             û�д��˵���ʷ��¼                                                        ||\n");
        }

        printf("||                                                                                                                       ||\n");
        printf("||                                                ��������˳�                                                           ||\n");
        printf("||                                                                                                                       ||\n");
        printf("===========================================================================================================================\n");
        gfree(ghead);
        pg=NULL;
        fflush(stdin);
        getch();
        show_gst_menu();
    }
}

void show_gst_date()//��������ʾ����
{
    int check_input_date(char dck[500]);
    struct Guest *creat_gst_link();
    char info[10][20]={{"��������"},{"�Ա�"},{"����"},{"���֤��"},{"��ס����"},{"�����"},{"Ԥ����סʱ��"},{"Ԥ�����ʱ��"},{"�Ǽ�ʱ��"},{"�˷�ʱ��"}};

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    struct Guest *pg,*ghead;
    char date[20]={"\0"},idt[20]={"\0"},odt[20]={"\0"};//�����洢Ҫ��ѯ������
    char dck[500];//�����洢�������� ������ڸ�ʽ�Ƿ���ȷ
    int i,flag=0;//flag==0��ʾû�и��յ���ʷ��¼
    int f_menu=1;//f_menu�������Ʋ˵�

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
            printf("||                                             û�и��յ���ʷ��¼                                                        ||\n");
        }

        printf("||                                                                                                                       ||\n");
        printf("||                                                ��������˳�                                                           ||\n");
        printf("||                                                                                                                       ||\n");
        printf("===========================================================================================================================\n");

        gfree(ghead);
        pg=NULL;
        fflush(stdin);
        getch();
        show_gst_menu();
    }
}

void show_gst_gid()//�����֤����ʾ����
{
    struct Guest *creat_gst_link();
    int check_input_gid(char gid[500]);
    struct Guest *pg,*ghead;
    char gid[50];//�����洢Ҫ���ҵ�ID
    char gid_ck[500];//�洢�������� ����û�ID��ʽ
    int flag=0;//flag==0��ʾû�и�ID����ʷ��¼
    char info[10][20]={{"��������"},{"�Ա�"},{"����"},{"���֤��"},{"��ס����"},{"�����"},{"Ԥ����סʱ��"},{"Ԥ�����ʱ��"},{"�Ǽ�ʱ��"},{"�˷�ʱ��"}};
    int f_menu=1;//���Ʋ˵�

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
            printf("||                                             û�д��˵���ʷ��¼                                                        ||\n");
        }

        printf("||                                                                                                                       ||\n");
        printf("||                                                ��������˳�                                                           ||\n");
        printf("||                                                                                                                       ||\n");
        printf("===========================================================================================================================\n");

        gfree(ghead);
        pg=NULL;
        fflush(stdin);
        getch();
        show_gst_menu();
    }

}

void show_gst_rid()//���������ʾ����
{
    char rid_ck[500]={'\0'};//���ڼ����������Ƿ���ȷ
    int rid;//�洢Ҫ��ѯ�ķ���ID
    int flag=0;//flag==0��ʾû�и÷������ʷ��¼
    struct Guest *pg,*ghead;
    char info[10][20]={{"��������"},{"�Ա�"},{"����"},{"���֤��"},{"��ס����"},{"�����"},{"Ԥ����סʱ��"},{"Ԥ�����ʱ��"},{"�Ǽ�ʱ��"},{"�˷�ʱ��"}};
    int f_menu=1;//���Ʋ˵�

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
            printf("||                                            û�и÷������ʷ��¼                                                       ||\n");
        }

        printf("||                                                                                                                       ||\n");
        printf("||                                                ��������˳�                                                           ||\n");
        printf("||                                                                                                                       ||\n");
        printf("===========================================================================================================================\n");

        gfree(ghead);
        pg = NULL;
        fflush(stdin);
        getch();
        show_gst_menu();
    }
}

void show_gst_all()//��ʾ���п���
{
    struct Guest *pg,*ghead;
    int flag=0;
    char info[10][20]={{"��������"},{"�Ա�"},{"����"},{"���֤��"},{"��ס����"},{"�����"},{"Ԥ����סʱ��"},{"Ԥ�����ʱ��"},{"�Ǽ�ʱ��"},{"�˷�ʱ��"}};
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
        printf("||                                              û����ʷ��¼                                                             ||\n");
    }

    printf("||                                                                                                                       ||\n");
    printf("||                                                ��������˳�                                                           ||\n");
    printf("||                                                                                                                       ||\n");
    printf("===========================================================================================================================\n");

    gfree(ghead);
    pg = NULL;
    fflush(stdin);
    getch();
    show_gst_menu();
}

int input(struct Guest *p)//Ԥ������ʱ�Ŀ�����Ϣ����
{
    char ch[500];//����û�����

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

void reserve_time()//Ԥ���ӵ㷿
{
    struct Guest gst={.in_time="δ��ס",.out_time="\0",.rstat='1'};
    struct room *proom;
    struct room *pr;
    char dtck[500];//���ʱ������
    char tb[11]={'\0'};//��ȡ��ǰʱ�� ���Ԥ����ס�����Ƿ�С�ڵ�ǰ
    char rid_ck[500]={'\0'};//��鷿�������
    char num_ck[500]={'\0'};//���ͬ����������
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
                printf("\n��������˳�");
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
                                "\t\t\t||                     ����#�˳�                          ||\n"
                                "\t\t\t||                                                        ||\n"
                                "\t\t\t||                ������ҪԤ���ķ����                    ||\n"
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

                    if(check_room(gst,pr)==0)//����0 ��ʾ���䲻��ѡ
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
                            if(check_gst_stat_reserve(gst)==1)//����1 ��ʾ����Ԥ��
                            {
                                strcpy(gst.in_time,"δ��ס");
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

int input_reserve_choose(char ch_ck[500])//���ѯ���Ƿ�Ԥ���ӵ㷿�����루y/n��
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

void reserve_choose()//Ԥ�� ѯ���Ƿ�Ԥ���ӵ㷿
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

int input_date_reserve(char dtck[500],int a)//�������� Ԥ��ʱ a==0��ʾ������סʱ�� a==1��ʾ�������ʱ��
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

int input_num_reserve(char num_ck[500])//������ס���� Ԥ��ʱ
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

int input_rid_reserve(char rid_ck[500],struct room *pr)//������ס�ķ���� Ԥ��ʱ
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
                                "\t\t\t||                     ����#�˳�                          ||\n"
                                "\t\t\t||                     ����ȷ���룡                       ||\n"
                                "\t\t\t||                ������ҪԤ���ķ����                    ||\n"
                                "\t\t\t||                                                        ||\n"
                                "\t\t\t||                                                        ||\n"
                                "\t\t\t************************************************************\n"
                                "\t\t\t------------------------------------------------------------\n");
                        GetConsoleScreenBufferInfo(hConsole, &csbi);
                        gotoxy(50,csbi.dwCursorPosition.Y-4);

    }

    return flag;
}

void reserve()//Ԥ�����ӵ㷿
{
    struct Guest gst={.in_time="δ��ס",.out_time="\0",.rstat='1'};
    struct room *proom;
    struct room *pr;
    char dtck[500];//���ʱ������
    char tb[11]={'\0'};//��ȡ��ǰʱ�� ���Ԥ����ס�����Ƿ�С�ڵ�ǰ
    char rid_ck[500]={'\0'};//��鷿�������
    char num_ck[500]={'\0'};//���ͬ����������
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
                        printf("\n��������˳�");
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
                                "\t\t\t||                     ����#�˳�                          ||\n"
                                "\t\t\t||                                                        ||\n"
                                "\t\t\t||                ������ҪԤ���ķ����                    ||\n"
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

                            if(check_room(gst,pr)==0)//����0 ��ʾ���䲻��ѡ
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
                                    if(check_gst_stat_reserve(gst)==1)//����1 ��ʾ����Ԥ��
                                    {
                                        strcpy(gst.in_time,"δ��ס");
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

int ckdt(char dtck[500])//���Ԥ��ʱ��ʱ������
{
    int i,flag=1,d,m;//flag==0��ʾ���벻���ϸ�ʽ d�洢���� m�洢�·�
    char mck[3]={"\0"},dck[3]={"\0"};//mck����·��Ƿ����� dck��������Ƿ�����

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

int check_gst_stat_reserve(struct Guest gst)//�������Ƿ����Ԥ��
{
    int flag,f_id=0,f_stat=1;//flag==0��ʾ���˲�����Ԥ�� f_id==0��ʾ��ǰ�޸��˵�Ԥ����¼�������ǵ�һ��Ԥ�� f_stat==0��ʾ���˲���Ԥ��״̬
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

int check_gst_stat_giveuprsv(struct Guest gst)//�������Ƿ����ȡ��Ԥ��
{
    int flag=0;//flag==0��ʾ���˲�����ȡ��Ԥ��
    struct Guest *pg,*ghead;
    char tim[11];

    ghead = creat_gst_link();
    pg=ghead;

    while(pg!=NULL)
    {
        if(strcmp(pg->gst_id,gst.gst_id)==0)
        {
            if(pg->rstat == '1'&&(strcmp(pg->in_time,"δ��ס")==0)&&atoi(pg->otdt)>=atoi(get_time_book(tim)))
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

int check_room(struct Guest gst,struct room *pr)//��������ѡ�����ܷ�Ԥ��
{
    struct room *proom;
    int flag=0;//flag=0��ʾ����ķ��䲻�ڿ�ѡ��Χ��

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

void giveup_reserve()//ȡ��Ԥ��
{
    FILE *fp;
    struct Guest gst={.rstat='0'};
    struct room *pr;
    char gid[50];//�洢�ͻ����id
    char gid_ck[500]={"\0"};//�洢�������룬���ڼ��id��ʽ
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
                        strcpy(gst.in_time,"��ȡ��");
                        strcpy(gst.out_time,"��ȡ��");
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

int cmp_mon(int mon,struct Guest gst)//ͳ��ĳ���м�����ס
{
    char dateFirst[10]={"20180100"};
    char dateSecond[10]={"20180200"};
    int flag;//flag==1��ʾ��סʱ�����������

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

void counting_gst(struct room *rhead)//������Ϣͳ��
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
        if(strcmp(pg->in_time,"δ��ס")!=0&&strcmp(pg->in_time,"��ȡ��")!=0)
        {
            population += pg->num;
        }
        pg=pg->next;
    }

    printf("\t*  *  *||           �ܹ���%-3d����ס          ||*  *  *\n",population);

    population=0;
    pg=ghead;
    while(pg!=NULL)
    {
        if(pg->sex=='m'&&strcmp(pg->in_time,"δ��ס")!=0&&strcmp(pg->in_time,"��ȡ��")!=0)
            population += pg->num;
        pg = pg->next;
    }
    printf("\t*  *  *||������%-3d��\t",population);

    population=0;
    pg=ghead;
    while(pg!=NULL)
    {
        if(pg->sex=='w'&&strcmp(pg->in_time,"δ��ס")!=0&&strcmp(pg->in_time,"��ȡ��")!=0)
            population += pg->num;
        pg = pg->next;
    }
    gotoxy(42,6);
    printf("Ů����%-3d��||*  *  *\n",population);

    for(mon=1;mon<=6;mon++)
    {
        population=0;
        pg=ghead;
        while(pg!=NULL)
        {
            if(cmp_mon(mon,*pg)==1&&strcmp(pg->in_time,"δ��ס")!=0&&strcmp(pg->in_time,"��ȡ��")!=0)
                population+=pg->num;
            pg=pg->next;
        }
        printf("\t*  *  *||%-2d����%-3d����ס\t\n",mon,population);
    }

    for(mon=7;mon<=12;mon++)
    {
        population=0;
        pg=ghead;
        while(pg!=NULL)
        {
            if(cmp_mon(mon,*pg)==1&&strcmp(pg->in_time,"δ��ס")!=0&&strcmp(pg->in_time,"��ȡ��")!=0)
                population+=pg->num;
            pg=pg->next;
        }
        gotoxy(38,mon);
        printf("%-2d����%-3d����ס||*  *  *\n",mon,population);
    }

    population=0;
    pg=ghead;
    while(pg!=NULL)
    {
        if(atoi(pg->age)>=18&&atoi(pg->age)<=30&&strcmp(pg->in_time,"δ��ס")!=0&&strcmp(pg->in_time,"��ȡ��")!=0)
            population += pg->num;
        pg = pg->next;
    }
    printf("\t*  *  *||18�굽30��%-3d��\t",population);

    population=0;
    pg=ghead;
    while(pg!=NULL)
    {
        if(atoi(pg->age)>=31&&atoi(pg->age)<=45&&strcmp(pg->in_time,"δ��ס")!=0&&strcmp(pg->in_time,"��ȡ��")!=0)
            population += pg->num;
        pg = pg->next;
    }
    gotoxy(38,13);
    printf("31�굽45��%-3d��||*  *  *\n",population);

    population=0;
    pg=ghead;
    while(pg!=NULL)
    {
        if(atoi(pg->age)>=46&&atoi(pg->age)<=60&&strcmp(pg->in_time,"δ��ס")!=0&&strcmp(pg->in_time,"��ȡ��")!=0)
            population += pg->num;
        pg = pg->next;
    }
    printf("\t*  *  *||46�굽60��%-3d��\t",population);

    population=0;
    pg=ghead;
    while(pg!=NULL)
    {
        if(atoi(pg->age)>=61&&atoi(pg->age)<=120&&strcmp(pg->in_time,"δ��ס")!=0&&strcmp(pg->in_time,"��ȡ��")!=0)
            population += pg->num;
        pg = pg->next;
    }
    gotoxy(38,14);
    printf("60��������%-3d��||*  *  *\n",population);

    population = 0;
    pg=ghead;
    while(pg!=NULL)
    {
        if(pg->num==1&&strcmp(pg->in_time,"δ��ס")!=0&&strcmp(pg->in_time,"��ȡ��")!=0)
            population++;
        pg=pg->next;
    }
    printf("\t*  *  *||һ����ס��%-3d��\t",population);

    population = 0;
    pg=ghead;

    while(pg!=NULL)
    {
        if(pg->num==2&&strcmp(pg->in_time,"δ��ס")!=0&&strcmp(pg->in_time,"��ȡ��")!=0)
            population++;
        pg=pg->next;
    }
    gotoxy(38,15);
    printf("������ס��%-3d��||*  *  *\n",population);

    population = 0;
    pg=ghead;

    while(pg!=NULL)
    {
        if(pg->num==3&&strcmp(pg->in_time,"δ��ס")!=0&&strcmp(pg->in_time,"��ȡ��")!=0)
            population++;
        pg=pg->next;
    }
    printf("\t*  *  *||������ס��%-3d��\t\t     ||*  *  *\n",population);

    population=0;
    pg=ghead;
    while(pg!=NULL)
    {
        if(strcmp(pg->out_time,"δ�˷�")==0)
            population += pg->num;
        pg = pg->next;
    }
    printf("\t*  *  *||           ��ǰ��%-3d����ס          ||*  *  *\n",population);

    gfree(ghead);
    pg=NULL;
    fflush(stdin);
    printf("\t*  *  *||            �����������            ||*  *  *\n");

    printf("\t*******=========================================*******\n");

    getch();
    Roomstat(rhead);
}

