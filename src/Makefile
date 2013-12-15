# CFLAGS is defined in the calling Makefile.
# This make file should not be called directly
SOURCES=$(wildcard $(CURDIR)/*.c)
OBJECTS=$(patsubst %.c, %.o, $(SOURCES))

all: build

build: $(OBJECTS)

clean:
	rm -f $(OBJECTS)
