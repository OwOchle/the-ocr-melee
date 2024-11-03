CC = gcc

PKGS = sdl2 SDL2_image
PKGCONFIG = `pkg-config --cflags --libs $(PKGS)`
LDFLAGS = -lm
CFLAGS = $(PKGCONFIG) -lm
OBJS = main.o grayscale.o gaussian_blur.o sobel.o line_detection.o threshold.o

debug: CFLAGS += -g -fsanitize=address
debug: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o image_processing 

release: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o image_processing

.PHONY: clean

clean:
	$(RM) $(OBJS) main
