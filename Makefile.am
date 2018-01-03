SUBDIRS = .

bin_PROGRAMS = pupper

pupper_SOURCES = main.cpp \
	properties.hpp properties.cpp \
	PupperApp.hpp PupperApp.cpp
pupper_CXXFLAGS = $(PUPPER_CXXFLAGS) $(NCURSES_CXX_CFLAGS)
pupper_LDADD = $(PUPPER_LIBS) $(NCURSES_CXX_LIBS) -lmysqlcppconn8

sysconf_DATA = pupper.xml

EXTRA_DIST = pupper.xml.in pupper.sql.in
