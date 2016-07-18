CXX = g++
SDL_LIB = -lSDL2 -lSDL2_mixer -lSDL2_image
SDL_INCLUDE = -I/usr/include
LOCAL_INCLUDE = -Iinclude
CXXFLAGS = -Wall -std=c++11 $(SDL_INCLUDE) $(RAPID_JSON_INCLUDE) $(LOCAL_INCLUDE)
LDFLAGS = $(SDL_LIB)
TARGET = $(BIN_DIR)/main 

BIN_DIR = bin
SRC_DIR = src

#all: $(TARGET)
main: src/main.cpp
	g++ src/main.cpp -o main $(CXXFLAGS) $(LDFLAGS) $(SDL_INCLUDE) $(LOCAL_INCLUDE)

# $(BIN_DIR)/$(TARGET): $(BIN_DIR)/main.o
#     $(CXX) $< $(LDFLAGS) -o $@

# $(BIN_DIR)/main.o: $(SRC_DIR)/main.cpp
#     $(CXX) $(CXXFLAGS) $< -o $@ -g3

clean:
	rm $(BIN_DIR)/*
	rm main 
