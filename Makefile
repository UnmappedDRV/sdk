include config.mk

BUILDDIR   = build
SRCDIR     = src
INCLUDEDIR = include

CFLAGS += -I$(INCLUDEDIR)

all : $(BUILDDIR)/udrv_driver.o $(BUILDDIR)/udrv_env.o

$(BUILDDIR)/%.o : $(SRCDIR)/%.c
	@mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) -o $@ -c $^
