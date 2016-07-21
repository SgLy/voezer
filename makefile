CXX = g++
SDL_LIB = -lSDL2 -lSDL2_mixer -lSDL2_image
SDL_INCLUDE = -I/usr/include
LOCAL_INCLUDE = -Iinclude
CXXFLAGS = -Wall -std=c++11 $(SDL_INCLUDE) $(RAPID_JSON_INCLUDE) $(LOCAL_INCLUDE)
LDFLAGS = $(SDL_LIB)
LIB = bin/sound.o

BIN_DIR = bin
SRC_DIR = src

main: src/main.cpp bin/sound.o
	$(CXX) src/main.cpp $(LIB) -o main $(CXXFLAGS) $(LDFLAGS)

bin/sound.o: src/sound.cpp src/sound.h
	$(CXX) -c src/sound.cpp -o bin/sound.o $(CXXFLAGS) $(LDFLAGS)

clean:
	rm $(BIN_DIR)/*
	rm main 
