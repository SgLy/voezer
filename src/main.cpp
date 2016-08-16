#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <rapidjson/document.h>
#include <algorithm>
#include <vector>

#include "sound.h"
#include "track.h"
#include "common.h"

std::vector<double> tim;

Music bgm;
Sound se_beat;
Tracks tracks;
SDL_Window* MainWindow;
SDL_Renderer* Renderer;
SDL_Surface* ScreenSurface;
SDL_Texture* Background;
SDL_Texture* Note;

bool init() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return false;
    }

    if (!mixer_init())
        return false;

    // Initialize SDL_image
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image cound not initialize! SDL_image Error: %s\n", IMG_GetError());
       return false;
    }

    // Create window
    MainWindow = SDL_CreateWindow("VOEZER - Gamegame.Special", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (MainWindow == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    } else {
        // Get window surface
        ScreenSurface = SDL_GetWindowSurface(MainWindow);
        Renderer = SDL_CreateRenderer(MainWindow, -1, SDL_RENDERER_ACCELERATED);
        SDL_SetRenderDrawBlendMode(Renderer, SDL_BLENDMODE_BLEND);
    }
    return true; 
}

void load_sound() {
    se_beat.load("res/snd/beat.wav");
    bgm.load("res/song/Gamegame/song_full.ogg");
}

bool load_texture() {
    Background = NULL;
    SDL_Surface * loadedSurface = IMG_Load("res/song/Gamegame/image_blur.png");
    if (loadedSurface == NULL) {
        printf("Load image error! SDL_image Error: %s\n", IMG_GetError());
        return false;
    }
    Background = SDL_CreateTextureFromSurface(Renderer, loadedSurface);
    if (Background == NULL) {
        printf("Load image error! SDL_image Error: %s\n", IMG_GetError());
        return false;
    }
    SDL_FreeSurface(loadedSurface);
    return true;
}

bool parsejson() {
    tracks = Tracks("res/song/Gamegame/track.extra.txt", Renderer);

    FILE * input_file = fopen("res/song/Gamegame/note.extra.txt", "r");
    if (input_file == NULL)
        return false;
    while (fgets(buf, MAX_BUF_LEN, input_file) != NULL)
        strcat(str, buf);
    fclose(input_file);

    rapidjson::Document doc;
    doc.Parse(str);
    
    for (rapidjson::SizeType i = 0; i < doc.Size(); ++i)
        tim.push_back(doc[i]["Time"].GetDouble());

    memset(buf, 0, sizeof(buf));
    memset(str, 0, sizeof(str));

    std::sort(tim.begin(), tim.end());
    return true;
}

void DrawTrack(int x, int track_width) {
    SDL_Rect fillRect = {x - track_width / 2, 0, track_width, SCREEN_HEIGHT};
    SDL_SetRenderDrawColor(Renderer, 128, 0, 0, 96);
    SDL_RenderFillRect(Renderer, &fillRect);
}


void main_loop() {
    Uint32 startTime = SDL_GetTicks();
    bgm.play();
    printf("%d %d\n", SDL_GetTicks(), startTime);
    int offset = 0;

    uint p = 0;
    bool quit = false;
    SDL_Event e;
    int loop_cnt = 0;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            loop_cnt++;
            if (e.type == SDL_QUIT)
                quit = true;
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_q:
                    case SDLK_ESCAPE:
                        quit = true;
                        break;
                }
            }
        }

        double time = double(SDL_GetTicks() - startTime) / 1000;
        //printf("%.10lf\n", time);
        loop_cnt++;

        // Put Background
        SDL_RenderCopy(Renderer, Background, NULL, NULL);

        tracks.Draw(time);

        // Put shadow downside
        SDL_Rect fillRect = {0, SCREEN_HEIGHT / 6 * 5, SCREEN_WIDTH, SCREEN_HEIGHT / 6};
        SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 128);
        SDL_RenderFillRect(Renderer, &fillRect);

        // Render
        SDL_RenderPresent(Renderer);
        
        while (p < tim.size() && offset + SDL_GetTicks() - startTime > tim[p] * 1000) {
            se_beat.play();
            printf("Note %3d: %03.4lf\n", p, time);
            p++;
        }
    }
    printf("%d\n", loop_cnt);
    printf("Loop per millisecond: %.10lf\n", (double)loop_cnt / (SDL_GetTicks() - startTime));
}

void finalize() {
    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char * argv[])
{
    if (!init())
        return 1;
    if (!parsejson())
        return 1;
    load_sound();
    if (!load_texture())
        return 1;
    main_loop();
    finalize();
    return 0;
}
