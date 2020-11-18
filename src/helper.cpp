#include "helper.h"

void clear_grid(bool g[8][8])
{
    for (int i = 0; i<8; ++i)
        for (int j = 0; j<8; ++j)
            g[i][j] = false;
}
