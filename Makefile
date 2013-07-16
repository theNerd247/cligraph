##############################
# Makefile for cligraph
##############################

SHELL=/bin/sh

CC=gcc

BINDIR=bin
EXEC=cligraph

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

.PHONY: setup clean 

all: setup $(OBJ) $(BINDIR)
	$(CC) $(LFLAGS) $(OBJ) -o $(BINDIR)/$(EXEC)

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
