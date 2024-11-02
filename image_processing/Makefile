CC = gcc

PKGS = sdl2 SDL2_image
PKGCONFIG = `pkg-config --cflags --libs $(PKGS)`
LDFLAGS = $(PKGCONFIG)
CFLAGS = $(PKGCONFIG)
OBJS = main.o grayscale.o gaussian_blur.o sobel.o

debug: CFLAGS += -g -fsanitize=address
debug: $(OBJS)
	$(CC) $(LDFLAGS) $^ -o main
    
.PHONY: clean

clean:
	$(RM) $(OBJS) main