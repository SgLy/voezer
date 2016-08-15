#ifndef SOUND_H_INCLUDED
#define SOUND_H_INCLUDED 1

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

bool mixer_init();

class Music {
public:
	Music();
    ~Music();
	Music(const char add[]);
    void load(const char add[]);
    void play();

private:
	Mix_Music * music;
};

class Sound {
public:
	Sound();
    ~Sound();
	Sound(const char add[]);
    void load(const char add[]);
    void play();

private:
	Mix_Chunk * sound;
};

#endif
