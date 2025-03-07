#ifndef BEHAVIOR_H
#define BEHAVIOR_H

#include <cmath>
#include "gladiator.h"
#include "followPathController.cpp"
#include "pathFinder.cpp"

enum ACTIONS {
    NONE,
    MOVE_TO
};

enum ORIENTATION {
    NORTH,
    EAST,
    SOUTH,
    WEST
};
class Behavior
{
    public: 
        Behavior(Gladiator *gladiator)
        {
            this->gladiator = gladiator;
            this->FPC = new FollowPathController(gladiator);
            this->PF = new PathFinder();
        }

        void calculate_action(){
            this->orientation = this->get_orientation();
        }

        void do_action(){
            switch (this->action)
            {
                case MOVE_TO:
                {
                    
                    if (this->cible == NULL){
                        this->cible = this->get_dbl_next_cible();
                        this->PF->set_path(this->gladiator->maze->getNearestSquare(), this->cible);
                    }
                    this->p = this->PF->get_next_move(); 
                    this->FPC->move_to(this->p.x, this->p.y);
                    break;
                }
                case NONE:
                    ;
                    break;
            }
        }
        void process(){
            RobotData rd = this->gladiator->robot->getData();
            float dist = sqrt(((this->p.x - rd.position.x) * (this->p.x - rd.position.x)) + ((this->p.y - rd.position.y) * (this->p.y - rd.position.y)));
            if (dist < 0.05){
                this->do_action();
            }
            this->orientation = this->get_orientation();
            this->gladiator->log("orientation => %d\n", this->orientation);
            this->FPC->process();
        }
    
    private:
        enum ACTIONS action = NONE; // action en cours du robot 
        FollowPathController *FPC; // controller du deplacement
        PathFinder *PF; // pathfinder
        Gladiator *gladiator; // instance principal du robot
        MazeSquare *cible = NULL; // masesquare en aproche
        POS p; // coordoner reel en aproche
    
        enum ORIENTATION orientation;
        // pour le parcour du maze en main droite
        ORIENTATION get_orientation()
        {
            float angle = this->gladiator->robot->getData().position.a;
            if (angle >= -0.785 && angle < 0.785){return EAST;}
            if (angle >= 0.785 && angle < 2.356){return SOUTH;}
            if (angle >= 2.356 || angle < -2.356){return WEST;}
            if (angle >= -2.356 && angle < -0.785){return SOUTH;}
            return EAST;
        }

        MazeSquare* get_dbl_next_cible(){
            
            MazeSquare *current = this->gladiator->maze->getNearestSquare();
            MazeSquare *next = get_next_cible(current);
            MazeSquare *dblNext = get_next_cible(next);

            return dblNext;
        }
        MazeSquare* get_next_cible(MazeSquare *current)
        {
            if (current == NULL)
            {
                return NULL;
            }
            switch (this->orientation)
            {
                case EAST:
                    if (current->southSquare){return current->southSquare;}
                    else if (current->eastSquare){return current->eastSquare;}
                    else if (current->northSquare){return current->northSquare;}
                    else if (current->westSquare){return current->westSquare;}
                    /* code */
                    break;
                case NORTH:
                    if (current->eastSquare){return current->eastSquare;}
                    else if (current->northSquare){return current->northSquare;}
                    else if (current->westSquare){return current->westSquare;}
                    else if (current->southSquare){return current->southSquare;}
                    /* code */
                    break;
                case WEST:
                    if (current->northSquare){return current->northSquare;}
                    else if (current->westSquare){return current->westSquare;}
                    else if (current->southSquare){return current->southSquare;}
                    else if (current->eastSquare){return current->eastSquare;}
                    /* code */
                    break;
                case SOUTH:
                    if (current->westSquare){return current->westSquare;}
                    else if (current->southSquare){return current->southSquare;}
                    else if (current->eastSquare){return current->eastSquare;}
                    else if (current->northSquare){return current->northSquare;}
                    /* code */
                    break;
                
                default:
                    break;
            }
            return NULL;
        }
        
};

#endif