CC = gcc

PKGS = sdl2 SDL2_image
PKGCONFIG = `pkg-config --cflags --libs $(PKGS)`
LDFLAGS = -lm
CFLAGS = $(PKGCONFIG) -I. -I.. -I../utils -g # Add -I../utils to include path

OBJS = main.o grayscale.o gaussian_blur.o sobel.o ht_line_detection.o threshold.o gaussian_binary.o letter_filtering.o objects_detection.o ../utils/linked_list.o ../utils/shapes.o cutter.o auto_rotate.o

debug: CFLAGS += -g
debug: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o image_processing 
    
.PHONY: clean

clean:
	$(RM) $(OBJS) image_processing
