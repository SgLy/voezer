#ifndef SOUND_H_INCLUDED
#define SOUND_H_INCLUDED 1

#include <SDL2/SDL.h>
#include <bass.h>
#include <bass_fx.h>

bool bass_init();

class Sound {
public:
    Sound();
    ~Sound();
    Sound(const char add[]);
    void load(const char add[]);
    void play();

private:
    HSAMPLE sample;
    HCHANNEL channel;
};

class Music {
public:
    Music();
    ~Music();
    Music(const char add[]);
    void load(const char add[]);
    void play();
    void setSpeed(double sp);
    bool stopped();

private:
    HSTREAM stream;
};

#endif
