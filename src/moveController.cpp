#ifndef MOVECONTROLLER_H
#define MOVECONTROLLER_H

#include <iostream>
#include "gladiator.h"

const float Kp = 0.5;   // Might be okay as is
const float Ki = 0.05;  // Consider reducing to ~0.008
const float Kd = 0.1;   // Consider reducing to ~0.016


const float DIST_ON_DEST = 0.05;


const float MAX_INTEGRAL = 1.0;

class MoveController{
    private:
    
        float target_x = 0.0;
        float target_y = 0.0;

        
        float prev_error = 0.0;
        float integral = 0.0;
        Gladiator *gladiator;

        // float spacing = 0.11; // Wheel spacing (distance between wheels)
        // float L = 0.0;
        // float R = 0.0;
        // Gladiator *gladiator;
    public: 
        MoveController(Gladiator *gladiator)
        : gladiator(gladiator) {}
        void reset()
        {
            // Reset PID variables
            this->prev_error = 0.0;
            this->integral = 0.0;
        }
        bool is_on_dest(){
            RobotData data = this->gladiator->robot->getData();
            Position pos = data.position;  // RobotData contains position struct

                    // Calculate error (distance to target)
            float dx = this->target_x - pos.x;
            float dy = this->target_y - pos.y;
            float error = sqrt(dx*dx + dy*dy);
            // this->gladiator->log("is on dest : %d",error < DIST_ON_DEST );
            return error < DIST_ON_DEST;
        }
        void set_target(float x, float y){
            this->target_x = x;
            this->target_y = y;
            this->gladiator->log("new des (%f;%f)", x, y);
        }
        void process()
        {
            
            // unsigned long current_time = millis();
            // if (current_time - this->last_target_change >= TARGET_CHANGE_INTERVAL) {
            //     // Move to next target position
            //     this->current_target_index = (this->current_target_index + 1) % NUM_TARGETS;
            //     this->target_x = TARGET_POSITIONS[this->current_target_index][0];
            //     this->target_y = TARGET_POSITIONS[this->current_target_index][1];
            //     this->last_target_change = current_time;
                
            //     // Log new target
            //     char buffer[64];
            //     sprintf(buffer, "New target %d: (%.2f, %.2f)", this->current_target_index + 1, this->target_x, this->target_y);
            //     this->gladiator->log("%s", buffer);
            // }
            
            // Get current robot position from RobotData
            RobotData data = this->gladiator->robot->getData();
            Position pos = data.position;  // RobotData contains position struct

                    // Calculate error (distance to target)
            float dx = this->target_x - pos.x;
            float dy = this->target_y - pos.y;
            float error = sqrt(dx*dx + dy*dy);

            float target_heading = atan2(dy, dx);
            float current_heading = pos.a;
            float heading_error = target_heading - current_heading;

            // Normalize heading error to [-π, π]
            while (heading_error > M_PI) heading_error -= 2*M_PI;
            while (heading_error < -M_PI) heading_error += 2*M_PI;
            
            // PID calculations with correct 50ms time step
            this->integral += error * 0.05;
            // float derivative = (error - this->prev_error) / 0.05;

            // Add anti-windup for integral term
            this->integral = std::max(-MAX_INTEGRAL, std::min(MAX_INTEGRAL, this->integral));
            
            // Calculate control signal for forward speed
            // float control = Kp * error + Ki * this->integral + Kd * derivative;
            
            // First correct heading, then move forward
            float base_speed = 0.0;  // Start with zero speed
            // float turning_factor = 0.03;  // Reduced from 0.3 for gentler turning
            float v_l = 0.0;
            float v_r = 0.0;

            if (fabs(heading_error) > 0.6) {  // Increased from 0.2 (~23 degrees instead of 11.5)
                // Pure rotation until heading is correct, but slower
                v_l = (heading_error > 0) ? -0.15 : 0.15;  // Reduced from 0.2
                v_r = (heading_error > 0) ? 0.15 : -0.15;  // Reduced from 0.2
                // this->gladiator->log("Pure rotation mode");
            } else {
                // Heading is good enough, move forward with small corrections
                base_speed = 0.2;  // Fixed forward speed
                v_l = base_speed;
                v_r = base_speed;
                
                // Gentler turning correction while moving
                v_l += heading_error * 0.05;  // Reduced from 0.1
                v_r -= heading_error * 0.05;  // Reduced from 0.1
                // this->gladiator->log("Forward motion mode");
            }
              // Clamp wheel speeds
            v_l = std::max(-0.3f, std::min(0.3f, v_l));
            v_r = std::max(-0.3f, std::min(0.3f, v_r));
            
            // Apply wheel speeds
            // this->gladiator->control->setWheelSpeed(WheelAxis::LEFT, v_l);
            // this->gladiator->control->setWheelSpeed(WheelAxis::RIGHT, v_r);

            float L = (base_speed - (heading_error * 0.11 / 2.0));
            float R = (base_speed + (heading_error * 0.11 / 2.0));
            // gladiator->log("speed : %f\nangle : %f\nL : %f\nR : %f", speed, angle, L, R); // GFA 4.5.1     
            gladiator->control->setWheelSpeed(WheelAxis::LEFT, L); //control de la roue gauche
            gladiator->control->setWheelSpeed(WheelAxis::RIGHT, R); //controle de la roue droite
            // Log all the debug information
            // char buffer[64];
            // sprintf(buffer, "Current target: (%.2f, %.2f)", this->target_x, this->target_y);
            // gladiator->log("%s", buffer);
            
            // sprintf(buffer, "Distance to target: %.2f", error);
            // gladiator->log("%s", buffer);
            
            // sprintf(buffer, "Heading error: %.2f", heading_error);
            // gladiator->log("%s", buffer);
            
            // sprintf(buffer, "Current pos: (%.2f, %.2f), heading: %.2f", pos.x, pos.y, pos.a);
            // gladiator->log("%s", buffer);
            // sprintf(buffer, "Target heading: %.2f", target_heading);
            // gladiator->log("%s", buffer);

            // sprintf(buffer, "Control: %.2f, Base speed: %.2f", control, base_speed);
            // gladiator->log("%s", buffer);
            
            // sprintf(buffer, "Wheel speeds - Left: %.2f, Right: %.2f", v_l, v_r);
            // gladiator->log("%s", buffer);
            
            // Update previous error for next iteration
           this->prev_error = error;
        }
        // MoveController(Gladiator *gladiator)
        //     : gladiator(gladiator) {}
        // float speed = 0.0;
        // float angle = 0.0;
        // void process(){
        //     L = (speed - (angle * spacing / 2.0));
        //     R = (speed + (angle * spacing / 2.0));
        //     gladiator->log("speed : %f\nangle : %f\nL : %f\nR : %f", speed, angle, L, R); // GFA 4.5.1     
        //     gladiator->control->setWheelSpeed(WheelAxis::LEFT, L); //control de la roue gauche
        //     gladiator->control->setWheelSpeed(WheelAxis::RIGHT, R); //controle de la roue droite
        // }
};



// class DiffDriveController {
// public:
//     DiffDriveController(double wheel_spacing, double wheel_radius)
//         : L(wheel_spacing), R(wheel_radius) {}

//     void computeWheelSpeeds(double linear_velocity, double angular_velocity, 
//                             double &left_wheel_speed, double &right_wheel_speed) {
//         left_wheel_speed  = (linear_velocity - (angular_velocity * L / 2.0)) / R;
//         right_wheel_speed = (linear_velocity + (angular_velocity * L / 2.0)) / R;
//     }

// private:
//     double L; // Wheel spacing (distance between wheels)
//     double R; // Wheel radius
// };

// int main() {
//     double wheel_spacing = 0.5;  // Example: 0.5 meters
//     double wheel_radius  = 0.1;  // Example: 0.1 meters

//     DiffDriveController controller(wheel_spacing, wheel_radius);

//     double linear_velocity = 1.0;  // m/s
//     double angular_velocity = 0.5; // rad/s
//     double left_wheel_speed, right_wheel_speed;

//     controller.computeWheelSpeeds(linear_velocity, angular_velocity, left_wheel_speed, right_wheel_speed);

//     std::cout << "Left Wheel Speed: " << left_wheel_speed << " rad/s" << std::endl;
//     std::cout << "Right Wheel Speed: " << right_wheel_speed << " rad/s" << std::endl;

//     return 0;
// }

#endif