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
#include "sim_flight_test_4_VelocityControl.h"

/********************************************************************************
 * Typedefs
 ********************************************************************************/

/********************************************************************************
 * Private macros and defines
 ********************************************************************************/

/********************************************************************************
 * Object definitions
 ********************************************************************************/
DebugTerm VehStateInfo("");
bool takeoff_dbc;
bool start_follow_mode;
uint16_t takeoff_dbc_cnt;

/********************************************************************************
 * Calibration definitions
 ********************************************************************************/

/********************************************************************************
 * Function definitions
 ********************************************************************************/
void follow_mode(void);

#ifdef ENABLE_CV

/********************************************************************************
 * Function: follow_target
 * Description: Pass control outputs from the follow algorithm to the vehicle.
 ********************************************************************************/
void follow_mode(void)
{
    float target_velocity[3] = {0.0, 0.0, 0.0};

    target_velocity[0] = vx_adjust;
    target_velocity[1] = vy_adjust;
    target_velocity[2] = vz_adjust;

    if (target_valid && target_too_close)
    {
        VelocityController::cmd_velocity_xy_NED(target_velocity);
    }
    else if (target_valid && !target_too_close)
    {
        VelocityController::cmd_velocity_NED(target_velocity);
    }
    else
    {
        VelocityController::cmd_velocity_NED(target_velocity);
    }
}

#endif // ENABLE_CV

/********************************************************************************
 * Function: VehicleController
 * Description: Constructor of the VehicleController class.
 ********************************************************************************/
VehicleController::VehicleController(void) {}

/********************************************************************************
 * Function: VehicleController
 * Description: Constructor of the VehicleController class.
 ********************************************************************************/
VehicleController::~VehicleController(void) {}

/********************************************************************************
 * Function: init
 * Description: Initial setup of vehicle controller.
 ********************************************************************************/
bool VehicleController::init(void)
{
    takeoff_dbc = false;
    start_follow_mode = false;
    takeoff_dbc_cnt = 500;

    return true;
}

/********************************************************************************
 * Function: loop
 * Description: Vehicle controller main loop.  Handles all
 ********************************************************************************/
void VehicleController::loop(void)
{
    if (system_state == SYSTEM_STATE::INIT)
    {
        MavCmd::set_mode_GUIDED();
    }
    else if (system_state == SYSTEM_STATE::PRE_ARM_GOOD)
    {
        MavCmd::arm_vehicle();
    }
    else if (system_state == SYSTEM_STATE::STANDBY)
    {
        MavCmd::takeoff_GPS_long((float)4.0);
    }
    else if (system_state == SYSTEM_STATE::IN_FLIGHT_GOOD)
    {
        /* Debounce counter to avoid sending vehicle commands before the vehicle is in position */
        if (takeoff_dbc_cnt > 0)
        {
            takeoff_dbc_cnt--;
        }
        else
        {
            takeoff_dbc_cnt = 0;
            takeoff_dbc = true;
        }

        if (takeoff_dbc && mav_veh_rngfdr_current_distance > 300 || mav_veh_rel_alt > 3000)
        {
            start_follow_mode = true;
        }

        if (start_follow_mode)
        {
            follow_mode();
            //test_flight();
        }
    }
}

/********************************************************************************
 * Function: shutdown
 * Description: Code needed to shutdown the vehicle controller.
 ********************************************************************************/
void VehicleController::shutdown(void)
{
    MavCmd::set_mode_LAND();
}
