#ifndef STATS_H
#define STATS_H

#include "board.h"
#include "sim.h"

typedef struct {
    double avg_rolls;
    size_t shortest_rolls;
    size_t *shortest_sequence;    // length == shortest_rolls
    size_t *jump_counts;          // length == b->n_jumps
    size_t total_jumps;
} Stats;

/** Compute stats from simulation. */
Stats *stats_compute(const Board *b, const Simulation *sim);

/** Print human-readable stats. */
void stats_print(const Stats *st, const Board *b);

/** Free */
void stats_free(Stats *s);

#endif /* STATS_H */
