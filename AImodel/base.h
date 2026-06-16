#ifndef BASE_H
#define BASE_H

#define EMPTY 0
#define BLACK 1
#define WHITE 2

typedef struct {
    int x, y;
} Move;

typedef struct {
    int w_corner;
    int w_edge;
    int w_danger;
    int w_flip;

    int miss_rate;
    int depth;
    int randomness;
    int aggression;
} AIModel;

int is_legal_move(int b[8][8], int x, int y, int color);
int simulate_move(int b[8][8], int x, int y, int color, AIModel *model);
Move choose_best_move(int b[8][8], int color, AIModel *model);

#endif
