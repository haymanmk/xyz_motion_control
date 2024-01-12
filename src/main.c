#include "core.h"

#define NUM_GCODE 3
#define MAX_STRING_LEN 80

// Global variables
system_t sys;
int32_t sys_position[N_AXIS];                    // Real-time machine (aka home) position vector in steps.
int32_t sys_probe_position[N_AXIS];              // Last probe position in machine coordinates and steps.
volatile uint8_t sys_probe_state;                // Probing state value.  Used to coordinate the probing cycle with stepper ISR.
volatile uint8_t sys_rt_exec_state;              // Global realtime executor bitflag variable for state management. See EXEC bitmasks.
volatile uint8_t sys_rt_exec_alarm;              // Global realtime executor bitflag variable for setting various alarms.
volatile uint8_t sys_rt_exec_motion_override;    // Global realtime executor bitflag variable for motion-based overrides.
volatile uint8_t sys_rt_exec_accessory_override; // Global realtime executor bitflag variable for spindle/coolant overrides.


void protocol_execute_realtime(void);

char G_CODE[NUM_GCODE][MAX_STRING_LEN] = {"G00X0Y0Z0F100", "G00X10Y10Z0", "G00 40Y-10Z0"};

int main(void)
{
    gc_init();

    for (;;)
    {
        for (uint8_t i = 0; i < NUM_GCODE; i++)
        {
            char line[MAX_STRING_LEN];
            strcpy(line, G_CODE[i]);

            protocol_execute_realtime();

            gc_execute_line(line);
        }
        protocol_execute_realtime();
    }

    return 0;
}
