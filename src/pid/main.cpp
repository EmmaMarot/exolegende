#include "gladiator.h"
Gladiator *gladiator;

// PID controller gains
const float Kp = 0.5;   // Might be okay as is
const float Ki = 0.05;  // Consider reducing to ~0.008
const float Kd = 0.1;   // Consider reducing to ~0.016

// Add at the top with other globals
unsigned long last_target_change = 0;  // Time of last target change
const unsigned long TARGET_CHANGE_INTERVAL = 10000;  // 10 seconds in milliseconds
const int NUM_TARGETS = 4;
const float TARGET_POSITIONS[NUM_TARGETS][2] = {
    {1.5, 1.5},  // First position
    {2.0, 2.0},  // Second position
    {1.5, 1.5},  // Third position
    {1.0, 1.0}   // Fourth position
};
int current_target_index = 0;  // Track which target we're on

// Remove or comment out the old static targets
// const float TARGET_X = 1.5;
// const float TARGET_Y = 1.0;

// Add new target variables that can change
float target_x = TARGET_POSITIONS[0][0];
float target_y = TARGET_POSITIONS[0][1];

// Error tracking variables
float prev_error = 0.0;
float integral = 0.0;

// Add anti-windup for integral term
const float MAX_INTEGRAL = 1.0;

void reset()
{
    // Reset PID variables
    prev_error = 0.0;
    integral = 0.0;
    gladiator->log("Reset function called - PID variables initialized");
}

void setup()
{
    // instanciation de l'objet gladiator
    gladiator = new Gladiator();
    // enregistrement de la fonction de reset qui s'éxecute à chaque fois avant qu'une partie commence
    gladiator->game->onReset(&reset); // GFA 4.4.1
}

void loop()
{
    if (gladiator->game->isStarted())
    {
        // Check if it's time to change target
        unsigned long current_time = millis();
        if (current_time - last_target_change >= TARGET_CHANGE_INTERVAL) {
            // Move to next target position
            current_target_index = (current_target_index + 1) % NUM_TARGETS;
            target_x = TARGET_POSITIONS[current_target_index][0];
            target_y = TARGET_POSITIONS[current_target_index][1];
            last_target_change = current_time;
            
            // Log new target
            char buffer[64];
            sprintf(buffer, "New target %d: (%.2f, %.2f)", current_target_index + 1, target_x, target_y);
            gladiator->log("%s", buffer);
        }

        // Get current robot position from RobotData
        RobotData data = gladiator->robot->getData();
        Position pos = data.position;  // RobotData contains position struct
        
        // Calculate error (distance to target)
        float dx = target_x - pos.x;
        float dy = target_y - pos.y;
        float error = sqrt(dx*dx + dy*dy);
        
        // Calculate target heading
        float target_heading = atan2(dy, dx);
        float current_heading = pos.a;
        float heading_error = target_heading - current_heading;
        
        // Normalize heading error to [-π, π]
        while (heading_error > M_PI) heading_error -= 2*M_PI;
        while (heading_error < -M_PI) heading_error += 2*M_PI;

        // PID calculations with correct 50ms time step
        integral += error * 0.05;
        float derivative = (error - prev_error) / 0.05;
        
        // Add anti-windup for integral term
        integral = std::max(-MAX_INTEGRAL, std::min(MAX_INTEGRAL, integral));
        
        // Calculate control signal for forward speed
        float control = Kp * error + Ki * integral + Kd * derivative;
        
        // First correct heading, then move forward
        float base_speed = 0.0;  // Start with zero speed
        float turning_factor = 0.03;  // Reduced from 0.3 for gentler turning
        float v_l = 0.0;
        float v_r = 0.0;
        
        if (fabs(heading_error) > 0.4) {  // Increased from 0.2 (~23 degrees instead of 11.5)
            // Pure rotation until heading is correct, but slower
            v_l = (heading_error > 0) ? -0.15 : 0.15;  // Reduced from 0.2
            v_r = (heading_error > 0) ? 0.15 : -0.15;  // Reduced from 0.2
            gladiator->log("Pure rotation mode");
        } else {
            // Heading is good enough, move forward with small corrections
            base_speed = 0.2;  // Fixed forward speed
            v_l = base_speed;
            v_r = base_speed;
            
            // Gentler turning correction while moving
            v_l += heading_error * 0.05;  // Reduced from 0.1
            v_r -= heading_error * 0.05;  // Reduced from 0.1
            gladiator->log("Forward motion mode");
        }
        
        // Clamp wheel speeds
        v_l = std::max(-0.3f, std::min(0.3f, v_l));
        v_r = std::max(-0.3f, std::min(0.3f, v_r));
        
        // Apply wheel speeds
        gladiator->control->setWheelSpeed(WheelAxis::LEFT, v_l);
        gladiator->control->setWheelSpeed(WheelAxis::RIGHT, v_r);
        
        // Log all the debug information
        char buffer[64];
        sprintf(buffer, "Current target: (%.2f, %.2f)", target_x, target_y);
        gladiator->log("%s", buffer);
        
        sprintf(buffer, "Distance to target: %.2f", error);
        gladiator->log("%s", buffer);
        
        sprintf(buffer, "Heading error: %.2f", heading_error);
        gladiator->log("%s", buffer);
        
        sprintf(buffer, "Current pos: (%.2f, %.2f), heading: %.2f", pos.x, pos.y, pos.a);
        gladiator->log("%s", buffer);
        sprintf(buffer, "Target heading: %.2f", target_heading);
        gladiator->log("%s", buffer);

        sprintf(buffer, "Control: %.2f, Base speed: %.2f", control, base_speed);
        gladiator->log("%s", buffer);
        
        sprintf(buffer, "Wheel speeds - Left: %.2f, Right: %.2f", v_l, v_r);
        gladiator->log("%s", buffer);
        
        // Update previous error for next iteration
        prev_error = error;
    }
    else
    {
        gladiator->log("Hello world - Game not Started yet"); // GFA 4.5.1
    }
    delay(50);  // Match the actual loop time step
}
