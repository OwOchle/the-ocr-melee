CC = gcc
OBJS = main.o network.o
CFLAGS = 

debug: CFLAGS += -g -fsanitize=address
debug: $(OBJS)
	$(CC) $^ $(CFLAGS) -o network


debug-no-asan: CFLAGS += -g
debug-no-asan: $(OBJS)
	$(CC) $^ $(CFLAGS) -o network

clean:
	rm -rf $(OBJS) solver
