#################################################################
#	makefile for common
#################################################################


COMM_DIR = .
COMM_INC_PATH = $(COMM_DIR)/inc
COMM_SRC_PATH = $(COMM_DIR)/src

INC = -I ../../common/log4cpp/include
LIB = -L ../../common/log4cpp/lib -llog4cpp

COMMON_OBJS = $(COMM_SRC_PATH)/shm.o $(COMM_SRC_PATH)/config.o $(COMM_SRC_PATH)/code_queue.o $(COMM_SRC_PATH)/log.o $(COMM_SRC_PATH)/object.o

# Comment the following line if you are not using the gnu c compiler
# C_ARGS = -Wall

#C_ARGS =  -g -D_DEBUG_  -DUSE_LOG4CPP -Wall  


# You might have to change this if your c compiler is not cc
CC = cc
CPP = g++

# You shouldn't need to make any more changes below this line.

#.cpp.o:
#	$(CPP) -c $*.cpp

all:$(COMMON_OBJS)

.cpp.o:
	$(CPP) $(C_ARGS) -I$(COMM_INC_PATH) $(INC) $(LIB) -o $@ -c $<

.c.o:
	$(CPP) $(C_ARGS) -I$(COMM_INC_PATH) $(INC) $(LIB) -o $@ -c $<

.C.o:
	$(CPP) $(C_ARGS) -I$(COMM_INC_PATH) $(INC) $(LIB) -o $@ -c $<



clean:
	rm -f $(COMMON_OBJS) 	
