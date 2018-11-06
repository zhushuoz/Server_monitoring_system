CC = gcc
CFLAGS = -g -Wall 
OBJS = main_client.o client_func.o common.o 
.PHONY:clean
a.out : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -lpthread
common.o : common.c common.h
	$(CC) -c $(CFLAGS) common.c -lpthread
master_func.o : client_func.c client_func.h
	$(CC) -c $(CFLAGS) client_func.c -lpthread
main_master.o : main_client.c main_client.h 
	$(CC) -c $(CFLAGS) client_master.c -lpthread
clean:
	rm -rf *.o a.out


