#LIBUSB_CONFIG=libusb-config
#CFLAGS+=-g -Wall -pedantic `$(LIBUSB_CONFIG) --cflags`
#LFLAGS+=`$(LIBUSB_CONFIG) --libs` -lusb

CFLAGS+=-I/opt/local/include
LFLAGS+=-L/opt/local/lib -lusb 

all: avrusbboot
clean:
	rm *.o
	rm avrusbboot


%.o: %.cpp
	g++  -c $(CFLAGS)  $? 

avrusbboot: main.cpp cflashmem.o cpage.o cbootloader.o
	g++ $(CFLAGS) main.cpp cflashmem.o cpage.o cbootloader.o -o avrusbboot $(LFLAGS)
