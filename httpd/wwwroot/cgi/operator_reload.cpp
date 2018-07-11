#include<iostream>
using namespace std;
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
       //定义运算符重载函数： 
       // //1. 赋值运算符重载参数为值传递：此时需要调用一次拷贝构造函数
       //      该方式可行，但代价较大，所以参数采用传引用的方式进行
       // void operator=(Date d)
       // {
       //     _year = d._year;
       //     _month = d._month;
       //     _day = d._day;
       // }

       //  //2. 赋值运算符重载参数为const引用传递，此时不会进行拷贝构造，代价较小
       // void operator=(const Date& d)
       // {
       //     _year = d._year;
       //     _month = d._month;
       //     _day = d._day;
       // }
       
       // //3. 赋值运算符重载函数：传值返回，此时在return时还要再调用一次拷贝构造函数创建临时变量，代价较大
       // Date operator=(const Date& d)
       // {
       //     _year = d._year;
       //     _month = d._month;
       //     _day = d._day;
       //     return d;//此处既可以返回*this，也可返回d,因为可以将d的值赋值给新创建的临时变量
       // }

       // //4. 赋值运算符重载函数：返回值为传引用，因为函数调用结束后，"="两边的对象均存在
       // //   所以可以将任意一个对象的引用返回，这样在连续赋值的时候，可以将右边"="的返回值赋值作为对象传递给左边的"="
       // //   如：d1 = d2 = d3，首先d2调用赋值运算符重载函数：d2.operator=($d2,d3);函数调用结束后返回d2的引用
       // //   然后d1再调用赋值运算符重载函数：d1.operator(&d1,d2)
       // Date& operator=(const Date& d)
       // {
       //     _year = d._year;
       //     _month = d._month;
       //     _day = d._day;
       //     return *this;//此处因为d是const引用，所以不能将d返回，因为权限不能由小到大，若d不为const引用，就可以
       // }
       
        Date& operator=(const Date& d);
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
       // //方法二：
       // Date& operator-=(int day)
       // {
       //     //如果当前的天数小于要减的天数，则先不减
       //     while(_day <= day)
       //     {
       //         _month--;//使月份减1来进行借位
       //         if(_month == 0)
       //         {
       //             _year--;
       //             _month = 12;//当年的月份已完，则向上一年借位
       //         }
       //         day = day - _day;//将要减的天数减去现有的天数
       //         _day = GetMonDay(_year,_month);//使现有的天数等于上一个月的天数
       //         //在进行下一轮判断当月的天数是否小于要减去的天数
       //     }
       //     //当月的天数大于要减的天数后，直接减去即可
       //     _day = _day - day;
       //     return *this;
       // }

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
        
        bool operator>(const Date& d);
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
            cout<<_year<<"-"<<_month<<"-"<<_day<<endl;
        }
    private:
        int _year;
        int _month;
        int _day;
};
////5. 赋值运算符重载函数：防止自己给自己赋值，提高效率
//Date& Date::operator=(const Date& d)
//{
//    if(this != &d)
//    {
//        _year = d._year;
//        _month = d._month;
//        _day = d._day;
//    }
//    return *this;
//}
////9. ">"运算符重载
//bool Date::operator>(const Date& d)
//{
//    if(_year > d._year)
//    {
//        return true;
//    }
//    else if(_year == d._year)
//    {
//        if(_month > d._month)
//        {
//            return true;
//        }
//        else if(_month == d._month)
//        {
//            if(_day > d._day)
//            {
//                return true;
//            }
//        }
//    }
//    return false;
//}

int main()
{
    Date d1;//调用缺省的构造函数进行初始化
    Date d2(2018,7,10);//调用自定义拷贝构造函数对d2进行初始化,
    Date d3(2021,4,5);

    //验证赋值运算符重载函数
   // d3 = d2 = d1;//相当于d2调用赋值运算符重载函数，等价于d2.operator=(&d2,d1)
   // d2 = d2;
   // d1.Display();
   // d2.Display();
   // d3.Display();

   ////验证计算天数的函数和重载"+="的函数
   //d2 += -1000;
   //d2.Display();

   ////验证"-="函数
   //d3 -= -1000;
   //d3.Display();

   // //验证前置++
   // (++d2).Display();
   // d2.Display();

   // //验证后置++
   // (d3++).Display();
   // d3.Display();

   // //验证前置--
   // (--d2).Display();
   // d2.Display();
   // //验证后置--
   // (d3--).Display();
   // d3.Display();

   // //验证">"运算符重载
   // cout<<(d2>d3)<<endl;
   // //验证"=="运算符进行重载
   // cout<<(d2==d3)<<endl;
   // //验证"<"运算符重载
   // cout<<(d2<d3)<<endl;
    //  //验证>=运算符重载
    //  cout<<(d2>=d3)<<endl;
    //  cout<<(d2<=d3)<<endl;
    //  cout<<(d2!=d3)<<endl;

    //验证日期减日期的重载函数
    cout<<(d3-d2)<<endl;
    cout<<(d2-d3)<<endl;

    
    return 0;
}
