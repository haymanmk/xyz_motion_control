#include "core.h"

void protocol_execute_realtime()
{
    st_prep_buffer();
}

void protocol_exec_rt_system()
{
    protocol_execute_realtime();
}

void protocol_auto_cycle_start()
{
    // do nothing;
}

// Block until all buffered steps are executed or in a cycle state. Works with feed hold
// during a synchronize call, if it should happen. Also, waits for clean cycle end.
void protocol_buffer_synchronize()
{
    // If system is queued, ensure cycle resumes if the auto start flag is present.
    protocol_auto_cycle_start();
    do
    {
        protocol_execute_realtime(); // Check and execute run-time commands
        if (sys.abort)
        {
            return;
        } // Check for system abort
    } while (plan_get_current_block() || (sys.state == STATE_CYCLE));
}