
#include "simplesoccer.h"
#include <unistd.h>
#include "naobehavior.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       

SimpleSoccerBehavior::SimpleSoccerBehavior( const std::string teamName, int uNum, const map<string, string>& namedParams_, const string& rsg_ )
    : NaoBehavior( teamName, uNum, namedParams_, rsg_ ) {
}

void SimpleSoccerBehavior::beam( double& beamX, double& beamY, double& beamAngle ) {
    VecPosition space = getSpaceForRole( static_cast<Role>(worldModel->getUNum()-1 ));
    beamX = space.getX();
    beamY = space.getY();
    beamAngle = 0;  
}

SkillType NaoBehavior::kickoff(double time)
{
    if (worldModel->getUNum() == ON_BALL) 
    {
        //kick to left forward or right ehhhh
        //VecPosition temp = getposition(FORWARD_RIGHT);
        VecPosition temp2 = (1,-3,0); //in front of position of right forward 
        // temp = temp + VecPosition(1,0,0);
        return kickBall(KICK_IK, temp2);
    }

    // if(worldModel->getUNum() == FORWARD_RIGHT && (time>1))           
    // {
    //     return kickBall(KICK_IK, VecPosition(15,-0.7,0));
    // }
    return SKILL_STAND;
}

static bool attacking = true; 
static int left_right = 0; //output 0 right, 1 left 
SkillType SimpleSoccerBehavior::selectSkill() {
    std::map<int, Role> agentRoles = getAgentRoles();

    if((ball.getDistanceTo(VecPosition(-HALF_FIELD_X,0,0))) < 8)
    {
        //change to defencing mode 
        attacking = false;
    }
    else
    {
        attacking = true; 
    }

    // Draw role assignments
    // worldModel->getRVSender()->clear(); // erases drawings from previous cycle
    // VecPosition space = getSpaceForRole(agentRoles[worldModel->getUNum()]);
    // worldModel->getRVSender()->drawLine("me_to_space", me.getX(), me.getY(), space.getX(), space.getY());
    // worldModel->getRVSender()->drawPoint("role_space", space.getX(), space.getY(), 10.0f);

    static double startTime = worldModel->getTime();    
    if (((worldModel->getPlayMode() == PM_KICK_OFF_LEFT && worldModel->getSide() == SIDE_LEFT) || (worldModel->getPlayMode() == PM_KICK_OFF_RIGHT && worldModel->getSide() == SIDE_RIGHT)) || worldModel->getTime()-startTime < 5)
    {
        VecPosition ball = worldModel->getBall();
        ball.setZ(0);
        // std::cout<<"kickof\n"<<std::endl;
        if (agentRoles[worldModel->getUNum()] == ON_BALL) 
        {
        //kick to left forward or right ehhhh
        //VecPosition temp = getposition(FORWARD_RIGHT);
            //VecPosition temp2 = ball+ VecPosition(0.5,4.5,0); //in front of position of right forward 
            VecPosition temp3 = ball+VecPosition(4.5,-3,0);
        // temp = temp + VecPosition(1,0,0);
            //return kickBall(KICK_IK, getposition(WING_RIGHT)+VecPosition(0.5,0.5,0)); 
            return kickBall(KICK_IK, temp3);
        }
        // std::cout<<"kickoff"<<std::endl;
        //return kickoff(worldModel->getTime() - startTime);
        return SKILL_STAND;
        
    }

    if (agentRoles[worldModel->getUNum()] == ON_BALL) {
        if(checkingFall()== true)
        {
            
        }
        if (me.getDistanceTo(ball) > 1) { //1
            // Walk to ball
            return goToTarget(ball);
        } else {
            // Kick ball toward opponent's goal // using 0.6 because opoenent goalkeeper always remain in the middle of goal
            /* negative y is to the right  */
            /* positive y is to the left  */
            /* position x is front */
            /* negative x is back */

            //check if the opponent is in front 
            if(opponentcount(ball,2.5) >= 1) // if there is opponent in front, then kick to the nearest teammate, or teamate that has most clearance 
            {
                // oppclosest if return 1, then it is the goalkeeper 
                /* if close to the opponent goal */
                if((ball.getDistanceTo(VecPosition(15,0,0))) < 8)
                {
                    /* if oponent too close then kick to the nearest teammates. */
                    // if(opponentcount(ball,1.5) >= 1)
                    // {
                    //     return kickBall(KICK_IK, getposition(SUPPORTER));
                    // }
                    // /* random attempt */
                    // return kickBall(KICK_IK, VecPosition(HALF_FIELD_X+3, 0.7, 0)); 

                    // if(left_right%4 == 0)
                    // {
                    //     return kickBall(KICK_IK, VecPosition(HALF_FIELD_X+3, 0.7, 0)); 
                    // }
                    // else if(left_right%4 == 1)
                    // {
                    //     return kickBall(KICK_IK, VecPosition(HALF_FIELD_X+3, -0.7, 0)); 
                    // }
                    // else if(left_right%4 == 2)
                    // {
                    //     return kickBall(KICK_IK, VecPosition(HALF_FIELD_X+3, 0.2, 0)); 
                    // }else
                    // {
                    //     return kickBall(KICK_IK, VecPosition(HALF_FIELD_X+3, -0.2, 0)); 
                    // }

                    
                    return kickBall(KICK_IK, VecPosition(HALF_FIELD_X+0.6, 0.6, 0)); 

                }
                /* randomly to to left forward or right forward */
                        
                /* if there is opennet on the right, kick left  */
                else if(left_right%2 == 0) //kick left
                {
                    left_right++;  //this make sure that the ball is not kicked to right or left all the time n-
                    /* if too far away from the goal, stop kicking to left or right */
                    
                    // if((ball.getDistanceTo(VecPosition(15,-15,0))) < 15 ){
                    //     return kickBall(KICK_IK, VecPosition(HALF_FIELD_X+0.6, 0.6, 0));
                    // }else{
                    //     return kickBall(KICK_IK,ball+VecPosition(4.5,3,0)); //position of left foward/ if x set to 3, likely to collise with oppenent body
                    // }
                    // VecPosition left = getposition(FORWARD_RIGHT);
                    // VecPosition = getposition(FORWARD_RIGHT);

                    /* if too close to edge */
                    if((ball.getDistanceTo(VecPosition(-15,10,0)) < 5) || (ball.getDistanceTo(VecPosition(-10,10,0)) < 5) || (ball.getDistanceTo(VecPosition(-5,10,0)) < 5) 
                            || (ball.getDistanceTo(VecPosition(-5,10,0)) < 5) || (ball.getDistanceTo(VecPosition(0,10,0)) < 5) || (ball.getDistanceTo(VecPosition(5,10,0)) < 5)
                            || (ball.getDistanceTo(VecPosition(10,10,0)) < 5) || (ball.getDistanceTo(VecPosition(15,10,0)) < 5))
                    {
                        // return kickBall(KICK_IK, VecPosition(HALF_FIELD_X+0.6, 0.6, 0)); 
                        return kickBall(KICK_IK,ball+VecPosition(4.5,-3,0)); 
                    }
                    else
                    {
                        //return kickBall(KICK_IK,getposition(FORWARD_LEFT)+VecPosition(0.5,0.5,0));
                        return kickBall(KICK_IK,ball+VecPosition(4.5,3,0));
                    }         
                    // return kickBall(KICK_IK,ball+VecPosition(4.5,3,0));
                }
                else // kick right 
                {
                    left_right++;  //this make sure that the ball is not kicked to right or left all the time 
                    // if((ball.getDistanceTo(VecPosition(15,15,0))) < 15 ){
                    //     return kickBall(KICK_IK, VecPosition(HALF_FIELD_X+0.6, 0.6, 0));
                    // }else{
                    //     return kickBall(KICK_IK,ball+VecPosition(4.5,-3,0));
                    // }
                    /* if too close to edge */
                    if((ball.getDistanceTo(VecPosition(-15,-10,0)) < 5) || (ball.getDistanceTo(VecPosition(-10,-10,0)) < 5) || (ball.getDistanceTo(VecPosition(-5,-10,0)) < 5) 
                            || (ball.getDistanceTo(VecPosition(-5,-10,0)) < 5) || (ball.getDistanceTo(VecPosition(0,-10,0)) < 5) || (ball.getDistanceTo(VecPosition(5,-10,0)) < 5)
                            || (ball.getDistanceTo(VecPosition(10,-10,0)) < 5) || (ball.getDistanceTo(VecPosition(15,-10,0)) < 5))
                    {
                        //return kickBall(KICK_IK, VecPosition(HALF_FIELD_X+0.6, 0.6, 0)); 
                        return kickBall(KICK_IK,ball+VecPosition(4.5,3,0)); 

                    }
                    else
                    {
                        //return kickBall(KICK_IK,getposition(FORWARD_RIGHT)+VecPosition(0.5,0.5,0));
                        return kickBall(KICK_IK,ball+VecPosition(4.5,-3,0));
                    }        
                    // return kickBall(KICK_IK,ball+VecPosition(4.5,-3,0));
                }
                

                // return kickBall(KICK_FORWARD,ball+VecPosition(3,-2,0));
            }
            else // if there is no opponent in front, then kick to the target 
            {
                //return kickBall(KICK_FORWARD,ball+VecPosition(4.5,-3,0));
                if((ball.getDistanceTo(VecPosition(15,0,0))) < 4)
                {
                    //random attempt
                    return kickBall(KICK_IK, VecPosition(HALF_FIELD_X+0.6, 0.6, 0)); 
                    //return kickBall(KICK_IK, VecPosition(HALF_FIELD_X-0.6, 0.6, 0)); 

                }
                else 
                {
                    return kickBall(SKILL_WALK_OMNI,VecPosition(HALF_FIELD_X, 0, 0)); 
                    // return kickBall(KICK_DRIBBLE, VecPosition(HALF_FIELD_X, 0, 0)); 
                }
                
            }
            return kickBall(KICK_IK, VecPosition(HALF_FIELD_X+0.6, 0.6, 0)); // may need to try using KICK_LONG once KICK_LONG is impplemented 
        }
        // std::cout<<"start waiting"<<std::endl;
        // usleep(10);
        // std::cout<<"10 sec later"<<std::endl;
        
    }
    if (agentRoles[worldModel->getUNum()] == SUPPORTER)
    {
        // away behehind the ball whenever it is attacking or not
        if(checkingFall() != true){
            VecPosition space = VecPosition(0,0,0);
            space = ball + VecPosition(-1,1,0);  // try to get the supoorter go as near as possible to the ball 
            space.setX(max(-HALF_FIELD_X, min(HALF_FIELD_X, space.getX())));
            space.setY(max(-HALF_FIELD_Y, min(HALF_FIELD_Y, space.getY())));

            return goToSpace(space);
            //return goToSpace(ball + VecPosition(-1,1,0)); 
        }
    }
    if (agentRoles[worldModel->getUNum()] == FORWARD_LEFT)
    {
        if(checkingFall() != true){
            if(attacking == true)
            {
                VecPosition space = VecPosition(0,0,0);
                space = ball + VecPosition(3,2,0);
                space.setX(max(-HALF_FIELD_X, min(HALF_FIELD_X, space.getX())));
                space.setY(max(-HALF_FIELD_Y, min(HALF_FIELD_Y, space.getY())));

                return goToSpace(space); //move to the front so that we always have robot cloest to the ball after each kick 
            }
            else 
            {
                VecPosition space = VecPosition(0,0,0);
                space = ball + VecPosition(1,2,0);
                space.setX(max(-HALF_FIELD_X, min(HALF_FIELD_X, space.getX())));
                space.setY(max(-HALF_FIELD_Y, min(HALF_FIELD_Y, space.getY())));

                return goToSpace(space); //move to the front so that we always have robot cloest to the ball after each kick 
            }
            
        }
    }
    if (agentRoles[worldModel->getUNum()] == FORWARD_RIGHT) //can Increase the distance to the ball once we have a better long kick 
    {
        if(checkingFall() != true){
            if(attacking == true)
            {
                VecPosition space = VecPosition(0,0,0);
                space = ball + VecPosition(3,-2,0);
                space.setX(max(-HALF_FIELD_X, min(HALF_FIELD_X, space.getX())));
                space.setY(max(-HALF_FIELD_Y, min(HALF_FIELD_Y, space.getY())));

                return goToSpace(space); 
            }
            else 
            {
                VecPosition space = VecPosition(0,0,0);
                space = ball + VecPosition(1,-2,0);
                space.setX(max(-HALF_FIELD_X, min(HALF_FIELD_X, space.getX())));
                space.setY(max(-HALF_FIELD_Y, min(HALF_FIELD_Y, space.getY())));

                return goToSpace(space); 
            }
        }
    }
    if (agentRoles[worldModel->getUNum()] == WING_LEFT)
    {
        if(checkingFall() != true){
            if(attacking == true)
            {
                // return goToSpace(ball + VecPosition(1.5,2,0));
                VecPosition space = VecPosition(0,0,0);
                space = ball + VecPosition(1.5,3,0);
                space.setX(max(-HALF_FIELD_X, min(HALF_FIELD_X, space.getX())));
                space.setY(max(-HALF_FIELD_Y, min(HALF_FIELD_Y, space.getY())));

                return goToSpace(space);
                //return goToSpace(getSpaceForRole(agentRoles[worldModel->getUNum()]));
            }
            else 
            {
                VecPosition space = VecPosition(0,0,0);
                space = ball + VecPosition(-1.5,2,0);
                space.setX(max(-HALF_FIELD_X, min(HALF_FIELD_X, space.getX())));
                space.setY(max(-HALF_FIELD_Y, min(HALF_FIELD_Y, space.getY())));

                return goToSpace(space); 
            }
            
        }
    }
    if (agentRoles[worldModel->getUNum()] == WING_RIGHT)
    {

        if(checkingFall() != true){
            if(attacking == true)
            {
                    //return goToSpace(ball + VecPosition(1.5,-2,0));
                VecPosition space = VecPosition(0,0,0);
                space = ball + VecPosition(1.5,-3,0);
                space.setX(max(-HALF_FIELD_X, min(HALF_FIELD_X, space.getX())));
                space.setY(max(-HALF_FIELD_Y, min(HALF_FIELD_Y, space.getY())));

                return goToSpace(space); //move to the front so that we always have robot cloest to the ball after each kick 
                //return goToSpace(getSpaceForRole(agentRoles[worldModel->getUNum()]));
            }
            else
            {
                VecPosition space = VecPosition(0,0,0);
                space = ball + VecPosition(-1.5,-2,0);
                space.setX(max(-HALF_FIELD_X, min(HALF_FIELD_X, space.getX())));
                space.setY(max(-HALF_FIELD_Y, min(HALF_FIELD_Y, space.getY())));

                return goToSpace(space); 
            }
           
        }
    }
    if (agentRoles[worldModel->getUNum()] == BACK_LEFT) // no need to change 
    {
        if(checkingFall() != true){
           return goToSpace(getSpaceForRole(agentRoles[worldModel->getUNum()])); 
        }
    }
    if (agentRoles[worldModel->getUNum()] == BACK_RIGHT)
    {
        if(checkingFall() != true){
           return goToSpace(getSpaceForRole(agentRoles[worldModel->getUNum()])); 
        }
    }
    if (agentRoles[worldModel->getUNum()] == GOALIE)
    {
        if(checkingFall() != true){
            return goToSpace(getSpaceForRole(agentRoles[worldModel->getUNum()]));
        }
    }

    return goToSpace(getSpaceForRole(agentRoles[worldModel->getUNum()]));
}

int SimpleSoccerBehavior::getPlayerClosestToBall() {
    // Find closest player to ball
    int playerClosestToBall = -1;
    double closestDistanceToBall = 10000;
    
    for(int i = WO_TEAMMATE1; i < WO_TEAMMATE1+NUM_AGENTS; ++i) { //loop through all teamate 
        VecPosition temp; //vector position 
        int playerNum = i - WO_TEAMMATE1 + 1;

        if (worldModel->getUNum() == playerNum) {
            //check if not fall
            if(checkingFall()!=true)
            {
            // This is us
                temp = worldModel->getMyPosition();
            }
            else
            {
                continue;
            }
            
        } else {
            if(checkingFall() != true){
                WorldObject* teammate = worldModel->getWorldObject( i );
                if (teammate->validPosition) {
                    temp = teammate->pos;
                } else {
                    continue;
                }
            }
            else
            {
                continue;
            }

        }
        temp.setZ(0);

        double distanceToBall = temp.getDistanceTo(ball);

        if (distanceToBall < closestDistanceToBall) {
            playerClosestToBall = playerNum;
            closestDistanceToBall = distanceToBall;
        }
    }
    return playerClosestToBall;
}

map<int, Role> SimpleSoccerBehavior::getAgentRoles() {
    map<int, Role> agentRoles;
    set<Role> assignedRoles;
    agentRoles[getPlayerClosestToBall()] = ON_BALL;
    assignedRoles.insert(ON_BALL);

    if (!agentRoles.count(1)) {
        // Assign goalie the goalie role if goalie is not already assigned a role
        agentRoles[1] = GOALIE;          //goalie means goalkeeper
        assignedRoles.insert(GOALIE);
    }
    // Simple greedy role assignment of remaining unassigned roles
    typedef std::list<std::pair<double, std::pair<int, Role>>> dist_to_roles_list;
    dist_to_roles_list agentsDistancesToRoles;
    // try to get the players closest to the ball to be forward 
    for (int r = 0; r < NUM_ROLES; r++) {
        Role role = static_cast<Role>(r);
        if (assignedRoles.count(role)) {
            continue;
        }

        if (!agentRoles.count(9)) {
        // Assign FOWARD
        agentRoles[9] = FORWARD_LEFT;          
        assignedRoles.insert(FORWARD_LEFT);
        }

        if (!agentRoles.count(10)) {
        // Assign FORWARD
        agentRoles[10] = FORWARD_RIGHT;          
        assignedRoles.insert(FORWARD_RIGHT);
        }

        for (int i = 1; i <= NUM_AGENTS; i++) {
            if (agentRoles.count(i)) {
                continue;
            }
            agentsDistancesToRoles.push_back(make_pair(getAgentDistanceToRole(i, role), make_pair(i, role)));
        }
    }

    agentsDistancesToRoles.sort();

    for (dist_to_roles_list::iterator it = agentsDistancesToRoles.begin(); it != agentsDistancesToRoles.end(); ++it) {
        pair<int, Role> assignment = it->second;
        int agent = assignment.first;
        Role role = assignment.second;

        if (agentRoles.count(agent) || assignedRoles.count(role)) {
            continue;
        }
        agentRoles[agent] = role;
        assignedRoles.insert(role);

        if (agentRoles.size() == NUM_AGENTS) {
            break;
        }
    }

    return agentRoles;
}

VecPosition SimpleSoccerBehavior::getSpaceForRole(Role role) {
    VecPosition ball = worldModel->getBall();

    if (beamablePlayMode()) {
        ball = VecPosition(0, 0, 0);
    }

    ball.setZ(0);

    // Keep ball position on the field
    ball.setX(max(min(ball.getX(), HALF_FIELD_X), -HALF_FIELD_X));
    ball.setY(max(min(ball.getY(), HALF_FIELD_Y), -HALF_FIELD_Y));

    VecPosition space = VecPosition(0,0,0);
    //position for each robot 
    switch(role) {
    case ON_BALL:
        space = ball + VecPosition(-2,0,0);
        break;
    case GOALIE:
        space = VecPosition(-HALF_FIELD_X+0.5, 0, 0);
        break;
    case SUPPORTER:
        space = ball + VecPosition(-4,0,0);
        break;
    case BACK_LEFT:
        space = ball + VecPosition(-10,3,0);
        break;
    case BACK_RIGHT:
        space = ball + VecPosition(-10,-3,0);
        break;
    case MID_LEFT:
        space = ball + VecPosition(-4,5,0);
        break;
    case MID_RIGHT:
        space = ball + VecPosition(-4,-5,0);
        break;
    case WING_LEFT:
        space = ball + VecPosition(-5,3,0);
        break;
    case WING_RIGHT:
        space = ball + VecPosition(-5,-3,0);
        break;
    case FORWARD_LEFT:
        space = ball + VecPosition(0,5,0);
        break;
    case FORWARD_RIGHT:
        space = ball + VecPosition(0,-5,0);
        break;
    default:
        cerr << "Unknown role: " << role << "\n";
    }

    space.setX(max(-HALF_FIELD_X, min(HALF_FIELD_X, space.getX())));
    space.setY(max(-HALF_FIELD_Y, min(HALF_FIELD_Y, space.getY())));

    if (beamablePlayMode()) {
        // Stay within your own half on kickoffs
        space.setX(min(-0.5, space.getX()));
    }

    return space;
}

double SimpleSoccerBehavior::getAgentDistanceToRole(int uNum, Role role) {
    VecPosition temp;

    if (worldModel->getUNum() == uNum) {
        // This is us
        temp = worldModel->getMyPosition();
    } else {
        WorldObject* teammate = worldModel->getWorldObject( WO_TEAMMATE1 + uNum - 1 );
        temp = teammate->pos;
    }
    temp.setZ(0);

    double distanceToRole = temp.getDistanceTo(getSpaceForRole(role));
    return distanceToRole;
}

SkillType SimpleSoccerBehavior::goToSpace(VecPosition space) {
    const double SPACE_THRESH = 0.5;

    if (me.getDistanceTo(space) < SPACE_THRESH ) {
        return watchPosition(ball);
    }

    // Adjust target to not be too close to teammates or the ball
    VecPosition target = collisionAvoidance(true /*teammate*/, true/*opponent*/, false/*ball*/, 1/*proximity thresh*/, .5/*collision thresh*/, space, true/*keepDistance*/);
    return goToTarget(target);
}

SkillType SimpleSoccerBehavior::watchPosition(VecPosition pos) {
    const double POSITION_CENTER_THRESH = 10;
    VecPosition localPos = worldModel->g2l(pos);

    SIM::AngDeg localPosAngle = atan2Deg(localPos.getY(), localPos.getX());

    if (abs(localPosAngle) > POSITION_CENTER_THRESH) {
        return goToTargetRelative(VecPosition(), localPosAngle);
    }

    return SKILL_STAND;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
