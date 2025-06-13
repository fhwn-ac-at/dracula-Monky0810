#include "sim.h"

#include <stdlib.h>
#include <string.h>

size_t simulate_one(const Board *b,
                    const Die *d,
                    size_t *out_sequence,
                    size_t max_steps)
{
    size_t pos = 0;
    for (size_t roll = 1; roll <= max_steps; ++roll) {
        size_t face = die_roll((Die*)d);
        out_sequence[roll-1] = face;

        size_t next = b->adj[pos][face-1];
        pos = next;

        if (pos == b->size - 1)
            return roll;
    }
    return 0;  /* aborted */
}

Simulation *simulate_many(const Board *b,
                          const Die *d,
                          size_t iterations,
                          size_t max_steps)
{
    Simulation *S = malloc(sizeof(Simulation));
    S->iterations = iterations;
    S->max_steps  = max_steps;
    S->results    = malloc(iterations * sizeof(GameResult));

    size_t *buffer = malloc(max_steps * sizeof(size_t));
    for (size_t i = 0; i < iterations; ++i) {
        size_t r = simulate_one(b, d, buffer, max_steps);
        S->results[i].rolls_to_win  = r;
        if (r > 0) {
            S->results[i].roll_sequence =
                malloc(r * sizeof(size_t));
            memcpy(S->results[i].roll_sequence,
                   buffer,
                   r * sizeof(size_t));
        } else {
            S->results[i].roll_sequence = NULL;
        }
    }
    free(buffer);
    return S;
}

void sim_free(Simulation *S) {
    if (!S) return;
    for (size_t i = 0; i < S->iterations; ++i)
        free(S->results[i].roll_sequence);
    free(S->results);
    free(S);
}
