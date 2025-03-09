#ifndef BEHAVIOR_H
#define BEHAVIOR_H

#include <cmath>
#include "gladiator.h"
// #include "followPathController.cpp"
#include "moveController.cpp"
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

const static bool karadoc = false;

class Behavior
{
    public:

        byte ally = 0;

        Behavior(Gladiator *gladiator)
        {
            this->gladiator = gladiator;
//          this->FPC = new FollowPathController(gladiator);
            this->MC = new MoveController(gladiator);
            this->PF = new PathFinder();
            
            this->set_next_dest();
        }
//      FollowPathController *FPC; // controller du deplacement
        MoveController *MC; // Nouveau controller du déplacement

        void calculate_action(){
            this->orientation = this->get_orientation();
        }

/*         void do_action(){
            switch (this->action)
            {
                case MOVE_TO:
                {
                    
                    // if (this->cible == NULL){
                    //     this->cible = this->get_dbl_next_cible();
                    //     this->PF->set_path(this->gladiator->maze->getNearestSquare(), this->cible);
                    // }
                    // this->p = this->PF->get_next_move(this->gladiator); 
                    
                    // this->FPC->move_to(0.14, 2.0);
                    // this->FPC->move_to(this->p.x, this->p.y);
                    break;
                }
                case NONE:
                    ;
                    break;
            }
        } */

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
            if (this->safe)
            {
                RobotData rd = this->gladiator->robot->getData();
                this->orientation = this->get_orientation();
                MazeSquare *next = this->get_next_cible(gladiator->maze->getNearestSquare());
                MazeSquare *dblnext = this->get_next_cible(next);
                MazeSquare *local = this->gladiator->maze->getNearestSquare();     
                if (next == NULL){
                    return;
                }
                POS ret = this->get_next_move(next, NULL);
                if (
                    next != NULL && dblnext != NULL && rd.teamId != local->possession &&
                    next->danger == 0 && local->danger == 0 && this->gladiator->weapon->getBombCount() > 0 &&
                dblnext->danger == 0 && ((dblnext->i != next->i || dblnext->j != next->j) && (dblnext->i != local->i || dblnext->j != local->j))
            ){
                    this->gladiator->weapon->dropBombs(1);
                }
                this->MC->set_target(ret.x, ret.y);
            }
        }
        void set_paniq_dest(){
            float ms = this->gladiator->maze->getSize();
            this->MC->set_target(ms / 2.0, ms / 2.0);
        }
        void process(){
            if (this->MC->is_on_dest()){
                this->set_next_dest();
            }
            RobotData rd = this->gladiator->robot->getData();
            float cms = this->gladiator->maze->getCurrentMazeSize();
            float ms = this->gladiator->maze->getSize();
            float out = ms - cms;
            if (rd.position.x < (out / 2.0) || rd.position.y < (out / 2.0) || rd.position.x > ms - (out / 2.0) || rd.position.y > ms - (out /2.0))
            {
                this->safe = false;

            }else{
                if (!this->safe){
                    this->safe = true;
                    this->set_next_dest();
                }
            }

            if (!this->safe){
                this->set_paniq_dest();
            }
            this->MC->process();
        }
    
    private:
        enum ACTIONS action = MOVE_TO; // action en cours du robot 
        PathFinder *PF; // pathfinder
        Gladiator *gladiator; // instance principal du robot
        MazeSquare *cible = NULL; // masesquare en aproche
        POS p; // coordoner reel en aproche
        bool safe = true;
        int explored[244] = {0};
    
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
        float eval(MazeSquare * tmp, ORIENTATION orientation){
            if (tmp == NULL){return -1;}
            float tmpVal = 0.0;
            if (this->orientation == orientation){tmpVal += 1;}
            if (this->gladiator->robot->getData().teamId != tmp->possession ){tmpVal += 2;}
            tmpVal -= this->explored[(tmp->j * 12) + tmp->i] /4.0;
            tmpVal += (abs(tmp->i -6) / 2.0);
            tmpVal += (abs(tmp->j -6) / 2.0);
            if (this->gladiator->weapon->getBombCount() <= tmp->coin.value )
            {
                tmpVal += tmp->coin.value * 2 ;
            }else{
                tmpVal += tmp->coin.value;
            }
            tmpVal -= tmp->danger;
            return tmpVal;
        }
        MazeSquare* get_next_cible(MazeSquare *current)
        {
            if (current == NULL)
            {
                return NULL;
            }
            this->explored[(current->j * 12) + current->i] += 1;
            MazeSquare *next = NULL;
            float value = -1;
            MazeSquare *tmp;
            tmp = current->eastSquare;
            if (tmp){
                float tmpVal = this->eval(tmp, EAST);
                tmpVal += this->eval(tmp->eastSquare, EAST) / 3.0;
                tmpVal += this->eval(tmp->northSquare, NORTH) / 3.0;
                tmpVal += this->eval(tmp->southSquare, SOUTH) / 3.0;
                if (tmpVal > value){
                    value = tmpVal;
                    next = tmp;
                }
            }
            tmp = current->northSquare;
            if (tmp){
                float tmpVal = this->eval(tmp, NORTH);
                tmpVal += this->eval(tmp->eastSquare, EAST) / 3.0;
                tmpVal += this->eval(tmp->northSquare, NORTH) / 3.0;
                tmpVal += this->eval(tmp->westSquare, WEST) / 3.0;
                if (tmpVal > value){
                    value = tmpVal;
                    next = tmp;
                }
            }
            tmp = current->westSquare;
            if (tmp){
                float tmpVal = this->eval(tmp, WEST);
                tmpVal += this->eval(tmp->westSquare, WEST) / 3.0;
                tmpVal += this->eval(tmp->northSquare, NORTH) / 3.0;
                tmpVal += this->eval(tmp->southSquare, SOUTH) / 3.0;
                if (tmpVal > value){
                    value = tmpVal;
                    next = tmp;
                }
            }
            tmp = current->southSquare;
            if (tmp){
                float tmpVal = this->eval(tmp, SOUTH);
                tmpVal += this->eval(tmp->eastSquare, EAST) / 3.0;
                tmpVal += this->eval(tmp->westSquare, WEST) / 3.0;
                tmpVal += this->eval(tmp->southSquare, SOUTH) / 3.0;
                if (tmpVal > value){
                    value = tmpVal;
                    next = tmp;
                }
            }
            return next;
        }
        
};

#endif