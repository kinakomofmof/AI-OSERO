#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "AImodel/base.h"

// personality_xxx.c の関数宣言
AIModel load_cautious();
AIModel load_aggressive();
AIModel load_corner_addict();
AIModel load_shallow();
AIModel load_endgame();
AIModel load_random_like();
AIModel load_balanced();
AIModel load_mood();

// ------------------------------
// 盤面初期化
// ------------------------------
void init_board(int board[8][8]) {
    for (int y = 0; y < 8; y++)
        for (int x = 0; x < 8; x++)
            board[y][x] = EMPTY;

    board[3][3] = WHITE;
    board[3][4] = BLACK;
    board[4][3] = BLACK;
    board[4][4] = WHITE;
}

// ------------------------------
// 盤面表示
// ------------------------------
void print_board(int board[8][8]) {
    printf("  0 1 2 3 4 5 6 7\n");
    for (int y = 0; y < 8; y++) {
        printf("%d ", y);
        for (int x = 0; x < 8; x++) {
            char c = '.';
            if (board[y][x] == BLACK) c = 'B';
            if (board[y][x] == WHITE) c = 'W';
            printf("%c ", c);
        }
        printf("\n");
    }
}

// ------------------------------
// ADMIN MODE
// ------------------------------
void admin_menu(int board[8][8], AIModel *model, int *turn) {
    while (1) {
        printf("\n=== ADMIN MODE ===\n");
        printf("1: 盤面表示\n");
        printf("2: 任意の位置に石を置く\n");
        printf("3: ターン変更\n");
        printf("4: 盤面リセット\n");
        printf("5: AI性格を変更\n");
        printf("6: ゲームに戻る\n");
        printf("選択: ");

        int cmd;
        scanf("%d", &cmd);

        if (cmd == 1) {
            print_board(board);
        }
        else if (cmd == 2) {
            int x, y, c;
            printf("x y color(1=黒,2=白): ");
            scanf("%d %d %d", &x, &y, &c);
            if (x>=0 && x<8 && y>=0 && y<8) {
                board[y][x] = c;
            }
        }
        else if (cmd == 3) {
            printf("次のターン (1=黒,2=白): ");
            scanf("%d", turn);
        }
        else if (cmd == 4) {
            init_board(board);
            printf("盤面を初期化しました。\n");
        }
        else if (cmd == 5) {
            printf("性格番号を入力 (0〜7): ");
            int n;
            scanf("%d", &n);

            AIModel (*funcs[8])() = {
                load_cautious,
                load_aggressive,
                load_corner_addict,
                load_shallow,
                load_endgame,
                load_random_like,
                load_balanced,
                load_mood
            };

            *model = funcs[n % 8]();
            printf("AI性格を変更しました。\n");
        }
        else if (cmd == 6) {
            printf("ADMIN MODE を終了します。\n");
            return;
        }
    }
}

// ------------------------------
// MAIN
// ------------------------------
int main() {
    srand((unsigned)time(NULL));

    AIModel model = load_balanced();  // デフォルト性格
    int board[8][8];
    init_board(board);

    int turn = BLACK;

    while (1) {
        print_board(board);

        if (turn == BLACK) {
            printf("AI（黒）の番です\n");

            Move m = choose_best_move(board, BLACK, &model);

            if (m.x == -1) {
                printf("黒はパス\n");
                turn = WHITE;
                continue;
            }

            printf("AI が (%d, %d) に置きます\n", m.x, m.y);
            simulate_move(board, m.x, m.y, BLACK, &model);

            turn = WHITE;
        }
        else {
            printf("あなた（白）の番です\n");

            char buf[32];
            printf("x y ': ");
            scanf("%s", buf);

            // ★ admin モード
            if (strcmp(buf, "admin") == 0) {
                admin_menu(board, &model, &turn);
                continue;
            }

            int x = atoi(buf);
            int y;
            scanf("%d", &y);

            if (!is_legal_move(board, x, y, WHITE)) {
                printf("そこには置けません\n");
                continue;
            }

            simulate_move(board, x, y, WHITE, &model);
            turn = BLACK;
        }
    }

    return 0;
}
