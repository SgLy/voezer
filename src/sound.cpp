#include "sound.h"

bool bass_init() {
    return BASS_Init(-1, 48000, 0, 0, 0);
}

Sound::Sound() {
}
Sound::Sound(const char add[]) {
    load(add);
}
Sound::~Sound() {
}
void Sound::load(const char add[]) {
    sample = BASS_SampleLoad(false, add, 0, 0, 65535, 0);
}
void Sound::play() {
    channel = BASS_SampleGetChannel(sample, false);
    BASS_ChannelPlay(channel, true);
}

Music::Music() {
}
Music::Music(const char add[]) {
    load(add);
}
Music::~Music() {
}
void Music::load(const char add[]) {
    stream = BASS_StreamCreateFile(false, add, 0, 0, BASS_STREAM_DECODE);
    if (stream)
        stream = BASS_FX_TempoCreate(stream, BASS_FX_FREESOURCE);
}
void Music::play() {
    BASS_ChannelPlay(stream, true);
}
void Music::setSpeed(double sp) {
    BASS_ChannelSetAttribute(stream, BASS_ATTRIB_TEMPO, (sp - 1) * 100);
}
bool Music::stoped(double current) {
    return BASS_ChannelIsActive(stream);
}
