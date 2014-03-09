##############################
# @file Makefile for cligraph
# @author Noah Harvey <noah.harvey247@gmail.com>
##############################
#
## PROJECT ##############################

SHELL=/bin/sh
PROJECT=cligraph
VERSION=0.1

## END PROJECT ############################
#
## DIRS ##############################

PREFIX=.
BINDIR=$(PREFIX)/bin
IDIR=$(PREFIX)/include
SRCDIR=$(PREFIX)/src
LIBDIR=$(PREFIX)/lib
CLEAN=$(LIBDIR) 
REQUIRED_DIRS=$(BINDIR) $(LIBDIR)

## END DIRS ############################
#
## FILES ##############################
DOXYGEN_CONFIG_FILE=$(PREFIX)/doxygen.conf
## END FILES ############################
#
## FLAGS ##############################

LLIBS=dl llist pthread ncurses menu m str
LLIBS:=$(patsubst %, -l%, $(LLIBS))

export CC=gcc
export CFLAGS=-I$(IDIR) -Wall -g -fPIC
export LFLAGS=$(LLIBS) 

SOFLAGS=-shared

## END FLAGS ############################
#
## OBJECTS ##############################

OBJS:=$(patsubst %.c, %.o, $(wildcard $(SRCDIR)/*.c))
LIBOBJS=plugins.o tui.o winmgr.o keyboard.o
LIBOBJS:=$(patsubst %.o, $(SRCDIR)/%.o, $(LIBOBJS))
LIBOBJECTS=$(filter $(LIBOBJS), $(OBJS))
LIBNAME=$(PROJECT)
LIBTARGET=$(LIBDIR)/lib$(LIBNAME).so
OBJECTS:=$(filter-out $(LIBOBJECTS), $(OBJS))

## END OBJECTS ############################
#
## TARGETS ##############################

TARGET=$(BINDIR)/$(PROJECT)

.PHONY: all setup clean package doc

all: setup $(TARGET)

#for debuging this make file
debugmk: 
	@echo $(OBJECTS)
	@echo $(LIBOBJECTS)

#create directories that are needed before building
setup:
	@mkdir -p $(REQUIRED_DIRS)

$(LIBTARGET): $(LIBOBJECTS)
	$(CC) -o $(LIBTARGET) $(LIBOBJECTS) $(SOFLAGS) $(LFLAGS)

$(TARGET): $(LIBTARGET) $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(LFLAGS) -L$(LIBDIR) -l$(LIBNAME)

#perform a recursive build 
#build: $(SUBDIRS)

#perform a recursive clean
#clean: $(SUBDIRS)
clean: CLEAN += $(OBJECTS) $(LIBOBJECTS)
clean: 
	rm -rf $(CLEAN)

doc:
	doxygen $(DOXYGEN_CONFIG_FILE)

## END TARGETS ############################
