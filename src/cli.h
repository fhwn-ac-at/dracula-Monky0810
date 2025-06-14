#ifndef CLI_H
#define CLI_H

#include <stddef.h>

/*
 * CLIOptions:
 *   Holds configuration options parsed from the command line.
 *   - N, M:          (unused here; present if needed for future extensions)
 *   - config_file:   Path to the board configuration file (required).
 *   - die_sides:     Number of faces on the die (default: 6).
 *   - die_probs:     Optional array of probabilities for each die face
 *                    (length = die_sides). If NULL, the die is fair.
 *   - iterations:    Number of game simulations to run (default: 10000).
 *   - max_steps:     Maximum rolls per game before aborting (default: 10000).
 *   - win_by_exceed: Non-zero to allow winning by exceeding the last square;
 *                    zero to require an exact roll (default: on).
 *   - seed:          Seed for the random number generator (default: time(NULL)).
 */
typedef struct {
    size_t N, M;
    char   *config_file;
    size_t  die_sides;
    double *die_probs;
    size_t  iterations;
    size_t  max_steps;
    int     win_by_exceed;
    unsigned seed;
} CLIOptions;

/*
 * parse_cli:
 *   Parse command-line arguments and populate a CLIOptions struct.
 *   Supported flags:
 *     -c <file>       (required) board configuration file path
 *     -d <sides>      die sides
 *     -p <p1,p2,…>    comma-separated die face probabilities
 *     -i <iters>      number of simulations
 *     -s <steps>      max rolls per game
 *     -e              enable win-by-exceed
 *     -x              require exact roll to win
 *     -S <seed>       RNG seed
 *   On invalid or missing required options, prints an error or usage message
 *   and exits the program.
 */
void parse_cli(int argc, char **argv, CLIOptions *opts);

#endif /* CLI_H */
