GCC = g++
CFLAGS = -O3

SOURCES = main.cpp AES.cpp Sha256.cpp
OBJECTS = main.o AES.o Sha256.o
RESULT = File_encryptor

all: $(RESULT)

$(RESULT): $(OBJECTS)
	$(GCC) -o $(RESULT) $(OBJECTS)

%.o: %.c
	$(GCC) $(CFLAGS) $< -o $@