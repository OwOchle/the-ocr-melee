CC = gcc

PKGS = sdl2 SDL2_image
PKGCONFIG = `pkg-config --cflags --libs $(PKGS)`
LDFLAGS = -lm
CFLAGS = $(PKGCONFIG) -lm -I. -I.. -I../utils  # Add -I../utils to include path

OBJS = main.o grayscale.o gaussian_blur.o sobel.o line_detection.o threshold.o gaussian_binary.o objects_detection.o ../utils/linked_list.o

debug: CFLAGS += -g -fsanitize=address
debug: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o image_processing 
    
.PHONY: clean

clean:
	$(RM) $(OBJS) image_processing