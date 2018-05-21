#编译器类型
CC=g++
CFLAGS=-g -Wall
SUBDIRS=$(shell ls -l | grep ^d | awk '{if($$9 != "debug") print $$9}')
#根目录
ROOT_DIR=$(shell pwd)
#bin文件
BIN=DCurlRest
#目标文件存放目录
OBJS_DIR=debug/obj
#二进制文件存放目录
BIN_DIR=debug/bin
#遍历当前目录中的.cpp文件
CUR_SOURCE=${wildcard *.cpp}
#编译后的中间文件
CUR_OBJS=${patsubst %.cpp, %.o, $(CUR_SOURCE)}
#include路径
INCLUDES=-I$(ROOT_DIR)/Include  \
    -I$(ROOT_DIR)/DCurlHttp \
    -I$(ROOT_DIR)/DCurlRest \
    -I$(ROOT_DIR)/CTaskTool \
    -I$(ROOT_DIR)/Common
#导入到子目录环境变量中
export CC BIN OBJS_DIR BIN_DIR ROOT_DIR INCLUDES

all:$(SUBDIRS) $(CUR_OBJS) DEBUG
$(SUBDIRS):ECHO
	make -C $@
DEBUG:ECHO
	make -C debug
ECHO:
	@echo $(SUBDIRS)
$(CUR_OBJS):%.o:%.cpp 
	$(CC) $(CFLAGS) $(INCLUDES) -c $^ -o $(ROOT_DIR)/$(OBJS_DIR)/$@
clean:
	@rm $(OBJS_DIR)/*.o
	@rm -rf $(BIN_DIR)/*
