//
//  main.cpp
//  Debug
//
//  Created by 欠费 on 2018/5/21.
//  Copyright © 2018年 AssassinQ. All rights reserved.
//

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <random>
#include "conio.h"     //检测键盘键入的函数
#define LEN 10     //棋盘大小
#define BOARD (LEN * 2 + 1)      //整体大小
#define NOTCHESS check[row][col] == 0   //格子对应值的判断
#define ISWHITE check[row][col] == -1    //白子对应值的判断
#define ISBLACK check[row][col] == 1     //黑子对应值的判断
#define COVERSPACE check[row][col] = 3   //光标覆盖空格
#define COVERWHITE check[row][col] = -2   //光标覆盖白子
#define COVERBLACK check[row][col] = 2    //光标覆盖黑子
#define ISCOVERSPACE check[row][col] == 3    //光标覆盖空格的判断
#define ISCOVERWHITE check[row][col] == -2   //光标覆盖白子的判断
#define ISCOVERBLACK check[row][col] == 2    //光标覆盖黑子的判断

void welcome(void);       //欢迎界面
void init_board(void);     //环境构建
void print(void);             //棋盘输出
int player_computer(void);     //人机
int player_player(void);      //人人
int check_chess(void);      //棋子布局检测
void regret(void);         //悔棋
void game_over(int);     //游戏结束后的判定输出
void go_to_xy(unsigned, unsigned);     //将输入光标移动到指定位置
void ai(void);        //电脑下子
void calculate_value(void);     //计算每格的价值
void run_my_code(void);     //代码运行

const char *board[BOARD][BOARD] = {0};        //棋盘格子样式
int check[LEN][LEN] = {0};       //格子的状态
int value[LEN][LEN] = {0};       //格子的价值
int reg[LEN * LEN][3] = {0};         //悔棋：回合下子记录
int turn = -1, order = 0;        //white == -1  black == 1  |   先后手
int row = 0, col = 0, sign;    //行列  |  输入记录
int count = 0;       //下子数
const char *BLACK = "○", *WHITE = "●", *POS = "¤";   // UTF-8 棋子、棋盘字符
const char *LT = "┌", *TOP = "┬", *RT = "┐";
const char *LEFT = "├", *CENTER = "┼", *RIGHT = "┤";
const char *LF = "└", *FOOT = "┴", *RF = "┘";
const char *SPACE = " ";

void welcome()    //欢迎界面
{
    printf("╔═══════════════════════════════════════════════════════════════════════╗\n");
    printf("║                                                                       ║\n");
    printf("║              WELCOME TO PLAY                                          ║\n");
    printf("║             FIVE IN LINE GAME                                         ║\n");
    printf("║         ._______________________.                                     ║\n");
    printf("║         | _____________________ |                                     ║\n");
    printf("║         | I                   I |          Tips:                      ║\n");
    printf("║         | I      Five In      I |             1. Use \'wasd\'           ║\n");
    printf("║         | I     Line Game     I |         or direction keys           ║\n");
    printf("║         | I      For Fun      I |         to control.                 ║\n");
    printf("║         | I___________________I |             2. Use \'r\' to           ║\n");
    printf("║         !_______________________!         regret.                     ║\n");
    printf("║             ._[__________]_.                  3. Use \'`\' to           ║\n");
    printf("║         .___|_______________|___.         quit.                       ║\n");
    printf("║          |::: ____             |                                      ║\n");
    printf("║          |    ~~~~ [CD-ROM]    |                                      ║\n");
    printf("║          !_____________________!                                      ║\n");
    printf("║                                                                       ║\n");
    printf("║                                                                       ║\n");
    printf("║               Input \'1\' to play with your partner                     ║\n");
    printf("║           Input \'2\' to play with computer(u go first)                 ║\n");
    printf("║           Input \'3\' to play with computer(u go later)                 ║\n");
    printf("║                                                                       ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════════╝\n");
    printf("\033[?25l");     //隐藏光标 '\033'是'ESC'的八进制表示形式
}

void init_board()    //棋盘构建
{
    memset(check, 0, sizeof(check));
    for(int i = 0; i < BOARD; i++)
        for(int j = 0; j < BOARD; j++)
            board[i][j] = SPACE;   // 先全部赋为空格
    //棋盘四个角的搭建
    board[0][0] = LT;
    board[0][BOARD - 1] = RT;
    board[BOARD - 1][0] = LF;
    board[BOARD - 1][BOARD - 1] = RF;
    for(int i = 2; i < BOARD - 2; i += 2)     //四边搭建
    {
        board[0][i] = TOP;
        board[BOARD - 1][i] = FOOT;
        board[i][0] = LEFT;
        board[i][BOARD - 1] = RIGHT;
    }
    for(int i = 2; i < BOARD - 2; i += 2)
        for(int j = 2; j < BOARD - 2;j += 2)
            board[i][j] = CENTER;         //非边角搭建
}

void print()       //打印棋盘
{
    system("clear");    //清除欢迎界面
    for(int i = 0; i < BOARD; i++)
    {
        for(int j = 0; j < BOARD; j++)
        {
            if((i - 1) % 2 == 0 && (j - 1) % 2 == 0)   //根据格子对应check[][]的值输出
            {
                int temp = check[(i - 1) / 2][(j - 1) / 2];
                if(temp == 0)      //打印空格
                    printf("  ");
                else if(temp == -1)      //打印白子
                    printf("%s ", WHITE);
                else if(temp == 1)      //打印黑子
                    printf("%s ", BLACK);
                else        //打印光标
                    printf("%s ", POS);
            }
            else        //打印棋盘
                printf("%s ", board[i][j]);
        }
        putchar('\n');
    }
    if(turn == 1)     //轮到黑子
        printf("  %s  Black Turn", BLACK);
    else       //轮到白子
        printf("  %s  White Turn", WHITE);
    printf("    Pos(%2d,%2d)  Done:%2d\n", row + 1, col + 1, count);     //光标的位置和反悔的次数
}

int player_player()     //人人
{
    COVERSPACE;      //初始光标位置
    print();                //棋盘整体输出
    system("stty -echo");   //Linux命令   命令不被显示
    while(1)
    {
        if(kbhit())       //检测键盘是否有输入   有输入情况下
        {
            while((sign = getch()) != '`')      //退出按键
            {
                if(sign == 'w' || sign == 's' || sign == 'a' || sign == 'd' || sign == ' ' || sign == 65 || sign == 66 || sign == 67 || sign == 68 || sign == '/' || sign == 'r')
                {
                    if(sign != ' ' && sign != '/')           //移动光标前，先把原格子复位
                    {
                        if(ISCOVERSPACE)
                        {
                            check[row][col] = 0;
                            go_to_xy(2 + 2 * row, 1 + 2 * (1 + 2 * col));
                            printf("%s ", SPACE);
                        }
                        else if(ISCOVERWHITE)
                        {
                            check[row][col] = -1;
                            go_to_xy(2 + 2 * row, 1 + 2 * (1 + 2 * col));
                            printf("%s ", WHITE);
                        }
                        else if(ISCOVERBLACK)
                        {
                            check[row][col] = 1;
                            go_to_xy(2 + 2 * row, 1 + 2 * (1 + 2 * col));
                            printf("%s ", BLACK);
                        }
                    }
                    switch(sign)                //光标移动、下子
                    {
                        case 'w':
                            if(row > 0)
                                row--;
                            else
                                row = LEN - 1;
                            if(NOTCHESS)
                                COVERSPACE;
                            else if(ISWHITE)
                                COVERWHITE;
                            else if(ISBLACK)
                                COVERBLACK;
                            go_to_xy(BOARD + 1, 0);
                            if(turn > 0)
                                printf("  %s  Black Turn", BLACK);
                            else
                                printf("  %s  White Turn", WHITE);
                            printf("    Pos(%2d,%2d)  Done:%2d\n", row + 1, col + 1, count);
                            go_to_xy(2 + 2 * row, 1 + 2 * (1 + 2 * col));
                            printf("%s ", POS);
                            break;
                        case 's':
                            if(row < LEN - 1)
                                row++;
                            else
                                row = 0;
                            if(NOTCHESS)
                                COVERSPACE;
                            else if(ISWHITE)
                                COVERWHITE;
                            else if(ISBLACK)
                                COVERBLACK;
                            go_to_xy(BOARD + 1, 0);
                            if(turn > 0)
                                printf("  %s  Black Turn", BLACK);
                            else
                                printf("  %s  White Turn", WHITE);
                            printf("    Pos(%2d,%2d)  Done:%2d\n", row + 1, col + 1, count);
                            go_to_xy(2 + 2 * row, 1 + 2 * (1 + 2 * col));
                            printf("%s ", POS);
                            break;
                            break;
                        case 'a':
                            if(col > 0)
                                col--;
                            else
                                col = LEN - 1;
                            if(NOTCHESS)
                                COVERSPACE;
                            else if(ISWHITE)
                                COVERWHITE;
                            else if(ISBLACK)
                                COVERBLACK;
                            go_to_xy(BOARD + 1, 0);
                            if(turn > 0)
                                printf("  %s  Black Turn", BLACK);
                            else
                                printf("  %s  White Turn", WHITE);
                            printf("    Pos(%2d,%2d)  Done:%2d\n", row + 1, col + 1, count);
                            go_to_xy(2 + 2 * row, 1 + 2 * (1 + 2 * col));
                            printf("%s ", POS);
                            break;
                        case 'd':
                            if(col < LEN - 1)
                                col++;
                            else
                                col = 0;
                            if(NOTCHESS)
                                COVERSPACE;
                            else if(ISWHITE)
                                COVERWHITE;
                            else if(ISBLACK)
                                COVERBLACK;
                            go_to_xy(BOARD + 1, 0);
                            if(turn > 0)
                                printf("  %s  Black Turn", BLACK);
                            else
                                printf("  %s  White Turn", WHITE);
                            printf("    Pos(%2d,%2d)  Done:%2d\n", row + 1, col + 1, count);
                            go_to_xy(2 + 2 * row, 1 + 2 * (1 + 2 * col));
                            printf("%s ", POS);
                            break;
                        case 65:
                            if(row > 0)
                                row--;
                            else
                                row = LEN - 1;
                            if(NOTCHESS)
                                COVERSPACE;
                            else if(ISWHITE)
                                COVERWHITE;
                            else if(ISBLACK)
                                COVERBLACK;
                            go_to_xy(BOARD + 1, 0);
                            if(turn > 0)
                                printf("  %s  Black Turn", BLACK);
                            else
                                printf("  %s  White Turn", WHITE);
                            printf("    Pos(%2d,%2d)  Done:%2d\n", row + 1, col + 1, count);
                            go_to_xy(2 + 2 * row, 1 + 2 * (1 + 2 * col));
                            printf("%s ", POS);
                            break;
                        case 66:
                            if(row < LEN - 1)
                                row++;
                            else
                                row = 0;
                            if(NOTCHESS)
                                COVERSPACE;
                            else if(ISWHITE)
                                COVERWHITE;
                            else if(ISBLACK)
                                COVERBLACK;
                            go_to_xy(BOARD + 1, 0);
                            if(turn > 0)
                                printf("  %s  Black Turn", BLACK);
                            else
                                printf("  %s  White Turn", WHITE);
                            printf("    Pos(%2d,%2d)  Done:%2d\n", row + 1, col + 1, count);
                            go_to_xy(2 + 2 * row, 1 + 2 * (1 + 2 * col));
                            printf("%s ", POS);
                            break;
                        case 68:
                            if(col > 0)
                                col--;
                            else
                                col = LEN - 1;
                            if(NOTCHESS)
                                COVERSPACE;
                            else if(ISWHITE)
                                COVERWHITE;
                            else if(ISBLACK)
                                COVERBLACK;
                            go_to_xy(BOARD + 1, 0);
                            if(turn > 0)
                                printf("  %s  Black Turn", BLACK);
                            else
                                printf("  %s  White Turn", WHITE);
                            printf("    Pos(%2d,%2d)  Done:%2d\n", row + 1, col + 1, count);
                            go_to_xy(2 + 2 * row, 1 + 2 * (1 + 2 * col));
                            printf("%s ", POS);
                            break;
                        case 67:
                            if(col < LEN - 1)
                                col++;
                            else
                                col = 0;
                            if(NOTCHESS)
                                COVERSPACE;
                            else if(ISWHITE)
                                COVERWHITE;
                            else if(ISBLACK)
                                COVERBLACK;
                            go_to_xy(BOARD + 1, 0);
                            if(turn > 0)
                                printf("  %s  Black Turn", BLACK);
                            else
                                printf("  %s  White Turn", WHITE);
                            printf("    Pos(%2d,%2d)  Done:%2d\n", row + 1, col + 1, count);
                            go_to_xy(2 + 2 * row, 1 + 2 * (1 + 2 * col));
                            printf("%s ", POS);
                            break;
                        case ' ':
                            if(ISCOVERSPACE)
                            {
                                if(turn == -1)
                                {
                                    go_to_xy(2 + 2 * row, 1 + 2 * (1 + 2 * col));
                                    printf("%s ", WHITE);
                                    check[row][col] = turn;
                                }
                                else if(turn == 1)
                                {
                                    go_to_xy(2 + 2 * row, 1 + 2 * (1 + 2 * col));
                                    printf("%s ", BLACK);
                                    check[row][col] = turn;
                                }
                                reg[count][0] = turn;
                                reg[count][1] = row;
                                reg[count][2] = col;
                                turn = -turn;
                                count++;
                                go_to_xy(BOARD + 1, 0);
                                if(turn > 0)
                                    printf("  %s  Black Turn", BLACK);
                                else
                                    printf("  %s  White Turn", WHITE);
                                printf("    Pos(%2d,%2d)  Done:%2d\n", row + 1, col + 1, count);
                            }
                            break;
                        case '/':
                            if(ISCOVERSPACE)
                            {
                                if(turn == -1)
                                {
                                    go_to_xy(2 + 2 * row, 1 + 2 * (1 + 2 * col));
                                    printf("%s ", WHITE);
                                    check[row][col] = turn;
                                }
                                else if(turn == 1)
                                {
                                    go_to_xy(2 + 2 * row, 1 + 2 * (1 + 2 * col));
                                    printf("%s ", BLACK);
                                    check[row][col] = turn;
                                }
                                reg[count][0] = turn;
                                reg[count][1] = row;
                                reg[count][2] = col;
                                turn = -turn;
                                count++;
                                go_to_xy(BOARD + 1, 0);
                                if(turn > 0)
                                    printf("  %s  Black Turn", BLACK);
                                else
                                    printf("  %s  White Turn", WHITE);
                                printf("    Pos(%2d,%2d)  Done:%2d\n", row + 1, col + 1, count);
                            }
                            break;
                        case 'r':       //悔棋按钮
                            regret();
                            go_to_xy(2 + 2 * row, 1 + 2 * (1 + 2 * col));
                            printf("%s ", POS);
                            break;
                        default:
                            break;
                    }
                }
                break;
            }
            if(sign == '`')         //强制退出
                return 2;
        }
        if(count == LEN * LEN)          //如果棋盘满 返回平局
            return 0;
        if(check_chess())              //如果一方赢了 返回分出胜负
            return 1;
    }
}

int player_computer()     //人机
{
    if(order == -1)
        COVERSPACE;   //初始光标位置
    print();
    system("stty -echo");   //使命令不被显示
    while(1)
    {
        if(kbhit())   //检测键盘是否有输入
        {
            while((sign = getch()) != '`')      //退出按键
            {
                if(sign == 'w' || sign == 's' || sign == 'a' || sign == 'd' || sign == ' ' || sign == 65 || sign == 66 || sign == 67 || sign == 68 || sign == '/' || sign == 'r')
                {
                    if(sign != ' ' && sign != '/')      //移动光标前把原格子复位
                    {
                        if(ISCOVERSPACE)
                        {
                            check[row][col] = 0;
                            go_to_xy(2 + 2 * row, 1 + 2 * (1 + 2 * col));
                            printf("%s ", SPACE);
                        }
                        else if(ISCOVERWHITE)
                        {
                            check[row][col] = -1;
                            go_to_xy(2 + 2 * row, 1 + 2 * (1 + 2 * col));
                            printf("%s ", WHITE);
                        }
                        else if(ISCOVERBLACK)
                        {
                            check[row][col] = 1;
                            go_to_xy(2 + 2 * row, 1 + 2 * (1 + 2 * col));
                            printf("%s ", BLACK);
                        }
                    }
                    switch(sign)    //光标移动和下子
                    {
                        case 65:
                            if(row > 0)
                                row--;
                            else
                                row = LEN - 1;
                            if(NOTCHESS)         //光标覆盖时空白处的值
                                COVERSPACE;
                            else if(ISWHITE)      //光标覆盖时白子的值
                                COVERWHITE;
                            else if(ISBLACK)      //光标覆盖时黑子的值
                                COVERBLACK;
                            go_to_xy(BOARD + 1, 0);
                            if(turn > 0)
                                printf("  %s  Black Turn", BLACK);
                            else
                                printf("  %s  White Turn", WHITE);
                            printf("    Pos(%2d,%2d)  Done:%2d\n", row + 1, col + 1, count);
                            go_to_xy(2 + 2 * row, 1 + 2 * (1 + 2 * col));
                            printf("%s ", POS);
                            break;
                        case 66:
                            if(row < LEN - 1)
                                row++;
                            else
                                row = 0;
                            if(NOTCHESS)
                                COVERSPACE;
                            else if(ISWHITE)
                                COVERWHITE;
                            else if(ISBLACK)
                                COVERBLACK;
                            go_to_xy(BOARD + 1, 0);
                            if(turn > 0)
                                printf("  %s  Black Turn", BLACK);
                            else
                                printf("  %s  White Turn", WHITE);
                            printf("    Pos(%2d,%2d)  Done:%2d\n", row + 1, col + 1, count);
                            go_to_xy(2 + 2 * row, 1 + 2 * (1 + 2 * col));
                            printf("%s ", POS);
                            break;
                        case 68:
                            if(col > 0)
                                col--;
                            else
                                col = LEN - 1;
                            if(NOTCHESS)
                                COVERSPACE;
                            else if(ISWHITE)
                                COVERWHITE;
                            else if(ISBLACK)
                                COVERBLACK;
                            go_to_xy(BOARD + 1, 0);
                            if(turn > 0)
                                printf("  %s  Black Turn", BLACK);
                            else
                                printf("  %s  White Turn", WHITE);
                            printf("    Pos(%2d,%2d)  Done:%2d\n", row + 1, col + 1, count);
                            go_to_xy(2 + 2 * row, 1 + 2 * (1 + 2 * col));
                            printf("%s ", POS);
                            break;
                        case 67:
                            if(col < LEN - 1)
                                col++;
                            else
                                col = 0;
                            if(NOTCHESS)
                                COVERSPACE;
                            else if(ISWHITE)
                                COVERWHITE;
                            else if(ISBLACK)
                                COVERBLACK;
                            go_to_xy(BOARD + 1, 0);
                            if(turn > 0)
                                printf("  %s  Black Turn", BLACK);
                            else
                                printf("  %s  White Turn", WHITE);
                            printf("    Pos(%2d,%2d)  Done:%2d\n", row + 1, col + 1, count);
                            go_to_xy(2 + 2 * row, 1 + 2 * (1 + 2 * col));
                            printf("%s ", POS);
                            break;
                        case ' ':
                            if(ISCOVERSPACE)
                            {
                                if(turn == -1)
                                {
                                    go_to_xy(2 + 2 * row, 1 + 2 * (1 + 2 * col));
                                    printf("%s ", WHITE);
                                    check[row][col] = turn;
                                }
                                else if(turn == 1)
                                {
                                    go_to_xy(2 + 2 * row, 1 + 2 * (1 + 2 * col));
                                    printf("%s ", BLACK);
                                    check[row][col] = turn;
                                }
                                reg[count][0] = turn;
                                reg[count][1] = row;
                                reg[count][2] = col;
                                turn = -turn;
                                count++;
                                go_to_xy(BOARD + 1, 0);
                                if(turn > 0)
                                    printf("  %s  Black Turn", BLACK);
                                else
                                    printf("  %s  White Turn", WHITE);
                                printf("    Pos(%2d,%2d)  Done:%2d\n", row + 1, col + 1, count);
                            }
                            break;
                        case 'r':       //悔棋按钮
                            if(count == 1)
                                regret();
                            else
                            {
                                regret();
                                regret();
                                go_to_xy(2 + 2 * row, 1 + 2 * (1 + 2 * col));
                                printf("%s ", POS);
                            }
                            break;
                        default:
                            break;
                    }
                }
                break;
            }
            if(sign == '`')   //强制退出
                return 2;
        }
        else if(turn == -order)   //电脑下棋
        {
            ai();
            if(turn == -1)
            {
                go_to_xy(2 + 2 * row, 1 + 2 * (1 + 2 * col));
                printf("%s ", WHITE);
                check[row][col] = turn;
            }
            else if(turn == 1)
            {
                go_to_xy(2 + 2 * row, 1 + 2 * (1 + 2 * col));
                printf("%s ", BLACK);
                check[row][col] = turn;
            }
            reg[count][0] = turn;
            reg[count][1] = row;
            reg[count][2] = col;
            turn = -turn;
            count++;
            go_to_xy(BOARD + 1, 0);
            if(turn > 0)
                printf("  %s  Black Turn", BLACK);
            else
                printf("  %s  White Turn", WHITE);
            printf("    Pos(%2d,%2d)  Done:%2d\n", row + 1, col + 1, count);
        }
        if(count == LEN * LEN)     //如果棋盘满 返回平局
            return 0;
        if(check_chess())   //如果一方赢了 返回分出胜负
            return 1;
    }
}

void ai()    //电脑下子
{
    memset(value, 0, sizeof(value));    //清空评分数组
    calculate_value();
    int sum_i[LEN * LEN], sum_j[LEN * LEN];
    int max = 0, sum = 0;
    for(int i = 0; i < LEN; i++)
        for(int j = 0; j < LEN; j++)
            if(check[i][j] == 0 && value[i][j] > max)
                max = value[i][j];
    for(int i = 0; i < LEN; i++)
    {
        for(int j = 0; j < LEN; j++)
        {
            if(check[i][j] == 0 && value[i][j] == max)
            {
                sum_i[sum] = i;
                sum_j[sum] = j;
                sum++;
            }
        }
    }
    srand((unsigned)time(0));
    row = sum_i[rand() % sum];
    col = sum_j[rand() % sum];
}

void calculate_value()
{
    //统计玩家或者电脑连成的子
    int white_num = 0;       //玩家连成子的个数
    int black_num = 0;       //AI连成子的个数
    int empty_num = 0;      //各方向空白位的个数
    int player = order, computer = -order;
    //计分
    for(int i = 0; i < LEN; i++)
    {
        for(int j = 0; j < LEN; j++)
        {
            if(check[i][j] == 0)    //空白点
            {
                //遍历周围八个方向
                for(int y = -1; y <= 1; y++)
                {
                    for(int x = -1; x <= 1; x++)
                    {
                        white_num = 0;
                        black_num = 0;
                        empty_num = 0;
                        if(!(y == 0 && x == 0))          //原坐标不算
                        {
                            //对玩家评分
                            for(int k = 1; k <= 4; k++)//每个方向延伸4个子
                            {
                                if(i + k * y > 0 && i + k * y < LEN && j + k * x > 0 && j + k * x < LEN && check[i + k * y][j + k * x] == player)            //玩家的子
                                    white_num++;
                                else if(i + k * y > 0 && i + k * y < LEN && j + k * x > 0 && j + k * x < LEN && check[i + k * y][j + k * x] == 0)        //空白位
                                {
                                    empty_num++;
                                    break;
                                }
                                else          //出边界
                                    break;
                            }
                            for(int k = 1; k <= 4; k++)
                            {
                                if(i - k * y > 0 && i - k * y < LEN && j - k * x > 0 && j - k * x < LEN && check[i - k * y][j - k * x] == player)     //玩家的子
                                    white_num++;
                                else if(i - k * y > 0 && i - k * y < LEN && j - k * x > 0 && j - k * x < LEN && check[i - k * y][j - k * x] == 0)      //空白位
                                {
                                    empty_num++;
                                    break;
                                }
                                else     //出边界
                                    break;
                            }
                            if(white_num == 1)
                                value[i][j] += 10;
                            else if(white_num == 2)
                            {
                                if(empty_num == 1)
                                    value[i][j] += 90;
                                else if(empty_num == 2)
                                    value[i][j] += 105;
                            }
                            else if(white_num == 3)
                            {
                                //量变空位不一样，优先级不一样
                                if(empty_num == 0)
                                    value[i][j] += 675;
                                else if(empty_num == 1)
                                    value[i][j] += 750;
                            }
                            else if(white_num == 4)
                                value[i][j] += 10100;
                            empty_num = 0;
                            //对电脑的子评分
                            for(int k = 1; k <= 4; k++)
                            {
                                if(i + k * y > 0 && i + k * y < LEN && j + k * x > 0 && j + k * x < LEN && check[i + k * y][j + k * x] == computer)      //电脑的子
                                    black_num++;
                                else if(i + k * y > 0 && i + k * y < LEN && j + k * x > 0 && j + k * x < LEN && check[i + k * y][j + k * x] == 0)    //空白位
                                {
                                    empty_num++;
                                    break;
                                }
                                else      //出边界
                                    break;
                            }
                            for(int k = 1; k <= 4; k++)
                            {
                                if(i - k * y > 0 && i - k * y < LEN && j - k * x > 0 && j - k * x < LEN && check[i - k * y][j - k * x] == computer) //电脑的子
                                    black_num++;
                                else if(i - k * y > 0 && i - k * y < LEN && j - k * x > 0 && j - k * x < LEN && check[i - k * y][j - k * x] == 0) //空白位
                                {
                                    empty_num++;
                                    break;
                                }
                                else      //出边界
                                    break;
                            }
                            if(black_num == 0)
                                value[i][j] += 5;
                            else if(black_num == 1)
                                value[i][j] += 15;
                            else if(black_num == 2)
                            {
                                if(empty_num == 1)
                                    value[i][j] += 85;
                                else if(empty_num == 2)
                                    value[i][j] += 95;
                            }
                            else if(black_num == 3)
                            {
                                if(empty_num == 0)
                                    value[i][j] += 650;
                                else if (empty_num == 1)
                                    value[i][j] += 700;
                            }
                            else if(black_num >= 4)
                                value[i][j] += 10000;
                        }
                    }
                }
            }
        }
    }
}

int check_chess()      //检测是否分出胜负
{
    bool judge;       //胜利判断
    int temp;       //当前格子对应值
    for(int i = 0; i < LEN; i++)     //行
    {
        for(int j = 0; j < LEN; j++)     //列
        {
            if(check[i][j] != 0)
            {
                temp = check[i][j];
                judge = true;
                if(i <= LEN - 5 && j <= LEN - 5)    //右斜
                {
                    for(int l = 1; l < 5; l++)
                        if(check[i + l][j + l] != temp)
                            judge = false;
                    if(judge == true)
                        return 1;
                }
                judge = true;
                if(i <= LEN - 5 && j >= 5 - 1)     //左斜
                {
                    for(int l = 1; l < 5; l++)
                        if(check[i + l][j - l] != temp)
                            judge = false;
                    if(judge == true)
                        return 1;
                }
                judge = true;
                if(j <= LEN - 5)   //水平
                {
                    for(int l = 1; l < 5; l++)
                        if(check[i][j + l] != temp)
                            judge = false;
                    if(judge == true)
                        return 1;
                }
                judge = true;
                if(i <= LEN - 5)   //竖直
                {
                    for(int l = 1; l < 5; l++)
                        if(check[i + l][j] != temp)
                            judge = false;
                    if(judge == true)
                        return 1;
                }
            }
        }
    }
    return 0;
}

void game_over(int res)    //游戏结束后的判定输出
{
    go_to_xy(BOARD + 2, 0);     //把输入光标移去底部
    if(res)
    {
        if(res == 2)     //s=2则平局
            printf("Force Exit\n");
        else if(turn == 1)    //turn=1即最后一个落子的为白棋，故白棋赢
            printf("\t-----%s White Won-----\n", WHITE);
        else if(turn == -1)    //同理
            printf("\t-----%s Black Won-----\n", BLACK);
        else
            printf("Error!\n");
    }
    else
        printf("\t-----Tied-----\n");
    system("stty echo");
    printf("\033[?25h");    //显示光标
}

void go_to_xy(unsigned x, unsigned y)  //将输入光标移动到指定位置
{
    printf("\033[%d;%dH", x, y);   //设置光标位置
}

void regret()    //悔棋
{
    if(count)
    {
        count--;
        turn = -turn;
        row = reg[count][1];      //根据数组记录内容悔棋
        col = reg[count][2];
        for(int j = 0; j < 3; j++)
            reg[count][j] = 0;
    }
    go_to_xy(BOARD + 1, 0);
    if(turn > 0)
        printf("  %s  Black Turn", BLACK);
    else
        printf("  %s  White Turn", WHITE);
    printf("    Pos(%2d,%2d)  Done:%2d\n", row + 1, col + 1, count);
    go_to_xy(2 + 2 * row, 1 + 2 * (1 + 2 * col));
    printf("%s ", SPACE);
    COVERSPACE;
}

int main()
{
    int res = 0;   //游戏结束的判定状态输出
    welcome();   //欢迎界面
    while(1)     //选择游戏模式
    {
        sign = getch();    //读取键盘输入
        if(sign == '1')     //选择人人模式
        {
            init_board();
            res = player_player();
            break;
        }
        else if(sign == '2')     //选择人机模式（先手
        {
            init_board();
            order = -1;
            res = player_computer();
            break;
        }
        else if(sign == '3')     //选择人机模式（后手
        {
            init_board();
            order = 1;
            res = player_computer();
            break;
        }
        else if(sign == '`')     //退出游戏
            break;
        else       //键入其他字符不会有任何显示
            continue;
    }
    game_over(res);      //游戏结束
    return 0;
}
