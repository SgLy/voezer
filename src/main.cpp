#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <rapidjson/document.h>
#include <algorithm>
#include <vector>

#include "sound.h"
#include "track.h"
#include "note.h"
#include "common.h"

Music bgm;
Sound se_beat;
Tracks tracks;
Pattern pattern;
SDL_Window* MainWindow;
SDL_Renderer* Renderer;
SDL_Surface* ScreenSurface;
SDL_Texture* Background;
SDL_Texture* Note;

const int STR_LEN = 256;
typedef char cstr[STR_LEN];
cstr WindowTitle;
cstr SongBgmAddress, BackgroundImageAddress;
cstr TrackAddress, NoteAddress;

bool init(const char * SongName = "citanLu") {
    // Initialize string constants
    sprintf(WindowTitle, "VOEZER - %s.Special", SongName);
    sprintf(SongBgmAddress, "res/song/%s/song_full.ogg", SongName);
    sprintf(BackgroundImageAddress, "res/song/%s/image_blur.png", SongName);
    sprintf(TrackAddress, "res/song/%s/track.extra.txt", SongName);
    sprintf(NoteAddress, "res/song/%s/note.extra.txt", SongName);

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return false;
    }

    // Initialize SoLoud
    if (!bass_init())
        return false;

    // Initialize SDL_image
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image cound not initialize! SDL_image Error: %s\n", IMG_GetError());
        return false;
    }

    if (TTF_Init() == -1) {
        printf("%s\n", TTF_GetError());
        return false;
    }

    // Create window
    MainWindow = SDL_CreateWindow(WindowTitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (MainWindow == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    } else {
        // Get window surface
        ScreenSurface = SDL_GetWindowSurface(MainWindow);
        Renderer = SDL_CreateRenderer(MainWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        // Renderer = SDL_CreateRenderer(MainWindow, -1, SDL_RENDERER_ACCELERATED);
        SDL_SetRenderDrawBlendMode(Renderer, SDL_BLENDMODE_BLEND);
    }
    return true;
}

void load_sound() {
    se_beat.load("res/snd/beat.ogg");
    bgm.load(SongBgmAddress);
}

bool load_texture() {
    Background = NULL;
    SDL_Surface * loadedSurface = IMG_Load(BackgroundImageAddress);
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
    tracks = Tracks(TrackAddress, Renderer);
    pattern = Pattern(NoteAddress, Renderer, &se_beat, &tracks);
    return true;
}

void main_loop() {

    bool quit = false;
    SDL_Event e;
    int loop_cnt = 0;

    double speed = 1;
    bgm.setSpeed(speed);

    Uint32 startTime = SDL_GetTicks();
    bgm.play();

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

        if (bgm.stopped())
            quit = true;

        double time = speed * double(SDL_GetTicks() - startTime) / 1000;
        loop_cnt++;

        // Put Background
        SDL_RenderCopy(Renderer, Background, NULL, NULL);

        tracks.Draw(time);
        pattern.Draw(time);

        // Put shadow downside
        SDL_Rect fillRect = {0, SCREEN_HEIGHT / 6 * 5, SCREEN_WIDTH, SCREEN_HEIGHT / 6};
        SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 128);
        SDL_RenderFillRect(Renderer, &fillRect);

        // Render
        SDL_RenderPresent(Renderer);

        // Play sound
        pattern.Play(time);
    }
    printf("%d\n", loop_cnt);
    printf("Loop per second: %.10lf\n", (double)loop_cnt / (SDL_GetTicks() - startTime) * 1000);
}

void finalize() {
    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char * argv[])
{
    if (argc > 1) {
        if (strcmp(argv[1], "list") == 0) {
            system("ls res/song");
            return 0;
        }
        if (!init(argv[1]))
            return 1;
    } else {
        if (!init())
            return 1;
    }
    load_sound();
    if (!parsejson())
        return 1;
    if (!load_texture())
        return 1;
    //test_ease(outinback);
    main_loop();
    finalize();
    return 0;
}
