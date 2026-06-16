// base.c
// 共通AIロジック（合法手判定・反転・評価・探索）
// 拡張パックは base_ext.h 経由で評価を追加できる想定

#include <stdio.h>
#include <stdlib.h>
#include "base.h"
#include "base_ext.h"

// 盤面サイズ
#define N 8

// 石の色
#define EMPTY 0
#define BLACK 1
#define WHITE -1

// 8方向
static const int DX[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };
static const int DY[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };

// ---------- ユーティリティ ----------

static int in_board(int x, int y) {
    return (x >= 0 && x < N && y >= 0 && y < N);
}

static int opponent(int color) {
    return -color;
}

void copy_board(int dst[N][N], int src[N][N]) {
    for (int y = 0; y < N; y++) {
        for (int x = 0; x < N; x++) {
            dst[y][x] = src[y][x];
        }
    }
}

// ---------- 合法手判定・着手処理 ----------

int is_legal_move(int board[N][N], int color, int x, int y) {
    if (!in_board(x, y) || board[y][x] != EMPTY) return 0;

    int opp = opponent(color);

    for (int dir = 0; dir < 8; dir++) {
        int nx = x + DX[dir];
        int ny = y + DY[dir];
        int found_opp = 0;

        while (in_board(nx, ny) && board[ny][nx] == opp) {
            found_opp = 1;
            nx += DX[dir];
            ny += DY[dir];
        }

        if (found_opp && in_board(nx, ny) && board[ny][nx] == color) {
            return 1;
        }
    }
    return 0;
}

int has_legal_move(int board[N][N], int color) {
    for (int y = 0; y < N; y++) {
        for (int x = 0; x < N; x++) {
            if (is_legal_move(board, color, x, y)) return 1;
        }
    }
    return 0;
}

void apply_move(int board[N][N], int color, int x, int y) {
    board[y][x] = color;
    int opp = opponent(color);

    for (int dir = 0; dir < 8; dir++) {
        int nx = x + DX[dir];
        int ny = y + DY[dir];
        int path[8][2];
        int path_len = 0;

        while (in_board(nx, ny) && board[ny][nx] == opp) {
            path[path_len][0] = nx;
            path[path_len][1] = ny;
            path_len++;
            nx += DX[dir];
            ny += DY[dir];
        }

        if (path_len > 0 && in_board(nx, ny) && board[ny][nx] == color) {
            for (int i = 0; i < path_len; i++) {
                int fx = path[i][0];
                int fy = path[i][1];
                board[fy][fx] = color;
            }
        }
    }
}

// ---------- 基本評価関数 ----------

static int count_flips_if_move(int board[N][N], int color, int x, int y) {
    if (!is_legal_move(board, color, x, y)) return 0;

    int opp = opponent(color);
    int flips = 0;

    for (int dir = 0; dir < 8; dir++) {
        int nx = x + DX[dir];
        int ny = y + DY[dir];
        int path_len = 0;

        while (in_board(nx, ny) && board[ny][nx] == opp) {
            path_len++;
            nx += DX[dir];
            ny += DY[dir];
        }

        if (path_len > 0 && in_board(nx, ny) && board[ny][nx] == color) {
            flips += path_len;
        }
    }
    return flips;
}

static int eval_corners(int board[N][N], int color) {
    int score = 0;
    int corners[4][2] = {
        {0, 0}, {N-1, 0}, {0, N-1}, {N-1, N-1}
    };

    for (int i = 0; i < 4; i++) {
        int x = corners[i][0];
        int y = corners[i][1];
        if (board[y][x] == color) score++;
        else if (board[y][x] == opponent(color)) score--;
    }
    return score;
}

static int eval_edges(int board[N][N], int color) {
    int score = 0;

    for (int i = 1; i < N-1; i++) {
        // 上
        if (board[0][i] == color) score++;
        else if (board[0][i] == opponent(color)) score--;

        // 下
        if (board[N-1][i] == color) score++;
        else if (board[N-1][i] == opponent(color)) score--;

        // 左
        if (board[i][0] == color) score++;
        else if (board[i][0] == opponent(color)) score--;

        // 右
        if (board[i][N-1] == color) score++;
        else if (board[i][N-1] == opponent(color)) score--;
    }
    return score;
}

static int eval_disc_diff(int board[N][N], int color) {
    int my = 0, opp = 0;
    for (int y = 0; y < N; y++) {
        for (int x = 0; x < N; x++) {
            if (board[y][x] == color) my++;
            else if (board[y][x] == opponent(color)) opp++;
        }
    }
    return my - opp;
}

// ---------- 拡張パック込みの評価 ----------

// base_ext_xxxx.c 側でこの関数を実装してもいいし、
// 何も拡張がなければ base_ext_stub.c で 0 を返す実装を置いておく想定。
int base_ext_eval_all(int board[N][N], int color, AIModel *model);

int evaluate_board(int board[N][N], int color, AIModel *model) {
    int score = 0;

    // 基本評価
    score += eval_corners(board, color) * model->w_corner;
    score += eval_edges(board, color)   * model->w_edge;
    score += eval_disc_diff(board, color) * model->w_disc;

    // 拡張パック評価（安定石・mobility・危険マスなど）
    score += base_ext_eval_all(board, color, model);

    return score;
}

// ---------- ミニマックス（簡易） ----------

static int minimax(int board[N][N], int color, AIModel *model, int depth, int maximizing) {
    if (depth == 0 || (!has_legal_move(board, color) && !has_legal_move(board, opponent(color)))) {
        return evaluate_board(board, model->root_color, model);
    }

    int best = maximizing ? -1000000 : 1000000;
    int cur_color = maximizing ? model->root_color : opponent(model->root_color);

    if (!has_legal_move(board, cur_color)) {
        // パス
        return minimax(board, color, model, depth - 1, !maximizing);
    }

    for (int y = 0; y < N; y++) {
        for (int x = 0; x < N; x++) {
            if (!is_legal_move(board, cur_color, x, y)) continue;

            int tmp[N][N];
            copy_board(tmp, board);
            apply_move(tmp, cur_color, x, y);

            int val = minimax(tmp, color, model, depth - 1, !maximizing);

            if (maximizing) {
                if (val > best) best = val;
            } else {
                if (val < best) best = val;
            }
        }
    }
    return best;
}

// ---------- 最善手選択 ----------

int choose_best_move(int board[N][N], int color, AIModel *model, int *out_x, int *out_y) {
    int best_score = -1000000;
    int best_x = -1, best_y = -1;

    model->root_color = color;  // 評価側で参照する用

    for (int y = 0; y < N; y++) {
        for (int x = 0; x < N; x++) {
            if (!is_legal_move(board, color, x, y)) continue;

            int tmp[N][N];
            copy_board(tmp, board);
            apply_move(tmp, color, x, y);

            int score = minimax(tmp, color, model, model->depth - 1, 0);

            // ミス率・ランダム性を入れたいならここで調整
            if (score > best_score) {
                best_score = score;
                best_x = x;
                best_y = y;
            }
        }
    }

    if (best_x == -1) return 0;

    *out_x = best_x;
    *out_y = best_y;
    return 1;
}