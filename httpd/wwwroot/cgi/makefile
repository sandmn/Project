ROOT=$(shell pwd)
INCLUDE=-I$(ROOT)/../../lib/include
LIB=-L$(ROOT)/../../lib/lib
LIB_NAME=-lmysqlclient
LIB_FLAG=-static

.PHONY:all
all:echoserver echoserver1 insertdata selectdata

echoserver:echoserver.c
	gcc -o $@ $^
echoserver1:echoserver1.cpp
	g++ -o $@ $^
insertdata:insertdata.c
	gcc -o $@ $^ $(INCLUDE) $(LIB) $(LIB_NAME)
selectdata:selectdata.c
	gcc -o $@ $^ $(INCLUDE) $(LIB) $(LIB_NAME)

.PHONY:clean
clean:
	rm -f echoserver echoserver1 insertdata selectdata
