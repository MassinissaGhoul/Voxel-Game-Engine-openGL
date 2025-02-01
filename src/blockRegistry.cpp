#include "include/chunk.hpp"
blockData g_blockRegistry[] = {
    /* [AIR] */
    {
        false, // isOpaque = false (AIR est transparent)
        0,     // uvTop = 0
        0,     // uvBottom = 0
        0      // uvSide = 0
    },
    /* [STONE] */
    {true, // isOpaque = true
     1,    // uvTop = 1 (peut-être la texture "stone")
     1, 1},
    /* [GRASS] */
    {
        true, // isOpaque = true
        2,    // uvTop
        3,    // uvBottom (par ex. terre)
        4     // uvSide (par ex. herbe sur le côté)
    }
    // etc. si tu as d’autres types
};
