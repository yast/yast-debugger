#
# Makefile.am for ycpdebugger/src
#

MOC = $(QTDIR)/bin/moc

bin_PROGRAMS = ycpdebugger
ycpdebugger_SOURCES =		\
	ycpdebugger.cc		\
	base.cc base.h		\
	oscar.cc oscar.h	\
	utils.cc utils.h

BUILT_SOURCES =			\
	base.moc.cc		\
	oscar.moc.cc

ycpdebugger_LDADD = -lycp			\
	-L$(QTLIBDIR) -lqt-mt -lXext -lX11

base.o base.lo:		base.moc.cc
base.moc.cc:		base.h

oscar.o oscar.lo:	oscar.moc.cc
oscar.moc.cc:		oscar.h

%.moc.cc: %.h
	$(MOC) $< -o $@

INCLUDES = -I$(QTDIR)/include -I$(includedir)

ycpdebugger_LDFLAGS = $(all_libraries)

EXTRA_DIST = *.xpm

