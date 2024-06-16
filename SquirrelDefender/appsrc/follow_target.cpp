#ifdef USE_JETSON

/********************************************************************************
 * @file    follow_target.cpp
 * @author  Cameron Rose
 * @date    6/7/2023
 * @brief   Follow the target and maintain a specified x, y, z offset.
 ********************************************************************************/

/********************************************************************************
 * Includes
 ********************************************************************************/
#include "follow_target.h"

/********************************************************************************
 * Typedefs
 ********************************************************************************/

/********************************************************************************
 * Private macros and defines
 ********************************************************************************/

/********************************************************************************
 * Object definitions
 ********************************************************************************/
DebugTerm FollowData("/dev/pts/2");

float x_actual;
float height_actual;
float y_actual;
float width_actual;
float x_centroid_err;
float target_height_err;
float y_centroid_err;
float target_left_side;
float target_right_side;
float target_left_err;
float target_right_err;
float target_height_err_rev;

/********************************************************************************
 * Calibration definitions
 ********************************************************************************/
float Kp_x;
float Ki_x;
float Kd_x;
float Kp_y;
float Ki_y;
float Kd_y;
float w1_x;
float w2_x;
float w3_x;
float w1_y;
float w2_y;
float w3_y;
float w1_z;
float w2_z;
float w3_z;
float Kp_x_rev;
float Ki_x_rev;
float Kd_x_rev;
float w1_x_rev;
float w2_x_rev;
float w3_x_rev;
float Kp_y_rev;
float Ki_y_rev;
float Kd_y_rev;
float w1_y_rev;
float w2_y_rev;
float w3_y_rev;
float x_desired;
float height_desired;
float y_desired;
float width_desired;

/********************************************************************************
 * Function definitions
 ********************************************************************************/

/********************************************************************************
 * Function: Follow
 * Description: Follow class constructor.
 ********************************************************************************/
Follow::Follow(void){};

/********************************************************************************
 * Function: ~Follow
 * Description: Follow class destructor.
 ********************************************************************************/
Follow::~Follow(void){};

/********************************************************************************
 * Function: get_control_params
 * Description: Read follow control parameters from a json or other file type.
 ********************************************************************************/
void Follow::get_control_params(void)
{
    Parameters veh_params("../params.json");
    // Accessing Vel_PID_x parameters
    Kp_x = veh_params.get_float_param("Vel_PID_x", "Kp");
    Ki_x = veh_params.get_float_param("Vel_PID_x", "Ki");
    Kd_x = veh_params.get_float_param("Vel_PID_x", "Kd");
    w1_x = veh_params.get_float_param("Vel_PID_x", "w1");
    w2_x = veh_params.get_float_param("Vel_PID_x", "w2");
    w3_x = veh_params.get_float_param("Vel_PID_x", "w3");

    // Accessing Vel_PID_y parameters
    Kp_y = veh_params.get_float_param("Vel_PID_y", "Kp");
    Ki_y = veh_params.get_float_param("Vel_PID_y", "Ki");
    Kd_y = veh_params.get_float_param("Vel_PID_y", "Kd");
    w1_y = veh_params.get_float_param("Vel_PID_y", "w1");
    w2_y = veh_params.get_float_param("Vel_PID_y", "w2");
    w3_y = veh_params.get_float_param("Vel_PID_y", "w3");

    // Accessing Vel_PID_x parameters for reverse movement
    Kp_x_rev = veh_params.get_float_param("Vel_PID_x_reverse", "Kp");
    Ki_x_rev = veh_params.get_float_param("Vel_PID_x_reverse", "Ki");
    Kd_x_rev = veh_params.get_float_param("Vel_PID_x_reverse", "Kd");
    w1_x_rev = veh_params.get_float_param("Vel_PID_x_reverse", "w1");
    w2_x_rev = veh_params.get_float_param("Vel_PID_x_reverse", "w2");
    w3_x_rev = veh_params.get_float_param("Vel_PID_x_reverse", "w3");

    // Accessing Vel_PID_y parameters for reverse movment
    Kp_y_rev = veh_params.get_float_param("Vel_PID_y_reverse", "Kp");
    Ki_y_rev = veh_params.get_float_param("Vel_PID_y_reverse", "Ki");
    Kd_y_rev = veh_params.get_float_param("Vel_PID_y_reverse", "Kd");
    w1_y_rev = veh_params.get_float_param("Vel_PID_y_reverse", "w1");
    w2_y_rev = veh_params.get_float_param("Vel_PID_y_reverse", "w2");
    w3_y_rev = veh_params.get_float_param("Vel_PID_y_reverse", "w3");
}

/********************************************************************************
 * Function: get_desired_target_size
 * Description: Read follow target parameters from a json or other file type.
 ********************************************************************************/
void Follow::get_desired_target_size(void)
{
    Parameters target_params("../params.json");

    x_desired = static_cast<float>(input_video_height)/2.0;
    y_desired = static_cast<float>(input_video_width)/2.0;
    height_desired = target_params.get_float_param("Target", "Desired_Height");
    width_desired = target_params.get_float_param("Target", "Desired_Width");
}

/********************************************************************************
 * Function: calc_target_size
 * Description: Calculate the parameters of a target.
 ********************************************************************************/
void Follow::calc_target_size(int n)
{
    x_actual = detections[n].Height()/2.0 + detections[n].Top;
    height_actual = detections[n].Height();
    y_actual = detections[n].Width()/2.0 + detections[n].Left;
    width_actual = detections[n].Width();
    target_left_side = detections[n].Left;
    target_right_side = detections[n].Right;
}

/********************************************************************************
 * Function: calc_follow_error
 * Description: Calculate the error of a target's position based 
 ********************************************************************************/
void Follow::calc_follow_error(void)
{
    float bounding_box_left_side = 320.0;
    float bounding_box_right_side = 960.0;

    target_left_err = (bounding_box_left_side - target_left_side);
    target_right_err = (bounding_box_right_side - target_right_side);
    x_centroid_err = x_desired - x_actual;
    target_height_err = height_desired - height_actual;
    y_centroid_err = y_actual - y_desired;
}

/********************************************************************************
 * Function: follow_target_init
 * Description: Initialize all follow target variables.  Run once at the start
 *              of the program.
 ********************************************************************************/
bool Follow::follow_target_init(void)
{
    Kp_x = 0.0;
    Ki_x = 0.0;
    Kd_x = 0.0;
    Kp_y = 0.0;
    Ki_y = 0.0;
    Kd_y = 0.0;
    w1_x = 0.0;
    w2_x = 0.0;
    w3_x = 0.0;
    w1_y = 0.0;
    w2_y = 0.0;
    w3_y = 0.0;
    w1_z = 0.0;
    w2_z = 0.0;
    w3_z = 0.0;
    Kp_x_rev = 0.0;
    Ki_x_rev = 0.0;
    Kd_x_rev = 0.0;
    w1_x_rev = 0.0;
    w2_x_rev = 0.0;
    w3_x_rev = 0.0;
    Kp_y_rev = 0.0;
    Ki_y_rev = 0.0;
    Kd_y_rev = 0.0;
    w1_y_rev = 0.0;
    w2_y_rev = 0.0;
    w3_y_rev = 0.0;
    x_desired = 0;
    x_actual = 0.0;
    height_desired = 0.0;
    height_actual = 0.0;
    y_desired = 0;
    y_actual = 0.0;
    width_desired = 0.0;
    width_actual = 0.0;
    x_centroid_err = 0.0;
    target_height_err = 0.0;
    y_centroid_err = 0.0;
    target_left_side = 0.0;
    target_right_side = 0.0;
    target_left_err = 0.0;
    target_right_err = 0.0;
    target_height_err_rev = 0.0;

    get_control_params();
    get_desired_target_size();

    return true;
}

/********************************************************************************
 * Function: follow_target_loop
 * Description: Control vehicle to follow a designated target at a specific
 *              distance.
 ********************************************************************************/
void Follow::follow_target_loop(void)
{
    float target_velocity[3] = {0.0,0.0,0.0};

    get_control_params();
    get_desired_target_size();

    for( int n=0; n < numDetections; n++ )
    {		
        if (detections[n].TrackID >= 0 && detections[n].ClassID == 1 && detections[n].Confidence > 0.5)
        {
            calc_target_size(n);
            calc_follow_error();        
            
            if (height_actual > height_desired)
            {
                PID pid_rev;

                float vx_adjust = pid_rev.pid_controller_3d(Kp_x_rev, Ki_x_rev, Kd_x_rev, 
                                                    target_height_err, 0.0, 0.0, 
                                                    w1_x_rev, w2_x_rev, 0.0, CONTROL_DIM::X);
                float vy_adjust = pid_rev.pid_controller_3d(Kp_y_rev, Ki_y_rev, Kd_y_rev, 
                                                    y_centroid_err, 0.0, 0.0, 
                                                    w1_y_rev, w2_y_rev, 0.0, CONTROL_DIM::Y);

                target_velocity[0] = vx_adjust;
                target_velocity[1] = vy_adjust; 

                FollowData.cpp_cout("Target too close...PID (x,y): " + std::to_string(target_velocity[0]) + ", " + 
                                                               std::to_string(target_velocity[1]));

                VehicleController::cmd_velocity_xy_NED(target_velocity);
            }
            else
            {
                PID pid_forwd;
                
                float vx_adjust = pid_forwd.pid_controller_3d(Kp_x, Ki_x, Kd_x, 
                                                    x_centroid_err, target_height_err, 0.0, 
                                                    w1_x, w2_x, w3_x, CONTROL_DIM::X);
                float vy_adjust = pid_forwd.pid_controller_3d(Kp_y, Ki_y, Kd_y, 
                                                    y_centroid_err, 0.0, 0.0, 
                                                    w1_y, w2_y, w3_y, CONTROL_DIM::Y);

                FollowData.cpp_cout("Target too far...PID (x,y): " + std::to_string(vx_adjust) + ", " + 
                                                             std::to_string(vy_adjust));

                target_velocity[0] = vx_adjust;
                target_velocity[1] = vy_adjust; 

                VehicleController::cmd_velocity_NED(target_velocity);
            }
        }
    }
}

#endif // USE_JETSON