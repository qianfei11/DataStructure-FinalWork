#include <termios.h>     //Linux专用头文件，用于实现getch()、kbhit()
#include <unistd.h>      //定义了大量针对系统调用的封装的接口   提供对 POSIX 操作系统 API 的访问功能
#include <fcntl.h>       //定义了很多宏和函数原型   对文件的打开、数据写入、数据读取、关闭文件的操作
#include <stdio.h>

int getch()             //输入不需要回车键便返回
{
    struct termios tm, tm_old;
    if(tcgetattr(STDIN_FILENO, &tm) < 0)      //STDIN_FILENO是终端文件描述符，是一个打开文件句柄
        return -1;
    tm_old = tm;
    cfmakeraw(&tm);     //设置串口属性   将终端设置成原始模式，此时终端是不可回显的
    if(tcsetattr(STDIN_FILENO, TCSANOW, &tm) < 0)       //更改终端设置
        return -1;
    int ch = getchar();
    if(tcsetattr(STDIN_FILENO, TCSANOW, &tm_old) < 0)      //恢复终端原始设置
        return -1;
    return ch;
}

int kbhit()        //检测键盘是否有输入
{
    struct termios oldt, newt;    //控制终端状态的数据结构
    int ch;
    int oldf;
    tcgetattr(STDIN_FILENO, &oldt);  //得到当前终端的状态
    newt = oldt;
    //设置新的终端属性：ICANON和ECHO取反，表示关闭输入行编辑模式，即能够直接读入字符
    newt.c_lflag &= ~(ICANON | ECHO); //ICANON：如果在输入中看到'\n'或者'EOF'，会返回缓冲区内容 //ECHO：控制回显
    tcsetattr(STDIN_FILENO, TCSANOW, &newt); //TSCSANOW：告诉函数立即改变终端的STDIN_FILENO的属性值
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0); //获取flags，即open函数的第二个参数
    //fcntl()的返回值与命令有关。如果出错，所有命令都返回－1，如果成功则返回某个其他值
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK); //增加一个flags，把文件设置成非阻塞  //F_SETFL设置文件状态标记
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); //恢复终端设置
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if(ch != EOF)
    {
        ungetc(ch, stdin); //把字符退回到输入流
        return 1;
    }
    return 0;
}