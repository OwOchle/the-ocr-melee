CC = gcc
OBJS = network.o file_io.o ../utils/array.o ../utils/matrix.o network_utils/activation_functions.o
TEST_DIRECTORY = tests
TEST_SRC = $(wildcard $(TEST_DIRECTORY)/*.c)
CFLAGS = -lm -I..

debug: CFLAGS += -g -fsanitize=address
debug: $(OBJS) main.o
	$(CC) $^ $(CFLAGS) -o network

debug-no-asan: CFLAGS += -g
debug-no-asan: $(OBJS) main.o
	$(CC) $^ $(CFLAGS) -o network

tests_bin:
	mkdir -p tests_bin

tests: ${TEST_SRC}

tests/%.c: CFLAGS += -g -fsanitize=address -I.
tests/%.c: $(OBJS) tests_bin
	${CC} ${CFLAGS} $(OBJS) $@ -o tests_bin/`basename $@ .c`

.PHONY: clean

clean:
	rm -rf $(OBJS) network main.o
	rm -rf tests_bin
