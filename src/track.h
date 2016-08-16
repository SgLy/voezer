#ifndef TRACK_H_INCLUDED
#define TRACK_H_INCLUDED 1

#include <stdio.h>
#include <math.h>
#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include <rapidjson/document.h>
#include "common.h"

#define EaseType int

class TrackAction {
    friend class Track;
    friend class Tracks;
public:
    TrackAction();
    TrackAction(const rapidjson::Value &val);

private:
    void read_action(const rapidjson::Value &val);
    double from, to, start, end;
    EaseType ease;
};      

class Track {
    friend class Tracks;
    friend class TrackAction;
public:
    Track();
    Track(const rapidjson::Value &val);
    void Draw(double time, SDL_Renderer * Renderer);
    double GetValue(const std::vector<TrackAction> &actions, double time, double init);
    double GetPosition(double time);
    double GetSize(double time);

private:
    void read_track(const rapidjson::Value &doc);
    bool entrance_on;
    double x, size, start, end;
    int color, id;
    std::vector<TrackAction> move;
    std::vector<TrackAction> scale;
    std::vector<TrackAction> colorchange;
};
    
class Tracks {
    friend class Track;
    friend class TrackAction;
public:
    Tracks();
    Tracks(const char address[], SDL_Renderer * Renderer_);
    void Draw(double time);

private:
    void attach_renderer(SDL_Renderer * Renderer_);
    void read_pattern(const char address[]);
    SDL_Renderer * Renderer;
    std::vector<Track> track;
};

#endif
