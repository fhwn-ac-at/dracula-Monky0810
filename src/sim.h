#ifndef SIM_H
#define SIM_H

#include "board.h"
#include "die.h"

typedef struct {
    size_t rolls_to_win;
    size_t *roll_sequence;  // length == rolls_to_win
} GameResult;

typedef struct {
    size_t iterations;
    size_t max_steps;
    GameResult *results;    // array of length iterations
} Simulation;

/** Run one single game; returns rolls used (or 0 if aborted). */
size_t simulate_one(const Board *b, const Die *d,
                    size_t *out_sequence, size_t max_steps);

/** Run many games. */
Simulation *simulate_many(const Board *b, const Die *d,
                          size_t iterations, size_t max_steps);

/** Free */
void sim_free(Simulation *s);

#endif /* SIM_H */
