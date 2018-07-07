#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<string.h>
#include<stdlib.h>
#include<sys/time.h>
#include<time.h>
#include"./keyboard/keyboard.h"

int FC = 5;
//#define FC 5//设置方块颜色
#define BC 0//规定背景色,灰白
#define W 10//规定地图的宽度
#define H 20//规定地图的高度

struct data
{
    int x;//列
    int y;//行
};
int cur = 0;

//图形的初始下落位置
struct data t = {5,0};//定义全局变量，表示图案的坐标

struct shape
{
    int s[5][5];//用数组表示图
};

int background[H][W] = {};//保存背景的数组


//定义7个数组存放7种图案
struct shape shape_arr[7] =
{
    {
        0,0,0,0,0,
        0,0,1,0,0,
        0,1,1,1,0,
        0,0,0,0,0,
        0,0,0,0,0,
    },
    {
        0,0,0,0,0,
        0,1,1,0,0,
        0,0,1,1,0,
        0,0,0,0,0,
        0,0,0,0,0,
    },
    {
        0,0,0,0,0,
        0,0,0,1,0,
        0,1,1,1,0,
        0,0,0,0,0,
        0,0,0,0,0,
    },
    {
        0,0,0,0,0,
        0,0,0,0,0,
        0,1,1,1,0,
        0,0,0,1,0,
        0,0,0,0,0,
    },
    {
        0,0,0,0,0,
        0,0,1,1,0,
        0,1,1,0,0,
        0,0,0,0,0,
        0,0,0,0,0,
    },
    {
        0,0,0,0,0,
        0,0,1,1,0,
        0,0,1,1,0,
        0,0,0,0,0,
        0,0,0,0,0,
    },
    {
        0,0,0,0,0,
        0,0,1,0,0,
        0,0,1,0,0,
        0,0,1,0,0,
        0,0,0,0,0,
    }
};

void drow_element(int x,int y,int c)
{
    //设置光标位置,第一个数字为纵坐标，第二个数字为横坐标，屏幕上坐标原点为(1,1),横：左到右，纵：上到下
    x *= 2;//一个方框占两个横坐标
    x++;//习惯上坐标原点从（0，0）开始，所以要自增
    y++;

    printf("\033[?25l");//隐藏光标

    printf("\033[%d;%dH",y,x);//控制坐标位置
    //设置前景色（字体颜色）为紫色,背景色为绿色，多个属性可以设置在一行
    printf("\033[3%dm\033[4%dm",c,c);//，设置颜色，前景色和背景色相同
    printf("[]");//输出方块
    fflush(stdout);//刷新缓存
    printf("\033[0m");//关闭所有属性
}

//在屏幕上绘制图案
//x表示第几列，y表示第几行
void drow_shape(int x,int y,struct shape p,int c)
{
    int i = 0;
    for(i = 0;i < 5;i++)
    {
        int j = 0;
        for(j = 0;j< 5;j++)
        {
            if(p.s[i][j] != 0)
            {
                drow_element(x + j,y + i,c);//j表示的是列，i表示的是行，x表示的是第几列，y表示的是第几行
            }
        }
    }
}

int can_move(int x,int y,struct shape p)
{
    int i = 0;
    for(i = 0;i < 5;i++)
    {
        int j = 0;
        for(j = 0;j < 5;j++)
        {
            if(p.s[i][j] == 0)
            {
                continue;
            }
            if(x + j >= W)
            {
                return 0;
            }
            if(y + i >= H -1)
            {
                return 0;
            }
            if(x + j < 0)
            {
                return 0;
            }
            if(background[y + i + 1][x + j] != 0)
            {
                return 0;
            }
        }
    }
    return 1;
}

//根据背景数组设置背景绘制背景（地图）
void drow_back()
{
    int i = 0;
    for(i = 0;i < H;i++)
    {
        int j = 0;
        for(j = 0;j < W;j++)
        {
            if(background[i][j] == 0)
            {
                drow_element(j,i,BC);
            }
            else
            {
                drow_element(j,i,background[i][j]);

            }
        }
    }
}

//设置背景数组
void set_back(struct data* t,struct shape p)
{
    int i = 0;
    for(i = 0;i < 5;i++)
    {
        int j = 0;
        for(j = 0;j < 5;j++)
        {
            if(p.s[i][j] != 0)
            {
                background[t->y + i][t->x + j] = FC;
            }
        }
    }
}

//清行
void mclean_line()
{
    int i = H - 1;
    for(i = 0;i < H;i++)
    {
        int j = 0;
        int total = 0;
        for(j = 0;j < W;j++)
        {
            if(background[i][j] != 0)
            {
                total++;
            }
        }
        if(total == W)
        {
            int k = i;
            for(;k > 0;k--)
            {
                memcpy(background[k],background[k - 1],sizeof(background[k]));
            }
            memset(background[0],0x00,sizeof(background[0]));
        }
    }
}

//图案定时下落,
void tetris_timer(struct data* t)
{
    drow_shape(t->x,t->y,shape_arr[cur],BC);//绘制图案
    if(can_move(t->x,t->y,shape_arr[cur]))
    {
        t->y++;
    }
    //如果不能下落了，就设置背景数组绘制背景
    else
    {
        set_back(t,shape_arr[cur]);
        mclean_line();
        drow_back();
        do
        {
           FC = rand() % 7;
        }while(FC == BC);
        t->y = 0;
        t->x = 0;
        cur = rand() % 7;
    }
    drow_shape(t->x,t->y,shape_arr[cur],FC);//绘制图案
}

struct shape turn_90(struct shape p)
{
    struct shape t;

    int i = 0;
    for(i = 0;i < 5;i++)
    {
        int j = 0;
        for(j = 0;j < 5;j++)
        {
            t.s[i][j] = p.s[4-j][i];
        }
    }

    return t;
}
//根据终端按键绘制图案
int tetirs(struct data* t)
{
    int ret = 0;
    int c = get_key();
    if(is_up(c))//向上进行旋转
    {
        drow_shape(t->x,t->y,shape_arr[cur],BC);
        shape_arr[cur] = turn_90(shape_arr[cur]);
        //如果旋转之后的图像还在地图内，就旋转，否则不旋转
        if(can_move(t->x,t->y,shape_arr[cur]) == 0)
        {
            shape_arr[cur] = turn_90(shape_arr[cur]);
            shape_arr[cur] = turn_90(shape_arr[cur]);
            shape_arr[cur] = turn_90(shape_arr[cur]);
        }
        drow_shape(t->x,t->y,shape_arr[cur],FC);
    }
    else if(is_left(c))
    {
        drow_shape(t->x,t->y,shape_arr[cur],BC);
        if(can_move(t->x - 1,t->y,shape_arr[cur]))
        {
            t->x--;
        }
        drow_shape(t->x,t->y,shape_arr[cur],FC);
    }
    else if(is_right(c))
    {
        drow_shape(t->x,t->y,shape_arr[cur],BC);
        if(can_move(t->x + 1,t->y,shape_arr[cur]))
        {
            t->x++;
        }
        drow_shape(t->x,t->y,shape_arr[cur],FC);
    }
    else if(is_down(c))
    {
        drow_shape(t->x,t->y,shape_arr[cur],BC);
        if(can_move(t->x,t->y + 1,shape_arr[cur]))
        {
            t->y++;
        }
        drow_shape(t->x,t->y,shape_arr[cur],FC);
    }
    else if(is_esc(c))//如果按了esc键，表示结束游戏，此时将返回值设置为1
    {
        ret = 1;
    }

    return ret;
}


void handler(int s)
{
    if(s == SIGALRM)
    {
        tetris_timer(&t);
    }
    else if(s == SIGINT)
    {
        printf("\033[0m");
        printf("\033[?25h");
        recover_keyboard();
        exit(0);
    }
}
int main()
{
    srand((unsigned int)time(NULL));
    init_keyboard();//将键盘的属性改为非标准输入，以及不回显
    //绘制背景
    drow_back();

    struct sigaction act;
    act.sa_handler = handler;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);//信号处理的时候的信号屏蔽字
    sigaction(SIGALRM,&act,NULL);
    sigaction(SIGINT,&act,NULL);


    struct itimerval it;//查看该结构体
    //初始设定的时间
    it.it_value.tv_sec = 0;
    it.it_value.tv_usec = 1;//如果二者均为0，表示永远不启动定时器
    
    //定时器的时间间隔
    it.it_interval.tv_sec = 1;
    it.it_interval.tv_usec = 0;
    


    //设置定时器，从0秒开始，1秒钟发送一个SIGARLM信号
    //然后对SIGALRM信号进行捕捉，在信号捕捉时对图形的下落函数进行捕捉
    setitimer(ITIMER_REAL,&it,NULL);//按桌面时钟进行

    //模拟方块下落的过程
  //  int i = 0;
  //  for(i = 0;i < 10;i++)
  //  {
  //      drow_shape(5,i,shape_arr[0],FC);//绘制图案
  //      sleep(1);
  //      drow_shape(5,i,shape_arr[0],BC);//清理图案

  //  }
    while(1)
    {
    //    tetris_timer(&t);
        if(tetirs(&t) == 1)
        {
            break;
        }
    //    sleep(1);
    }
    printf("\033[?25h");//显示光标
    recover_keyboard();//把键盘的输入恢复成标准输入
    return 0;
}
