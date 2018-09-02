#include"echoserver1.h"
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





