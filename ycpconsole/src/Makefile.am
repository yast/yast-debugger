#
# Makefile.am for debugger/ycpconsole/src
#

MOC = $(QTDIR)/bin/moc

bin_PROGRAMS = ycpconsole
ycpconsole_SOURCES =		\
	main.cpp		\
	ycpdemoview.cpp		\
	ycpdemodoc.cpp		\
	ycpdemo.cpp		\
	ycpdemo.h		\
	ycpdemodoc.h		\
	ycpdemoview.h

BUILT_SOURCES =			\
	ycpdemo.moc.cpp		\
	ycpdemodoc.moc.cpp	\
	ycpdemoview.moc.cpp

ycpconsole_LDADD   = -L$(QTLIBDIR) -lqt-mt -lXext -lX11

ycpdemo.o:		ycpdemo.moc.cpp
ycpdemodoc.o:		ycpdemodoc.moc.cpp
ycpdemoview.o:		ycpdemoview.moc.cpp

ycpdemo.moc.cpp:	ycpdemo.h
ycpdemodoc.moc.cpp:	ycpdemodoc.h
ycpdemoview.moc.cpp:	ycpdemoview.h

%.moc.cpp: %.h
	$(MOC) $< -o $@

INCLUDES= -I$(QTDIR)/include

ycpconsole_LDFLAGS = $(all_libraries)

EXTRA_DIST =			\
	filenew.xpm		\
	fileopen.xpm		\
	filesave.xpm
