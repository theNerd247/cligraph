##############################
# Makefile for cligraph
##############################

SHELL=/bin/sh

CC=gcc

BINDIR=bin
EXEC=cligraph

IDIR=./include
LFLAGS=-I$(IDIR) -lm
CFLAGS=-c -Wall #-g #uncomment for debuging with gdb

SRCS=*.c 
SRCDIR=./src
SRC:=$(wildcard $(SRCDIR)/$(SRCS))

OBJDIR=obj
OBJ := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))

all: $(OBJ) $(BINDIR)
	$(CC) $(LFLAGS) $(OBJ) -o $(BINDIR)/$(EXEC)

debug: $(OBJ) $(BINDIR)
	$(CC) $(LFLAGS) -g $(OBJ) -o $(BINDIR)/$(EXEC)

$(OBJDIR)/%.o: $(SRCDIR)/%.c 
	$(CC) $(LFLAGS) $(CFLAGS) $< -o $@

$(OBJ): | $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR): 
	mkdir -p $(BINDIR)

clean: 
	rm -rf $(OBJDIR)
