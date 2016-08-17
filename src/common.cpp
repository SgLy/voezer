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

void test_ease(double (*ease)(double, double, double,  double, double)) {
    FILE * data = fopen("in.dat", "w");
    double start = 2, end = 5;
    double from = 4, to = 2;
    for (double x = start; x <= end; x += 0.01)
        fprintf(data, "%.10lf\t%.10lf\n", x, ease(start, end, from, to, x));
    fclose(data);
}
