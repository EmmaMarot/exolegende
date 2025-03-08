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
        POS get_next_move(Gladiator *gladiator)
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

            float sqrs = gladiator->maze->getSquareSize();
            if (next == NULL){
                ret.y = (current->i * sqrs) + (sqrs / 2.0);
                ret.x = (current->j * sqrs) + (sqrs / 2.0);
                // TODO retourner le centre de la celule
            }else{
                ret.y = current->i * sqrs;
                ret.x = current->j * sqrs;
                if (current->i == next->i ){
                    if (current->j < next->j){
                        ret.x += sqrs;
                    }
                    ret.y += sqrs / 2.0;
                }else{
                    if (current->i < next->i ){
                        ret.y += sqrs;
                    }
                    ret.x += sqrs / 2.0;

                }
                // TODO retourner la sortie de la celule
            }
            // this->actual_index ++;
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