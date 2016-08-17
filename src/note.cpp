#include "note.h"

// Pattern class definition

Pattern::Pattern() {}

Pattern::Pattern(const char address[], SDL_Renderer * Renderer_, Sound * beat_, Tracks * tracks_) {
    read_pattern(address);
    attach_renderer(Renderer_);
    attach_beat(beat_);
    attach_tracks(tracks_);
    beat_played_cnt = 0;
}

void Pattern::read_pattern(const char address[]) {
    FILE * input_file = fopen(address, "r");
    if (input_file == NULL)
        return;
    while (fgets(buf, MAX_BUF_LEN, input_file) != NULL)
        strcat(str, buf);
    fclose(input_file);
    
    rapidjson::Document doc;
    doc.Parse(str);

    notes.clear();
    for (rapidjson::SizeType i = 0; i < doc.Size(); ++i)
        notes.push_back(Note(doc[i]));

    memset(buf, 0, sizeof(buf));
    memset(str, 0, sizeof(str));
}

void Pattern::attach_renderer(SDL_Renderer * Renderer_) {
    Renderer = Renderer_;
}

void Pattern::attach_beat(Sound * beat_) {
    beat = beat_;
}

void Pattern::attach_tracks(Tracks * tracks_) {
    tracks = tracks_;
}

void Pattern::Play(double time) {
    for (uint i = beat_played_cnt; i < notes.size(); ++i)
        if (notes[i].Play(time, beat))
            ++beat_played_cnt;
}

void Pattern::Draw(double time) {
    for (auto note: notes)
        note.Draw(time, Renderer, (*tracks)[note.track]);
}

// Note class definition

Note::Note() {}

Note::Note(const rapidjson::Value &val) {
    id = val["Id"].GetInt();
    track = val["Track"].GetInt();
    dir = val["Dir"].GetInt();
    time = val["Time"].GetDouble();
    hold = val["Hold"].GetDouble();
    std::string t = val["Type"].GetString();
    if (t == "click")
        type = NOTE_CLICK;
    if (t == "swipe")
        type = NOTE_SWIPE;
    if (t == "slide")
        type = NOTE_SLIDE;
    if (t == "hold")
        type = NOTE_HOLD;
}

bool Note::Play(double time, Sound * beat) {
    if (time > this->time) {
        beat->play();
        return true;
    } else
        return false;
}

void Note::Draw(double time, SDL_Renderer * Renderer, Track * track) {
    if (time > this->time + hold + 0.1)
        return;
    int PIXEL_PER_SECOND = SCREEN_HEIGHT, NOTE_SIZE = TRACK_BASIC_WIDTH / 2;
    int y = SCREEN_HEIGHT / 6 * 5 - (this->time - time) * PIXEL_PER_SECOND;
    if (y < 0)
        return;
    int x = track->GetPosition(time);
    SDL_Rect fillRect = {x - NOTE_SIZE / 2, y - NOTE_SIZE, NOTE_SIZE, NOTE_SIZE};
    if (type == NOTE_CLICK)
        SDL_SetRenderDrawColor(Renderer, 128, 0, 0, 192);
    if (type == NOTE_HOLD) {
        int y2 = SCREEN_HEIGHT / 6 * 5 - (this->time + hold - time) * PIXEL_PER_SECOND;
        int height = NOTE_SIZE + std::min(SCREEN_HEIGHT / 6 * 5 - y2, y - y2);
        fillRect = {x - NOTE_SIZE / 2, y2, NOTE_SIZE, height};
        SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 192);
    }
    if (type == NOTE_SLIDE)
        SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 192);
    if (type == NOTE_SWIPE)
        SDL_SetRenderDrawColor(Renderer, 0, 0, 128, 192);
    SDL_RenderFillRect(Renderer, &fillRect);
}

