# The default rules for C++ is
# $(CXX) $(CPPFLAGS) $(CXXFLAGS) -c

# CPPFLAGS should be -Dmacro -Idir -M -MM -MG -MF etc
CPPFLAGS := $(shell pkg-config --cflags-only-I ncurses++w ncursesw) \
	-I/usr/local/include -I/home/mike/Projects/mysql-connector-cpp/include

# CXXFLAGS are everything but -D and -I
CXXFLAGS := -std=c++17 \
	-fdiagnostics-color=auto -march=native \
	$(shell pkg-config --cflags-only-other ncurses++w ncursesw) \
	$(shell mysql_config --cflags) \
	-D_FORTIFY_SOURCE=2 -ggdb -O0 \
	-fstack-protector-strong -fstrict-aliasing -fstrict-overflow \
	-Wall -Wextra -Wformat=2 -Wformat-signedness \
	-Wformat-security -Werror=format-security \
	-Wsuggest-attribute=format -Wsuggest-final-types \
	-Wsuggest-final-methods \
	-Wduplicated-cond -Wduplicated-branches \
	-Wshadow -Wsign-conversion -Wlogical-op 

# A standard for linking is
# $(CC) $(LDFLAGS) *.o $(LDLIBS)

# Extra flags given to compilers when invoking the linker,
# such as -L flags
LDFLAGS := \
	$(shell pkg-config --libs-only-L ncurses++w ncursesw) \
	-L/home/mike/Projects/mysql-connector-cpp

# Library flags or names given to compilers when invoking the
# linker.
LDLIBS := $(shell pkg-config --libs-only-l --libs-only-other ncurses++w ncursesw) \
	-pthread 

	#-lmysqlcppconn8 \
#	$(shell mysql_config --libs) \

all: pupper

OBJS =  main.o properties.o PupperApp.o

%o : %.cpp
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $< -o $@

pupper: $(OBJS)
	@rm -f pupper
	$(CXX) $(LDFLAGS) -o $@ $(OBJS) /home/mike/Projects/mysql-connector-cpp/libmysqlcppconn8-static.a $(LDLIBS)

# Clean up everything but the .EXEs
clean :
	-rm *.o
	-rm *.map
	-rm pupper

check-syntax:
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o /dev/null -S $(CHK_SOURCES)
