CC = gcc
PKGS = gtk+-3.0
CFLAGS = `pkg-config --cflags --libs $(PKGS)` -export-dynamic -rdynamic
OBJS = main.o

debug: CFLAGS += -fsanitize=address -g
debug: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o main

release: CFLAGS += -O3
release: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o main-release

.PHONY: clean

clean:
	$(RM) -rf main
	$(RM) -rf main-release
	$(RM) -rf $(OBJS)