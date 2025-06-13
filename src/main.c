#include "cli.h"
#include "board.h"
#include "die.h"
#include "sim.h"
#include "stats.h"

#include <stdlib.h>
#include <stdio.h>

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
