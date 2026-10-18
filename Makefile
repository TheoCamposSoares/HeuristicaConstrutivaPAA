CXX ?= g++
CXXFLAGS ?= -std=c++11 -Wall -Wextra -pedantic
TARGET ?= programa.exe

SOURCES := $(wildcard src/*.cpp)
HEADERS := $(wildcard include/*.hpp)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $@

clean:
	$(RM) $(TARGET)
