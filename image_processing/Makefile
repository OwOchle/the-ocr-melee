CC = gcc

PKGS = sdl2 SDL2_image
PKGCONFIG = `pkg-config --cflags --libs $(PKGS)`
LDFLAGS = -lm
CFLAGS = $(PKGCONFIG) -I. -I.. -I../utils

OBJS = main.o grayscale.o gaussian_blur.o sobel.o threshold.o gaussian_binary.o letter_filtering.o grid_detection.o objects_detection.o ../utils/linked_list.o ../utils/shapes.o cutter.o auto_rotate.o ../utils/verbose.o

debug: CFLAGS += -g -fsanitize=address
debug: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o image_processing

debug-no-asan: CFLAGS += -g
debug-no-asan: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o image_processing

release: CFLAGS += -O3
release: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o image_processing

.PHONY: clean

clean:
	$(RM) $(OBJS) image_processing
