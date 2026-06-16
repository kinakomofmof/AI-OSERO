#include "base.h"

AIModel load_corner_addict() {
    AIModel m;

    m.w_corner   = 300;
    m.w_edge     = 0;
    m.w_danger   = 0;
    m.w_flip     = 1;

    m.miss_rate  = 10;
    m.depth      = 1;
    m.randomness = 10;
    m.aggression = 40;

    return m;
}