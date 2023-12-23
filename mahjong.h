#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <termios.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <string.h>

#define RED "\e[;38;5;196;1m"
#define BLOCK "\e[;47;30"
#define GRN "\x1b[;38;5;2;1m"
#define WHT "\x1b[;47;37;1m"
#define BWHT "\x1b[;47;1m"
#define CYAN_BOLD_ITALIC "\x1b[;36;1;3m"
#define RESET "\x1b[0;m"

#define COl 15

char CPGT[4][8] = {" 吃 ", " 碰 ", " 槓 ", "跳過"};
char LZH[3][8] = {"立直", "自摸", " 和 "};
char CHAR[3][4] = {"萬", "條", "筒"};
char NUM[9][4] = {"一", "二", "三", "四", "五", "六", "七", "八", "九"};
char HON[7][90] = {"東", "南", "西", "北", RED"中"RESET, GRN"發"RESET, "白"};
char inttop[10][50] = {"▟▀▙\e[1B\e[3D█▞█\e[1B\e[3D▜▄▛",
                      "▗█ \e[1B\e[3D █ \e[1B\e[3D▗█▖",
                      "▟▀▙\e[1B\e[3D▗▄▛\e[1B\e[3D█▄▄",
                      "▝▀▙\e[1B\e[3D ▀▙\e[1B\e[3D▜▄▛",
                      " ▟▌\e[1B\e[3D▟▜▌\e[1B\e[3D▀▜▛",
                      "█▀▘\e[1B\e[3D▀▀▙\e[1B\e[3D▜▄▛",
                      "▟▀▘\e[1B\e[3D█▀▙\e[1B\e[3D▜▄▛",
                      "█▀█\e[1B\e[3D ▗▛\e[1B\e[3D █ ",
                      "▟▀▙\e[1B\e[3D▟▀▙\e[1B\e[3D▜▄▛",
                      "▟▀▙\e[1B\e[3D▜▄█\e[1B\e[3D▗▄▛"};

void drawS(int x, int y, int index){
    printf("\e[%i;%iH      ", x, y);
    printf("\e[%iB\e[%iD╔════╗", 1, 6);
    for(int i = 0 ; i < 2 ; i++){
        printf("\e[%iB\e[%iD║    ║", 1, 6);
    }
    printf("\e[%iB\e[%iD╚════╝", 1, 6);
    printf(RESET"\e[%iB\e[%iD      ", 1, 6);
    printf("\e[%i;%iH", x, y);
    /*
    */
    if(index == 0)
        return;
    if((index/10)%10 == 4){
        printf("\e[%iB\e[%iC%s", 2, 2, HON[(index%10)-1]);
    }
    else{
        printf("\e[%iB\e[%iC%s", 2, 2, NUM[(index%10)-1]);
        printf("\e[%iB\e[%iD%s", 1, 2, CHAR[((index/10)%10)-1]);
    }
}

void SdrawS(int x, int y, int index){
    printf("\e[%i;%iH╔══╗", x, y);
    for(int i = 0 ; i < 2 ; i++){
        printf("\e[%iB\e[%iD║  ║", 1, 4);
    }
    printf("\e[%iB\e[%iD╚══╝"RESET, 1, 4);
    printf("\e[%i;%iH", x, y);
    if((index/10)%10 == 4){
        printf("\e[%iB\e[%iC%s", 1, 1, HON[(index%10)-1]);
        printf("\e[%iB\e[%iD", 1, 1);
    }
    else{
        printf("\e[%iB\e[%iC%s", 1, 1, NUM[(index%10)-1]);
        printf("\e[%iB\e[%iD%s", 1, 2, CHAR[((index/10)%10)-1]);
    }
}

void drawH(int x, int y, int index){
    x += 1;
    printf("\e[%i;%iH       ", x, y);
    printf("\e[%iB\e[%iD╔═════╗", 1, 7);
    for(int i = 0 ; i < 1 ; i++){
        printf("\e[%iB\e[%iD║     ║", 1, 7);
    }
    printf("\e[%iB\e[%iD╚═════╝", 1, 7);
    printf(RESET"\e[%iB\e[%iD       ", 1, 7);
    printf("\e[%i;%iH", x, y);
    if((index/10)%10 == 4){
        printf("\e[%iB\e[%iC%s", 2, 1, HON[(index%10)-1]);
    }
    else{
        printf("\e[%iB\e[%iC%s %s", 2, 1, NUM[(index%10)-1], CHAR[((index/10)%10)-1]);
    }
}

void SdrawH(int x, int y, int index){
    printf("\e[%i;%iH      ", x, y);
    printf("\e[%iB\e[%iD╔════╗", 1, 6);
    for(int i = 0 ; i < 1 ; i++){
        printf("\e[%iB\e[%iD║    ║", 1, 6);
    }
    printf("\e[%iB\e[%iD╚════╝"RESET, 1, 6);
    printf("\e[%i;%iH", x, y);
    if((index/10)%10 == 4){
        printf("\e[%iB\e[%iC%s", 2, 1, HON[(index%10)-1]);
    }
    else{
        printf("\e[%iB\e[%iC%s%s", 2, 1, NUM[(index%10)-1], CHAR[((index/10)%10)-1]);
    }
}