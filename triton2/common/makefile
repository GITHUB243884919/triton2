#################################################################
#	makefile for common
#################################################################

COMM_DIR = .
COMM_INC_PATH = $(COMM_DIR)/inc
COMM_SRC_PATH = $(COMM_DIR)/src

COMMON_OBJS = $(COMM_DIR)/comm/linux/epollsocket.o $(COMM_DIR)/comm/linux/linuxnetwork.o \
	$(COMM_DIR)/proclient.o $(COMM_DIR)/template.o \
	
TINYXML_OBJS = 	$(COMM_DIR)/tinystr.o $(COMM_DIR)/tinyxml.o $(COMM_DIR)/tinyxmlerror.o $(COMM_DIR)/tinyxmlparser.o 



INC = -I $(SERVER_COMM_INC_PATH) -I $(COMM_DIR)

# Comment the following line if you are not using the gnu c compiler
# C_ARGS = -Wall

#C_ARGS =  -g -D_DEBUG_ -Wall  


# You might have to change this if your c compiler is not cc
CC = cc
CPP = g++

# You shouldn't need to make any more changes below this line.

#.cpp.o:
#	$(CPP) -c $*.cpp

all:$(COMMON_OBJS)

.cpp.o:
	$(CPP) $(C_ARGS) $(INC) -o $@ -c $<

.c.o:
	$(CPP) $(C_ARGS) $(INC) -o $@ -c $<

.C.o:
	$(CPP) $(C_ARGS) $(INC) -o $@ -c $<



clean:
	rm -f $(COMMON_OBJS) 	
