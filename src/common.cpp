#include "common.h"

char buf[MAX_BUF_LEN];
char str[MAX_FILE_LEN];

double sqr(double x) {
    return x * x;
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
