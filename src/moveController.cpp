#ifndef MOVECONTROLLER_H
#define MOVECONTROLLER_H

#include <iostream>
#include "gladiator.h"

// PID parameters
const float Kp_heading = 2.1;
// integral coef
const float Ki_heading = 0.025;
// derivative coeff
const float Kd_heading = 0.3;

const float DIST_ON_DEST = 0.05;
const float MAX_INTEGRAL = 0.8;

class MoveController{
    private:
        float time_step = 0.1;
    
        float target_x = 0.0;
        float target_y = 0.0;
        
        float prev_error_heading = 0.0;

        float error_x = 0.0;
        float error_y = 0.0;

        float error_heading = 0.0;
        float integral_heading = 0.0;
        float derivative_heading = 0.0;

        Gladiator *gladiator;

        float robot_radius = gladiator->robot->getRobotRadius();

    public: 
        MoveController(Gladiator *gladiator)
        : gladiator(gladiator) {}
        void reset()
        {
            // Reset PID variables
            this->prev_error_heading = 0.0;
            this->integral_heading = 0.0;
        }
        bool is_on_dest(){
            RobotData data = this->gladiator->robot->getData();
            Position pos = data.position;  // RobotData contains position struct

                    // Calculate error (distance to target)
            float dx = this->target_x - pos.x;
            float dy = this->target_y - pos.y;
            float error = sqrt(dx*dx + dy*dy);

            return error < DIST_ON_DEST;
        }
        void set_target(float x, float y){
            this->target_x = x;
            this->target_y = y;
            this->gladiator->log("new des (%f;%f)", x, y);
        }
        void process()
        {   
            // Get current robot position from RobotData
            RobotData data = this->gladiator->robot->getData();
            Position pos = data.position;  // RobotData contains position struct

            // Calculate error (distance)
            this->error_x = this->target_x - pos.x;
            this->error_y = this->target_y - pos.y;

            // Calculate error (heading)
            float target_heading = atan2(this->error_y, this->error_x);
            float current_heading = pos.a;
            float error_heading = target_heading - current_heading;

            // Normalize heading error to [-π, π]
            while (error_heading > M_PI) error_heading -= 2*M_PI;
            while (error_heading < -M_PI) error_heading += 2*M_PI;

            this->integral_heading += error_heading*this->time_step;
            this->integral_heading = std::max(-MAX_INTEGRAL, std::min(this->integral_heading, MAX_INTEGRAL));
            this->derivative_heading = (error_heading - prev_error_heading)/this->time_step;
            
            float error_dist = sqrt(error_x*error_x + error_y*error_y);
            float desired_linear_velocity = sqrt(2*error_dist*0.03) + 0.18;


            float angular_velocity = (Kp_heading * error_heading) + 
                             (Ki_heading * integral_heading) + 
                             (Kd_heading * derivative_heading);
            
            // Reduce speed if heading is not aligned
            float scaling_factor = cos(error_heading);
            desired_linear_velocity *= std::max(0.0f, scaling_factor);


            float L = (desired_linear_velocity - (angular_velocity * this->robot_radius));
            float R = (desired_linear_velocity + (angular_velocity * this->robot_radius));

  
            gladiator->control->setWheelSpeed(WheelAxis::LEFT, L); //control de la roue gauche
            gladiator->control->setWheelSpeed(WheelAxis::RIGHT, R); //controle de la roue droite


           this->prev_error_heading = error_heading;
        };
};

#endif