#
# Makefile.am for modules/y2m_menu
#

MOC=$(QTDIR)/bin/moc

etcdir = $(YAST2DIR)/etc

#etc_DATA = translate.conf


bin_PROGRAMS = ycpconsole
ycpconsole_SOURCES = \
	main.cpp		\
	ycpdemoview.cpp	\
	ycpdemodoc.cpp	\
	ycpdemo.cpp	\
	ycpdemo.h	\
	ycpdemodoc.h	\
	ycpdemoview.h

BUILT_SOURCES =	 \
	ycpdemo.moc.cpp \
	ycpdemodoc.moc.cpp \
	ycpdemoview.moc.cpp


ycpconsole_LDADD   = -L$(QTDIR)/lib -lqt-mt -lXext -lX11

ycpdemo.o:		ycpdemo.moc.cpp
ycpdemodoc.o:		ycpdemodoc.moc.cpp
ycpdemoview.o:		ycpdemoview.moc.cpp

ycpdemo.moc.cpp:	ycpdemo.h
ycpdemodoc.moc.cpp:	ycpdemodoc.h
ycpdemoview.moc.cpp:	ycpdemoview.h

%.moc.cpp: %.h
	$(MOC) $< -o $@

#translate.conf: ycpconsole
#	@./make_translate_conf *.cpp

# set the include path for X, qt and KDE
INCLUDES= -I$(QTDIR)/include -DSYSCONFDIR=\"$(sysconfdir)\" -DYAST2DIR=\"$(YAST2DIR)\" -DICONDIR=\"$(ICONDIR)\"


# the library search path. 
ycpconsole_LDFLAGS = $(all_libraries) 


EXTRA_DIST =	 \
	filenew.xpm	\
	fileopen.xpm		\
	filesave.xpm
