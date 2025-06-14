#include "sim.h"

#include <stdlib.h>
#include <string.h>

/*
 * simulate_one:
 *   Play a single game on board b using die d.
 *   - b: pointer to an initialized Board with adjacency graph built.
 *   - d: pointer to a Die used for rolling.
 *   - out_sequence: caller-allocated array of length max_steps to record each die face rolled.
 *   - max_steps: maximum number of rolls allowed before aborting.
 *   Returns the number of rolls taken to reach the last square (b->size - 1),
 *   or 0 if the game did not finish within max_steps.
 *   The faces rolled on each step (1..sides) are written into out_sequence[0..rolls-1].
 */
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
    return 0;  /* did not reach the end within max_steps */
}

/*
 * simulate_many:
 *   Run multiple game simulations and collect results.
 *   - b: pointer to an initialized Board.
 *   - d: pointer to a Die.
 *   - iterations: number of independent games to simulate.
 *   - max_steps: maximum rolls per game.
 *   Allocates and returns a Simulation struct containing:
 *     results: an array of GameResult of length iterations,
 *              where each GameResult has rolls_to_win and a dynamically
 *              allocated roll_sequence (or NULL if the game aborted).
 *   Caller is responsible for freeing the returned Simulation via sim_free().
 */
Simulation *simulate_many(const Board *b,
                          const Die *d,
                          size_t iterations,
                          size_t max_steps)
{
    Simulation *S = malloc(sizeof(Simulation));
    S->iterations = iterations;
    S->max_steps  = max_steps;
    S->results    = malloc(iterations * sizeof(GameResult));

    /* Temporary buffer to record each game's rolls */
    size_t *buffer = malloc(max_steps * sizeof(size_t));
    for (size_t i = 0; i < iterations; ++i) {
        size_t r = simulate_one(b, d, buffer, max_steps);
        S->results[i].rolls_to_win = r;
        if (r > 0) {
            /* Allocate and copy only the rolls actually used */
            S->results[i].roll_sequence =
                malloc(r * sizeof(size_t));
            memcpy(S->results[i].roll_sequence,
                   buffer,
                   r * sizeof(size_t));
        } else {
            /* Game aborted */
            S->results[i].roll_sequence = NULL;
        }
    }
    free(buffer);
    return S;
}

/*
 * sim_free:
 *   Free all memory associated with a Simulation.
 *   - Frees each GameResult.roll_sequence array (if non-NULL),
 *     then frees the results array and the Simulation struct itself.
 *   - Safe to call with a NULL pointer.
 */
void sim_free(Simulation *S) {
    if (!S) return;
    for (size_t i = 0; i < S->iterations; ++i)
        free(S->results[i].roll_sequence);
    free(S->results);
    free(S);
}
