CC=gcc
CXX=g++
FLAG_DEBUG=1

OBJS =  bash_script.o kbhit.o main.o read_parameter.o revision.o waitKeyboard.o  

KERN_INC=/lib/modules/`uname -r`/build/include

ICS1640_SDK_PATH=./ICS1640-SDK
ICS1640_INCLUDE_PATH=$(ICS1640_SDK_PATH)/ICS1640/source/library/include/
ICS1640_LIBRARY_PATH=$(ICS1640_SDK_PATH)/ICS1640/linux/library/

ifeq ($(FLAG_DEBUG),1)
	CFLAGS=-ggdb -g3 -DICS1640_OS_DEBUG -Wall -fno-strict-aliasing -DLINUX -I$(ICS1640_INCLUDE_PATH) -I$(KERN_INC)
else
	CFLAGS=-O2 -Wall -fno-strict-aliasing -DLINUX -I$(ICS1640_INCLUDE_PATH) -I$(KERN_INC)
endif

LDFLAGS=-L$(ICS1640_LIBRARY_PATH) -lpthread -lics1640api -lrt 

#PLLIB_LDC	= `/opt/plplot/bin/plplot-config --libs`
#LDFLAGS_PLPLOT	=  -L/usr/X11R6/lib -lX11 -ldl -lg2c -lpthread

EXE = tomo_frankie

all: tomo_frankie

tomo_frankie: $(OBJS)
	#$(CXX) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)
	$(CXX) $(CFLAGS) $(OBJS) -o $(EXE) $(LDFLAGS)

clean:
	rm $(OBJS) $(EXE) *.out lixo_*.txt

