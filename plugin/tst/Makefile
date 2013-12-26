##############################
# @file Makefile for tst plugin
# @author Noah Harvey <noah.harvey247@gmail.com>
##############################
#
## PROJECT ##############################
SHELL=/bin/sh 
PROJECT=tst
VERSION=0.1

## END PROJECT ############################
#
## DIRS ##############################

PREFIX=.
BINDIR=$(PREFIX)/bin
IDIR=$(PREFIX)/include
SRCDIR=$(PREFIX)/src
TSTDIR=$(PREFIX)/test
LIBDIR=$(PREFIX)/lib

CLEAN=$(LIBDIR) $(BINDIR)
REQUIRED_DIRS=$(BINDIR) $(LIBDIR)

## END DIRS ############################
#
#
## FLAGS ##############################

#libraries to link in
LLIBS=cligraph
LLIBS:=$(patsubst %, -l%, $(LLIBS))
LLIB_SDIR=-L../../lib/

export CC=gcc
export CFLAGS=-I$(IDIR) -I../../include -Wall -g -fPIC
export LFLAGS=$(LLIB_SDIR) $(LLIBS) 

SOFLAGS=-shared

## END FLAGS ############################
#
## OBJECTS ##############################

OBJECTS:=$(patsubst %.c, %.o, $(wildcard $(SRCDIR)/*.c))
LIBNAME=$(LIBDIR)/lib$(PROJECT).a
TSTOBJECTS=$(patsubst %.c, %.o, $(wildcard $(TSTDIR)/*.c))

## END OBJECTS ############################
#
## TARGETS ##############################

TARGET=lib$(PROJECT).so

.PHONY: all setup clean package test

all: setup $(TARGET)

#for debuging this make file
debugmk:
	@echo $(OBJECTS)
	@echo $(LIBOBJECTS)

#create directories that are needed before building
setup:
	@mkdir -p $(REQUIRED_DIRS)

$(LIBNAME): $(OBJECTS)
	ar rcs $@ $(OBJECTS)
	ranlib $@

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(SOFLAGS) $(LFLAGS)  

#build: $(SUBDIRS)

#clean: $(SUBDIRS)
clean: CLEAN += $(OBJECTS) $(TSTOBJECTS)
clean:
	rm -rf $(CLEAN)

test: all $(TSTOBJECTS)
	$(CC) -o tst $(TSTOBJECTS) $(LFLAGS) -L. -l$(PROJECT) 

#$(SUBDIRS):
#	$(MAKE) -C $@ $(MAKECMDGOALS)
## END TARGETS ############################
