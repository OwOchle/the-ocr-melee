CC = gcc
OBJS = main.o network.o file_io.o arrays.o

debug: CFLAGS += -g -fsanitize=address
debug: $(OBJS)
	$(CC) $^ $(CFLAGS) -o network


debug-no-asan: CFLAGS += -g
debug-no-asan: $(OBJS)
	$(CC) $^ $(CFLAGS) -o network

clean:
	rm -rf $(OBJS) solver
