#include "track.h"

// Class Tracks definition

Tracks::Tracks() {}
Tracks::Tracks(const char address[], SDL_Renderer * Renderer_) {
    read_pattern(address);
    attach_renderer(Renderer_);
}

void Tracks::attach_renderer(SDL_Renderer * Renderer_) {
    Renderer = Renderer_;
}

void Tracks::read_pattern(const char address[]) {
    FILE * input_file = fopen(address, "r");
    if (input_file == NULL)
        return;
    while (fgets(buf, MAX_BUF_LEN, input_file) != NULL)
        strcat(str, buf);
    fclose(input_file);
    
    rapidjson::Document doc;
    doc.Parse(str);

    track.clear();
    for (rapidjson::SizeType i = 0; i < doc.Size(); ++i)
        track.push_back(Track(doc[i]));

    memset(buf, 0, sizeof(buf));
    memset(str, 0, sizeof(str));
}

void Tracks::Draw(double time) {
    for (auto t: track)
        t.Draw(time, Renderer);
}
    
Track * Tracks::operator[] (int x) {
    for (uint i = 0; i < track.size(); ++i)
        if (track[i].id == x)
            return &track[i];
}

// Class Track definition

Track::Track() {}

Track::Track(const rapidjson::Value &val) {
    read_track(val);
}

void Track::read_track(const rapidjson::Value &val) {
    id = val["Id"].GetInt();
    entrance_on = val["EntranceOn"].GetBool();
    x = val["X"].GetDouble();
    size = val["Size"].GetDouble();
    start = val["Start"].GetDouble();
    end = val["End"].GetDouble();
    color = val["Color"].GetInt();

    move.clear();
    for (rapidjson::SizeType i = 0; i < val["Move"].Size(); ++i) {
        move.push_back(TrackAction(val["Move"][i]));
        if (i == 0)
            move[i].from = x;
        else
            move[i].from = move[i - 1].to;
    }
    
    scale.clear();
    for (rapidjson::SizeType i = 0; i < val["Scale"].Size(); ++i) {
        scale.push_back(TrackAction(val["Scale"][i]));
        if (i == 0)
            scale[i].from = size;
        else
            scale[i].from = scale[i - 1].to;
    }
}

double Track::GetValue(const std::vector<TrackAction> &actions, double time, double init) {
    double res = init;

    if (actions.size() > 0)
        if (time > actions.back().end)
            res = actions.back().to;

    for (uint i = 0; i < actions.size(); ++i) {
        TrackAction action = actions[i];
        if (action.start <= time && time <= action.end) {
            res = EASE_FUNC[action.ease](action.start, action.end, action.from, action.to, time);
            return res;
        }
        if (i + 1 == actions.size())
            continue;

        TrackAction next_action = actions[i + 1];
        if (action.end <= time && time <= next_action.start)
            return action.to; 
    }
    return res;
}

int Track::GetPosition(double time) {
    double ratio = GetValue(move, time, x);
    double pos = ratio * (SCREEN_WIDTH - 2 * TRACK_BASIC_WIDTH) + TRACK_BASIC_WIDTH;
    return int(round(pos));
}

int Track::GetSize(double time) {
    double ratio = GetValue(scale, time, size);
    return int(round(ratio * TRACK_BASIC_WIDTH));
}

void Track::Draw(double time, SDL_Renderer * Renderer) {
    if (time < start || end < time)
        return;
    int pos = GetPosition(time);
    int size = GetSize(time);
    SDL_Rect fillRect = {pos - size / 2, 0, size, SCREEN_HEIGHT};
    SDL_SetRenderDrawColor(Renderer, 128, 0, 0, 96);
    SDL_RenderFillRect(Renderer, &fillRect);
}


// Class TrackAction definition

TrackAction::TrackAction() {}

TrackAction::TrackAction(const rapidjson::Value &val) {
    read_action(val);
}

void TrackAction::read_action(const rapidjson::Value &val) {
    to = val["To"].GetDouble();
    start = val["Start"].GetDouble();
    end = val["End"].GetDouble();
    std::string s = val["Ease"].GetString();
    ease = -1;
    if (s == "easelinear")
        ease = EASE_LINEAR;
    if (s == "easeoutcirc")
        ease = EASE_OUT_CIRC;
    if (s == "easeincirc")
        ease = EASE_IN_CIRC;
    if (s == "easeinquad")
        ease = EASE_IN_QUAD;
    if (s == "easeoutquad")
        ease = EASE_OUT_QUAD;
    if (s == "easeinoutquad")
        ease = EASE_INOUT_QUAD;
    if (ease == -1) {
        printf("New easing function: %s\n", val["Ease"].GetString());
        ease = EASE_LINEAR;
    }
}
