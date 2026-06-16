#include "base.h"

AIModel load_shallow() {
    AIModel m;

    m.w_corner   = 50;
    m.w_edge     = 10;
    m.w_danger   = 20;
    m.w_flip     = 1;

    m.miss_rate  = 30;
    m.depth      = 1;
    m.randomness = 20;
    m.aggression = 50;

    return m;
}
