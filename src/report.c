/*
  report.c - reporting and messaging methods
  Part of Grbl

  Copyright (c) 2012-2016 Sungeun K. Jeon for Gnea Research LLC

  Grbl is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Grbl is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Grbl.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
  This file functions as the primary feedback interface for Grbl. Any outgoing data, such
  as the protocol status messages, feedback messages, and status reports, are stored here.
  For the most part, these functions primarily are called from protocol.c methods. If a
  different style feedback is desired (i.e. JSON), then a user can change these following
  methods to accomodate their needs.
*/

#include "core.h"

// Internal report utilities to reduce flash with repetitive tasks turned into functions.
void report_util_setting_prefix(uint8_t n)
{
  // do nothing
}
static void report_util_line_feed()
{
  // do nothing
}
static void report_util_feedback_line_feed()
{
  // do nothing
}
static void report_util_gcode_modes_G()
{
  // do nothing
}
static void report_util_gcode_modes_M()
{
  // do nothing
}
// static void report_util_comment_line_feed() { serial_write(')'); report_util_line_feed(); }
static void report_util_axis_values(float *axis_value)
{
  // do nothing
}

static void report_util_uint8_setting(uint8_t n, int val)
{
  // do nothing
}
static void report_util_float_setting(uint8_t n, float val, uint8_t n_decimal)
{
  // do nothing
}

// Handles the primary confirmation protocol response for streaming interfaces and human-feedback.
// For every incoming line, this method responds with an 'ok' for a successful command or an
// 'error:'  to indicate some error event with the line or some critical system error during
// operation. Errors events can originate from the g-code parser, settings module, or asynchronously
// from a critical error, such as a triggered hard limit. Interface should always monitor for these
// responses.
void report_status_message(uint8_t status_code)
{
  // do nothing
}

// Prints alarm messages.
void report_alarm_message(uint8_t alarm_code)
{
  // do nothing
}

// Prints feedback messages. This serves as a centralized method to provide additional
// user feedback for things that are not of the status/alarm message protocol. These are
// messages such as setup warnings, switch toggling, and how to exit alarms.
// NOTE: For interfaces, messages are always placed within brackets. And if silent mode
// is installed, the message number codes are less than zero.
void report_feedback_message(uint8_t message_code)
{
  // do nothing
}

// Welcome message
void report_init_message()
{
  // do nothing
}

// Grbl help message
void report_grbl_help()
{
  // do nothing
}

// Grbl global settings print out.
// NOTE: The numbering scheme here must correlate to storing in settings.c
void report_grbl_settings()
{
  // do nothing
}

// Prints current probe parameters. Upon a probe command, these parameters are updated upon a
// successful probe or upon a failed probe with the G38.3 without errors command (if supported).
// These values are retained until Grbl is power-cycled, whereby they will be re-zeroed.
void report_probe_parameters()
{
  // do nothing
}

// Prints Grbl NGC parameters (coordinate offsets, probing)
void report_ngc_parameters()
{
  // do nothing
}

// Print current gcode parser mode state
void report_gcode_modes()
{
  // do nothing
}

// Prints specified startup line
void report_startup_line(uint8_t n, char *line)
{
  // do nothing
}

void report_execute_startup_message(char *line, uint8_t status_code)
{
  // do nothing
}

// Prints build info line
void report_build_info(char *line)
{
  // do nothing
}

// Prints the character string line Grbl has received from the user, which has been pre-parsed,
// and has been sent into protocol_execute_line() routine to be executed by Grbl.
void report_echo_line_received(char *line)
{
  // do nothing
}

// Prints real-time data. This function grabs a real-time snapshot of the stepper subprogram
// and the actual location of the CNC machine. Users may change the following function to their
// specific needs, but the desired real-time data report must be as short as possible. This is
// requires as it minimizes the computational overhead and allows grbl to keep running smoothly,
// especially during g-code programs with fast, short line segments and high frequency reports (5-20Hz).
void report_realtime_status()
{
  // do nothing
}

#ifdef DEBUG
void report_realtime_debug()
{
}
#endif
