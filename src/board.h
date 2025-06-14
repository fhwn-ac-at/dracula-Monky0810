#ifndef BOARD_H
#define BOARD_H

#include <stddef.h>

/*
 * Jump:
 *   Represents a “snake” or “ladder” on the board.
 *   - start: index of the square where the jump begins.
 *   - end:   index of the square where the jump lands.
 */
typedef struct {
    size_t start;  /* where the snake/ladder begins */
    size_t end;    /* where it takes you */
} Jump;

/*
 * Board:
 *   Encapsulates the game board configuration and its graph representation.
 *   - N, M:     dimensions of the board (rows × columns).
 *   - size:     total number of squares (N * M).
 *   - n_jumps:  number of snakes + ladders.
 *   - jumps:    array of Jump structs defining each snake/ladder.
 *   - mapping:  for each square i, the destination after applying any jump.
 *   - adj:      adjacency lists: adj[i] is an array of neighbor square indices.
 *   - adj_cnt:  for each square i, the number of neighbors in adj[i].
 */
typedef struct {
    size_t N, M;        /* dimensions */
    size_t size;        /* N * M */
    size_t n_jumps;     /* total snakes + ladders */
    Jump   *jumps;      /* array of all snakes & ladders */
    size_t *mapping;    /* mapping[i] = destination after applying jump */

    /* Graph representation: adjacency lists for die rolls */
    size_t **adj;       /* adj[i] = array of neighbor square indices */
    size_t  *adj_cnt;   /* adj_cnt[i] = number of neighbors in adj[i] */
} Board;

/*
 * board_load:
 *   Load a board configuration from a text file.
 *   File format:
 *     First line:  N M
 *     Subsequent lines: "S start end" or "L start end"
 *   Returns:
 *     - Pointer to a newly allocated Board on success.
 *     - NULL on failure (e.g., file I/O error or invalid format).
 *   The returned Board has mapping[] initialized and adj/adj_cnt allocated
 *   (but the adjacency lists themselves are filled later by board_build_graph).
 */
Board *board_load(const char *filename);

/*
 * board_build_graph:
 *   Construct the adjacency lists for a loaded Board given die properties.
 *   - die_sides:    number of faces on the die (D).
 *   - win_by_exceed:
 *       * non-zero: moves past the last square clamp to the last square.
 *       * zero:      must land exactly on the last square to win.
 *   This fills Board->adj and Board->adj_cnt so that for every square i,
 *   adj[i][k] gives the destination square for a roll of (k+1).
 */
void board_build_graph(Board *b, size_t die_sides, int win_by_exceed);

/*
 * board_free:
 *   Free all memory associated with a Board.
 *   Safe to call with a NULL pointer.
 */
void board_free(Board *b);

#endif /* BOARD_H */
