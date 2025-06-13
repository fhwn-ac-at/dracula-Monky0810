#include "stats.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

Stats *stats_compute(const Board *b,
                     const Simulation *sim)
{
    Stats *st = malloc(sizeof(Stats));
    size_t wins = 0, total_rolls = 0;
    size_t shortest = SIZE_MAX, short_idx = SIZE_MAX;

    /* find avg & shortest */
    for (size_t i = 0; i < sim->iterations; ++i) {
        size_t r = sim->results[i].rolls_to_win;
        if (r > 0) {
            wins++;
            total_rolls += r;
            if (r < shortest) {
                shortest = r;
                short_idx = i;
            }
        }
    }
    st->avg_rolls = wins
                   ? (double)total_rolls / wins
                   : 0.0;
    st->shortest_rolls = (short_idx<sim->iterations)
                        ? shortest : 0;
    if (st->shortest_rolls) {
        st->shortest_sequence =
           malloc(shortest * sizeof(size_t));
        memcpy(st->shortest_sequence,
               sim->results[short_idx].roll_sequence,
               shortest * sizeof(size_t));
    } else {
        st->shortest_sequence = NULL;
    }

    /* jump frequencies */
    st->jump_counts = calloc(b->n_jumps, sizeof(size_t));
    st->total_jumps = 0;

    for (size_t i = 0; i < sim->iterations; ++i) {
        size_t pos = 0;
        size_t *seq = sim->results[i].roll_sequence;
        for (size_t j = 0; j < sim->results[i].rolls_to_win; ++j) {
            size_t face = seq[j];
            size_t raw  = pos + face;
            size_t dest = (raw >= b->size
                           ? b->size-1 
                           : raw);
            dest = b->mapping[dest];

            /* see if this was a jump */
            for (size_t k = 0; k < b->n_jumps; ++k) {
                if (b->jumps[k].start == raw &&
                    b->jumps[k].end   == dest) {
                    st->jump_counts[k]++;
                    st->total_jumps++;
                }
            }
            pos = dest;
        }
    }

    return st;
}

void stats_print(const Stats *st, const Board *b) {
    printf("Average rolls to win: %.2f\n", st->avg_rolls);

    printf("Shortest game (%zu rolls):",
           st->shortest_rolls);
    for (size_t i = 0; i < st->shortest_rolls; ++i)
        printf(" %zu", st->shortest_sequence[i]);
    printf("\n\nJump traversal counts:\n");

    for (size_t k = 0; k < b->n_jumps; ++k) {
        double pct = st->total_jumps
                   ? 100.0 * st->jump_counts[k]
                     / (double)st->total_jumps
                   : 0.0;
        printf("  %3zu→%-3zu : %6zu times  (%5.2f%%)\n",
               b->jumps[k].start,
               b->jumps[k].end,
               st->jump_counts[k],
               pct);
    }
}

void stats_free(Stats *st) {
    if (!st) return;
    free(st->shortest_sequence);
    free(st->jump_counts);
    free(st);
}
