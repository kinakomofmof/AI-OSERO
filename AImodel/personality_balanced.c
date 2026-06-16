#include "base.h"

AIModel load_balanced() {
    AIModel m;

    m.w_corner   = 100;
    m.w_edge     = 20;
    m.w_danger   = 40;
    m.w_flip     = 2;

    m.miss_rate  = 15;
    m.depth      = 2;
    m.randomness = 10;
    m.aggression = 50;

    return m;
}
