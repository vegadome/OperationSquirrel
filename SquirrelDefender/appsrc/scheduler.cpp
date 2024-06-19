/********************************************************************************
 * @file    scheduler.cpp
 * @author  Cameron Rose
 * @date    6/7/2023
 * @brief   Create the task(s) that run at the specified rates.
 ********************************************************************************/

/********************************************************************************
 * Includes
 ********************************************************************************/
#include "scheduler.h"

/********************************************************************************
 * Typedefs
 ********************************************************************************/

/********************************************************************************
 * Private macros and defines
 ********************************************************************************/

/********************************************************************************
 * Object definitions
 ********************************************************************************/
timer_t timerID;
struct sigaction sa;
struct sigevent sev;
struct itimerspec its;

/********************************************************************************
 * Calibration definitions
 ********************************************************************************/

/********************************************************************************
 * Function definitions
 ********************************************************************************/
