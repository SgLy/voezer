CXX = g++
SDL_LIB = -lSDL2 -lSDL2_mixer -lSDL2_image
SDL_INCLUDE = -I/usr/include
LOCAL_INCLUDE = -Iinclude
DEBUG = -Wall -g3
CXXFLAGS = -std=c++11 $(SDL_INCLUDE) $(RAPID_JSON_INCLUDE) $(LOCAL_INCLUDE) $(DEBUG)
LDFLAGS = $(SDL_LIB)
LIB = bin/sound.o bin/track.o bin/common.o bin/note.o

BIN_DIR = bin
SRC_DIR = src

main: src/main.cpp bin $(LIB)
	$(CXX) src/main.cpp $(LIB) -o main $(CXXFLAGS) $(LDFLAGS)

bin:
	mkdir bin

bin/common.o: src/common.cpp src/common.h
	$(CXX) -c src/common.cpp -o bin/common.o $(CXXFLAGS) $(LDFLAGS)

bin/sound.o: src/sound.cpp src/sound.h
	$(CXX) -c src/sound.cpp -o bin/sound.o $(CXXFLAGS) $(LDFLAGS)

bin/track.o: src/track.cpp src/track.h
	$(CXX) -c src/track.cpp -o bin/track.o $(CXXFLAGS) $(LDFLAGS)


bin/note.o: src/note.cpp src/note.h
	$(CXX) -c src/note.cpp -o bin/note.o $(CXXFLAGS) $(LDFLAGS)

clean:
	rm -rf $(BIN_DIR)
	rm main 
