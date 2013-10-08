##############################
# Makefile for lib tst 
##############################

SHELL=/bin/sh

CC=gcc

VERSION=0.0.1

DESTDIR=
PREFIX=$(DESTDIR)/usr
INSTALLDIR=$(PREFIX)/lib
HDRINSTALLDIR=$(PREFIX)/include

LIBNAME=libtst
LIBDIR=lib
SONAME=$(LIBNAME).so
OUTNAME=$(LIBNAME).so.$(VERSION)

PKGDIR=pkgs
PKGNAME=$(LIBNAME)-$(VERSION)
PKGLIST=Makefile src/ include/ 

IDIR=include
LFLAGS=-I$(IDIR) -fPIC -g #uncomment for debuging with gdb
SHRDFLAGS= -shared -Wl,-soname,$(SONAME)
CFLAGS=-c -g #uncomment for debuging with gdb

HEADERS:=$(wildcard $(IDIR)/*.h)
HDRS=$(patsubst $(IDIR)/%.h, $(HDRINSTALLDIR)/%.h, $(HEADERS))
SRCS=*.c 
SRCDIR=src
SRC:=$(wildcard $(SRCDIR)/$(SRCS))

TSRCS=tst.c
TSRCDIR=tst
TSRC=$(patsubst %.c, $(TSRCDIR)/%.c, $(TSRCS))
TOBJ := $(patsubst $(TSRCDIR)/%.c, $(TSRCDIR)/%.o, $(TSRC))

OBJDIR=obj
OBJ := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))

.PHONY: setup clean pkg

all: setup $(OBJ) $(BINDIR)
	$(CC) $(LFLAGS) $(SHRDFLAGS) $(OBJ) -o $(LIBDIR)/$(OUTNAME)

pkg:
	mkdir -p $(PKGNAME)
	cp -r $(PKGLIST) $(PKGNAME)
	tar -vc -f $(PKGNAME).tar $(PKGNAME)
	gzip $(PKGNAME).tar 
	rm -r $(PKGNAME)	
	md5sum $(PKGNAME).tar.gz >> md5.txt
	mv $(PKGNAME).tar.gz $(PKGDIR)

testing: setup $(TOBJ) $(OBJ)
	$(CC) $(LFLAGS) $(TOBJ) $(OBJ) -o $(TSRCDIR)/tst

setup: 
	mkdir -p $(LIBDIR)
	mkdir -p $(OBJDIR)

clean: 
	rm -rf $(OBJDIR)

reinstall: uninstall install clean 

install: all clean
	mkdir -p $(HDRINSTALLDIR)
	mkdir -p $(INSTALLDIR) 
	install $(HEADERS) -t $(HDRINSTALLDIR)
	install $(LIBDIR)/$(OUTNAME) -t $(INSTALLDIR)
	ldconfig -n $(INSTALLDIR) 

uninstall: 
	rm -I $(INSTALLDIR)/$(SONAME)* 
	rm -I $(HDRS)

$(TSRCDIR)/%.o: $(TSRCDIR)/%.c
	$(CC) $(LFLAGS) $(CFLAGS) $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c 
	$(CC) $(LFLAGS) $(CFLAGS) $< -o $@

