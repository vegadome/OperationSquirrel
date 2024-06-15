/********************************************************************************
 * @file    vehicle_controller.cpp
 * @author  Cameron Rose
 * @date    6/7/2023
 * @brief   Control the position, velocity, and acceleration of the drone by 
 *          sending the following MAVLINK message to the drone.  Control the
 *          vector position, velocity, acceleration, and yaw/yaw rate.
 * 
 ********************************************************************************/

/********************************************************************************
 * Includes
 ********************************************************************************/
#include "vehicle_controller.h"

/********************************************************************************
 * Typedefs
 ********************************************************************************/

/********************************************************************************
 * Private macros and defines
 ********************************************************************************/

/********************************************************************************
 * Object definitions
 ********************************************************************************/

/********************************************************************************
 * Calibration definitions
 ********************************************************************************/

/********************************************************************************
 * Function definitions
 ********************************************************************************/

/********************************************************************************
* Function: VehicleController
* Description: Constructor of the VehicleController class.
********************************************************************************/
VehicleController::VehicleController(void){}

/********************************************************************************
* Function: VehicleController
* Description: Constructor of the VehicleController class.
********************************************************************************/
VehicleController::~VehicleController(void){}

/********************************************************************************
 * Function: cmd_position_NED
 * Description: Move to an x,y,z coordinate in the NED frame.
 ********************************************************************************/
void VehicleController::cmd_position_NED(float position_target[3]) 
{
    VelocityController::cmd_position_NED(position_target);
}

/********************************************************************************
 * Function: cmd_velocity_NED
 * Description: Move in direction of vector vx,vy,vz in the NED frame.
 ********************************************************************************/
void VehicleController::cmd_velocity_NED(float velocity_target[3]) 
{
    VelocityController::cmd_velocity_NED(velocity_target);
}

/********************************************************************************
 * Function: cmd_velocity_xy_NED
 * Description: Move in xy plane given a vector vx,vy in the NED frame.
 ********************************************************************************/
void VehicleController::cmd_velocity_xy_NED(float velocity_target[3]) 
{
    VelocityController::cmd_velocity_xy_NED(velocity_target);
}

/********************************************************************************
 * Function: cmd_velocity_x_NED
 * Description: Move in direction of vector vx in the NED frame.
 ********************************************************************************/
void VehicleController::cmd_velocity_x_NED(float velocity_target) 
{
    VelocityController::cmd_velocity_x_NED(velocity_target);
}

/********************************************************************************
 * Function: cmd_velocity_y_NED
 * Description: Move in direction of vector vy in the NED frame.
 ********************************************************************************/
void VehicleController::cmd_velocity_y_NED(float velocity_target) 
{
    VelocityController::cmd_velocity_y_NED(velocity_target);
}

/********************************************************************************
 * Function: cmd_acceleration_NED
 * Description: Move in direction of vector ax,ay,az in the NED frame.
 ********************************************************************************/
void VehicleController::cmd_acceleration_NED(float acceleration_target[3]) 
{
    VelocityController::cmd_acceleration_NED(acceleration_target);
}