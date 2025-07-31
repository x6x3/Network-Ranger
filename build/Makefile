CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2 $(shell pkg-config --cflags jsoncpp)
LDFLAGS = -lcurl -ljsoncpp -lpthread $(shell pkg-config --libs jsoncpp)

TARGET = network-ranger
SOURCES = ranger.c++ network-utils.cpp
OBJECTS = ranger.o network-utils.o

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.c++
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

install-deps-ubuntu:
	sudo apt-get update
	sudo apt-get install -y libcurl4-openssl-dev libjsoncpp-dev build-essential

install-deps-macos:
	brew install curl jsoncpp

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean install-deps-ubuntu install-deps-macos run 
