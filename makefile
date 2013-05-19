CC=gcc
OBJS = ab.o
CFLAGS = -Wall -g

ab : $(OBJS)
	$(CC) $(CFLAG) $< -o $@
clean:
	-rm -rf $(OBJS) 
 
