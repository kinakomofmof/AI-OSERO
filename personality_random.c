#include "base.h"

AIModel load_random_like() {
    AIModel m;

    m.w_corner   = 20;
    m.w_edge     = 5;
    m.w_danger   = 10;
    m.w_flip     = 1;

    m.miss_rate  = 40;
    m.depth      = 1;
    m.randomness = 50;
    m.aggression = 30;

    return m;
}