#include<iostream>
#include<gtest/gtest.h>
#include"echoserver1.h"
//利用gtest进行单元测试
//注册事件机制
//全局级别
class GlobalTest:public testing::Environment
{
    public:
        //全局初始化函数
        void SetUp()
        {
            std::cout<<"Global SetUp"<<std::endl;
        }
        //全局清理函数
        void TearDown()
        {
            std::cout<<"Global TearDown"<<std::endl;
        }
};

//自动生成类对象并注册进gtest框架中
int main(int argc,char* argv[])
{
    //初始化函数
    testing::InitGoogleTest(&argc,argv);
    //生成全局子类对象并由父类指针指向
    testing::Environment* env = new GlobalTest();
    //将其注册进gtest框架中
    testing::AddGlobalTestEnvironment(env);
    return RUN_ALL_TESTS();
}


TEST(TestCase1,TestGetMonDay)
{
    //两种类型的闰年/平年 ：31天/30天的月份/2月
    ASSERT_EQ(GetMonDay(2004,1),31);
    ASSERT_EQ(GetMonDay(2004,2),29);
    ASSERT_EQ(GetMonDay(2004,4),30);
    ASSERT_EQ(GetMonDay(2000,3),31);
    ASSERT_EQ(GetMonDay(2000,2),29);
    ASSERT_EQ(GetMonDay(2000,6),30);
    ASSERT_EQ(GetMonDay(1997,7),31);
    ASSERT_EQ(GetMonDay(1997,2),28);
    ASSERT_EQ(GetMonDay(1997,9),30);
}
TEST(TestCase2,TestAddDays)
{
    Date d1(2000,1,1);
    Date d1_1(2000,1,11);
    Date d1_2(2000,2,1);
    Date d1_3(2000,2,10);
    Date d1_4(2001,1,1);
    Date d1_5(1999,1,1);
    ASSERT_TRUE(d1_1 == (d1 + 10));
    ASSERT_TRUE(d1_2 == (d1 + 31));
    ASSERT_TRUE(d1_3 == (d1 + 40));
    ASSERT_TRUE(d1_4 == (d1 + 366));
    ASSERT_TRUE(d1_5 == (d1 - 365));

    Date d2(2008,2,1);
    Date d2_1(2008,2,15);
    Date d2_2(2008,3,1);
    Date d2_3(2008,3,5);
    Date d2_4(2009,2,1);
    Date d2_5(2007,2,1);
    ASSERT_TRUE(d2_1 == (d2 + 14));
    ASSERT_TRUE(d2_2 == (d2 + 29));
    ASSERT_TRUE(d2_3 == (d2 + 33));
    ASSERT_TRUE(d2_4 == (d2 + 366));
    ASSERT_TRUE(d2_5 == (d2 - 365));

    Date d3(2007,6,1);
    Date d3_1(2007,6,20);
    Date d3_2(2007,7,1);
    Date d3_3(2007,7,7);
    Date d3_4(2008,6,1);
    Date d3_5(2006,6,1);
    ASSERT_TRUE(d3_1 == (d3 + 19));
    ASSERT_TRUE(d3_2 == (d3 + 30));
    ASSERT_TRUE(d3_3 == (d3 + 36));
    ASSERT_TRUE(d3_4 == (d3 + 366));
    ASSERT_TRUE(d3_5 == (d3 - 365));
}

TEST(TestCase3,TestSubDate)
{
    Date d1(2400,1,10);
    Date d1_1(2400,1,20);
    Date d1_2(2400,2,10);
    Date d1_3(2401,1,10);
    ASSERT_EQ(d1_1 - d1,10);
    ASSERT_EQ(d1_2 - d1,31);
    ASSERT_EQ(d1_3 - d1,366);
    ASSERT_EQ(d1 - d1_3,-366);

    Date d2(2012,2,5);
    Date d2_1(2012,2,18);
    Date d2_2(2012,3,5);
    Date d2_3(2013,2,5);
    ASSERT_EQ(d2_1 - d2,13);
    ASSERT_EQ(d2_2 - d2,29);
    ASSERT_EQ(d2_3 - d2,366);
    ASSERT_EQ(d2 - d2_3,-366);

    Date d3(2017,4,2);
    Date d3_1(2017,4,15);
    Date d3_2(2017,5,2);
    Date d3_3(2018,4,2);
    ASSERT_EQ(d3_1 - d3,13);
    ASSERT_EQ(d3_2 - d3,30);
    ASSERT_EQ(d3_3 - d3,365);
    ASSERT_EQ(d3 - d3_3,-365);
}

























