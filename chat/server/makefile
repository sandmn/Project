
CUR_PATH=$(shell pwd)#打印当前路径
#罗列需编译的源文件
SRC=$(shell ls *.cpp)
#定义生成的可执行文件
BIN=char_server
#定义编译器
CC=g++

.PHONY:clean

$(BIN):$(SRC)
	$(CC) -o $@ -g $^

clean:
	rm $(BIN)
