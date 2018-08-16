
CUR_PATH=$(shell pwd)
SERVER_PATH=$(CUR_PATH)/server
CLIENT_PATH=$(CUR_PATH)/client
DATA_POOL_PATH=$(CUR_PATH)/data_pool
COMM_PATH=$(CUR_PATH)/comm
WIN_PATH=$(CUR_PATH)/window
LIB_PATH=$(CUR_PATH)/lib

cc=g++

#将所有路径下的.cpp文件生成.o文件
#编译生成.o文件时需要引入指定头文件所在的路径

#指定头文件所在的路径
INCLUDEFLAG=-I$(SERVER_PATH) -I$(CLIENT_PATH) -I$(DATA_POOL_PATH) -I$(COMM_PATH) -I$(WIN_PATH) -I$(LIB_PATH)/include

#将所有的.o文件生成一个客户端文件，服务器端文件
#列出所有的.o文件
#客户端需要连接的.o文件
ser_src=$(shell ls $(SERVER_PATH)/*.cpp $(DATA_POOL_PATH)/*.cpp $(COMM_PATH)/*.cpp| sed 's/.cpp/.o/'|sed 's/.*\///g')

cli_src=$(shell ls $(CLIENT_PATH)/*.cpp $(COMM_PATH)/*.cpp $(WIN_PATH)/*.cpp| sed 's/.cpp/.o/'|sed 's/.*\///g')

#cli_src=$(shell ls $(CLIENT_PATH)/%.o $(WIN_PATH)/%.o $(COMM_PATH)/%.o)
##服务器端需要连接的.o文件
#ser_src=$(shell ls $(SERVER_PATH)/%.o $(DATA_POOL_PATH)/%.o $(COMM_PATH)/%.o)
#除此之外，客户端和服务器端还需要链接线程库，json库
LIBFLAG=-L$(LIB_PATH)/lib -ljsoncpp -lpthread 

#开始连接生成目标文件
#一次连接生成多个目标文件
.PHONY:all

all:chat_server chat_client

#连接生成服务器端可执行程序
chat_server:$(ser_src)
	@$(cc) -o $@ $^ $(LIBFLAG)
	@echo "linking [$^] to [$@] ... done"
	#lncurses已安装在系统默认的路径下，所以编译链接时会自动到系统指定路径下查找头文件和源文件 
chat_client:$(cli_src)
	@$(cc) -o $@ $^ $(LIBFLAG) -lncurses -static 
	@echo "linking [$^] to [$@] ... done"

#在各自的路径下将所有.cpp文件生成.o文件
#$<:表示一个.cpp文件生成一个.o文件，而不是所有的.cpp文件生成一个.o文件
%.o:$(SERVER_PATH)/%.cpp
	@$(cc) -c $< $(INCLUDEFLAG)
	@echo "Comping [$<] to [$@] ... done "
%.o:$(CLIENT_PATH)/%.cpp
	@$(cc) -c $< $(INCLUDEFLAG)
	@echo "Comping [$<] to [$@] ... done "

%.o:$(DATA_POOL_PATH)/%.cpp
	@$(cc) -c $< $(INCLUDEFLAG)
	@echo "Comping [$<] to [$@] ... done "

%.o:$(COMM_PATH)/%.cpp
	@$(cc) -c $< $(INCLUDEFLAG)
	@echo "Comping [$<] to [$@] ... done "

%.o:$(WIN_PATH)/%.cpp
	@$(cc) -c $< $(INCLUDEFLAG)
	@echo "Comping [$<] to [$@] ... done "
#删除所有的.o文件和可执行程序
.PHONY:clean
clean:
	@rm chat_server chat_client *.o 


