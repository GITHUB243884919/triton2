####### Compiler, tools and options
COMMONPATH      = ../../common# common file include path
CC              = gcc
CXX             = g++
LEX             = flex
YACC            = yacc
CFLAGS          = -g
CXXFLAGS        = -g -D_DEBUG_ -DUSE_LK_ASSERT -D_USE_MESSAGE_BUFF_ -DOTHERSERVER -DUSE_COREDUMPER  -DUSE_LOG4CPP -DENCODING_UTF8 \
		 -DNDEBUG -D_USE_TIMER_ -pipe -Werror 
LEXFLAGS        =
YACCFLAGS       = -d
CXXFLAGS_3RD = -pipe -Wall  -DLEAK_CHK_DEBUG_NEW -DLINUX -DUSE_LK_ASSERT -DNDEBUG
INCPATH = -I./inc   -I./$(COMMONPATH)/comm -I./$(COMMONPATH)/comm/linux -I./$(COMMONPATH)/inc -I./$(COMMONPATH)/tiny \
 -I../servercommon/inc -I./$(COMMONPATH)/log4cpp/include -I./$(COMMONPATH)/protobuf/include -I./$(COMMONPATH)/coredumper/include \
 -I./$(COMMONPATH)/interface 
LINK            = g++
LFLAGS          =
LIBS            = -lpthread -L../../common/log4cpp/lib -llog4cpp -L$(COMMONPATH)/protobuf/lib -lprotobuf -L./$(COMMONPATH)/coredumper/lib -lcoredumper -rdynamic

DEL_FILE        = rm -f

MSG_OBJ :=  $(patsubst  $(COMMONPATH)/inc/%.cc,  ./debug/%.o,$(wildcard $(COMMONPATH)/inc/*.cc))
SERVER_MSG_OBJ := $(patsubst ../servercommon/inc/%.cc, ./debug/%.o,$(wildcard ../servercommon/inc/*.cc))

OBJECTS = ./debug/dungeonserver.o ./debug/dungeonlogic.o ./debug/sharemem.o ./debug/log.o ./debug/servermessage_in.o \
 ./debug/shm.o ./debug/tinyxml.o ./debug/tinystr.o \
./debug/tinyxmlerror.o ./debug/tinyxmlparser.o ./debug/object.o ./debug/base.o ./debug/oi_tea.o ./debug/client_comm_engine.o \
./debug/charset.o ./debug/message_factory.o ./debug/modulemanager.o ./debug/dungeonmanager.o ./debug/dungeonmodule.o \
./debug/dunobjmanager.o

TARGET = ./bin/globalserver
first : $(TARGET)

####### Compile

./debug/dungeonserver.o : ./src/dungeonserver.cpp
	 $(CXX) -c -DLINUX -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/dungeonlogic.o : ./src/dungeonlogic.cpp
	 $(CXX) -c -DLINUX -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/sharemem.o : ../servercommon/src/sharemem.cpp
	 $(CXX) -c -DLINUX -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/servermessage_in.o : ../servercommon/src/servermessage_in.cpp
	$(CXX) -c -DLINUX -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/log.o : ../servercommon/src/log.cpp
	$(CXX) -c -DLINUX -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/shm.o : ../servercommon/src/shm.cpp
	$(CXX) -c -DLINUX -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/tinyxml.o : $(COMMONPATH)/tiny/tinyxml.cpp
	$(CXX) -c -DLINUX -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/tinystr.o : $(COMMONPATH)/tiny/tinystr.cpp
	$(CXX) -c -DLINUX -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/tinyxmlerror.o : $(COMMONPATH)/tiny/tinyxmlerror.cpp
	$(CXX) -c -DLINUX -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/tinyxmlparser.o : $(COMMONPATH)/tiny/tinyxmlparser.cpp
	$(CXX) -c -DLINUX -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/object.o : ../servercommon/src/object.cpp
	$(CXX) -c -DLINUX -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/base.o : ../servercommon/src/base.cpp
	 $(CXX) -c -DLINUX -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/oi_tea.o : $(COMMONPATH)/comm/oi_tea.cpp
	$(CXX) -c -DLINUX -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<
	
./debug/client_comm_engine.o : $(COMMONPATH)/comm/client_comm_engine.cpp
	$(CXX) -c -DLINUX -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/charset.o : ../servercommon/src/charset.c
	$(CXX) -c -DLINUX -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/message_factory.o : $(COMMONPATH)/src/message_factory.cpp
	$(CXX) -c -DLINUX -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<


$(MSG_OBJ) :./debug/%.o :  $(COMMONPATH)/inc/%.cc
	$(CXX) -c  -DLINUX -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<

$(SERVER_MSG_OBJ) : ./debug/%.o : ../servercommon/inc/%.cc
	$(CXX) -c  -DLINUX  -DWEST_SERVER $(CXXFLAGS) $(INCPATH)  -o $@ $<

./debug/modulemanager.o : ./src/modulemanager.cpp
	 $(CXX) -c -DLINUX -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<
	
 ./debug/dungeonmanager.o : ./src/dungeonmanager.cpp
	$(CXX) -c -DLINUX -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<
	
./debug/dungeonmodule.o: ./src/dungeonmodule.cpp
	$(CXX) -c -DLINUX -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<

./debug/dunobjmanager.o: ./src/dunobjmanager.cpp		
	$(CXX) -c -DLINUX -DWEST_SERVER $(CXXFLAGS) $(INCPATH) -o $@ $<
####### Build rules

$(TARGET) : $(MSG_OBJ) $(SERVER_MSG_OBJ) $(OBJECTS) 
	$(LINK) $(LFLAGS) -o $(TARGET)  $(MSG_OBJ) $(SERVER_MSG_OBJ) $(OBJECTS) $(LIBS)

clean :
	$(DEL_FILE) ./debug/*

