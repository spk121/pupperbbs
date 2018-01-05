SUBDIRS = .

CXXFLAGS += -ggdb -O0

AM_CXXFLAGS = \
	-I/usr/local/include/ncursesw \
	-I/usr/local/mysql/connector-c++-8.0/include/
AM_LDFLAGS = \
	-L/usr/local/lib \
	-L/usr/local/mysql/connector-c++-8.0/lib64/debug

bin_PROGRAMS = pupper

# pupper_SOURCES = main.cpp \
# 	properties.hpp properties.cpp \
# 	PupperApp.hpp PupperApp.cpp

pupper_SOURCES = app.cpp \
	PupperLoginForm.hpp PupperLoginForm.cpp \
	PupperMainMenu.hpp PupperMainMenu.cpp \
	PupperMessageListMenu.hpp PupperMessageListMenu.cpp \
	PupperMessageEditor.hpp PupperMessageEditor.cpp \
	PupperTopics.hpp PupperTopics.cpp \
	PupperTopicMenu.hpp PupperTopicMenu.cpp \
	properties.hpp properties.cpp \
	PupperDB.hpp PupperDB.cpp
pupper_LDADD = -lmysqlcppconn8 -lncurses++w -lncursesw -lformw -lpanelw -lmenuw

sysconf_DATA = pupper.xml

EXTRA_DIST = pupper.xml.in pupper.sql.in
