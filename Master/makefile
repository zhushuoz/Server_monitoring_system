CC = gcc
CFLAGS = -g -Wall 
OBJS = main_master.o master_func.o common.o 

master : $(OBJS)
	$(CC) -g -o master $(OBJS) -lpthread

common.o : common.c common.h
	$(CC) -c $(CFLAGS) common.c -lpthread

master_func.o : master_func.c master_func.h
	$(CC) -c $(CFLAGS) master_func.c -lpthread

main_master.o : main_master.c main_master.h 
	$(CC) -c $(CFLAGS) main_master.c -lpthread

.PHONY: clean install uninstall

clean:
	@rm -rf *.o master 

install:
	@mkdir /etc/pihealth_zhushuo/
	@cp ./master /etc/pihealth_zhushuo/
	@cp ./con_info.log /etc/pihealth_zhushuo/
	@cp ./pihealth_zhushuo.service /lib/systemd/system/
	@chmod 755 /lib/systemd/system/pihealth_zhushuo.service
	@cp ./pihealth_zhushuo.start /etc/pihealth_zhushuo/
	@chmod 755 /etc/pihealth_zhushuo/pihealth_zhushuo.start
	@cp ./pihealth_zhushuo.stop /etc/pihealth_zhushuo/
	@chmod 755 /etc/pihealth_zhushuo/pihealth_zhushuo.stop
	@touch /etc/pihealth_zhushuo/pihealth_zhushuo.pid
	@systemctl daemon-reload
	@systemctl enable pihealth_zhushuo.service
	@systemctl start pihealth_zhushuo.service

uninstall:
	@rm -rf /etc/pihealth_zhushuo/ 
	@systemctl stop pihealth_zhushuo.service
	@rm -rf /lib/systemd/system/pihealth_zhushuo.service
	

