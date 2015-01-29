# compiler settings
CXX      = g++
CXXFLAGS = -Wall -fexceptions -g -std=c++11
INCLUDES = -Iinclude -Iinclude/Assets -Iinclude/Composites -Iinclude/Effects -Iinclude/Shapes -Iinclude/Transitions
LDFLAGS  = -static
MKDIR    = mkdir -p $(@D)

# libs to link with
LIBS = 

# source files
SRCS =  main.cpp \
		src/ConsoleEngine.cpp \
		src/AssetManager.cpp \

# object path obj/Debug folder
OBJS_TARGET = obj/Debug/
CXX_OBJECTS = $(patsubst %.cpp, $(OBJS_TARGET)%.o, $(SRCS))

# target binary
MAIN = bin/Debug/ascii_demo

.PHONY: depend clean

all:    $(MAIN)
		@echo  $(MAIN) has been successfully compiled.

$(MAIN): $(CXX_OBJECTS) 
		$(MKDIR)
		$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(MAIN) $(CXX_OBJECTS) $(LDFLAGS) $(LIBS)

obj/Debug/%.o: %.cpp
		$(MKDIR)
		$(CXX) $(INCLUDES) $(CXXFLAGS) -c $<  -o $@

clean:
		$(RM) $(OBJS_TARGET)*.o $(OBJS_TARGET)src/*.o $(MAIN).exe

depend: $(SRCS)
		makedepend $(INCLUDES) $^
