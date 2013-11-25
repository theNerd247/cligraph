##############################
# Makefile for cligraph
##############################

SHELL=/bin/sh

## PROJECT ##############################
CC=gcc

EXEC=cligraph
VERSION=0.1
PKGNAME=cligraph
PKGLIST=Makefile src/ include/ testing/
## END PROJECT ############################
#
#
## DIRS ##############################
BINDIR=bin
IDIR=./include
SRCDIR=./src
OBJDIR=obj
SHARED_LIBDIR=lib
SHARED_OBJDIR=libobj
REQUIRED_DIRS = $(OBJDIR) $(BINDIR) $(SHARED_OBJDIR) $(SHARED_LIBDIR)
CLEAN_DIRS = $(OBJDIR) $(SHARED_OBJDIR)
## END DIRS ############################
#
#
## SRC ##############################
SRCS=*.c
SRC=$(wildcard $(SRCDIR)/$(SRCS))
OBJ=$(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))
## END SRC ############################
#
#
## LIBCLIGRAPH ##############################
SHARED_SRCS=plugins.c
SHARED_OBJS=$(patsubst %.c, $(OBJDIR)/%.o, $(SHARED_SRCS))
OBJ:=$(filter-out $(SHARED_OBJS), $(OBJ))
SHARED_OBJS:=$(patsubst $(OBJDIR)/%.o, $(SHARED_OBJDIR)/%.o, $(SHARED_OBJS))

SHARED_LIBNAME=lib$(EXEC).so
SHARED_OUTNAME=$(SHARED_LIBDIR)/$(SHARED_LIBNAME).$(VERSION)
## END LIBCLIGRAPH ############################
#
#
## FLAGS ##############################
LLIBS=dl llist pthread 
LLIBS:=$(patsubst %, -l%, $(LLIBS))
LLFLAGS=-I$(IDIR) $(LLIBS) -g #uncomment for debuging with gdb
SHARED_LFLAGS=$(LLFLAGS) -fPIC 
LFLAGS=$(LLFLAGS) -L. -l$(EXEC)
SHARED_SFLAGS=-shared -Wl,-soname,$(SHARED_LIBNAME)

CFLAGS=-Wall -c -g #uncomment for debuging with gdb
## END FLAGS ############################
#
#
## TESTING ##############################
TSRCS=tst.c
TSRCDIR=./testing
TSRC=$(patsubst %.c, $(TSRCDIR)/%.c, $(TSRCS))
TOBJ := $(patsubst $(TSRCDIR)/%.c, $(TSRCDIR)/%.o, $(TSRC))
## END TESTING ############################

.PHONY: setup clean package libcligraph

all: setup $(SHARED_OUTNAME) $(OBJ) 
	$(CC) $(LFLAGS) $(OBJ) -o $(BINDIR)/$(EXEC)

run: all
	echo "export LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:$$(pwd)/$(SHARED_LIBDIR)" > $(BINDIR)/run.sh
	echo "exec $(BINDIR)/$(EXEC)" >> $(BINDIR)/run.sh
	$(BINDIR)/run.sh

#for debuging Makefile
dbg: 
	echo $(OBJ)
	echo $(SHARED_OBJS)

pkg:
	mkdir -p $(PKGNAME)
	cp -r  $(PKGLIST) $(PKGNAME)
	tar -vc -f $(PKGNAME).tar $(PKGNAME)
	rm -r $(PKGNAME)	

testing: setup $(TOBJ) $(OBJ)
	$(CC) $(LFLAGS) $(TOBJ) $(OBJ) -o $(TSRCDIR)/tst

setup: 
	mkdir -p $(REQUIRED_DIRS)
	touch $(BINDIR)/run.sh
	chmod 700 $(BINDIR)/run.sh

clean: 
	rm -rf $(CLEAN_DIRS)

$(TSRCDIR)/%.o: $(TSRCDIR)/%.c
	$(CC) $(LFLAGS) $(CFLAGS) $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c 
	$(CC) $(LFLAGS) $(CFLAGS) $< -o $@

$(SHARED_OUTNAME): $(SHARED_OBJS)
	$(CC) $(SHARED_SFLAGS) $(SHARED_OBJS) -o $(SHARED_OUTNAME)
	ln -sfT $(SHARED_OUTNAME) $(SHARED_LIBNAME)

$(SHARED_OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(SHARED_LFLAGS) $(CFLAGS) $< -o $@
