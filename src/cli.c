#include "cli.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     // for getopt
#include <time.h>       // for time()

void parse_cli(int argc, char **argv, CLIOptions *opts) {
    /* defaults */
    opts->config_file  = NULL;
    opts->die_sides    = 6;
    opts->die_probs    = NULL;
    opts->iterations   = 10000;
    opts->max_steps    = 10000;
    opts->win_by_exceed= 1;
    opts->seed         = (unsigned)time(NULL);

    int c;
    while ((c = getopt(argc, argv, "c:d:p:i:s:exS:")) != -1) {
        switch (c) {
        case 'c':
            opts->config_file = strdup(optarg);
            break;
        case 'd':
            opts->die_sides = (size_t)atoi(optarg);
            break;
        case 'p': {
            /* parse comma-separated probabilities */
            size_t n = opts->die_sides;
            opts->die_probs = malloc(n * sizeof(double));
            char *tmp = strdup(optarg), *tok, *save;
            size_t i = 0;
            for (tok = strtok_r(tmp, ",", &save);
                 tok && i < n;
                 tok = strtok_r(NULL, ",", &save), ++i) {
                opts->die_probs[i] = atof(tok);
            }
            free(tmp);
            if (i != n) {
                fprintf(stderr,
                    "Error: expected %zu probabilities, got %zu\n",
                    n, i);
                exit(1);
            }
            break;
        }
        case 'i':
            opts->iterations = (size_t)atoi(optarg);
            break;
        case 's':
            opts->max_steps = (size_t)atoi(optarg);
            break;
        case 'e':
            opts->win_by_exceed = 1;
            break;
        case 'x':
            opts->win_by_exceed = 0;
            break;
        case 'S':
            opts->seed = (unsigned)atoi(optarg);
            break;
        case '?':
        default:
            fprintf(stderr,
                "Usage: %s -c board.txt [-d sides] [-p p1,p2,...]"
                " [-i iters] [-s steps] [-e | -x] [-S seed]\n",
                argv[0]);
            exit(1);
        }
    }

    if (!opts->config_file) {
        fprintf(stderr, "Error: board config file required (-c)\n");
        exit(1);
    }
}
