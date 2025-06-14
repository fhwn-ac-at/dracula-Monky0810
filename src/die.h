#ifndef DIE_H
#define DIE_H

#include <stddef.h>

/*
 * Die:
 *   Represents a die with a specified number of faces.
 *   - sides: number of faces on the die.
 *   - probs: NULL for a fair (uniform) die; otherwise an array of length `sides`
 *            containing prefix sums for weighted random sampling.
 */
typedef struct {
    size_t sides;
    double *probs;    /* NULL for uniform, otherwise length == sides */
} Die;

/*
 * die_create:
 *   Allocate and initialize a Die object.
 *   - sides: number of faces on the die.
 *   - probs: optional array of length `sides` with weights for each face.
 *     If NULL, the die will be fair; otherwise, a copy is made and converted
 *     in-place to prefix sums for weighted sampling.
 *   Returns a pointer to the newly allocated Die, or NULL on allocation failure.
 */
Die *die_create(size_t sides, const double *probs /* NULL for uniform */);

/*
 * die_roll:
 *   Roll the die and return a face value in the range [1 .. sides].
 *   - For a fair die (probs == NULL), returns a uniform random integer.
 *   - For a weighted die, generates a random double in [0, total_weight)
 *     then returns the first face whose prefix sum exceeds that value.
 */
size_t die_roll(Die *d);

/*
 * die_free:
 *   Free all memory associated with a Die object.
 *   Safe to call with a NULL pointer.
 */
void die_free(Die *d);

#endif /* DIE_H */
