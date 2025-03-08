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
            
            this->set_next_dest();
        }
        FollowPathController *FPC; // controller du deplacement

        void calculate_action(){
            this->orientation = this->get_orientation();
        }

        void do_action(){
            switch (this->action)
            {
                case MOVE_TO:
                {
                    
                    // if (this->cible == NULL){
                    //     this->cible = this->get_dbl_next_cible();
                    //     this->PF->set_path(this->gladiator->maze->getNearestSquare(), this->cible);
                    // }
                    // this->p = this->PF->get_next_move(this->gladiator); 
                    
                    this->FPC->move_to(0.14, 2.0);
                    // this->FPC->move_to(this->p.x, this->p.y);
                    break;
                }
                case NONE:
                    ;
                    break;
            }
        }
        POS get_next_move(MazeSquare *current, MazeSquare *next)
        {
            struct POS ret;
            ret.x = 0.0;
            ret.y = 0.0;

            float sqrs = gladiator->maze->getSquareSize();
            ret.x = current->i * sqrs;
            ret.y = current->j * sqrs;
            // if (current->i == next->i ){
            //     if (current->j < next->j){
            //         ret.y += sqrs;
            //     }
                ret.x += sqrs / 2.0;
            // }else{
            //     if (current->i < next->i ){
            //         ret.x += sqrs;
            //     }
                ret.y += sqrs / 2.0;

            // }
            return ret;
        }
        void set_next_dest(){
            RobotData rd = this->gladiator->robot->getData();
            this->orientation = this->get_orientation();
            MazeSquare *next = this->get_next_cible(gladiator->maze->getNearestSquare());
            MazeSquare *dblNext = this->get_next_cible(next);

            POS ret = this->get_next_move(next, dblNext);
            this->FPC->MC->set_target(ret.x, ret.y);
        }
        void process(){
            if (this->FPC->MC->is_on_dest()){
                this->set_next_dest();
            }
            // this->gladiator->log("x, y => %f, %f\n", ret.x, ret.y);
            // this->gladiator->log("x, y => %f, %f\n", rd.position.x, rd.position.y);
            // exit(0);
            // float dist = sqrt(((this->p.x - rd.position.x) * (this->p.x - rd.position.x)) + ((this->p.y - rd.position.y) * (this->p.y - rd.position.y)));
            // if (dist < 0.05){
            //     this->do_action();
            // }
            // if (cible == NULL)
            // {
                // this->do_action();
            // }
            // gladiator->log("\ncible x : %f\ncible y :%f", this->p.x, this->p.y);
            this->FPC->process();
        }
    
    private:
        enum ACTIONS action = MOVE_TO; // action en cours du robot 
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
            if (angle >= 0.785 && angle < 2.356){return NORTH;}
            if (angle >= 2.356 || angle < -2.356){return WEST;}
            if (angle >= -2.356 && angle < -0.785){return SOUTH;}
            return EAST;
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