#ifndef DIE_H
#define DIE_H

#include <stddef.h>

typedef struct {
    size_t sides;
    double *probs;    // NULL for uniform, otherwise length == sides
} Die;

/** Create a uniform or weighted die. */
Die *die_create(size_t sides, const double *probs /*NULL for uniform*/);

/** Roll: returns 1..sides */
size_t die_roll(Die *d);

/** Free */
void die_free(Die *d);

#endif /* DIE_H */
