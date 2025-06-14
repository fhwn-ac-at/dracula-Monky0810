#include "die.h"

#include <stdlib.h>
#include <string.h>

/*
 * die_create:
 *   Allocate and initialize a Die with the given number of sides.
 *   - sides: the number of faces on the die.
 *   - probs: optional array of length `sides` containing weights for each face.
 *     If non-NULL, a copy is made and converted in-place to prefix sums for
 *     weighted random sampling. The prefix sums array has the property that
 *     probs[i] = sum of original weights up to face i.
 *   - If probs is NULL, the die is fair and rolls will be uniform.
 *   Returns a pointer to the newly allocated Die, or NULL on allocation failure.
 */
Die *die_create(size_t sides, const double *probs) {
    Die *d = malloc(sizeof(Die));
    if (!d) return NULL;
    d->sides = sides;
    if (probs) {
        d->probs = malloc(sides * sizeof(double));
        if (!d->probs) {
            free(d);
            return NULL;
        }
        memcpy(d->probs, probs, sides * sizeof(double));
        /* build prefix sums in-place for weighted sampling */
        for (size_t i = 1; i < sides; ++i)
            d->probs[i] += d->probs[i-1];
    } else {
        d->probs = NULL;
    }
    return d;
}

/*
 * die_roll:
 *   Roll the die and return a face value in the range [1 .. sides].
 *   - If no probability array is set (d->probs == NULL), returns a uniform
 *     random integer between 1 and sides inclusive.
 *   - If a prefix-sum array is present, generates a uniform random double in
 *     [0, total_weight), then finds the first index i such that r <= probs[i]
 *     and returns i+1 as the face.
 */
size_t die_roll(Die *d) {
    if (!d->probs) {
        /* fair die */
        return (rand() % d->sides) + 1;
    }
    /* weighted die */
    double r = ((double)rand() / RAND_MAX) * d->probs[d->sides - 1];
    for (size_t i = 0; i < d->sides; ++i) {
        if (r <= d->probs[i])
            return i + 1;
    }
    /* fallback: should not happen, but return highest face */
    return d->sides;
}

/*
 * die_free:
 *   Free a Die object and its associated resources.
 *   - Frees the internal probability array (if any) and the Die struct itself.
 *   - Safe to call with a NULL pointer.
 */
void die_free(Die *d) {
    if (!d) return;
    free(d->probs);
    free(d);
}
