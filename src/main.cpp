#include <stdio.h>
#include <unistd.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <rapidjson/document.h>
#include <algorithm>
#include <vector>

const int MAX_BUF_LEN = 1024;
const int MAX_FILE_LEN = 131072;
const int SCREEN_WIDTH = 1024, SCREEN_HEIGHT = 768;
char buf[MAX_BUF_LEN];
char str[MAX_FILE_LEN];
std::vector<double> tim;

Mix_Music * bgm;
Mix_Chunk * se_beat;
SDL_Window * MainWindow = NULL;
SDL_Surface* ScreenSurface = NULL;
SDL_Surface* Background = NULL;

bool init() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return false;
    }

    // Initialize SDL_mixer
    if (Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, 512) < 0) {
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
        return false;
    }

    // Initialize SDL_image
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image cound not initialize! SDL_image Error: %s\n", IMG_GetError());
       return false;
    }

	// Create window
    MainWindow = SDL_CreateWindow("VOEZER - citanLu.Special", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (MainWindow == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    } else {
        // Get window surface
        ScreenSurface = SDL_GetWindowSurface(MainWindow);
    }
    return true; 
}

bool load_sound() {
    se_beat = Mix_LoadWAV("res/snd/beat.wav");
    if (se_beat == NULL) {
        printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
        return false;
    }
    bgm = Mix_LoadMUS("res/song/citanLu/song_full.ogg");
    if (bgm == NULL) {
        printf("Failed to load gameplay music! SDL_mixer Error: %s\n", Mix_GetError());
        return false;
    }
    return true;
}

bool load_texture() {
    Background = IMG_Load("res/song/citanLu/image_blur.png");
    if (Background == NULL) {
        printf("Load image error! SDL Error: %s\n", SDL_GetError());
        return false;
    }
    return true;
}

bool parsejson() {
    FILE * input_file = fopen("res/song/citanLu/note.extra.txt", "r");
    if (input_file == NULL)
        return false;
    while (fgets(buf, MAX_BUF_LEN, input_file) != NULL)
        strcat(str, buf);
    fclose(input_file);
    rapidjson::Document doc;
    doc.Parse(str);
    for (rapidjson::SizeType i = 0; i < doc.Size(); ++i)
        tim.push_back(doc[i]["Time"].GetDouble());
    std::sort(tim.begin(), tim.end());
    return true;
}

void main_loop() {
    Uint32 startTime = SDL_GetTicks();
    Mix_PlayMusic(bgm, -1);
    int offset = 0;
    printf("%d\n", startTime);
    SDL_Surface * optimizedSurface = SDL_ConvertSurface(Background, ScreenSurface->format, NULL);
    SDL_Rect stretchRect;
    stretchRect.x = 0;
    stretchRect.y = 0;
    stretchRect.w = SCREEN_WIDTH;
    stretchRect.h = SCREEN_HEIGHT;
    SDL_BlitScaled(optimizedSurface, NULL, ScreenSurface, &stretchRect);
    //SDL_BlitSurface(Background, NULL, ScreenSurface, NULL);
    SDL_UpdateWindowSurface(MainWindow);

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
        loop_cnt++;
            while (p < tim.size() && offset + SDL_GetTicks() - startTime > tim[p] * 1000) {
                Mix_PlayChannel(-1, se_beat, 0);
                printf("Note %3d: %03.4lf\n", p, tim[p]);
                p++;
            }
    }
    printf("%d\n", loop_cnt);
    printf("%.10lf\n", (double)loop_cnt / (SDL_GetTicks() - startTime));
}

void finalize() {
    Mix_FreeMusic(bgm);
    bgm = NULL;
    Mix_FreeChunk(se_beat);
    se_beat = NULL;
    SDL_FreeSurface(Background);
    Background = NULL;
    SDL_FreeSurface(ScreenSurface);
    ScreenSurface = NULL;
    SDL_Quit();
}

int main(int argc, char * argv[])
{
    if (!init())
        return 1;
    if (!parsejson())
        return 1;
    if (!load_sound())
        return 1;
    if (!load_texture())
        return 1;
    main_loop();
    finalize();
    return 0;
}
