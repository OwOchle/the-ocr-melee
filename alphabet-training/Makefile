CC = gcc
OBJS = main.o read_image.o batch_conversion.o progress.o ../utils/verbose.o ../utils/matrix.o ../utils/array.o ../network/network_utils/cost_functions.o ../network/network_utils/batch.o $\
 ../network/network_utils/gradiant.o ../network/training/backpropagation.o ../network/training/update_mini_batch.o ../network/training/stochastic_gradient_descent.o $\
 ../network/evaluate.o ../network/network_utils/activation_functions.o ../network/network.o ../utils/thpool.o ../utils/threaded_matrix.o ../network/file_io.o
CFLAGS = -lm -I..

debug: CFLAGS += -g -fsanitize=address -D VERBOSE
debug: $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o training

debug-profiling: CFLAGS += -pg -g -D VERBOSE
debug-profiling: $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o training

debug-no-asan: CFLAGS += -g
debug-no-asan: $(OBJS)
	$(CC) $^ $(CFLAGS) -o training

debug-optimized: CFLAGS += -O3 -D VERBOSE
debug-optimized: $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o training

release: CFLAGS += -O3 -march=native
release: $(OBJS)
	$(CC) $^ $(CFLAGS) -o training

release-profiling: CFLAGS += -O3 -pg -march=native
release-profiling: $(OBJS)
	$(CC) $^ $(CFLAGS) -o training

.PHONY: clean $(GROUPS)

$(GROUPS):
	$(MAKE) -C ../$@ $(MAKECMDGOALS)-objs

clean: $(GROUPS)
	rm -rf $(OBJS) training
