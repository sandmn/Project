#include "comm.h"

void Init(char arr[ROW][COL], int x, int y)//数组的初始化函数定义
{
    memset(arr, ' ', x*y*sizeof(arr[0][0]));
}

void menu()//菜单函数定义
{
    printf("*****************************\n");
    printf("****1.开始游戏 0.退出游戏****\n");
    printf("*****************************\n");
}

void print(char arr[ROW][COL], int x, int y)//打印棋盘函数定义
{
    int j = 0;//j表示列信息

    printf("   ");
    for(j = 0;j < y;j++)
    {
        printf(" %2d ",j+1);
    }
    printf("\n");
    printf("  ");
    for(j = 0;j < y;j++)
    {
        printf("----");
    }
    printf("-");
    printf("\n");
    int i = 0;//i表示行信息
    for(i = 0;i < x;i++)
    {
        printf("%-2d|",i+1);
        for(j = 0;j < y;j++)
        {
            printf(" %c |",arr[i][j]);
        }
        printf("\n");
        printf("  ");
        for(j = 0;j < y;j++)
        {
            printf("----");
        }
        printf("-");
        printf("\n");
    }
}

int who_win(int ret)//判断是谁赢
{
    if (ret == 'Y')
    {
        printf("玩家1赢\n");
        return 1;
    }
    else if (ret == '*')
    {
        printf("玩家2赢\n");
        return 1;
    }
    else if (ret == 0)
    {
        printf("平局\n");
        return 1;
    }
    return 0;
}

//判断是否有人赢
int  iswin(char arr[ROW][COL], int x, int y)
{
    int i = 0;//i表示行信息
    int j = 0;//j表示列信息
    int count = 0;//统计棋盘上空格的个数
    
    //正对角赢
    for(i = 0;i <= x - 5;i++)
    {
        for(j = 0;j <= y - 5;j++)
        {
            if(( arr[i][j] == arr[i+1][j+1] )
                    &&( arr[i+1][j+1] == arr[i+2][j+2] )
                    &&( arr[i+2][j+2] == arr[i+3][j+3] )
                    &&( arr[i+3][j+3] == arr[i+4][j+4] ) )
            {
                if(arr[i][j] == '*' || arr[i][j] == 'Y')
                {
                    printf("正对角赢\n");
                    return arr[i][j];
                }
            }
        }
    }
    //斜对角赢
    for(i = 0;i <= y - 5;i++)
    {
        for(j = 4;j < x;j++)
        {
            if(( arr[i][j] == arr[i+1][j-1] )
                    &&( arr[i+1][j-1] == arr[i+2][j-2] )
                    &&( arr[i+2][j-2] == arr[i+3][j-3] )
                    &&( arr[i+3][j-3] == arr[i+4][j-4] ))
            {
                if(arr[i][j] == '*' || arr[i][j] == 'Y')
                {
                    printf("斜对角赢\n");
                    return arr[i][j];
                }
            }
        }
    }
    //横向赢
    for(i = 0;i < x;i++)
    {
        for(j = 0;j <= y - 5;j++)
        {
            if(( arr[i][j] == arr[i][j+1] )&&
                    ( arr[i][j+1] == arr[i][j+2] )&&
                    ( arr[i][j+2] == arr[i][j+3] )&&
                    ( arr[i][j+3] == arr[i][j+4] ))
            {
                if(arr[i][j] == '*' || arr[i][j] == 'Y')
                {
                    printf("横向赢\n");
                    return arr[i][j];
                }
            }
        }
    }
    //竖向赢
    for(j = 0;j < y;j++)
    {
        for(i = 0;i <= x - 5;i++)
        {
            if(( arr[i][j] == arr[i+1][j] )&&
                ( arr[i+1][j] == arr[i+2][j] )&& 
                ( arr[i+2][j] == arr[i+3][j] )&&
                ( arr[i+3][j] == arr[i+4][j] ))
            {
                if(arr[i][j] == '*' || arr[i][j] == 'Y')
                {
                    printf("竖向赢\n");
                    return arr[i][j];
                }
            }
        }
    }
    //平局
    for (i = 0; i < x; i++)
    {
        for (j = 0; j < y; j++)
        {
            if (arr[i][j] == ' ')
            {
                count++;//统计棋盘上空格的个数，如果没有空格表示棋盘满了还没分出胜负，则平局
            }
        }
    }
    if (count == 0)
    {
        printf("平局\n");
        return 0;
    }

}

//移动函数
//flag表示是服务器移动还是客户端移动
void move(char arr[ROW][COL],int x,int y,location* loc,char flag)
{
    //count用于标记是否移动，如果移动了count不为0，则本次移动结束
    //如果因为选择位置错误而不能移动，此时count必为0,则需要再次进入循环进行坐标的选择
    int count = 0;
    while(!count)
    {
        scanf("%d%d",&(loc->x),&(loc->y));//服务器走
        if (loc->x >= 1 && loc->x <= x && loc->y >= 1 && loc->y <= y)
        {
            if (arr[loc->x - 1][loc->y - 1] == ' ')
            {
                arr[loc->x - 1][loc->y - 1] = flag;
                count++;
            }
            else
            {
                printf("该位置已满！请重新选择：");
            }
        }
        else
        {
            printf("位置错误！请重新选择：");
        }
    }
}
