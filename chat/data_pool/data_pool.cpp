

#include"data_pool.h"
//封装一个数据池的类，其中包含数据池的相应成员函数以及数据池的相关操作

//构造函数
data_pool::data_pool()
:_cap(MAX_CAP)
{
    pool.resize(_cap);
    product_index = 0;
    consume_index = 0;
    //第二个0表示线程间共享，如果是非零表示进程间共享。第三个变量表示初始值
    //在本模型中只有一个生产者和一个消费者，所以只有生产者与消费者之间的同步与互斥关系
    sem_init(&product_sem,0,MAX_CAP);//初始时生产者的信号量个数就是vector的容量个数
    sem_init(&consume_sem,0,0);//初始时还没有产品，所以消费者的信号量个数为0
}
//往数据池中存放数据
void data_pool::putData(const std::string& s)
{
    //生产者生产数据时，首先需要申请信号量，信号量申请成功之后才能生产数据
    //生产者生产数据时申请的是空格的信号量
    sem_wait(&product_sem);
    //申请成功之后，将数据放入数据池中
    pool[product_index] = s;
    //然后有关生产者的下标要前移，如果打到结尾，要从头开始构成一个类似环形队列的场所
    product_index++;
    product_index = product_index % MAX_CAP;
    //然后消费者的信号量加1
    sem_post(&consume_sem);
}
//消费者从数据池中消费数据
void data_pool::getData(std::string& s)
{
    //消费者消费数据时首先要申请信号量，申请成功之后才能获取数据
    sem_wait(&consume_sem);
    //申请成功之后，在将数据传给s
    s = pool[consume_index];
    //然后有关消费者的下标要前移
    consume_index++;
    consume_index = consume_index % MAX_CAP;
    //最后生产者的信号量加1
    sem_post(&product_sem);
}
//析构函数:销毁信号量
data_pool::~data_pool()
{
    //int sem_destroy(sem_t *sem);
    sem_destroy(&product_sem);
    sem_destroy(&consume_sem);
}

