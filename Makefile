CXX := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -Iinclude
PKG_CONFIG := pkg-config
OPENCV_CFLAGS := $(shell $(PKG_CONFIG) --cflags opencv4)
OPENCV_LIBS := $(shell $(PKG_CONFIG) --libs opencv4)

SOURCES := $(wildcard src/*.cpp)
OBJECTS := $(SOURCES:.cpp=.o)
TARGET := videts

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS) $(OPENCV_LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(OPENCV_CFLAGS) -c $< -o $@

run: all
	./$(TARGET) $(ARGS)

clean:
	-rm -f $(OBJECTS) $(TARGET)

.PHONY: all run clean


