/**
 * core.h - main and common include files
 */

#ifndef CORE_H
#define CORE_H

// Firmware Version
#define FW_VERSION "0.0.1"

// Standard libraries
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

// System required libraries
#include "config.h"
#include "nuts_bolts.h"
#include "system.h"
#include "cpu_map.h"
#include "settings.h"
#include "planner.h"
#include "gcode.h"
#include "protocol.h"
#include "motion_control.h"
#include "stepper.h"
#include "report.h"

// Global variables
system_t sys;
int32_t sys_position[N_AXIS];                    // Real-time machine (aka home) position vector in steps.
int32_t sys_probe_position[N_AXIS];              // Last probe position in machine coordinates and steps.
volatile uint8_t sys_probe_state;                // Probing state value.  Used to coordinate the probing cycle with stepper ISR.
volatile uint8_t sys_rt_exec_state;              // Global realtime executor bitflag variable for state management. See EXEC bitmasks.
volatile uint8_t sys_rt_exec_alarm;              // Global realtime executor bitflag variable for setting various alarms.
volatile uint8_t sys_rt_exec_motion_override;    // Global realtime executor bitflag variable for motion-based overrides.
volatile uint8_t sys_rt_exec_accessory_override; // Global realtime executor bitflag variable for spindle/coolant overrides.

#endif // CORE_H