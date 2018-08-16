
#include<semaphore.h>
#include<string>
#include<vector>
#define MAX_CAP 256

class data_pool 
{
    public:
        //构造函数
        data_pool();
        //往数据池中存放数据
        void putData(const std::string& s);
        //消费者从数据池中消费数据
        void getData(std::string& s);
        //析构函数:销毁信号量
        ~data_pool();
    private:
        //首先数据池是由一个vector构成，其中包含的数据类型为string
        int _cap;//数据池的容量，即vector的最大容量
        std::vector<std::string> pool;
        //因为要往数据池中添加数据或者要从数据池中获取数据
        //在添加，获取的时候，要对容量进行判断，所以需要用生产者消费者模型维护同步
        //所以，此时需要维护两个变量，一个表示生产者使用的下标
        //一个表示消费者使用的下标
        //为维护同步机制，需要在定义两个信号量进行维护
        int product_index;//生产者使用的下标
        int consume_index;//消费者使用的下标
        sem_t product_sem;//生产者生产时需要申请的锁
        sem_t consume_sem;//消费者消费时需要申请的锁
};
