#include "mahjong.h"

void draw(int x, int index);
void touch(int now, int prev);
int hand[13] = {11, 12, 21, 24, 28, 28, 31, 32, 34, 36, 41, 41, 42};
int ROW;
struct winsize w;

void handle_sigwinch(int signo){
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    ROW = w.ws_row;
    printf("\e[2J");
    for(int i = 0 ; i < 13 ; i++){
        draw(ROW-7, i);
    }
    printf("\n");
    printf("\e[%iAROW: %d, COL: %d", ROW, w.ws_row, w.ws_col);
}

int main(void)
{
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
    // Switch to canonical mode, disable echo
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    ROW = w.ws_row;

    signal(SIGWINCH, handle_sigwinch);

    int quit = 0;
    int now = -1;
    int prev = -1;
    int n;
    printf("\e[2J");
    printf("\e[%iAROW: %d, COL: %d", ROW, w.ws_row, w.ws_col);
    //printf("\e[?25l");
    printf("\e[%iA", ROW + 2);
    for(int i = 0 ; i < 13 ; i++){
        draw(ROW-7, i);
    }
    printf("\n");
    while(!quit){
        struct timeval tv;
        fd_set fds;
        tv.tv_sec = 0;
        tv.tv_usec = 0;
        
        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds);
        n = select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
        if (FD_ISSET(STDIN_FILENO, &fds)) {
            int ch = getchar();
            if (ch == 27) {
                ch = getchar();
                if(ch == EOF)
                    quit = 1;
            } else if (ch == 'q' ) {
                now = 0;
            } else if (ch == 'w') {
                now = 1;
            } else if (ch == 'e') {
                now = 2;
            } else if (ch == 'r') {
                now = 3;
            } else if (ch == 't') {
                now = 4;
            } else if (ch == 'y') {
                now = 5;
            } else if (ch == 'u') {
                now = 6;
            } else if (ch == 'i') {
                now = 7;
            } else if (ch == 'o') {
                now = 8;
            } else if (ch == 'p') {
                now = 9;
            } else if (ch == '[') {
                now = 10;
            } else if (ch == ']') {
                now = 11;
            } else if (ch == '\\') {
                now = 12;
            } 
        }
        touch(now, prev);
        prev = now;
    }
    for(int i = 0 ; i < 10 ; i++){printf("\n");}
    printf("\n"RESET);

    //printf("\e[?25h");
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return 0;
}

void draw(int x, int index){
    int y = (w.ws_col/2)-55+(index*8);
    printf("\e[%i;%iH       ", x, y);
    printf(GRN"\e[%iB\e[%iD╔═════╗", 1, 7);
    printf(   "\e[%iB\e[%iD║     ║", 1, 7);
    printf(   "\e[%iB\e[%iD║     ║", 1, 7);
    printf(   "\e[%iB\e[%iD║     ║", 1, 7);
    printf(   "\e[%iB\e[%iD╚═════╝", 1, 7);
    printf(RESET"\e[%iB\e[%iD       ", 1, 7);
    printf("\e[%i;%iH", x, y);
    //printf(WHT"\e[%iB\e[%iC%d",2, 2, hand[index]/10);
    printf(RED"\e[%iB\e[%iC┏━╋━┓",2, 1);
    //printf("\e[%iB\e[%iD%d", 1, 1, hand[index]%10);
    printf("\e[%iB\e[%iD┗━╋━┛", 1, 5);
    printf("\e[%iB\e[%iD  ┃  "RESET, 1, 5);
    printf("\e[%iG", ROW);
}

void touch(int now, int prev){
    if(now == prev) return;
    if(prev >= 0)
        draw(ROW-7, prev);
    if(now >= 0)
        draw(ROW-8, now);
    fflush(stdout);
}

/*

┏━╋━┓
┗━╋━┛
  ┃
*/