#include "common.h"

TTF_Font * font;

char buf[MAX_BUF_LEN];
char str[MAX_FILE_LEN];

std::string EASE_STRING[] = {
    "easelinear",
    "easeoutcirc",
    "easeincirc",
    "easeinquad",
    "easeoutquad",
    "easeinoutquad",
    "easeinback",
    "easeoutback",
    "easeinoutback",
    "easeoutinback",
    "easeinexpo",
    "easeoutexpo"
};

double sqr(double x) {
    return x * x;
}

double cub(double x) {
    return x * x * x;
}

double linear(double start, double end, double from, double to, double x) {
    return (to - from) / (end - start) * (x - start) + from;
}

double incirc(double start, double end, double from, double to, double x) {
    x = (x - start) / (end - start);
    return sqrt(1 - sqr(x)) * (from - to) + to;
}

double outcirc(double start, double end, double from, double to, double x) {
    x = (end - x) / (end - start);
    return sqrt(1 - sqr(x)) * (to - from) + from;
}

double inquad(double start, double end, double from, double to, double x) {
    x = (x - start) / (end - start);
    return sqr(x) * (to - from) + from;
}

double outquad(double start, double end, double from, double to, double x) {
    x = (end - x) / (end - start);
    return sqr(x) * (from - to) + to;
}

double inoutquad(double start, double end, double from, double to, double x) {
    if (x < (end - start) / 2 + start)
        return inquad(start, (end - start) / 2 + start, from, (to - from) / 2 + from, x);
    else
        return outquad((end - start) / 2 + start, end, (to - from) / 2 + from, to, x);
}

double bezier(double p1, double p2, double x) {
    return 3 * x * sqr(1 - x) * p1 + 3 * sqr(x) * (1 - x) * p2 + cub(x);
}

double bezier_gety(double x1, double y1, double x2, double y2, double x) {
    double l, r, mid;
    for (l = 0, r = 1; fabs(r - l) > 1e-6;) {
        mid = (l + r) / 2;
        double t = bezier(x1, x2, mid);
        if (t > x)
            r = mid;
        else
            l = mid;
    }
   return bezier(y1, y2, (l + r) / 2);
}

double inback(double start, double end, double from, double to, double x) {
    x = (x - start) / (end - start);
    double s = 1.70158;
    double res = sqr(x) * ((1 + s) * x - s);
    //double res = bezier_gety(0.6, -0.28, 0.755, 0.045, x);
    return res * (to - from) + from;
}

double outback(double start, double end, double from, double to, double x) {
    x = (x - start) / (end - start) - 1;
    double s = 1.70158;
    double res = sqr(x) * ((s + 1) * x + s) + 1;
    //double res = bezier_gety(0.175, 0.885, 0.32, 1.275, x);
    return res * (to - from) + from;
}

double inoutback(double start, double end, double from, double to, double x) {
    double s = 1.70158 * 1.525;
    x = (x - start) / (end - start) * 2;
    double res;
    if (x < 1)
        res = sqr(x) * ((s + 1) * x - s);
    else {
        x -= 2;
        res = sqr(x) * ((s + 1) * x + s) + 2;
    }
    //double res = bezier_gety(0.68, -0.55, 0.265, 1.55, x);
    return res * (to - from) / 2 + from;
}

double outinback(double start, double end, double from, double to, double x) {
    double s = 1.70158 * 1.525;
    x = (x - start) / (end - start) * 2;
    double res;
    if (x < 1) {
        x -= 1;
        res = sqr(x) * ((s + 1) * x + s) + 1;
    } else {
        x -= 1;
        res = sqr(x) * ((s + 1) * x - s) + 1;
    }
    //double res = bezier_gety(0.68, -0.55, 0.265, 1.55, x);
    return res * (to - from) / 2 + from;
}

double inexpo(double start, double end, double from, double to, double x) {
    x = (x - start) / (end - start) - 1;
    double y = pow(2, 10 * x);
    return y * (to - from) + from;
}

double outexpo(double start, double end, double from, double to, double x) {
    x = (x - start) / (end - start);
    double y = pow(2, -10 * x);
    return y * (from - to) + to;
}

void test_ease(double (*ease)(double, double, double,  double, double)) {
    FILE * data = fopen("in.dat", "w");
    double start = 2, end = 5;
    double from = 2, to = 4;
    for (double x = start; x <= end; x += 0.01)
        fprintf(data, "%.10lf\t%.10lf\n", x, ease(start, end, from, to, x));
    fclose(data);
}

Color::Color() {}
Color::Color(double r, double g, double b): r(r), g(g), b(b) {}
Color operator+ (const Color &x, const Color &y) {
    return Color(x.r + y.r, x.g + y.g, x.b + y.b);
}
Color operator- (const Color &x, const Color &y) {
    return Color(x.r - y.r, x.g - y.g, x.b - y.b);
}
Color operator* (const Color &x, double y) {
    return Color(x.r * y, x.g * y, x.b * y);
}
Color operator/ (const Color &x, double y) {
    return Color(x.r / y, x.g / y, x.b / y);
}

void SetColor(SDL_Renderer * Renderer, Color c, uint opacity) {
    int R = int(c.r), G = int(c.g), B = int(c.b), A = int(opacity);
    SDL_SetRenderDrawColor(Renderer, R, G, B, A);
}

const Color COLOR_PRESET[] = {
    Color(255, 127, 127), // 0 Red
    Color(255, 255, 127), // 1 Light yellow
    Color(255, 255, 255), // 2 White
    Color(127, 255, 255), // 3 Cyan
    Color(127, 255, 127), // 4 Green
    Color(255, 127,   0), // 5 Orange
    Color(255, 127, 255), // 6 Pink
    Color( 96, 135, 255), // 7 Blue
    Color(127, 255, 127), // 8 Green
    Color(127, 127, 255), // 9 Purple
    Color(255, 255, 255)
};
