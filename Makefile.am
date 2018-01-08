SUBDIRS = .

CXXFLAGS += -ggdb -O0

AM_CXXFLAGS = \
	-I/usr/local/include/ncursesw \
	-I/usr/include/mysql \
	-fdiagnostics-color=auto \
	-std=c++17
AM_LDFLAGS = \
	-L/usr/local/lib \
	-L/usr/lib64/mysql

# 	-I/usr/local/mysql/connector-c++-8.0/include/ \
#	-L/usr/local/mysql/connector-c++-8.0/lib64/debug

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
	PupperDB.hpp PupperDB.cpp \
	PupperMessageViewer.hpp PupperMessageViewer.cpp 
pupper_LDADD = -lmysqlclient -lpthread -lm -lrt -ldl \
	-lncurses++w -lncursesw -lformw -lpanelw -lmenuw

#pupper_LDADD = -lmysqlcppconn8 -lncurses++w -lncursesw -lformw -lpanelw -lmenuw

sysconf_DATA = pupper.xml

EXTRA_DIST = pupper.xml.in pupper.sql.in
