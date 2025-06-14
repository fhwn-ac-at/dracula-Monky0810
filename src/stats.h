#ifndef STATS_H
#define STATS_H

#include "board.h"
#include "sim.h"

/*
 * Stats:
 *   Holds summary statistics computed from a set of game simulations.
 *   - avg_rolls:        Average number of rolls taken across all winning games.
 *   - shortest_rolls:   Number of rolls in the quickest (fewest-roll) winning game.
 *   - shortest_sequence: Array of face values rolled in the quickest win
 *                        (length == shortest_rolls), or NULL if no wins.
 *   - jump_counts:      Array counting how many times each snake/ladder jump
 *                        was traversed (length == b->n_jumps).
 *   - total_jumps:      Total number of jump traversals across all games
 *                        (for percentage calculations).
 */
typedef struct {
    double avg_rolls;
    size_t shortest_rolls;
    size_t *shortest_sequence;    /* length == shortest_rolls */
    size_t *jump_counts;          /* length == b->n_jumps */
    size_t total_jumps;
} Stats;

/*
 * stats_compute:
 *   Analyze simulation results to produce summary statistics.
 *   - b:   Pointer to the Board (for jump definitions).
 *   - sim: Pointer to the Simulation containing game results.
 *   Returns a newly allocated Stats struct (or NULL on failure).
 *   Caller must free the returned Stats via stats_free().
 */
Stats *stats_compute(const Board *b, const Simulation *sim);

/*
 * stats_print:
 *   Print computed statistics to stdout in a human-readable format.
 *   - st: Pointer to Stats produced by stats_compute.
 *   - b:  Pointer to Board (for jump order and labels).
 */
void stats_print(const Stats *st, const Board *b);

/*
 * stats_free:
 *   Free all memory associated with a Stats object.
 *   - Frees the shortest_sequence array, the jump_counts array,
 *     and the Stats struct itself.
 *   Safe to call with a NULL pointer.
 */
void stats_free(Stats *s);

#endif /* STATS_H */
