#ifndef NOTE_H_INCLUDED
#define NOTE_H_INCLUDED 1

#include <stdio.h>
#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include <rapidjson/document.h>
#include "common.h"
#include "track.h"
#include "sound.h"

class Note {
    friend class Pattern;
    friend class Tracks;
public:
    Note();
    Note(const rapidjson::Value &val);
    void Draw(double time, SDL_Renderer * Renderer);
    bool Play(double time, Sound * beat);

private:
    int id, track, dir;
    double time, hold;
	NoteType type;
};

class Pattern {
    friend class Note;
public:
    Pattern();
    Pattern(const char address[], SDL_Renderer * Renderer_, Sound * beat_);
    void Draw(double time);
    void Play(double time);

private:
    void attach_renderer(SDL_Renderer * Renderer_);
    void attach_beat(Sound * beat_);
    void read_pattern(const char address[]);
    SDL_Renderer * Renderer;
    Sound * beat;
    std::vector<Note> notes;
    uint beat_played_cnt;
};


#endif
