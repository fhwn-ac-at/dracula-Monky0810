#ifndef CLI_H
#define CLI_H

#include <stddef.h>

typedef struct {
    size_t N, M;
    char *config_file;
    size_t die_sides;
    double *die_probs;
    size_t iterations;
    size_t max_steps;
    int win_by_exceed;
    unsigned seed;
} CLIOptions;

/**
 * Parse command-line args. Exits on failure.
 */
void parse_cli(int argc, char **argv, CLIOptions *opts);

#endif /* CLI_H */
