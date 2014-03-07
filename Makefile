CROSS=
#arm-hisiv200-linux-
#arm-linux-
CC=$(CROSS)gcc
CXX=$(CROSS)g++
LINK=$(CROSS)g++ -o
LIBRARY_LINK=$(CROSS)ar cr

OUT=./OUT
SRCDIR=./src

LIBDIR_X264=/home/xy/mywork/libsrc/x264-snapshot-20140218-2245
#LIBJDIR_JRTP=/home/xy/mywork/git/jrtplib
LIBJDIR_JRTP=/workplace/mywork/git/jrtplib
#头文件
INCLUDE= -I ./include -I$(LIBDIR_X264) -I$(LIBJDIR_JRTP)/src


#库文件
LIBDIR= -L./lib  -L$(LIBDIR_X264) -L$(LIBJDIR_JRTP)/src
LIBS =  -ljrtp -lpthread -ldl -ljthread
LDLIBS=$(LIBDIR) $(LIBS)

#编译选项
CXXFLAGS=-g
CXXFLAGS+= $(INCLUDE) $(LDLIBS) 
CFLAGS=-g
CFLAGS+=   $(INCLUDE) $(LDLIBS)

#目标
all: test

$(OUT)/%.o:$(SRCDIR)/%.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)
$(OUT)/%.o:$(SRCDIR)/%.c
	$(CC) -c $< -o $@ $(CFLAGS)

CPPOBJS = 
CPPOBJS= 

test:$(OUT)/x264encoder.o $(CPPOBJS)
	$(LINK) $@ $< $(CPPOBJS) $(CXXFLAGS) 	
$(OUT)/x264encoder.o: $(SRCDIR)/x264encoder.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)


	
clean:
	rm $(OUT)/*

	
