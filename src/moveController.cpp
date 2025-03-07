#include <iostream>
#include "gladiator.h"

class MoveController{
    private:
        float spacing = 0.11; // Wheel spacing (distance between wheels)
        float L;
        float R;
        Gladiator *gladiator;
    public: 
        MoveController(Gladiator *gladiator)
            : gladiator(gladiator) {}
        float speed;
        float angle;
        void process(){
            L = (speed - (angle * spacing / 2.0));
            R = (speed + (angle * spacing / 2.0));
            
            if (L > R){ R = R / L ; L = L / L;}
            if (L < R){ L = L / R ; R = R / R;}
            gladiator->log("speed : %f\nangle : %f\nL : %f\nR : %f", speed, angle, L, R); // GFA 4.5.1     
            gladiator->control->setWheelSpeed(WheelAxis::LEFT, L); //control de la roue gauche
            gladiator->control->setWheelSpeed(WheelAxis::RIGHT, R); //controle de la roue droite
        }
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