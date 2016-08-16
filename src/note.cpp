#include "note.h"

// Pattern class definition

Pattern::Pattern() {}

Pattern::Pattern(const char address[], SDL_Renderer * Renderer_, Sound * beat_) {
    read_pattern(address);
    attach_renderer(Renderer_);
    attach_beat(beat_);
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

void Pattern::Play(double time) {
    for (uint i = beat_played_cnt; i < notes.size(); ++i)
        if (notes[i].Play(time, beat))
            ++beat_played_cnt;
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
