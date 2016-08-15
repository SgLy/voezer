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
    for (auto t: track) {
        t.Draw(time, Renderer);
    }
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

double Track::GetPosition(double time) {
    double res = x;

    if (move.size() > 0) {
        TrackAction lastmove = move[move.size() - 1];
        if (time > lastmove.end)
            res = lastmove.to;
    }

    for (int i = 0; i < move.size(); ++i) {
        TrackAction action = move[i];
        if (action.start <= time && time <= action.end) {
            //if (action.ease == EASE_LINEAR) {
                res = action.to - action.from;
                res /= action.end - action.start;
                res *= time - action.start;
                res += action.from;
            //}
            return res;
        }
        if (i + 1 == move.size())
            continue;

        TrackAction next_action = move[i + 1];
        if (action.end <= time && time <= next_action.start)
            return action.to; 
    }
    return res;
}

double Track::GetSize(double time) {
    double res = size;

    if (scale.size() > 0) {
        TrackAction lastscale = scale[scale.size() - 1];
        if (time > lastscale.end)
            res = lastscale.to;
    }

    for (int i = 0; i < scale.size(); ++i) {
        TrackAction action = scale[i];
        if (action.start <= time && time <= action.end) {
            //if (action.ease == EASE_LINEAR) {
                res = action.to - action.from;
                res /= action.end - action.start;
                res *= time - action.start;
                res += action.from;
            //}
            return res;
        }
        if (i + 1 == scale.size())
            continue;

        TrackAction next_action = scale[i + 1];
        if (action.end <= time && time <= next_action.start)
            return action.to; 
    }
    return res;
}

void Track::Draw(double time, SDL_Renderer * Renderer) {
    if (time < start || end < time)
        return;
    double pos = GetPosition(time);
    pos = pos * (SCREEN_WIDTH - 160) + 80;
    pos = round(pos);
    int size = int(round(GetSize(time) * 80));
    SDL_Rect fillRect = {int(pos) - size / 2, 0, size, SCREEN_HEIGHT};
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
    if (s == "easelinear")
        ease = EASE_LINEAR;
    if (s == "easeoutcirc")
        ease = EASE_OUT_CIRC;
}
