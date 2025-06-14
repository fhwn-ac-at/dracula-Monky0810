#include "cli.h"
#include "board.h"
#include "die.h"
#include "sim.h"
#include "stats.h"

#include <stdlib.h>
#include <stdio.h>

/*
 * main:
 *   Entry point for the board game simulation program.
 *   - Parses command-line options into a CLIOptions struct.
 *   - Seeds the RNG.
 *   - Loads the board configuration and builds its graph.
 *   - Creates a Die (with optional weighted faces).
 *   - Runs the specified number of simulations, each up to a maximum number of steps.
 *   - Computes statistics over all simulations and prints the results.
 *   - Cleans up all allocated resources before exiting.
 *   Returns 0 on success, or 1 if any step fails.
 */
int main(int argc, char **argv) {
    CLIOptions opts;
    parse_cli(argc, argv, &opts);

    /* seed rng */
    srand(opts.seed);

    /* load and build board */
    Board *b = board_load(opts.config_file);
    if (!b) {
        fprintf(stderr, "Error: failed to load board '%s'\n",
                opts.config_file);
        return 1;
    }
    board_build_graph(b, opts.die_sides, opts.win_by_exceed);

    /* create die */
    Die *d = die_create(opts.die_sides, opts.die_probs);
    if (!d) {
        fprintf(stderr, "Error: could not create die\n");
        board_free(b);
        return 1;
    }

    /* run simulation */
    Simulation *sim = simulate_many(b, d,
                                    opts.iterations,
                                    opts.max_steps);
    if (!sim) {
        fprintf(stderr, "Error: simulation failed\n");
        die_free(d);
        board_free(b);
        return 1;
    }

    /* compute & print stats */
    Stats *st = stats_compute(b, sim);
    stats_print(st, b);

    /* clean up */
    stats_free(st);
    sim_free(sim);
    die_free(d);
    board_free(b);
    free(opts.config_file);
    free(opts.die_probs);
    return 0;
}
