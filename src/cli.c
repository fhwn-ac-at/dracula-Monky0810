#include "cli.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
 * A simple replacement for strdup() using only ISO-C
 */
static char *iso_strdup(const char *s) {
    size_t len = strlen(s) + 1;
    char *dup = malloc(len);
    if (dup) {
        memcpy(dup, s, len);
    }
    return dup;
}

/*
 * Parse command-line options into opts.
 * Supports:
 *   -c <file>       board config file (required)
 *   -d <sides>      die sides (default 6)
 *   -p <p1,p2,…>    comma-separated die probs (must match sides)
 *   -i <iters>      number of simulations (default 10000)
 *   -s <steps>      max steps per game (default 10000)
 *   -e              win by exceeding last square (default on)
 *   -x              win only by exact roll
 *   -S <seed>       RNG seed (default = time(NULL))
 *
 * Exits with error message if something is invalid or missing.
 */
void parse_cli(int argc, char **argv, CLIOptions *opts) {
    /* set defaults */
    opts->config_file   = NULL;
    opts->die_sides     = 6;
    opts->die_probs     = NULL;
    opts->iterations    = 10000;
    opts->max_steps     = 10000;
    opts->win_by_exceed = 1;
    opts->seed          = (unsigned)time(NULL);

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-c") == 0 && i+1 < argc) {
            opts->config_file = iso_strdup(argv[++i]);
        }
        else if (strcmp(argv[i], "-d") == 0 && i+1 < argc) {
            opts->die_sides = (size_t)atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "-p") == 0 && i+1 < argc) {
            /* parse comma-separated probabilities */
            char *list = iso_strdup(argv[++i]);
            size_t n = opts->die_sides;
            opts->die_probs = malloc(n * sizeof(double));
            if (!opts->die_probs) {
                fprintf(stderr, "Error: out of memory\n");
                exit(1);
            }
            char *tok = strtok(list, ",");
            size_t j = 0;
            while (tok && j < n) {
                opts->die_probs[j++] = atof(tok);
                tok = strtok(NULL, ",");
            }
            free(list);
            if (j != n) {
                fprintf(stderr,
                        "Error: expected %zu die probabilities, got %zu\n",
                        n, j);
                exit(1);
            }
        }
        else if (strcmp(argv[i], "-i") == 0 && i+1 < argc) {
            opts->iterations = (size_t)atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "-s") == 0 && i+1 < argc) {
            opts->max_steps = (size_t)atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "-e") == 0) {
            opts->win_by_exceed = 1;
        }
        else if (strcmp(argv[i], "-x") == 0) {
            opts->win_by_exceed = 0;
        }
        else if (strcmp(argv[i], "-S") == 0 && i+1 < argc) {
            opts->seed = (unsigned)atoi(argv[++i]);
        }
        else {
            fprintf(stderr,
                "Usage: %s -c board.txt [-d sides] [-p p1,p2,...] "
                "[-i iters] [-s steps] [-e|-x] [-S seed]\n",
                argv[0]);
            exit(1);
        }
    }

    if (!opts->config_file) {
        fprintf(stderr, "Error: board config file required (-c)\n");
        exit(1);
    }
}
