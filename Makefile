##############################
# Makefile for cligraph
##############################

SHELL=/bin/sh

CC=gcc

BINDIR=bin
EXEC=cligraph

IDIR=./include
LFLAGS=-I$(IDIR) -I.
CFLAGS=-c -Wall #-g #uncomment for debuging with gdb

SRCS= graph.c table.c parsemath.c
SRCDIR=./src
SRC=$(patsubst %,$(SRCDIR)/%,$(SRCS))

OBJDIR=obj
OBJ=$(patsubst %.c,$(OBJDIR)/%.o,$(SRCS))

MAINSRC= cligraph.c
MAIN=$(SRCDIR)/$(MAINSRC)
MAINOBJ=$(patsubst %.c,$(OBJDIR)/%.o,$(MAINSRC))

TSTSRC=test.c
TSTDIR=./testing
TSTOBJ=$(patsubst %.c,$(OBJDIR)/%.o,$(TSTSRC))

all: $(OBJ) $(MAINOBJ)
	$(CC) $(LFLAGS) $(OBJ) $(MAINOBJ) -o $(BINDIR)/$(EXEC)


$(OBJDIR)/%.o: $(SRCDIR)/%.c 
	$(CC) $(LFLAGS) $(CFLAGS) $< -o $@

$(OBJ): | $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR): 
	mkdir -p $(BINDIR)

test: $(OBJ) $(TSTOBJ)
	$(CC) $(LFLAGS) $(OBJ) $(TSTOBJ) -o $(TSTDIR)/test

$(TSTOBJ): $(TSTDIR)/$(TSTSRC)
	$(CC) $(LFLAGS) $(CFLAGS) $< -o $@

clean: 
	rm -rf $(OBJDIR)

# Needed Header Files
graph.c table.c: $(IDIR)/table.h 
cligraph.c graph.c: $(IDIR)/graph.h
