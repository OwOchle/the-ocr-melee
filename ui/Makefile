CC = gcc
PKGS = gtk+-3.0
CFLAGS = `pkg-config --cflags --libs $(PKGS)` -export-dynamic -rdynamic
OBJS = main.o proc_steps.o fs_utils.o

define EMBED_CODE
    .section .rodata

    .global thing
    .type   thing, @object
    .balign 4
thing:
    .incbin "main.glade"
ui_end:

    .global ui_size
    .type   ui_size, @object
    .balign 4
ui_size:
    .int    ui_end - thing
endef

main2: CFLAGS = -fsanitize=address -g -I..
main2: main2.o word_search.o ../utils/array.o
	$(CC) $(CFLAGS) $^ -o main2

glade.s:
	$(file >glade.s, $(EMBED_CODE))
	echo glade.s

release: CFLAGS += -O3
release: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o gui

debug: CFLAGS += -fsanitize=address -g
debug: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o gui

out:
	mkdir -p out

all: CFLAGS += -DIMAGE_PROCESSING_EXE="\"./image_processing\"" -DSOLVER_EXE="\"./solver\"" -DNETWORK_EXE="\"./network\"" -DFROM_STRING
all: OBJS += glade.s
all: glade.s release out
	$(MAKE) -C ../image_processing release
	$(MAKE) -C ../network release
	$(MAKE) -C ../solver release
	
	cp ../image_processing/image_processing ./out/
	cp ../network/network ./out/
	cp ../solver/solver ./out/
	mv gui ./out
	cp ../assets/OCR_Logo.png ./out


.PHONY: clean

clean:
	$(RM) -r gui
	$(RM) -r $(OBJS) glade.s

clean-all: clean
	$(MAKE) -C ../image_processing clean
	$(MAKE) -C ../network clean
	$(MAKE) -C ../solver clean
	$(RM) -r out