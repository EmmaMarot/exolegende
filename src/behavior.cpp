#ifndef BEHAVIOR_H
#define BEHAVIOR_H

#include "gladiator.h"
#include "followPathController.cpp"
#include "pathFinder.cpp"

enum ACTIONS {
    NONE,
    MOVE_TO
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

        }

        void do_action(){
            switch (this->action)
            {
                case MOVE_TO:
                    POS p = PF->get_next_move();
                    FPC->move_to(p.x, p.y);
                    break;
            }
        }
        void process(){
            this->FPC->process();
        }
    
    private:
        enum ACTIONS action = NONE; 
        FollowPathController *FPC;
        PathFinder *PF;
        Gladiator *gladiator;
};

#endif