PROJECTDIR = $(shell pwd)
BUILDDIR = $(PROJECTDIR)/build
export BINDIR = $(BUILDDIR)/bin
export OBJDIR = $(BUILDDIR)/obj
export LIBDIR = $(BUILDDIR)/lib

SRC = ./src
TEST = ./test

all : $(BUILDDIR)
	$(MAKE) -C $(SRC) 

run : $(SRC) $(BUILDDIR)
	$(MAKE) -C $(SRC) run

test : $(BUILDDIR)
	$(MAKE) -C $(SRC) lib
	$(MAKE) -C $(TEST) run

debug : $(BUILDDIR)
	$(MAKE) -C $(SRC) lib
	$(MAKE) -C $(TEST) debug
	# $(MAKE) -C $(SRC) debug
.PHONY: test 

$(BUILDDIR):
	@echo "create build dir"
	mkdir -p $(BINDIR) $(OBJDIR) $(LIBDIR)

