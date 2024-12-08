CC = gcc
OBJS = main.o solver.o file_reader.o helpers.o ../utils/verbose.o
CFLAGS = -I..

debug: CFLAGS += -g -fsanitize=address -D VERBOSE
debug: $(OBJS)
	$(CC) $^ $(CFLAGS) -o solver

release: CFLAGS += -O3
release: $(OBJS)
	$(CC) $^ $(CFLGAS) -o solver

clean:
	rm -rf $(OBJS) solver
