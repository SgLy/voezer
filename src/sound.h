#ifndef SOUND_H_INCLUDED
#define SOUND_H_INCLUDED 1

#include <SDL2/SDL.h>
#include <soloud/soloud.h>
#include <soloud/soloud_wav.h>

extern SoLoud::Soloud gSoloud;

bool soloud_init();

class Sound {
public:
	Sound();
    ~Sound();
	Sound(const char add[]);
    void load(const char add[]);
    void play();

private:
    SoLoud::Wav sound;
};

#endif
