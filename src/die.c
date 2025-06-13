#include "die.h"

#include <stdlib.h>
#include <string.h>

Die *die_create(size_t sides, const double *probs) {
    Die *d = malloc(sizeof(Die));
    d->sides = sides;
    if (probs) {
        d->probs = malloc(sides * sizeof(double));
        memcpy(d->probs, probs, sides * sizeof(double));
        /* build prefix sums in-place */
        for (size_t i = 1; i < sides; ++i)
            d->probs[i] += d->probs[i-1];
    } else {
        d->probs = NULL;
    }
    return d;
}

size_t die_roll(Die *d) {
    if (!d->probs) {
        return (rand() % d->sides) + 1;
    }
    double r = ((double)rand()/RAND_MAX) * d->probs[d->sides-1];
    for (size_t i = 0; i < d->sides; ++i)
        if (r <= d->probs[i])
            return i+1;
    return d->sides;
}

void die_free(Die *d) {
    if (!d) return;
    free(d->probs);
    free(d);
}
