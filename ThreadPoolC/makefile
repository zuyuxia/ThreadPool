.PHONY:clean
CC=gcc
ALL=main
all:$(ALL)
OBJS=thread_pool.o main.o condition.o
.c.o:
	$(CC) -c $<
main:$(OBJS)
	$(CC) $^ -o $@ -lpthread -lrt
clean:
	rm -f $(ALL) *.o
