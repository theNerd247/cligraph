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
LFLAGS=-I$(PROJIDIR) -I$(IDIR) -fPIC -lm -ldl -lpthread -lllist -lncurses -lmenu -L../.. -lcligraph -g #uncomment for debuging with gdb

SHRDFLAGS= -shared -Wl,-soname,$(SONAME)
CFLAGS=-c -g #uncomment for debuging with gdb

HEADERS:=$(wildcard $(IDIR)/*.h)
HDRS=$(patsubst $(IDIR)/%.h, $(HDRINSTALLDIR)/%.h, $(HEADERS))

SRCS=*.c
SRCDIR=src
SRC:=$(wildcard $(SRCDIR)/$(SRCS))

OBJDIR=obj
OBJ:= $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))

TSRCS=tst3.c
TSRCDIR=testing
TLFLAGS=-I$(IDIR) -Llib -l$(PLUGNAME) -L../.. -lcligraph
TSRC=$(patsubst %.c, $(TSRCDIR)/%.c, $(TSRCS))
TOBJ:=$(patsubst $(TSRCDIR)/%.c, $(TSRCDIR)/%.o, $(TSRC))

.PHONY: setup clean pkg test testing

$(TSRCDIR)/%.o: $(TSRCDIR)/%.c
	$(CC) $(LFLAGS) $(CFLAGS) $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c 
	$(CC) $(LFLAGS) $(CFLAGS) $< -o $@

all: setup $(OBJ) $(BINDIR)
	$(CC) $(LFLAGS) $(SHRDFLAGS) $(OBJ) -o $(LIBDIR)/$(OUTNAME)

testing: all $(TOBJ)
	$(CC) $(TLFLAGS) $(TOBJ) -o $(TSRCDIR)/tst

setup: 
	mkdir -p $(LIBDIR)
	mkdir -p $(OBJDIR)

clean: 
	rm -rf $(OBJDIR)

$(TSRCDIR)/%.o: $(TSRCDIR)/%.c
	$(CC) $(TLFLAGS) $(CFLAGS) $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c 
	$(CC) $(LFLAGS) $(CFLAGS) $< -o $@
