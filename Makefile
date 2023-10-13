# get my-lib:
# https://github.com/ehmcruz/my-lib
MYLIB = ../my-lib

# https://github.com/zeux/pugixml
PUGIXML = ../../git-others/pugixml/src

CC = gcc
CPP = g++
LD = g++
FLAGS = -std=c++23

CFLAGS = $(FLAGS)
CPPFLAGS = $(FLAGS) -I$(MYLIB)/include -I$(PUGIXML) -Wall
LDFLAGS =
BIN_NAME = cache-sim
RM = rm

# -fprofile-arcs -ftest-coverage

########################################################

SRC = $(wildcard *.cpp) $(PUGIXML)/pugixml.cpp

headerfiles = $(wildcard *.h) $(wildcard $(PUGIXML)/*.hpp) $(wildcard $(MYLIB)/include/my-lib/*.h)

OBJS = ${SRC:.cpp=.o}

########################################################

# implicit rules

%.o : %.c $(headerfiles)
	$(CC) -c $(CFLAGS) $< -o $@

%.o : %.cpp $(headerfiles)
	$(CPP) -c $(CPPFLAGS) $< -o $@

########################################################

all: $(BIN_NAME)
	@echo program compiled!
	@echo yes!

$(BIN_NAME): $(OBJS)
	$(LD) -o $(BIN_NAME) $(OBJS) $(LDFLAGS)

clean:
	-$(RM) $(OBJS)
	-$(RM) $(BIN_NAME)

