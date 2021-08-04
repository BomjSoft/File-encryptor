CXX = g++
CXXFLAGS = -O3

SOURCES = main.cpp AES.cpp Sha256.cpp
HEADERS = AES.h Sha256.h
OBJECTS = main.o AES.o Sha256.o
TARGET = File_encryptor

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) -o $(TARGET) $(OBJECTS)

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) $< -o $@
