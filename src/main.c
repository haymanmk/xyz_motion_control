#include "core.h"

#define NUM_GCODE 3
#define MAX_STRING_LEN 80

void protocol_execute_realtime(void);

char G_CODE[NUM_GCODE][MAX_STRING_LEN] = {"G00 X0Y0Z0", "G00 X10Y10Z0", "G00 X40Y-10Z0"};

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
