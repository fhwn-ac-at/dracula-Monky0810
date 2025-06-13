#ifndef BOARD_H
#define BOARD_H

#include <stddef.h>

typedef struct {
    size_t start;  // where the snake/ladder begins
    size_t end;    // where it takes you
} Jump;

typedef struct {
    size_t N, M;        // dimensions
    size_t size;        // N*M
    size_t n_jumps;     // total snakes + ladders
    Jump *jumps;        // array of all snakes & ladders
    size_t *mapping;    // mapping[i] = destination square after applying jump
    // Graph representation: adjacency lists
    size_t **adj;       // adj[i] = array of neighbors
    size_t *adj_cnt;    // adj_cnt[i] = number of neighbors
} Board;

/**
 * Load a board from a simple text file:
 *   First line: N M
 *   Subsequent lines: either "S start end" or "L start end"
 * Returns NULL on error.
 */
Board *board_load(const char *filename);

/** Build adjacency lists given a die with D sides and exact/exceed rule. */
void board_build_graph(Board *b, size_t die_sides, int win_by_exceed);

/** Free everything. */
void board_free(Board *b);

#endif /* BOARD_H */
