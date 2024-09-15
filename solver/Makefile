CC = gcc
OBJS = main.o
CFLAGS = 

debug: CFLAGS += -g -fsanitize=address
debug: $(OBJS)
	$(CC) $^ $(CFLAGS) -o solver

clean:
	rm -rf $(OBJS) solver
