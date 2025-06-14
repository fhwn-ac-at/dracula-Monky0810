#ifndef SIM_H
#define SIM_H

#include "board.h"
#include "die.h"

/*
 * GameResult:
 *   Stores the outcome of a single game simulation.
 *   - rolls_to_win: number of die rolls taken to reach the final square,
 *                   or 0 if the game was aborted (exceeded max_steps).
 *   - roll_sequence: array of face values rolled, length == rolls_to_win
 */
typedef struct {
    size_t rolls_to_win;
    size_t *roll_sequence;  /* length == rolls_to_win */
} GameResult;

/*
 * Simulation:
 *   Aggregates the results of multiple game simulations.
 *   - iterations: number of games simulated.
 *   - max_steps:  maximum rolls allowed per game.
 *   - results:    array of GameResult of length iterations.
 */
typedef struct {
    size_t iterations;
    size_t max_steps;
    GameResult *results;    /* array of length iterations */
} Simulation;

/*
 * simulate_one:
 *   Play a single game on board b using die d.
 *   - out_sequence: caller-provided buffer of length max_steps to record each roll.
 *   - max_steps:    maximum number of rolls before aborting.
 *   Returns the number of rolls actually used to win (1..max_steps),
 *   or 0 if the game did not finish within max_steps.
 */
size_t simulate_one(const Board *b, const Die *d,
                    size_t *out_sequence, size_t max_steps);

/*
 * simulate_many:
 *   Run multiple independent game simulations.
 *   - iterations: number of games to simulate.
 *   - max_steps:  maximum rolls allowed per game.
 *   Allocates and returns a Simulation struct containing all GameResults.
 *   Caller must free the returned Simulation via sim_free().
 */
Simulation *simulate_many(const Board *b, const Die *d,
                          size_t iterations, size_t max_steps);

/*
 * sim_free:
 *   Free all memory associated with a Simulation.
 *   - Frees each GameResult.roll_sequence, the results array,
 *     and the Simulation struct itself.
 *   Safe to call with a NULL pointer.
 */
void sim_free(Simulation *s);

#endif /* SIM_H */
