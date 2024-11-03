TOPTARGETS := debug release clean

SUBDIRS := $(dir $(wildcard */Makefile))

all:
	$(MAKE) release

$(TOPTARGETS): $(SUBDIRS)
$(SUBDIRS):
	-$(MAKE) -C $@ $(MAKECMDGOALS)

.PHONY: $(TOPTARGETS) $(SUBDIRS)
