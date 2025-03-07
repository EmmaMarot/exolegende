#ifndef FOLLOWPATH_H
#define FOLLOWPATH_H

#include "gladiator.h"
#include "moveController.cpp"

class FollowPathController
{
    public:
        FollowPathController(Gladiator *gladiator)
        {
            this->gladiator = gladiator;
            this->MC = new MoveController(gladiator);
        }

        void move_to(float x, float y)
        {
            this->to_x = x;
            this->to_y = y;
        }
        void process(){
            RobotData rd = this->gladiator->robot->getData();
            
            
            this->gladiator->log("pos x :%f\npos y : %f", rd.position.x, rd.position.y);
            this->MC->process();
        }
        MoveController *MC;

    private:
        Gladiator *gladiator;
        float to_x;
        float to_y;

        
};

#endif