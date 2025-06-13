#include "board.h"

#include <stdio.h>
#include <stdlib.h>

Board *board_load(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) return NULL;

    size_t N, M;
    if (fscanf(f, "%zu %zu\n", &N, &M) != 2) {
        fclose(f);
        return NULL;
    }

    Board *b = calloc(1, sizeof(Board));
    b->N    = N;
    b->M    = M;
    b->size = N * M;

    /* read jumps */
    Jump *tmp = NULL;
    size_t cap = 0, cnt = 0;
    while (1) {
        char t; size_t s,e;
        if (fscanf(f, " %c %zu %zu\n", &t, &s, &e) != 3)
            break;
        if (cnt == cap) {
            cap = cap ? cap * 2 : 8;
            tmp = realloc(tmp, cap * sizeof(Jump));
        }
        tmp[cnt++] = (Jump){s,e};
    }
    fclose(f);

    b->n_jumps = cnt;
    b->jumps   = tmp;

    /* mapping[i] = i or jump destination */
    b->mapping = malloc(b->size * sizeof(size_t));
    for (size_t i = 0; i < b->size; ++i)
        b->mapping[i] = i;
    for (size_t j = 0; j < cnt; ++j) {
        size_t s = tmp[j].start;
        if (s < b->size)
            b->mapping[s] = tmp[j].end;
    }

    /* prepare adjacency storage (fill later) */
    b->adj     = calloc(b->size, sizeof(size_t*));
    b->adj_cnt = calloc(b->size, sizeof(size_t));

    return b;
}

void board_build_graph(Board *b,
                       size_t die_sides,
                       int win_by_exceed)
{
    /* every node has exactly die_sides neighbors */
    for (size_t i = 0; i < b->size; ++i)
        b->adj_cnt[i] = die_sides;

    for (size_t i = 0; i < b->size; ++i)
        b->adj[i] = malloc(die_sides * sizeof(size_t));

    for (size_t i = 0; i < b->size; ++i) {
        for (size_t f = 1; f <= die_sides; ++f) {
            size_t raw = i + f;
            size_t dest;
            if (raw >= b->size) {
                dest = win_by_exceed
                       ? b->size - 1
                       : i;
            } else {
                dest = raw;
            }
            /* apply snake/ladder */
            dest = b->mapping[dest];
            b->adj[i][f-1] = dest;
        }
    }
}

void board_free(Board *b) {
    if (!b) return;
    free(b->jumps);
    free(b->mapping);
    for (size_t i = 0; i < b->size; ++i)
        free(b->adj[i]);
    free(b->adj);
    free(b->adj_cnt);
    free(b);
}
