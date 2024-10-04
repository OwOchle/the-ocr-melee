CC = gcc
OBJS = main.o network.o file_io.o utils/array.o utils/matrix.o utils/activation_functions.o
TEST_OBJS = tests/tests.o utils/array.o utils/matrix.o
CFLAGS = -lm

debug: CFLAGS += -g -fsanitize=address
debug: $(OBJS)
	$(CC) $^ $(CFLAGS) -o network

tests: CFLAGS += -g -fsanitize=address -Iutils
tests: $(TEST_OBJS)
	$(CC) $^ $(CFLAGS) -o test


debug-no-asan: CFLAGS += -g
debug-no-asan: $(OBJS)
	$(CC) $^ $(CFLAGS) -o network

clean:
	rm -rf $(OBJS) network $(TEST_OBJS) test
