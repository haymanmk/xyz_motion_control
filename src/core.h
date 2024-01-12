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

#endif // CORE_H