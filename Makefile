##############################
# Makefile for cligraph
##############################

SHELL=/bin/sh

CC=gcc

BINDIR=bin
EXEC=cligraph

IDIR=./include
LFLAGS=-I$(IDIR) -lm -lllist -g #uncomment for debuging with gdb
CFLAGS=-c -Wall -g #uncomment for debuging with gdb

SRCS=*.c 
SRCDIR=./src
SRC:=$(wildcard $(SRCDIR)/$(SRCS))

OBJDIR=obj
OBJ := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))

.PHONY: setup clean 

all: setup $(OBJ) $(BINDIR)
	$(CC) $(LFLAGS) $(OBJ) -o $(BINDIR)/$(EXEC)

$(OBJDIR)/%.o: $(SRCDIR)/%.c 
	$(CC) $(LFLAGS) $(CFLAGS) $< -o $@

setup: 
	mkdir -p $(OBJDIR)

clean: 
	rm -rf $(OBJDIR)
