/*
 CURHELLO.C
 ==========
 (c) Copyright Paul Griffiths 1999
 Email: mail@paulgriffiths.net
 
 "Hello, world!", ncurses style.
 
 */





#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <curses.h>
#include <term.h>
#include <termios.h>
#include <unistd.h>



typedef int BOOL;
#define TRUE 1
#define FALSE 0
#define ESC 27
#define LEFT 68
#define RIGHT 67
#define UP 65
#define DOWN 66
#define MAXSTAGE 2

void gotoxy(int x, int y)
{
    printf("\033[%d;%df", y, x);
    fflush(stdout);
}


#define clean() printf("\033[H\033[J")
#define putchxy(x,y,c) {gotoxy(x,y); putchar(c);}


void loadStage(void);
void drawStage(void);
void playerMoveAction(void);
int _getch(void);
BOOL testStageClear(void);
void printInfo(void);
void restart(void);




int _getch(void)
{
    int ch;
    struct termios buf;
    struct termios save;
    
    tcgetattr(0, &save);
    buf = save;
    buf.c_lflag &= ~(ICANON|ECHO);
    buf.c_cc[VMIN] = 1;
    buf.c_cc[VTIME] = 0;
    tcsetattr(0, TCSAFLUSH, &buf);
    ch = getchar();
    tcsetattr(0, TCSAFLUSH, &save);
    return ch;
}


const char stageData[MAXSTAGE][18][21] =
{
    {
        "#####################",
        "#####################",
        "#####################",
        "#####################",
        "#####################",
        "#####################",
        "#####################",
        "##   O            .##",
        "## @ O            .##",
        "##   O            .##",
        "#####################",
        "#####################",
        "#####################",
        "#####################",
        "#####################",
        "#####################",
        "#####################",
        "#####################"
    },
    {
        "#####################",
        "#####################",
        "#####################",
        "#####################",
        "#####################",
        "##########.##########",
        "########## ##########",
        "##########O@O.#######",
        "########. OO ########",
        "########### #########",
        "###########.#########",
        "#####################",
        "#####################",
        "#####################",
        "#####################",
        "#####################",
        "#####################",
        "#####################",
    }
};

char stage[18][21];
int roundNum;
int roundx, roundy;
int numMove = 0;
int onRestart;

void drawStage()
{
    for(int stagey = 0; stagey < 18; stagey++)
    {
        for(int stagex = 0; stagex < 21; stagex++)
        {
            putchxy(stagex + 1, stagey, stage[stagey][stagex]);
        }
    }
    
    putchxy(roundx + 1, roundy,'@');
}

BOOL testStageClear()
{
    int cx, cy;
    
    for(cy = 0; cy < 18; cy++)
    {
        for(cx = 0; cx < 21; cx++)
        {
            if(stageData[roundNum][cy][cx] == '.' && stage[cy][cx] != 'O')
            {
                return FALSE;
            }
        }
    }
    return TRUE;
}




void loadingStage()
{
    int i;
    int long_term = 100000;

    for(i = 0; i < 45; i++)
    {
        putchxy(10 + i, 5, '-');
        usleep(10000);
    }
    for(i = 0; i < 45; i++)
    {
        putchxy(70-i, 10, '-');
        usleep(10000);
    }

    usleep(800000);
    gotoxy(35, 7);
    printf("소");
    usleep(long_term);
    gotoxy(38, 7);
    printf("코");
    usleep(long_term);
    gotoxy(41, 7);
    printf("반");
    usleep(long_term);
    putchxy(48, 8, 'M');
    usleep(long_term);
    putchxy(49, 8, 'a');
    usleep(long_term);
    putchxy(50, 8, 'd');
    usleep(long_term);
    putchxy(51, 8, 'e');
    usleep(long_term);
    putchxy(52, 8, 'b');
    usleep(long_term);
    putchxy(53, 8, 'y');
    usleep(long_term);
    gotoxy(58, 8);
    printf("Yunsoo");
    
    printf("\n\n\n\n");
    usleep(1000000);
    
}


void playerMoveAction()
{
    
    int dx, dy;
    char ch;
    ch = _getch();
    if(ch == 'r' || ch == 'R')
    {
        restart();
    }
    
    if(ch == 27)
    {
        ch = _getch();
        if(ch == 91)
        {
            ch = _getch();
            switch (ch) {
                case RIGHT:
                {
                    dx = 1;
                    dy = 0;
                    break;
                }
                case LEFT:
                {
                    dx = -1;
                    dy = 0;
                    break;
                }
                case UP:
                {
                    dx = 0;
                    dy = -1;
                    break;
                }
                case DOWN:
                {
                    dx = 0;
                    dy = 1;
                    break;
                }
            }
        }
    }
    
    else return;
    if(stage[roundy + dy][roundx + dx] == '#')
    {
        return ;
    }
    
    if(stage[roundy + dy][roundx + dx] == 'O')
    {
        if(stageData[roundNum][roundy + dy * 2][roundx + dx * 2] == ' ')
        {
            stage[roundy + dy * 2][roundx + dx * 2] = 'O';
            stage[roundy + dy][roundx + dx] = ' ';
        }
        else if(stageData[roundNum][roundy + dy * 2][roundx + dx * 2] == '#')
        {
            return ;
        }
        
        else if(stageData[roundNum][roundy + dy * 2][roundx + dx * 2] == '.')
        {
            stage[roundy + dy][roundx + dx] = ' ';
            stage[roundy + dy * 2][roundx + dx * 2] = 'O';
        }
    }
    roundx += dx;
    roundy += dy;
    
    numMove++;
}

void printInfo()
{
    gotoxy(59, 1); printf("┏━━━━━━━━━━━━━━━┓");
    gotoxy(59, 2); printf("┃ S O K O B A N ┃");
    gotoxy(59, 3); printf("┗━━━━━━━━━━━━━━━┛");
    
    gotoxy(61, 5); printf("스테이지 : %d", MAXSTAGE -1);
    gotoxy(61, 7); printf("움직인 횟수 : %d", numMove);
    
    gotoxy(67, 10); printf("UP");
    gotoxy(67, 11); printf("↑");
    gotoxy(60, 12); printf("LEFT ←");
    gotoxy(68, 12); printf("→ RIGHT");
    gotoxy(67, 13); printf("↓");
    gotoxy(66, 14); printf("DOWN");
    gotoxy(61, 16); printf("다시하기 : R");
}

void restart()
{
    onRestart = 1;
}



int main()
{

    int x,y;
//    initscr();
    clean();
    curs_set(0);

    loadingStage();
    roundNum = 0;

    
    


    while(1)
    {
        numMove = 0;
        memcpy(stage, stageData[roundNum], sizeof(stage));
        for(y = 0; y < 18; y++)
        {
            for(x = 0; x < 21; x++)
            {
                if(stage[y][x] == '@')
                {
                    roundx = x;
                    roundy = y;
                    stage[y][x] = ' ';
                }
            }
        }
        clean();

        while(1)
        {
            drawStage();
            printInfo();
            playerMoveAction();
            if(onRestart == 1)
            {
                onRestart = 0;
                break;
            }
            
            if(testStageClear())
            {
                clean();
                printf("축하합니다.");
                gotoxy(29, 5);
                printf("%d 스테이지 클리어", roundNum + 1);
                gotoxy(34, 8);
                printf("다음 스테이지로 이동하겠습니다.");
                usleep(1000000);
                if(roundNum < MAXSTAGE - 1)
                {
                    roundNum++;
                }
                break;

            }

        }
    }
    return 0;
}













