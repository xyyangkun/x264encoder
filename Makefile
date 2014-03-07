CROSS=
#arm-hisiv200-linux-
#arm-linux-
CC=$(CROSS)gcc
CXX=$(CROSS)g++
LINK=$(CROSS)g++ -o
LIBRARY_LINK=$(CROSS)ar cr

OUT=./OUT
SRCDIR=./src

LIBDIR_X264= $(pwd)/extern_lib/x264-snapshot-20140218-2245
#头文件
INCLUDE= -I ./include -I$(LIBDIR_X264) 


#库文件
LIBDIR= -L./lib  -L$(LIBDIR_X264) 
LIBS = -lx264 
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
CPPOBJS= $(OUT)/x264encoder.o $(OUT)/PicSource.o
$(SRCDIR)/x264encoder.cpp:$(SRCDIR)/x264encoder.h
$(SRCDIR)/PicSource.cpp:$(SRCDIR)/PicSource.h

test:$(OUT)/encoder.o $(CPPOBJS)
	$(LINK) $@ $< $(CPPOBJS) $(CXXFLAGS) 	
$(OUT)/encoder.o: $(SRCDIR)/encoder.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)


	
clean:
	rm $(OUT)/*

	
