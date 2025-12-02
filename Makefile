include config.mk

BUILDDIR   = build
SRCDIR     = src
INCLUDEDIR = include

CFLAGS += -I$(INCLUDEDIR)

all : $(BUILDDIR)/udrv_driver.o $(BUILDDIR)/udrv_env.a

$(BUILDDIR)/udrv_env.a : $(BUILDDIR)/udrv_env.o $(BUILDDIR)/udrv_loader.o $(BUILDDIR)/udrv_utils.o
	$(AR) rcs $@ $^

$(BUILDDIR)/%.o : $(SRCDIR)/%.c
	@mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) -o $@ -c $^

clean :
	rm -fr $(BUILDDIR)
