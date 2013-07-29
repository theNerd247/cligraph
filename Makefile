##############################
# Makefile for cligraph
##############################

SHELL=/bin/sh

CC=gcc

BINDIR=bin
EXEC=cligraph
PKGNAME=cligraph
PKGLIST=Makefile src/ include/ testing/

IDIR=./include
LFLAGS=-I$(IDIR) -lm -lllist -g #uncomment for debuging with gdb
CFLAGS=-c -g #uncomment for debuging with gdb

SRCS=*.c 
SRCDIR=./src
SRC:=$(wildcard $(SRCDIR)/$(SRCS))

TSRCS=tstgraph.c
TSRCDIR=./testing
TSRC=$(patsubst %.c, $(TSRCDIR)/%.c, $(TSRCS))
TOBJ := $(patsubst $(TSRCDIR)/%.c, $(TSRCDIR)/%.o, $(TSRC))

OBJDIR=obj
OBJ := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))

.PHONY: setup clean package

all: setup $(OBJ) $(BINDIR)
	$(CC) $(LFLAGS) $(OBJ) -o $(BINDIR)/$(EXEC)

pkg:
	mkdir -p $(PKGNAME)
	cp -r  $(PKGLIST) $(PKGNAME)
	tar -vc -f $(PKGNAME).tar $(PKGNAME)
	rm -r $(PKGNAME)	

testing: setup $(TOBJ) $(OBJ)
	$(CC) $(LFLAGS) $(TOBJ) $(OBJ) -o $(TSRCDIR)/tst

$(TSRCDIR)/%.o: $(TSRCDIR)/%.c
	$(CC) $(LFLAGS) $(CFLAGS) $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c 
	$(CC) $(LFLAGS) $(CFLAGS) $< -o $@

setup: 
	mkdir -p $(BINDIR)
	mkdir -p $(OBJDIR)

clean: 
	rm -rf $(OBJDIR)
