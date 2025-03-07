#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "gladiator.h"
#include "bfs.h"

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
            if (this->actual_index >= 20)
            {
                ret.x = 9999.0; // code erreur pas de chemin
                ret.y = 9999.0; // code erreur pas de chemin
                return ret;
            }
            MazeSquare *current = this->path[this->actual_index];
            if (current == NULL)
            {
                ret.x = 9999.0; // code erreur pas de chemin
                ret.y = 9999.0; // code erreur pas de chemin
                return ret;
            }
            MazeSquare *next = NULL;
            if (this->actual_index < 19)
            {
                next = this->path[this->actual_index + 1];
            }

            if (next == NULL){
                // Gladiator::Maze::getSquareSize()

                // TODO retourner le centre de la celule
            }else{
                // TODO retourner la sortie de la celule
            }
            return ret;
        }
        void set_path(MazeSquare *from, MazeSquare *to)
        {
            int pathLength ;
            path = bfsPathFinder(from, to, pathLength);
        }
    private :
        MazeSquare **path;
        int actual_index = 0;
};

#endif