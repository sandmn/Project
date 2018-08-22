
#include<iostream>
using namespace std;
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>

#define MAX 1024

//year=1900&month=1&day=1&days=0
//根据年份和月份计算天数
static int GetMonDay(int year,int month)
{
    static int mon_day[13] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
    int day = mon_day[month];
    if(month == 2 &&
            ((year % 4 == 0) && (year % 100 != 0))
            ||(year % 400 == 0))
    {
        day = day + 1;
    }
    return day;
}


//对"<"进行重载
class Date
{
    public:
        //定义缺省的构造函数
        Date(int year = 1997,int month = 2,int day = 11)
        {
            _year = year;
            _month = month;
            _day = day;
        }
        //定义拷贝构造函数
        Date(const Date& d)
        {
            _year = d._year;
            _month = d._month;
            _day = d._day;
        }
        //定义析构函数
        ~Date()
        {

        }
       
        //5. 赋值运算符重载函数：防止自己给自己赋值，提高效率
        Date& operator=(const Date& d)
        {
            if(this != &d)
            {
                _year = d._year;
                _month = d._month;
                _day = d._day;
            }
            return *this;
        }

        //各类运算符重载
        //1.+=运算符进行重载
        Date& operator+=(int day)
        {
            if(day < 0)
            {
                return *this -= (-day);
            }
            _day = _day + day;
            //加完之后的天数不合法
            while(_day > GetMonDay(_year,_month))
            {
                //要对月进行进位，所以先减去当前年当前月的天数，再进行进位
                _day = _day - GetMonDay(_year,_month);
                _month++;//月份加1
                if(_month == 13)//如果加完之后的月份不合法，则对年进行进位，然后月份从1月开始计算
                {
                    _year++;
                    _month = 1;
                }
           }
            return *this;//返回加完天数之后的日期
        }
        //2. "+"运算符进行重载：因为加完之后的原日期不能改变，但是返回值必须是之后加完之后的值
        //                      所以要新创建日期类对象保存加完之后的值然后进行返回
        //                      又因为新创建的对象在函数调用结束后不存在，，所以只能传值返回，不能传引用返回
        Date operator+(int day)
        {
            Date ret(*this);//拷贝构造加之前的对象值
            ret += day;//调用+=对新创建的对象进行加天数计算，加完之后新创建的对象值改变，原对象的真正不改变
            return ret;//返回加完之后的对象
        }
        //因为"+"运算符重载时，必须要进行两次的拷贝构造，如果用"+="调用"+"，则在使用"+="时也要进行两次的拷贝构造
        //所以，为减小代价提高效率，使"+"调用"+="，这样便可以在调用"+="时不用进行两次的拷贝构造
        //3. "-="运算符重载：
        //方法一：
        Date& operator-=(int day)
        {
            if(day < 0)
            {
                return *this += (-day);
            }
            _day = _day - day;
            while(_day <= 0)//减完后的天数不合法
            {
                //此时，需要借位上个月的天数
                _month--;
                if(_month == 0)//如果本年的天数都用完了，此时需要借用上一年的天数
                {
                    _year--;
                    _month = 12;//使上一年从12月开始计算
                }
                _day = _day + GetMonDay(_year,_month);//然后将不合法的天数加新计算月份的天数
            }
            return *this;//返回减完天数之后的月份
        }

        //4. "-"运算符重载
        Date operator-(int day)
        {
            Date ret(*this);
            ret -= day;
            return ret;
        }
        //5. 前置"++"运算符重载：原对象运算完之后的值改变，返回值与改变之后的值相等，即都要改变
        Date& operator++()
        {
            *this += 1;
            return *this;
        }
        //6. 后置"++"运算符重载:原对象运算完之后的值改变，但是返回值与改变之前的值相等
        Date operator++(int)
        {
            Date ret(*this);
            *this += 1;
            return ret;
        }
        //7. 前置"--"运算符重载
        Date& operator--()
        {
            *this -= 1;
            return *this;
        }
        //8. 后置"--"运算符重载
        Date operator--(int)
        {
            Date ret(*this);
            *this -= 1;
            return ret;
        }
        
        //9. ">"运算符重载
        bool operator>(const Date& d)
        {
            if(_year > d._year)
            {
                return true;
            }
            else if(_year == d._year)
            {
                if(_month > d._month)
                {
                    return true;
                }
                else if(_month == d._month)
                {
                    if(_day > d._day)
                    {
                        return true;
                    }
                }
            }
            return false;
        }
        //10."=="运算符重载
        bool operator==(const Date& d)
        {
            return _year == d._year
                && _month == d._month
                && _day == d._day;
        }
        //11. "<"运算符进行重载
        bool operator<(const Date& d)
        {
            return !((*this > d) || (*this == d));
        }
        //12. >= <= !=
        bool operator>=(const Date& d)
        {
            return (*this > d) || (*this == d);
        }
        bool operator<=(const Date& d)
        {
            return (*this < d)||(*this == d);
        }
        bool operator!=(const Date& d)
        {
            return !(*this == d);
        }
        //13. -:日期减日期进行重载
        int operator-(const Date& d)
        {
            //此处应为拷贝构造一个对象
            //如果是一个指针指向两个参数对象，则后面就会利用指针对原对象进行改变
            //但是原对象是不能改变的，所以要重新拷贝构造两个新对象
            Date max(*this);
            Date min(d);
            int flag = 1;//设置符号标志
            if(max < min)
            {
                max = d;
                min = *this;
                flag = -1;
            }
            //设置计数器变量
            int day = 0;
            while(min < max)
            {
                min++;
                day++;
            }
            return day*flag;
        }


        void Display()
        {
            printf("%d-%d-%d\n",_year,_month,_day);
            //cout<<_year<<"-"<<_month<<"-"<<_day<<endl;
        }
    //private:
        int _year;
        int _month;
        int _day;
};
//year=1900&month=1&day=1&add_days=0&add_ans=&sub_days=0&sub_ans=
void mycal(char* buf)
{

    //r=1900&month=1&day=1&add_days=0&add_ans=null&s_year=1900&s_month=1&s_day=1&e_year=1900&e_month=1&e_day=1&f_day=0
    int year,month,day,add_days;
    char* add_ans;

    int s_year,s_month,s_day;
    int e_year,e_month,e_day;
    int f_day;

    strtok(buf,"=&");
    year = atoi((strtok(NULL,"=&")));
    
    strtok(NULL,"=&");
    month = atoi(strtok(NULL,"=&"));
    
    strtok(NULL,"=&");
    day = atoi(strtok(NULL,"=&"));
    
    strtok(NULL,"=&");
    add_days = atoi(strtok(NULL,"=&"));
    
    strtok(NULL,"=&");
    add_ans = strtok(NULL,"=&");
    
    strtok(NULL,"=&");
    s_year = atoi(strtok(NULL,"=&"));

    strtok(NULL,"=&");
    s_month = atoi(strtok(NULL,"=&"));

    strtok(NULL,"=&");
    s_day = atoi(strtok(NULL,"=&"));
    
    strtok(NULL,"=&");
    e_year = atoi(strtok(NULL,"=&"));
    
    strtok(NULL,"=&");
    e_month = atoi(strtok(NULL,"=&"));
   
    strtok(NULL,"=&");
    e_day = atoi(strtok(NULL,"=&"));
   
    strtok(NULL,"=&");
    f_day = atoi(strtok(NULL,"=&"));

   //定义一个日期类对象
   Date d(year,month,day);
   Date add_d = d + add_days;
   sprintf(add_ans,"%d-%d-%d",add_d._year,add_d._month,add_d._day);

   Date d1(s_year,s_month,s_day);
   Date d2(e_year,e_month,e_day);
   f_day = d2 - d1; 

    printf("<html>\n");
    printf("<body bgcolor=\"#00FFFF\">\n");
    printf("<h1 align=\"center\">Welcome to Sandmn's Date calculator!</h1>\n");
    printf("<form>\n");
    printf("<table align=\"center\" border=\"1\">");
    printf("<tr><td>start date<br>");
    printf("<input type=\"TEXT\" name=\"year\" value=\"%d\" style=\"height:20px;width:50px\">\n",year);
    printf("year:\n");
    printf("<input type=\"TEXT\" name=\"month\" value=\"%d\" style=\"height:20px;width:50px\">\n",month);
    printf("month:\n");
    printf("<input type=\"TEXT\" name=\"day\" value=\"%d\" style=\"height:20px;width:50px\">\n",day);
    printf("day:\n");
    printf("<br>\n");
    printf("after:\n");
    printf("<br>");
    printf("<input type=\"TEXT\" name=\"add_days\" value=\"%d\" style=\"height:20px;width:50px\">\n",add_days);
    printf("days\n");
    printf("<br><br>\n");
    printf("end date\n");
    printf("<input type=\"TEXT\" name=\"add_ans\" value=\"%s\">\n",add_ans);
    printf("<br><br></td><td>\n");
    printf("start date:<br>\n");
    printf("<input type=\"TEXT\" name=\"s_year\" value=\"%d\" style=\"height:20px;width:50px\">\n",s_year);
    printf("year\n");
    printf("<input type=\"TEXT\" name=\"s_month\" value=\"%d\" style=\"height:20px;width:50px\">\n",s_month);
    printf("month\n");
    printf("<input type=\"TEXT\" name=\"s_day\" value=\"%d\" style=\"height:20px;width:50px\">\n",s_day);
    printf("day\n");
    printf("<br>end date:<br>\n");
    printf("<input type=\"TEXT\" name=\"e_year\" value=\"%d\" style=\"height:20px;width:50px\">\n",e_year);
    printf("year\n");
    printf("<input type=\"TEXT\" name=\"e_month\" value=\"%d\" style=\"height:20px;width:50px\">\n",e_month);
    printf("month\n");
    printf("<input type=\"TEXT\" name=\"e_day\" value=\"%d\" style=\"height:20px;width:50px\">\n",e_day);
    printf("day\n");
    printf("<br><br>difference\n");
    printf("<input type=\"TEXT\" name=\"f_day\" value=\"%d\" style=\"height:20px;width:50px\">\n",f_day);
    printf("days\n");
    printf("</td></tr></table>");
    printf(" <input type=\"SUBMIT\" value=\"Submit\">\n");
    printf("</form>\n");
    printf("</body>\n");
    printf("</html>\n");

}

int main()
{
    //写任何cgi,都用到下面这部分代码
    char buf[MAX] = {0};
    if(getenv("METHOD"))
    {
        if(strcasecmp(getenv("METHOD"), "GET") == 0)//GET方法->拿query_string
        {
            strcpy(buf, getenv("QUERY_STRING"));
        }
        else//POST方法->拿content-length
        {
            int content_length = atoi(getenv("CONTENT_LENGTH"));
            int i = 0;
            char c;
            for(; i<content_length; ++i)
            {
                read(0, &c, 1);
                buf[i] = c;
            }
            buf[i] = '\0';
        }
    }

    //printf("%s\n", buf);
    mycal(buf);

    return 0;
}





