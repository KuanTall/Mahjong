#include "unp.h"
#include "mahjong.h"

int gameState();
void UI();
int login();
int initset();
void driver(int x, int y, int seat);
void touch(int now, int prev);
void bau();
void info();
void otherPlayer(int seat);
int seatnum = 1;
int mont = 74;
int Bau[6] = {11, 42, 12, -1};
int score[4] = {25000, 25000, 25000, 25000};
int hand[14] = {11, 12, 21, 24, 28, 28, 31, 32, 34, 36, 41, 41, -1};
int take = 11;
char name[4][1024] = {"Kuan", "Jack", "Toyz", "GOD"};
int pair[4][17] = {{421, 420, 420, 1420, 441, 440, 440, 1440, 441, 440, 440, 1440,  -1},
                   {451, 450, 450, 1450, 461, 460, 460, 1460, 471, 470, 1470, 441, 440, 440, 1440,  -1},
                   {421, 420, 420, 1420, 441, 440, 1440, 451, 450, 450, 1450, 441, 440, 440, 1440,  -1}, 
                   {421, 420, 420, 1420, 441, 440, 440, 1440, 451, 450, 450, 1450, 441, 440, 440, 1440,  -1}};
int river[4][31] = {{110, 120, 130, 140, 150, 160, 170, 180, 190, 210, 220, 230, 240, 250, 260, 270, 280, 290, 310, 320, 330, 340, 350, 360, 370, 380, 390, 410, 420, 431, -1},
                    {-1},
                    {110, 120, 130, 140, 150, 160, 170, 180, 190, 210, 220, 230, 240, 250, 260, 270, 280, 290, 310, 320, 330, 340, 350, 360, 370, 380, 390, 410, 420, 431, -1},
                    {110, 120, 130, 140, 150, 160, 170, 180, 190, 210, 220, 230, 240, 250, 260, 270, 280, 290, 310, 320, 331, 340, 350, 360, 370, 380, 390, 410, 420, 430, -1}};
int ROW;
int COL;
int last[4] = {4, 4, 4, 4};
int cpgt[4] = {1, 1, 1, 1};
int lzh[3] = {1, 1, 1};

struct winsize w;

void clearInputBuffer() {
    tcflush(STDIN_FILENO, TCIFLUSH);
}

void handle_sigwinch(int signo){
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    ROW = w.ws_row;
    COL = w.ws_col/2-78;
    printf("\e[2J");
}

void handle_sigwinchb(int signo){
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    ROW = w.ws_row;
    COL = w.ws_col/2-78;
    printf("\e[2J");
    if(w.ws_row >= 41 && w.ws_col >= 163){
        printf("\e[%i;%iH以達到最適大小 按下SPACE鍵開始", w.ws_row/2, w.ws_col/2-15);
    }
    else{
        printf("\e[%i;%iH為了能夠達到最好遊戲體驗請調整視窗或字體大小", w.ws_row/2, w.ws_col/2-22);
        if(w.ws_row < 41){
            printf("\e[%iB\e[%iG畫面長度不足", 1, w.ws_col/2-6);
        }
        else{
            printf("\e[%iB\e[%iG畫面長度足夠", 1, w.ws_col/2-6);
        }
        if(w.ws_col < 163){
            printf("\e[%iB\e[%iG畫面寬度不足", 1, w.ws_col/2-6);
        }
        else{
            printf("\e[%iB\e[%iG畫面寬度足夠", 1, w.ws_col/2-6);
        }
        printf("\e[%iB\e[%iG可以拉動視窗調整或使用'Ctrl' + '-'調整字體大小", 1, w.ws_col/2-23);
    }
}

int main(void)
{
    int state = 0;//0 initset  1 game
    
    
    // Switch to canonical mode, disable echo
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    ROW = w.ws_row;
    COL = w.ws_col/2-78;


    int quit = 0;
    int now = -1;
    int prev = -1;
    int n;
    printf("\e[2J");
    
    fflush(stdout);
    printf("\e[%iA", ROW + 2);

    while(1){
        if(state == 0){
            state = initset();
        }
        else if(state == 1){
            state = login();
        }
        else if(state == 2){
            state = gameState();
        }
        else if (state == -1)
            break;
    }
    
    for(int i = 0 ; i < 10 ; i++){printf("\n");}
    printf("\n"RESET);

    
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    
    return 0;
}

int initset(){
    printf("\e[?25l");
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
    signal(SIGWINCH, handle_sigwinchb);
    int quit = 0;
    if(w.ws_row >= 41 && w.ws_col >= 163){
        printf("\e[%i;%iH以達到最適大小 按下SPACE鍵開始", w.ws_row/2, w.ws_col/2-15);
    }
    else{
        printf("\e[%i;%iH為了能夠達到最好遊戲體驗請調整視窗或字體大小", w.ws_row/2, w.ws_col/2-22);
        if(w.ws_row < 41){
            printf("\e[%iB\e[%iG畫面長度不足", 1, w.ws_col/2-6);
        }
        else{
            printf("\e[%iB\e[%iG畫面長度足夠", 1, w.ws_col/2-6);
        }
        if(w.ws_row < 41){
            printf("\e[%iB\e[%iG畫面長度不足", 1, w.ws_col/2-6);
        }
        else{
            printf("\e[%iB\e[%iG畫面寬度足夠", 1, w.ws_col/2-6);
        }
        printf("\e[%iB\e[%iG可以拉動視窗調整或使用'Ctrl' + '-'減少字體大小", 1, w.ws_col/2-23);
    }
    while(!quit){  
        clearInputBuffer();
        int ch = getchar();
        if(ch == ' ' && w.ws_row >= 41 && w.ws_col >= 163)
            quit = 1;
    }
    printf("\e[?25h");
    return 1;
}

int login(){
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags & ~O_NONBLOCK);

    int		sockfd;
    int     c;
    char    serverip[1024];
	struct sockaddr_in	servaddr;

	sockfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT+4);
    printf("\e[2J");
    clearInputBuffer();
    int i = 0;
    while((c = getchar()) != '\n'){
        serverip[i] = c;
        printf("%c", c);
        i++;
    }
    serverip[i] = 0;
    printf("%s", serverip);
    Inet_pton(AF_INET, serverip, &servaddr.sin_addr);

	//Connect(sockfd, (SA *) &servaddr, sizeof(servaddr));
    return 2;
}

int gameState(){
    printf("\e[?25l");
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

    signal(SIGWINCH, handle_sigwinch);
    printf("\e[2J");
    UI();
    
    int quit = 0;
    int now = -1;
    int prev = -1;
    while(!quit){
        struct timeval tv;
        fd_set fds;
        tv.tv_sec = 0;
        tv.tv_usec = 0;
        
        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds);
        select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
        if (FD_ISSET(STDIN_FILENO, &fds)) {
            UI();
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
            } else if (ch == ' ') {
                now = 14;
            } 
        }
        touch(now, prev);
        prev = now;
    }
    printf("\e[?25h");
    return -1;
}

void UI(){
    printf("\e[%i;%iHROW: %d, COL: %d", 1, 130, w.ws_row, w.ws_col);
    int ps;
    int y = 1+COL;
    for(int i = 0 ; i < last[seatnum] ; i++){
        drawS(ROW-5, y, hand[i]);
        y += 6;
    }
    y += 6;
    drawS(ROW-5, y, take);
    y = COL+150;
    for(int i = 0 ; i < 17 ; i++)
        if(pair[seatnum][i] == -1){
            ps = i-1;
            break;
        }
    for(int i = ps ; i >= 0 ; i--){
        if(pair[seatnum][i]%10 == 1){
            y -= 7;
            drawH(ROW-5, y, pair[seatnum][i]/10);
        }
        else{
            y -= 6;
            drawS(ROW-5, y, pair[seatnum][i]/10);
        }
    }
    //lzh
    for(int i = 0 ; i < 3 ; i++){
        if(lzh[i] == 1){
            printf("\e[%i;%iH┌──────┐", 30, COL+29+i*8);
            printf("\e[%iB\e[%iD│ %s │", 1, 8, LZH[i]);
            printf("\e[%iB\e[%iD└──────┘", 1, 8);
        }
    }

    //cpgt
    for(int i = 0 ; i < 4 ; i++){
        if(cpgt[i] == 1){
            printf("\e[%i;%iH┌──────┐", 30, 112+i*8);
            printf("\e[%iB\e[%iD│ %s │", 1, 8, CPGT[i]);
            printf("\e[%iB\e[%iD└──────┘", 1, 8);
        }
    }

    for(int i = 0 ; i < 4 ; i++){
        otherPlayer(i);
    }
    info();
    bau();
    mont--;
    printf("\n");
}

void otherPlayer(int seat){
    int x;
    int y = COL;
    int z;
    switch ((seat+4-seatnum)%4){
    case 0:
        x = 18;
        y += 105;
        break;
    case 2:
        x = 3;
        y += 105;
        break;
    case 3:
        x = 12;
        y += 53;
        break;
    case 1:
        x = 12;
        y += 157;
        break;
    }
    
    printf("\e[%i;%iH", x+2, y);
    printf("\e[%iB\e[%iD┌──────────────────────────────────────────────────┐", 1, 52);
    for(int i = 0 ; i < 12 ; i++)
        printf("\e[%iB\e[%iD│                                                  │", 1, 52);
    printf("\e[%iB\e[%iD└──────────────────────────────────────────────────┘", 1, 52);
    printf("\e[%iA\e[%iD", 3, 4);
    for(int temp = score[seat] ; temp > 0 ; temp /= 10){
        printf("%s", inttop[temp%10]);
        printf("\e[%iA\e[%iD", 2, 7);
    }
    printf("\e[%i;%iH%s", x+3, y-51, name[seat]);
    driver(x, y-51, seat);
    
    y -= 52;
    z = y;
    int temp;
    for(int i = 0 ; i < 17 ; i++)
        if(pair[seat][i] == -1){
            temp = i;
            break;
        }
    if((seat+4-seatnum)%4 == 2){
        for(int i = 0 ; i < temp ; i++){
            if(pair[seat][i]%10 == 1){
                SdrawH(x-1, z, pair[seat][i]/10);
                z += 6;
            }
            else{
                SdrawS(x-1, z, pair[seat][i]/10);
                z += 4;
            }
        }
    }
    else if((seat+4-seatnum)%4 == 1 || (seat+4-seatnum)%4 == 3){
        int ps = 0;
        for(int i = 0 ; i < temp ; i++){
            if(ps == 2){
                ps = 0;
                x -= 4;
                z = y;
            }
            if(pair[seat][i]/1000 == 1)
                ps++;
            if(pair[seat][i]%10 == 1){
                SdrawH(x-1, z, pair[seat][i]/10);
                z += 6;
            }
            else{
                SdrawS(x-1, z, pair[seat][i]/10);
                z += 4;
            }
        }
    }
}

void touch(int now, int prev){
    if(now == prev) return;
    
    if(now >= 0 && now < last[0])
        drawS(ROW-6, 1+COL+(now)*6, hand[now]);
    if(prev >= 0 && prev < last[0])
        drawS(ROW-5, 1+COL+(prev)*6, hand[prev]);
    if(now == 14)
        drawS(ROW-6, 1+COL+(now-(13-last[0]))*6, take);
    if(prev == 14)
        drawS(ROW-5, 1+COL+(prev-(13-last[0]))*6, take);
    fflush(stdout);
}

void bau(){
    int i = 0;
    for( ; Bau[i] != -1 ; i++){
        drawS(1, 2+COL+i*6, Bau[i]);
    }
    for( ; i < 5 ; i++){
        printf(GRN);
        drawS(1, 2+COL+i*6, 0);
    }
}

void info(){
    printf("\e[%i;%iH┌──────────────┐", 1, 34+COL );
    printf("\e[%i;%iH│   %s    局   │", 2, 34+COL, "東");
    printf("\e[%iD%d", 9, 1);
    printf("\e[%i;%iH│   第   本場  │", 3, 34+COL);
    printf("\e[%iD%d", 9, 1);
    printf("\e[%i;%iH│    余        │", 4, 34+COL);
    printf("\e[%iD%d", 9, mont);
    printf("\e[%i;%iH└──────────────┘", 5, 34+COL );
}

void driver(int x, int y, int seat){
    int yb = y;
    for(int i = 0 ; river[seat][i] != -1 ; i++){
        if(i%12 == 0){
            x += 4;
            y = yb;
        }
        if(river[seat][i]%10 == 0){
            SdrawS(x, y, river[seat][i]/10);
            y += 4;
        }
        else{
            SdrawH(x, y, river[seat][i]/10);
            y += 6;
        }
    }
}