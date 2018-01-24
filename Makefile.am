SUBDIRS = .

CXXFLAGS += -ggdb -O0

AM_CXXFLAGS = \
	-I/usr/local/include/ncursesw \
	-I/usr/local/include/libzmodem \
	-I/usr/include/mysql \
	-fdiagnostics-color=auto \
	-std=c++17 -Wall -Wextra

AM_LDFLAGS = \
	-L/usr/local/lib \
	-L/usr/lib64/mysql

bin_PROGRAMS = pupper

pupper_SOURCES = app.cpp \
	login.hpp login.cpp \
	form.hpp form.cpp \
	mainmenu.hpp mainmenu.cpp \
	msglist.hpp msglist.cpp \
	editor.hpp editor.cpp \
	topic.hpp topic.cpp \
	topicmnu.hpp topicmnu.cpp \
	prop.hpp prop.cpp \
	db.hpp db.cpp \
	viewer.hpp viewer.cpp \
	message.hpp message.cpp \
	pfile.hpp pfile.cpp \
	filemenu.hpp filemenu.cpp \
	upform.hpp upform.cpp

BUILT_SOURCES = login.hpp.gch \
	mainmenu.hpp.gch \
	msglist.hpp.gch \
	editor.hpp.gch \
	topicmnu.hpp.gch \
	prop.hpp.gch \
	db.hpp.gch \
	viewer.hpp.gch

%.hpp.gch: %.hpp
	$(CXX) $(CFLAGS) $(CXXFLAGS) $(AM_CFLAGS) $(AM_CXXFLAGS) -c $<

pupper_LDADD = -lmysqlclient -lpthread -lm -lrt -ldl \
	-lncurses++w -lncursesw -lformw -lpanelw -lmenuw -lzmodem

sysconf_DATA = pupper.xml

EXTRA_DIST = pupper.xml.in pupper.sql.in
CLEANFILES = $(BUILT_SOURCES)
include $(top_srcdir)/flymake.mk

