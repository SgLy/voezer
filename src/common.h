#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED 1

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

extern TTF_Font * font;
const int SCREEN_WIDTH = 800, SCREEN_HEIGHT = 600;
const int TRACK_BASIC_WIDTH = SCREEN_WIDTH / 10;

typedef int EaseType;
const int EASE_TYPE_COUNT = 10;
const EaseType
    EASE_LINEAR = 0,
    EASE_OUT_CIRC = 1,
    EASE_IN_CIRC = 2,
    EASE_IN_QUAD = 3,
    EASE_OUT_QUAD = 4,
    EASE_INOUT_QUAD = 5,
    EASE_IN_BACK = 6,
    EASE_OUT_BACK = 7,
    EASE_INOUT_BACK = 8,
    EASE_OUTIN_BACK = 9;
extern std::string EASE_STRING[];

typedef double (*EaseFunction)(double start, double end, double from, double to, double x);

double linear(double start, double end, double from, double to, double x);
double incirc(double start, double end, double from, double to, double x);
double outcirc(double start, double end, double from, double to, double x);
double inquad(double start, double end, double from, double to, double x);
double outquad(double start, double end, double from, double to, double x);
double inoutquad(double start, double end, double from, double to, double x);
double inback(double start, double end, double from, double to, double x);
double outback(double start, double end, double from, double to, double x);
double inoutback(double start, double end, double from, double to, double x);
double outinback(double start, double end, double from, double to, double x);

const EaseFunction EASE_FUNC[] = {
    linear,
    outcirc,
    incirc,
    inquad,
    outquad,
    inoutquad,
    inback,
    outback,
    inoutback,
    outinback
};
void test_ease(EaseFunction ease);

typedef int NoteType;
const NoteType NOTE_CLICK = 0,
               NOTE_HOLD = 1,
               NOTE_SWIPE = 2,
               NOTE_SLIDE = 3;

const int MAX_BUF_LEN = 1024;
const int MAX_FILE_LEN = 131072;
extern char buf[MAX_BUF_LEN];
extern char str[MAX_FILE_LEN];

double sqr(double x);

class Color {
public:
    Color(); 
    Color(double r, double g, double b);
    friend Color operator+ (const Color &x, const Color &y);
    friend Color operator- (const Color &x, const Color &y);
    friend Color operator* (const Color &x, double y);
    friend Color operator/ (const Color &x, double y);
    friend void SetColor(SDL_Renderer * Renderer, Color c, uint capacity);

private:
    double r, g, b;
};

extern const Color COLOR_PRESET[];

#endif
