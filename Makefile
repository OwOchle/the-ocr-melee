SUBDIRS := $(dir $(wildcard */Makefile))

clean: $(SUBDIRS)
$(SUBDIRS):
	-$(MAKE) -C $@

.PHONY: clean $(SUBDIRS)