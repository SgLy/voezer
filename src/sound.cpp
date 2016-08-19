#include "sound.h"

SoLoud::Soloud gSoloud;

bool soloud_init() {
    gSoloud.init();
    return true;
}

Sound::Sound() {
}
Sound::Sound(const char add[]) {
    load(add);
}
Sound::~Sound() {
}
void Sound::load(const char add[]) {
    sound.load(add);
}
void Sound::play() {
    gSoloud.play(sound);
}
