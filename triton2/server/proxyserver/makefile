####### Compiler, tools and options
COMMONPATH	= ../../common# common file include path
CC       	= gcc
CXX      	= g++
LEX      	= flex
YACC     	= yacc
CFLAGS   	= -g
CXXFLAGS 	= -g -D_DEBUG_ -D_POSIX_MT_ -DUSE_LOG4CPP -DOTHERSERVER -DENCODING_UTF8 -DNDEBUG
LEXFLAGS 	= 
YACCFLAGS	= -d
INCPATH	= -I./inc  -I./src -I./$(COMMONPATH)/tiny -I./$(COMMONPATH)/comm -I./$(COMMONPATH)/comm/linux -I./$(COMMONPATH)/inc -I../servercommon/inc -I./$(COMMONPATH)/log4cpp/include -I./$(COMMONPATH)/protobuf/include -I./$(COMMONPATH)/coredumper/include 
LINK     	= g++
LFLAGS  	= 
LIBS     	= -lpthread -L../../common/log4cpp/lib -llog4cpp -L$(COMMONPATH)/protobuf/lib -lprotobuf -L./$(COMMONPATH)/coredumper/lib -lcoredumper
DEL_FILE 	= rm -f

####### Files

OBJECTS =	./debug/base.o   ./debug/proxy_conn.o  ./debug/code_queue.o ./debug/thread.o ./debug/proxy_handle.o ./debug/proxymessage_pb.hxx.pb.o \
			./debug/proxy_ctrl.o ./debug/proxy_comm.o ./debug/config.o ./debug/file_lock.o ./debug/shm.o \
			./debug/tinyxml.o ./debug/tinystr.o ./debug/tinyxmlerror.o ./debug/tinyxmlparser.o ./debug/log.o ./debug/charset.o \
			./debug/oi_tea.o 

			#./debug/servertool.o

			# ./debug/message_factory.o ./debug/servermessage_in.o 
			#./debug/coremessage_pb.hxx.pb.o \
			#./debug/servermessage_pb.hxx.pb.o ./debug/dbinterface.o ./debug/db_msg_pb.hxx.pb.o


TARGET	= ./bin/proxyd

first: all
####### Compile

./debug/base.o : ../servercommon/src/base.cpp
	$(CXX) -c -DLINUX -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/shm.o : ../servercommon/src/shm.cpp
	$(CXX) -c -DLINUX -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/file_lock.o : ../servercommon/src/file_lock.cpp
	$(CXX) -c -DLINUX -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/config.o : ../servercommon/src/config.cpp
	$(CXX) -c -DLINUX -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/code_queue.o : ../servercommon/src/code_queue.cpp
	$(CXX) -c -DLINUX -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/thread.o : ../servercommon/src/thread.cpp
	$(CXX) -c -DLINUX -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/proxy_handle.o : src/proxy_handle.cpp
	$(CXX) -c -DLINUX $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/proxymessage_pb.hxx.pb.o : ../servercommon/inc/proxymessage_pb.hxx.pb.cc
	$(CXX) -c -DLINUX -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/proxy_ctrl.o : src/proxy_ctrl.cpp
	$(CXX) -c -DLINUX -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/proxy_svrd.o : src/proxy_svrd.cpp
	$(CXX) -c -DLINUX -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/proxy_comm.o : src/proxy_comm.cpp
	$(CXX) -c -DLINUX -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/proxy_conn.o : src/proxy_conn.cpp
	$(CXX) -c -DLINUX -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/test_send.o : src/test_send.cpp
	$(CXX) -c -DLINUX -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/tinyxml.o : $(COMMONPATH)/tiny/tinyxml.cpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/tinystr.o : $(COMMONPATH)/tiny/tinystr.cpp
	$(CXX) -c -DLINUX -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/tinyxmlerror.o : $(COMMONPATH)/tiny/tinyxmlerror.cpp
	$(CXX) -c -DLINUX -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/tinyxmlparser.o : $(COMMONPATH)/tiny/tinyxmlparser.cpp
	$(CXX) -c -DLINUX -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/log.o : ../servercommon/src/log.cpp
	$(CXX) -c -DLINUX -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/message_pb.hxx.pb.o : $(COMMONPATH)/inc/message_pb.hxx.pb.cc
	$(CXX) -c -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/servermessage_in.o : ../servercommon/src/servermessage_in.cpp
	$(CXX) -c -DLINUX -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/message_factory.o : $(COMMONPATH)/src/message_factory.cpp
	$(CXX) -c -DLINUX -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/coremessage_pb.hxx.pb.o : $(COMMONPATH)/inc/coremessage_pb.hxx.pb.cc
	$(CXX) -c -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/servermessage_pb.hxx.pb.o : ../servercommon/inc/servermessage_pb.hxx.pb.cc
	$(CXX) -c -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/dbinterface.o : ../servercommon/src/dbinterface.cpp
	$(CXX) -c -DLINUX -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/db_msg_pb.hxx.pb.o : ../servercommon/inc/db_msg_pb.hxx.pb.cc
	$(CXX) -c -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/charset.o : ../servercommon/src/charset.c
	$(CXX) -c -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/oi_tea.o : $(COMMONPATH)/comm/oi_tea.cpp
	$(CXX) -c  $(CXXFLAGS) $(INCPATH) -o $@ $<

####### Build rules

all: makefile $(TARGET)

$(TARGET): $(OBJECTS) ./debug/proxy_svrd.o
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) ./debug/proxy_svrd.o $(LIBS)

$(TARGET2): $(OBJECTS) ./debug/test_send.o
	$(LINK) $(LFLAGS) -o $(TARGET2) $(OBJECTS) ./debug/test_send.o $(LIBS)

clean:
	-$(DEL_FILE) $(OBJECTS) ./debug/proxy_svrd.o ./debug/test_send.o
install:
	-cp $(TARGET) $(TRITON_BIN)
	
