#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED 1

#include <math.h>

const int SCREEN_WIDTH = 800, SCREEN_HEIGHT = 600;

typedef int EaseType;
const int EASE_LINEAR = 0,
          EASE_OUT_CIRC = 1,
          EASE_IN_CIRC = 2;

const int MAX_BUF_LEN = 1024;
const int MAX_FILE_LEN = 131072;
extern char buf[MAX_BUF_LEN];
extern char str[MAX_FILE_LEN];

double sqr(double x);
double linear(double start, double end, double from, double to, double x);
double incirc(double start, double end, double from, double to, double x);
double outcirc(double start, double end, double from, double to, double x);

#endif
