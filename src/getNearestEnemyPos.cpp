#include <cfloat>
#include "getNearestEnemyPos.h"
#include <cmath>  // Ensure sqrt function is available

Position *getNearestEnemyPos(Gladiator *gladiator) {

    RobotList robotList = gladiator->game->getPlayingRobotsId();
    byte *IDs = robotList.ids;

    RobotData mydata = gladiator->robot->getData();
    byte myTeamId = mydata.teamId;
    Position myPos = mydata.position;

    Position *pos = NULL;
    float minDist = FLT_MAX;  // Initialize to a large value

    // Iterate over robot IDs to find the nearest enemy
    for (int i = 0; i < 4; i++)  // Assuming 'count' gives the number of robots
    {
        uint8_t current_id = (uint8_t) IDs[i];
        if (current_id && current_id != mydata.id)  // Ensure we're not checking ourselves
        {
            RobotData robotData = gladiator->game->getOtherRobotData(IDs[i]);
            byte robotTeamID = robotData.teamId;

            if (robotTeamID != myTeamId)  // Check if it's an enemy
            {
                Position pos_temp = robotData.position;

                float dist_x = pos_temp.x - myPos.x;
                float dist_y = pos_temp.y - myPos.y;
                float dist_tmp = sqrt(dist_x * dist_x + dist_y * dist_y);

                if (dist_tmp < minDist) {
                    minDist = dist_tmp;
                    pos = &pos_temp;
                }
            }
        }
    }

    return pos;  // Ensure function returns a value
}