#include "base.h"

AIModel load_aggressive() {
    AIModel m;

    m.w_corner   = 40;
    m.w_edge     = 10;
    m.w_danger   = 10;
    m.w_flip     = 6;

    m.miss_rate  = 20;
    m.depth      = 1;
    m.randomness = 15;
    m.aggression = 90;

    return m;
}
