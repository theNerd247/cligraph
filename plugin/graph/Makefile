##############################
# Makefile for cligraph plugin graph 
##############################

SHELL=/bin/sh

CC=gcc

VERSION=0.0.1

PLUGNAME=graph
LIBNAME=lib$(PLUGNAME)
LIBDIR=lib
SONAME=$(LIBNAME).so
OUTNAME=$(LIBNAME).so

IDIR=include
PROJIDIR=../../include/
LFLAGS=-I$(PROJIDIR) -I$(IDIR) -fPIC -lllist -lncurses -lmenu -lm -g #uncomment for debuging with gdb
SHRDFLAGS= -shared -Wl,-soname,$(SONAME)
CFLAGS=-c -g #uncomment for debuging with gdb

HEADERS:=$(wildcard $(IDIR)/*.h)
HDRS=$(patsubst $(IDIR)/%.h, $(HDRINSTALLDIR)/%.h, $(HEADERS))

SRCS=*.c
SRCDIR=src
SRC:=$(wildcard $(SRCDIR)/$(SRCS))

TSRCS=tst.c
TSRCDIR=testing
TLFLAGS=-I$(IDIR) -L. -l$(PLUGNAME)
TSRC=$(patsubst %.c, $(TSRCDIR)/%.c, $(TSRCS))
TOBJ := $(patsubst $(TSRCDIR)/%.c, $(TSRCDIR)/%.o, $(TSRC))

OBJDIR=obj
OBJ := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))

.PHONY: setup clean pkg test

$(TSRCDIR)/%.o: $(TSRCDIR)/%.c
	$(CC) $(LFLAGS) $(CFLAGS) $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c 
	$(CC) $(LFLAGS) $(CFLAGS) $< -o $@

all: setup $(OBJ) $(BINDIR)
	$(CC) $(LFLAGS) $(SHRDFLAGS) $(OBJ) -o $(LIBDIR)/$(OUTNAME)

testing: setup $(OBJ)
	$(CC) $(LFLAGS) $(OBJ) -o $(TSRCDIR)/tst

test: testing
	export LD_LIBRARY_PATH_TMP=$$LD_LIBRARY_PATH
	export LD_LIBRARY_PATH=$$(pwd):$$LD_LIBRARY_PATH
	echo $$LD_LIBRARY_PATH
	$(TSRCDIR)/tst 2> $(TSRCDIR)/log.txt

setup: 
	mkdir -p $(LIBDIR)
	mkdir -p $(OBJDIR)

clean: 
	rm -rf $(OBJDIR)
