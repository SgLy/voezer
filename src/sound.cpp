#include "sound.h"

bool mixer_init() {
    if (Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, 512) < 0) {
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
        return false;
    }
	return true;
}


Music::Music() {
    music = NULL;
}
Music::Music(const char add[]) {
	music = Mix_LoadMUS(add);
}
Music::~Music() {
    if (music != NULL)
        Mix_FreeMusic(music);
    music = NULL;
}
void Music::load(const char add[]) {
    music = Mix_LoadMUS(add);
}
void Music::play() {
    Mix_PlayMusic(music, -1);
}

Sound::Sound() {
    sound = NULL;
}
Sound::Sound(const char add[]) {
    sound = Mix_LoadWAV(add);
}
Sound::~Sound() {
    if (sound != NULL)
        Mix_FreeChunk(sound);
    sound = NULL;
}
void Sound::load(const char add[]) {
    sound = Mix_LoadWAV(add);
}
void Sound::play() {
    Mix_PlayChannel(-1, sound, 0);
}
