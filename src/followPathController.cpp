#ifndef FOLLOWPATH_H
#define FOLLOWPATH_H

#include <cmath>
#include "gladiator.h"
#include "moveController.cpp"

class FollowPathController
{
    public:
        FollowPathController(Gladiator *gladiator)
        {
            // this->gladiator = gladiator;
            this->MC = new MoveController(gladiator);
        }

        MoveController *MC;
        float rotation = 0.0;
        float vitesse = 0.0;

        void move_to(float x, float y)
        {
            this->to_x = x;
            this->to_y = y;
        }

        void process()
        {
            // RobotData rd = this->gladiator->robot->getData();
            
            // if (to_x != rd.position.x or to_y != rd.position.y)
            //     moving(rd);

            // this->gladiator->log("pos x :%f\npos y : %f", rd.position.x, rd.position.y);
            // this->MC->angle = rotation;
            // this->MC->speed = vitesse;
            this->MC->process();
        }

    private:
        const float max_angle = PI/2;
        const float base_vitesse = 1;

        Gladiator *gladiator;
        float to_x = 0.0;
        float to_y = 0.0;
        float angle = 0.0;

        void moving(RobotData rd)
        {
            // angle = getAngle(rd, this->to_x, this->to_y);

            // if (angle < max_angle)
            // {

            //     vitesse = 0.05;
            // }
            // else{

            //     vitesse = 0;
            // }
            //     // vitesse = base_vitesse - (angle / max_angle);

            // rotation = angle;
        }

        float getAngle(RobotData rd, float x, float y)
        {
            // float origin_angle = rd.position.a;
            // float origin_x = rd.position.x;
            // float origin_y = rd.position.y;
            // float angle = 0;

            // float adj = origin_x - x;
            // float opp = origin_y - y;

            // angle = origin_angle - atanf(opp/adj);


            // this->gladiator->log("Angle à rotationner :%f\nox:%f\noy:%f\nx:%f\ny:%f", angle, origin_x, origin_y, x, y);
            // return angle;
        }
        
};

#endif