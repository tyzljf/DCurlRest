#����������
CC=g++
CFLAGS=-g -Wall
SUBDIRS=$(shell ls -l | grep ^d | awk '{if($$9 != "debug") print $$9}')
#��Ŀ¼
ROOT_DIR=$(shell pwd)
#bin�ļ�
BIN=DCurlRest
#Ŀ���ļ����Ŀ¼
OBJS_DIR=debug/obj
#�������ļ����Ŀ¼
BIN_DIR=debug/bin
#������ǰĿ¼�е�.cpp�ļ�
CUR_SOURCE=${wildcard *.cpp}
#�������м��ļ�
CUR_OBJS=${patsubst %.cpp, %.o, $(CUR_SOURCE)}
#include·��
INCLUDES=-I$(ROOT_DIR)/Include  \
    -I$(ROOT_DIR)/DCurlHttp \
    -I$(ROOT_DIR)/DCurlRest \
    -I$(ROOT_DIR)/CTaskTool \
    -I$(ROOT_DIR)/Common
#���뵽��Ŀ¼����������
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
