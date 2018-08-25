//#include "muse5.2.c"
//#include "room.h"
#include "quanjv.h"
#include <windows.h>
int main ()
{
        creat();
        int a = 50;
        system("mode con cols=80 lines=25");
        SetConsoleTitleA("酒店预订管理系统v6.6—————————by 陈嘉尊 吴斯铭 于小斌");
        set_back(back);
        /*HANDLE hOut;
        hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hOut,
                            BACKGROUND_RED |
                            BACKGROUND_GREEN |
                            BACKGROUND_BLUE);*/
         menus();
        return 0;
}
