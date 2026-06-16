#include "base.h"

AIModel load_cautious() {
    AIModel m;

    m.w_corner   = 150;
    m.w_edge     = 40;
    m.w_danger   = 90;
    m.w_flip     = 1;

    m.miss_rate  = 5;
    m.depth      = 2;
    m.randomness = 5;
    m.aggression = 20;

    return m;
}