#include "cli.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
 * iso_strdup:
 *   Duplicate a C string using ISO C memory allocation.
 *   - Allocates len = strlen(s) + 1 bytes.
 *   - Copies the contents of s (including the terminating '\0') into the new buffer.
 *   - Returns a pointer to the newly allocated string, or NULL on allocation failure.
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
 * parse_cli:
 *   Parse command-line arguments into a CLIOptions struct.
 *   Supported options:
 *     -c <file>       Path to the board configuration file (required).
 *     -d <sides>      Number of die sides (default: 6).
 *     -p <p1,p2,…>    Comma-separated probabilities for each die face (must match die_sides).
 *     -i <iters>      Number of simulations to run (default: 10000).
 *     -s <steps>      Maximum steps allowed per game (default: 10000).
 *     -e              Enable “win by exceeding” the last square (default: on).
 *     -x              Require exact roll to land on the last square (disables win-by-exceed).
 *     -S <seed>       Seed for the random number generator (default: time(NULL)).
 *
 *   Behavior:
 *     - Sets all fields of opts to their defaults.
 *     - Iterates through argv[], handling each supported flag.
 *     - On invalid usage or missing required options, prints usage or error and exits.
 */
void parse_cli(int argc, char **argv, CLIOptions *opts) {
    /* Set defaults */
    opts->config_file   = NULL;
    opts->die_sides     = 6;
    opts->die_probs     = NULL;
    opts->iterations    = 10000;
    opts->max_steps     = 10000;
    opts->win_by_exceed = 1;
    opts->seed          = (unsigned)time(NULL);

    /* Parse each argument */
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-c") == 0 && i+1 < argc) {
            opts->config_file = iso_strdup(argv[++i]);
        }
        else if (strcmp(argv[i], "-d") == 0 && i+1 < argc) {
            opts->die_sides = (size_t)atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "-p") == 0 && i+1 < argc) {
            /* Parse comma-separated die probabilities */
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

    /* Ensure required config file was provided */
    if (!opts->config_file) {
        fprintf(stderr, "Error: board config file required (-c)\n");
        exit(1);
    }
}
