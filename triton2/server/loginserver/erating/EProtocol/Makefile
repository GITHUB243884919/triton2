####### Compiler, tools and options

CC       = gcc
CXX      = g++
LEX      = flex
YACC     = yacc
CFLAGS   = -DLINUX -pipe -Wall -W -O2 -march=i386 -mtune=i686 -g -DGLX_GLXEXT_LEGACY -fno-use-cxa-atexit -fno-exceptions
CXXFLAGS = -DLINUX -pipe -Wall -W -O2 -march=i386 -mtune=i686 -g -DGLX_GLXEXT_LEGACY -fno-use-cxa-atexit -fno-exceptions
LEXFLAGS = 
YACCFLAGS= -d
INCPATH  = -I. -I$(ESUITE_HOME)/includes  -I../EBase -I../EAlgorithm
LINK     = g++
LFLAGS   =
LIBS     = 
AR       = ar cqs
RANLIB   = 
TAR      = tar -cf
GZIP     = gzip -9f
COPY     = cp -f
COPY_FILE= $(COPY) -p
COPY_DIR = $(COPY) -pR
DEL_FILE = rm -f
SYMLINK  = ln -sf
DEL_DIR  = rmdir
MOVE     = mv -f
CHK_DIR_EXISTS= test -d
MKDIR    = mkdir -p

####### Output directory

OBJECTS_DIR = 

####### Files

HEADERS = 
SOURCES =
OBJECTS = $(addsuffix .o,$(basename $(shell ls -t *.cpp | cut -d . -f 1)))
DESTDIR  = 
TARGET   = libEProtocol.a

first: all
####### Implicit rules

.SUFFIXES: .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

####### Build rules

all: Makefile $(TARGET) 

staticlib: $(TARGET)

$(TARGET): $(OBJECTS) 
	-$(DEL_FILE) $(TARGET)
	$(AR) $(TARGET) $(OBJECTS) 

yaccclean:
lexclean:
clean:
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) $(TARGET)


FORCE:

####### Compile


####### Install

install: all 

uninstall: 

