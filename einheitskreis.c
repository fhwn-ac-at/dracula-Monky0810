#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

typedef struct {
    double x;
    double y;
} Point;

int is_point_in_unit_circle(Point p) {
    return (p.x * p.x + p.y * p.y) <= 1.0;
}

Point random_point() {
    Point p;
    p.x = ((double)rand() / RAND_MAX) * 2.0 - 1.0; 
    p.y = ((double)rand() / RAND_MAX) * 2.0 - 1.0; 
    return p;
}


Point random_point_in_unit_circle() {
    Point p;
    do {
        p = random_point();
    } while (!is_point_in_unit_circle(p));
    return p;
}

int main() {
    srand((unsigned int)time(NULL));
    Point p = random_point_in_unit_circle();
    printf("Gefundener Punkt im Einheitskreis: (%.4f, %.4f)\n", p.x, p.y);

    return 0;
}
