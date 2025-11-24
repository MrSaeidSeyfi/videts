CXX = g++
CXXFLAGS = -std=c++11 -Wall -O2
LIBS = `pkg-config --cflags --libs opencv4`
TARGET = video_editor
SOURCE = video_editor.cpp

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCE) $(LIBS)

clean:
	rm -f $(TARGET)

.PHONY: all clean


