CC = gcc
PKGS = gtk+-3.0
CFLAGS = `pkg-config --cflags --libs $(PKGS)` -export-dynamic -rdynamic
OBJS = main.o proc_steps.o fs_utils.o

release: CFLAGS += -O3
release: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o main-release

debug: CFLAGS += -fsanitize=address -g
debug: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o main


.PHONY: clean

clean:
	$(RM) -rf main
	$(RM) -rf main-release
	$(RM) -rf $(OBJS)