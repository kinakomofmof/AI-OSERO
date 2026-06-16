#include "base.h"
#include <stdio.h>

#define SIZE 8

// 8方向
int dx[8] = {1,1,0,-1,-1,-1,0,1};
int dy[8] = {0,1,1,1,0,-1,-1,-1};

int in_board(int x, int y) {
    return (x >= 0 && x < SIZE && y >= 0 && y < SIZE);
}

// ------------------------------
// 合法手判定
// ------------------------------
int is_legal_move(int b[SIZE][SIZE], int x, int y, int color) {
    if (!in_board(x,y)) return 0;
    if (b[y][x] != EMPTY) return 0;

    int opp = (color == BLACK) ? WHITE : BLACK;

    for (int dir = 0; dir < 8; dir++) {
        int nx = x + dx[dir];
        int ny = y + dy[dir];
        int found_opp = 0;

        while (in_board(nx,ny) && b[ny][nx] == opp) {
            found_opp = 1;
            nx += dx[dir];
            ny += dy[dir];
        }

        if (found_opp && in_board(nx,ny) && b[ny][nx] == color)
            return 1;
    }
    return 0;
}

// ------------------------------
// 盤面評価
// ------------------------------
int evaluate_board(int b[SIZE][SIZE], int color, AIModel *model) {
    int score = 0;
    int opp = (color == BLACK) ? WHITE : BLACK;

    // 角の重み
    int corners[4][2] = {{0,0},{7,0},{0,7},{7,7}};
    for (int i = 0; i < 4; i++) {
        int x = corners[i][0];
        int y = corners[i][1];
        if (b[y][x] == color) score += model->w_corner;
        if (b[y][x] == opp)   score -= model->w_corner;
    }

    // 辺の重み
    for (int i = 1; i < 7; i++) {
        if (b[0][i] == color) score += model->w_edge;
        if (b[0][i] == opp)   score -= model->w_edge;

        if (b[7][i] == color) score += model->w_edge;
        if (b[7][i] == opp)   score -= model->w_edge;

        if (b[i][0] == color) score += model->w_edge;
        if (b[i][0] == opp)   score -= model->w_edge;

        if (b[i][7] == color) score += model->w_edge;
        if (b[i][7] == opp)   score -= model->w_edge;
    }

    return score;
}

// ------------------------------
// 石を置いて盤面を更新し、評価値を返す
// ------------------------------
int simulate_move(int b[SIZE][SIZE], int x, int y, int color, AIModel *model) {
    int tmp[SIZE][SIZE];

    // 盤面コピー
    for (int yy = 0; yy < SIZE; yy++)
        for (int xx = 0; xx < SIZE; xx++)
            tmp[yy][xx] = b[yy][xx];

    int opp = (color == BLACK) ? WHITE : BLACK;
    tmp[y][x] = color;

    int flipped = 0;

    // 8方向反転
    for (int dir = 0; dir < 8; dir++) {
        int nx = x + dx[dir];
        int ny = y + dy[dir];
        int path[SIZE][2];
        int path_len = 0;

        while (in_board(nx,ny) && tmp[ny][nx] == opp) {
            path[path_len][0] = nx;
            path[path_len][1] = ny;
            path_len++;
            nx += dx[dir];
            ny += dy[dir];
        }

        if (path_len > 0 && in_board(nx,ny) && tmp[ny][nx] == color) {
            for (int i = 0; i < path_len; i++) {
                int fx = path[i][0];
                int fy = path[i][1];
                tmp[fy][fx] = color;
                flipped++;
            }
        }
    }

    // ★ tmp を b に反映（本物の盤面更新）
    for (int yy = 0; yy < SIZE; yy++)
        for (int xx = 0; xx < SIZE; xx++)
            b[yy][xx] = tmp[yy][xx];

    // ★ 評価値を返す（AI探索用）
    int base = evaluate_board(b, color, model);
    base += flipped * model->w_flip;

    return base;
}

// ------------------------------
// 最善手探索
// ------------------------------
Move choose_best_move(int b[SIZE][SIZE], int color, AIModel *model) {
    Move best = {-1,-1};
    int best_score = -999999;

    for (int y = 0; y < SIZE; y++) {
        for (int x = 0; x < SIZE; x++) {

            if (!is_legal_move(b, x, y, color)) continue;

            int tmp[SIZE][SIZE];
            for (int yy = 0; yy < SIZE; yy++)
                for (int xx = 0; xx < SIZE; xx++)
                    tmp[yy][xx] = b[yy][xx];

            int score = simulate_move(tmp, x, y, color, model);

            if (score > best_score) {
                best_score = score;
                best.x = x;
                best.y = y;
            }
        }
    }

    return best;
}
