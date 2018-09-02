ROOT=$(shell pwd)
INCLUDE=-I$(ROOT)/../../lib/include
LIB=-L$(ROOT)/../../lib/lib
LIB_NAME=-lmysqlclient
LIB_FLAG=-static

.PHONY:all
all:echoserver1 test  
	#echoserver insertdata selectdata 

echoserver1:echoserver1.cpp
	g++ -o $@ $^
test:test.cc
	g++ -o $@ $^ -I ~/third_part/include/ -L ~/third_part/lib/ -lgtest -lgtest_main -lpthread
#echoserver:echoserver.c
#	gcc -o $@ $^
#insertdata:insertdata.c
#	gcc -o $@ $^ $(INCLUDE) $(LIB) $(LIB_NAME)
#selectdata:selectdata.c
#	gcc -o $@ $^ $(INCLUDE) $(LIB) $(LIB_NAME)


.PHONY:clean
clean:
	rm -f  echoserver1 test #echoserver insertdata selectdata 
