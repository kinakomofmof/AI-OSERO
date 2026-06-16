#include "base.h"

AIModel load_endgame() {
    AIModel m;

    m.w_corner   = 80;
    m.w_edge     = 20;
    m.w_danger   = 40;
    m.w_flip     = 1;

    m.miss_rate  = 10;
    m.depth      = 1;   // 序盤は浅い（終盤で3に変更するのは性格側のロジック）
    m.randomness = 10;
    m.aggression = 40;

    return m;
}