#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "gladiator.h"

struct POS {
    float x;
    float y;
};

class PathFinder
{
    public :
        PathFinder(){}
        POS get_next_move()
        {
            struct POS ret;
            ret.x = 0.0;
            ret.y = 0.0;
            return ret;
        }
        void set_path(MazeSquare *from, MazeSquare *to)
        {
            
        }
};

#endif