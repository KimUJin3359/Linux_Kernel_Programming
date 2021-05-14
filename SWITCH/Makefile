KERNEL_HEADERS = /lib/modules/${shell uname -r}/build
CC = gcc

TARGET := app
obj-m := SWITCH.o

all : driver app

driver :
	make -C $(KERNEL_HEADERS) M=$(PWD) modules

app :
	$(CC) -o $@ $@.c -lpthread

clean :
	make -C $(KERNEL_HEADERS) M=$(PWD) clean
	rm $(TARGET)
