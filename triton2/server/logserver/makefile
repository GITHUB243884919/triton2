
####### Compiler, tools and options
COMMONPATH	= ../../common# common file include path
CC		= gcc
CXX		= g++
LEX		= flex
YACC     	= yacc
CFLAGS   	= -g
CXXFLAGS 	= -g -DOTHERSERVER -DENCODING_UTF8 -D_DEBUG_ -D_POSIX_MT_ -DUSE_LOG4CPP -DUSE_SHM -DLINUX -DUSE_LK_ASSERT -DMEMLEAK -DWEST_SERVER 
LEXFLAGS 	= 
YACCFLAGS	= -d
INCPATH		= -I./inc -I./$(COMMONPATH)/tiny -I./$(COMMONPATH)/comm -I./$(COMMONPATH)/comm/linux -I./$(COMMONPATH)/inc \
	-I../logic/inc -I../servercommon/inc -I../logic/inc -I./$(COMMONPATH)/interface -I./$(COMMONPATH)/log4cpp/include \
	-I./$(COMMONPATH)/lua -I $(COMMONPATH)/mysql/inc/ -I./$(COMMONPATH)/protobuf/include \
	-I./$(COMMONPATH)/coredumper/include
LINK     	= g++
LFLAGS   	= 
LIBS     	= -lpthread -L./$(COMMONPATH)/log4cpp/lib -llog4cpp -L $(COMMONPATH)/mysql/lib/ -lmysqlclient -lmysys \
			-L./$(COMMONPATH)/tiny -ltinyxml -L./$(COMMONPATH)/protobuf/lib -lprotobuf -lz -dl
DEL_FILE 	= rm -f

####### Files

OBJECTS = ./debug/Database.o ./debug/DatabaseMysql.o ./debug/QueryResultMysql.o ./debug/logmonitor.o\
	./debug/loghandle.o ./debug/logctrl.o ./debug/logsvrd.o ./debug/client_comm_engine.o ./debug/oi_tea.o \
	./debug/sock_stream.o ./debug/thread.o ./debug/logevent.o ./debug/rankmessage_pb.hxx.pb.o ./debug/template.o\
	./debug/code_queue.o ./debug/log.o ./debug/base.o ./debug/Field.o ./debug/shm.o ./debug/sharemem.o ./debug/object.o \
	./debug/proxymessage_pb.hxx.pb.o ./debug/message_pb.hxx.pb.o ./debug/servermessage_in.o ./debug/message_factory.o \
	./debug/servermessage_pb.hxx.pb.o ./debug/coremessage_pb.hxx.pb.o ./debug/db_msg_pb.hxx.pb.o \
	./debug/log_msg_pb.hxx.pb.o ./debug/template_pb.hxx.pb.o ./debug/boursemessage_pb.hxx.pb.o\
	./debug/gmmessage_pb.hxx.pb.o ./debug/property_pb.hxx.pb.o ./debug/ibstore_pb.hxx.pb.o ./debug/charset.o \
	./debug/countrymessage_pb.hxx.pb.o ./debug/corpsinfo_pb.hxx.pb.o ./debug/familydbinfo_pb.hxx.pb.o  ./debug/globalserver_pb.hxx.pb.o 

TARGET = ./bin/logsvrd

first: all
####### Compile

./debug/Database.o : ./src/Database.cpp
	$(CXX) -c -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/boursemessage_pb.hxx.pb.o: ../../common/inc/boursemessage_pb.hxx.pb.cc
	$(CXX) -c -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<
	

./debug/logevent.o : ./src/logevent.cpp
	$(CXX) -c -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/DatabaseMysql.o : ./src/DatabaseMysql.cpp
	$(CXX) -c -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/QueryResultMysql.o : ./src/QueryResultMysql.cpp
	$(CXX) -c -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/loghandle.o : ./src/loghandle.cpp
	$(CXX) -c -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/logmonitor.o : ./src/logmonitor.cpp
	$(CXX) -c -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/template.o : $(COMMONPATH)/src/template.cpp
	$(CXX) -c -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<
	
./debug/logctrl.o : ./src/logctrl.cpp
	$(CXX) -c -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/logsvrd.o : ./src/logsvrd.cpp
	$(CXX) -c -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/sock_stream.o : ../servercommon/src/sock_stream.cpp
	$(CXX) -c -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/thread.o : ../servercommon/src/thread.cpp
	$(CXX) -c -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/code_queue.o : ../servercommon/src/code_queue.cpp
	$(CXX) -c -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<	
	
./debug/log.o : ../servercommon/src/log.cpp
	$(CXX) -c -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<	

./debug/base.o : ../servercommon/src/base.cpp
	$(CXX) -c -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<	

./debug/charset.o : ../servercommon/src/charset.c
	$(CXX) -c -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<	

./debug/Field.o : ./src/Field.cpp
	$(CXX) -c -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<	
	
./debug/shm.o : ../servercommon/src/shm.cpp
	$(CXX) -c -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<	
			
./debug/sharemem.o : ../servercommon/src/sharemem.cpp
	$(CXX) -c -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<	

./debug/object.o : ../servercommon/src/object.cpp
	$(CXX) -c -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<	
	
./debug/proxymessage_pb.hxx.pb.o : ../servercommon/inc/proxymessage_pb.hxx.pb.cc
	$(CXX) -c -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<	

./debug/template_pb.hxx.pb.o : $(COMMONPATH)/inc/template_pb.hxx.pb.cc
	$(CXX) -c -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<	
				
./debug/message_pb.hxx.pb.o : $(COMMONPATH)/inc/message_pb.hxx.pb.cc
	$(CXX) -c -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<					

./debug/servermessage_in.o : ../servercommon/src/servermessage_in.cpp
	$(CXX) -c -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<		

./debug/message_factory.o : $(COMMONPATH)/src/message_factory.cpp
	$(CXX) -c -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/servermessage_pb.hxx.pb.o : ../servercommon/inc/servermessage_pb.hxx.pb.cc
	$(CXX) -c -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/coremessage_pb.hxx.pb.o : $(COMMONPATH)/inc/coremessage_pb.hxx.pb.cc
	$(CXX) -c -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/client_comm_engine.o : $(COMMONPATH)/comm/client_comm_engine.cpp
	$(CXX) -c -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/oi_tea.o : $(COMMONPATH)/comm/oi_tea.cpp
	$(CXX) -c -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/db_msg_pb.hxx.pb.o : ../servercommon/inc/db_msg_pb.hxx.pb.cc
	$(CXX) -c -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<
 
./debug/log_msg_pb.hxx.pb.o : ../servercommon/inc/log_msg_pb.hxx.pb.cc
	$(CXX) -c -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<
	
./debug/gmmessage_pb.hxx.pb.o : ../servercommon/inc/gmmessage_pb.hxx.pb.cc
	$(CXX) -c -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<
 
./debug/property_pb.hxx.pb.o : $(COMMONPATH)/inc/property_pb.hxx.pb.cc
	$(CXX) -c -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/ibstore_pb.hxx.pb.o : $(COMMONPATH)/inc/ibstore_pb.hxx.pb.cc
	$(CXX) -c -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/rankmessage_pb.hxx.pb.o : $(COMMONPATH)/inc/rankmessage_pb.hxx.pb.cc
	$(CXX) -c -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/countrymessage_pb.hxx.pb.o : $(COMMONPATH)/inc/countrymessage_pb.hxx.pb.cc
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/corpsinfo_pb.hxx.pb.o : $(COMMONPATH)/inc/corpsinfo_pb.hxx.pb.cc
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<
	
./debug/familydbinfo_pb.hxx.pb.o : $(COMMONPATH)/inc/familydbinfo_pb.hxx.pb.cc
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/globalserver_pb.hxx.pb.o : ../servercommon/inc/globalserver_pb.hxx.pb.cc
	$(CXX) -c  $(CXXFLAGS) $(INCPATH) -o $@ $<

.cpp.o:; $(CXX) -c -DLINUX -DWEST_SERVER $(CXXFLAGS) $(INCPATH)  $< -o $@

####### Build rules

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(LINK)	$(LFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)

install:all
	cp $(TARGET) ../bin
	cp $(TARGET) $(TRITON_BIN)

clean:
	-$(DEL_FILE)  
	-$(DEL_FILE) *~ core *.core
	rm -f ./debug/*.o
	rm -f ../logic/src/*.o
	rm -f $(TARGET)
