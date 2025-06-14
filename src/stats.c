#include "stats.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

/*
 * stats_compute:
 *   Analyze the results of multiple game simulations to produce summary statistics.
 *   - b: pointer to the Board containing jump definitions.
 *   - sim: pointer to the Simulation with per-game results.
 *   Computes:
 *     1) Average number of rolls across all winning games.
 *     2) Identifies the single game with the fewest rolls to win, and records its roll sequence.
 *     3) Counts how often each snake/ladder jump was traversed across all games.
 *   Allocates and returns a Stats struct containing:
 *     - avg_rolls: mean rolls per win.
 *     - shortest_rolls and shortest_sequence: data for the fastest win.
 *     - jump_counts: array of length b->n_jumps with traversal counts.
 *     - total_jumps: sum of all jump traversals (for percentage calculations).
 *   Caller must free the returned Stats with stats_free().
 */
Stats *stats_compute(const Board *b, const Simulation *sim)
{
    Stats *st = malloc(sizeof(Stats));
    if (!st) return NULL;

    size_t wins        = 0;
    size_t total_rolls = 0;
    /* Use (size_t)-1 as “infinite” sentinel for finding minimum */
    size_t shortest    = (size_t)-1;
    size_t short_idx   = (size_t)-1;

    /* 1) Compute average rolls and locate the shortest-winning game */
    for (size_t i = 0; i < sim->iterations; ++i) {
        size_t r = sim->results[i].rolls_to_win;
        if (r > 0) {
            wins++;
            total_rolls += r;
            if (r < shortest) {
                shortest  = r;
                short_idx = i;
            }
        }
    }

    st->avg_rolls = wins
        ? (double)total_rolls / wins
        : 0.0;

    /* Copy the roll sequence of the fastest win, if any */
    if (short_idx < sim->iterations) {
        st->shortest_rolls    = shortest;
        st->shortest_sequence = malloc(shortest * sizeof(size_t));
        memcpy(
            st->shortest_sequence,
            sim->results[short_idx].roll_sequence,
            shortest * sizeof(size_t)
        );
    } else {
        st->shortest_rolls    = 0;
        st->shortest_sequence = NULL;
    }

    /* 2) Count jump traversals across all games */
    st->jump_counts = calloc(b->n_jumps, sizeof(size_t));
    st->total_jumps = 0;

    for (size_t i = 0; i < sim->iterations; ++i) {
        size_t pos = 0;
        size_t len = sim->results[i].rolls_to_win;
        size_t *seq = sim->results[i].roll_sequence;

        for (size_t j = 0; j < len; ++j) {
            size_t face = seq[j];
            size_t raw  = pos + face;
            /* Apply win-by-exceed logic: clamp to last square if needed */
            size_t dest = (raw >= b->size)
                        ? b->size - 1
                        : raw;
            /* Then apply any snake/ladder jump mapping */
            dest = b->mapping[dest];

            /* Check each defined jump to see if it matches this move */
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

/*
 * stats_print:
 *   Display computed statistics to stdout.
 *   - st: pointer to Stats produced by stats_compute.
 *   - b: pointer to Board (for jump definitions and ordering).
 *   Prints:
 *     - Average rolls to win (to two decimal places).
 *     - The roll sequence of the shortest game.
 *     - For each jump, the count of traversals and the percentage of all jumps.
 */
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

/*
 * stats_free:
 *   Release all memory associated with a Stats struct.
 *   - Frees the shortest_sequence array, the jump_counts array, and the Stats struct itself.
 *   - Safe to call with a NULL pointer.
 */
void stats_free(Stats *st) {
    if (!st) return;
    free(st->shortest_sequence);
    free(st->jump_counts);
    free(st);
}
