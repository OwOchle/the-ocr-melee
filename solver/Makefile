CC = gcc
OBJS = main.o solver.o file_reader.o helpers.o
CFLAGS = 

debug: CFLAGS += -g -fsanitize=address
debug: $(OBJS)
	$(CC) $^ $(CFLAGS) -o solver

clean:
	rm -rf $(OBJS) solver