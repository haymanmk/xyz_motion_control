/*
  settings.c - eeprom configuration handling
  Part of Grbl

  Copyright (c) 2011-2016 Sungeun K. Jeon for Gnea Research LLC
  Copyright (c) 2009-2011 Simen Svale Skogsrud

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

#include "core.h"
#include "defaults.h"

settings_t settings;

// The `defaults` variable is intentionally stored in the Flash memory
// by using `__flash` which is a proprietary keyword to declare a variable in Program Memory.
// const __flash settings_t defaults = {
const settings_t defaults = {
    .pulse_microseconds = DEFAULT_STEP_PULSE_MICROSECONDS,
    .stepper_idle_lock_time = DEFAULT_STEPPER_IDLE_LOCK_TIME,
    .step_invert_mask = DEFAULT_STEPPING_INVERT_MASK,
    .dir_invert_mask = DEFAULT_DIRECTION_INVERT_MASK,
    .status_report_mask = DEFAULT_STATUS_REPORT_MASK,
    .junction_deviation = DEFAULT_JUNCTION_DEVIATION,
    .arc_tolerance = DEFAULT_ARC_TOLERANCE,
    .rpm_max = DEFAULT_SPINDLE_RPM_MAX,
    .rpm_min = DEFAULT_SPINDLE_RPM_MIN,
    .homing_dir_mask = DEFAULT_HOMING_DIR_MASK,
    .homing_feed_rate = DEFAULT_HOMING_FEED_RATE,
    .homing_seek_rate = DEFAULT_HOMING_SEEK_RATE,
    .homing_debounce_delay = DEFAULT_HOMING_DEBOUNCE_DELAY,
    .homing_pulloff = DEFAULT_HOMING_PULLOFF,
    .flags = (DEFAULT_REPORT_INCHES << BIT_REPORT_INCHES) |
             (DEFAULT_LASER_MODE << BIT_LASER_MODE) |
             (DEFAULT_INVERT_ST_ENABLE << BIT_INVERT_ST_ENABLE) |
             (DEFAULT_HARD_LIMIT_ENABLE << BIT_HARD_LIMIT_ENABLE) |
             (DEFAULT_HOMING_ENABLE << BIT_HOMING_ENABLE) |
             (DEFAULT_SOFT_LIMIT_ENABLE << BIT_SOFT_LIMIT_ENABLE) |
             (DEFAULT_INVERT_LIMIT_PINS << BIT_INVERT_LIMIT_PINS) |
             (DEFAULT_INVERT_PROBE_PIN << BIT_INVERT_PROBE_PIN),
    .steps_per_mm[X_AXIS] = DEFAULT_X_STEPS_PER_MM,
    .steps_per_mm[Y_AXIS] = DEFAULT_Y_STEPS_PER_MM,
    .steps_per_mm[Z_AXIS] = DEFAULT_Z_STEPS_PER_MM,
    .max_rate[X_AXIS] = DEFAULT_X_MAX_RATE,
    .max_rate[Y_AXIS] = DEFAULT_Y_MAX_RATE,
    .max_rate[Z_AXIS] = DEFAULT_Z_MAX_RATE,
    .acceleration[X_AXIS] = DEFAULT_X_ACCELERATION,
    .acceleration[Y_AXIS] = DEFAULT_Y_ACCELERATION,
    .acceleration[Z_AXIS] = DEFAULT_Z_ACCELERATION,
    .max_travel[X_AXIS] = (-DEFAULT_X_MAX_TRAVEL),
    .max_travel[Y_AXIS] = (-DEFAULT_Y_MAX_TRAVEL),
    .max_travel[Z_AXIS] = (-DEFAULT_Z_MAX_TRAVEL)};

/**
 * TODO:
 * - Build features to facilitate Flash memory reading on target board.
 */
/*
// Method to store startup lines into EEPROM
void settings_store_startup_line(uint8_t n, char *line)
{
#ifdef FORCE_BUFFER_SYNC_DURING_EEPROM_WRITE
  protocol_buffer_synchronize(); // A startup line may contain a motion and be executing.
#endif
  uint32_t addr = n * (LINE_BUFFER_SIZE + 1) + EEPROM_ADDR_STARTUP_BLOCK;
  memcpy_to_eeprom_with_checksum(addr, (char *)line, LINE_BUFFER_SIZE);
}

// Method to store build info into EEPROM
// NOTE: This function can only be called in IDLE state.
void settings_store_build_info(char *line)
{
  // Build info can only be stored when state is IDLE.
  memcpy_to_eeprom_with_checksum(EEPROM_ADDR_BUILD_INFO, (char *)line, LINE_BUFFER_SIZE);
}
*/

// Method to store coord data parameters into EEPROM
void settings_write_coord_data(uint8_t coord_select, float *coord_data)
{
#ifdef FORCE_BUFFER_SYNC_DURING_EEPROM_WRITE
  protocol_buffer_synchronize();
#endif
  uint32_t addr = coord_select * (sizeof(float) * N_AXIS + 1) + EEPROM_ADDR_PARAMETERS;
  // memcpy_to_eeprom_with_checksum(addr, (char *)coord_data, sizeof(float) * N_AXIS);
}

// Method to store Grbl global settings struct and version number into EEPROM
// NOTE: This function can only be called in IDLE state.
void write_global_settings()
{
  return;
}

// Method to restore EEPROM-saved Grbl global settings back to defaults.
void settings_restore(uint8_t restore_flag)
{
  return;
}

// Reads startup line from EEPROM. Updated pointed line string data.
uint8_t settings_read_startup_line(uint8_t n, char *line)
{
  return (true);
}

// Reads startup line from EEPROM. Updated pointed line string data.
uint8_t settings_read_build_info(char *line)
{
  return (true);
}

// Read selected coordinate data from EEPROM. Updates pointed coord_data value.
uint8_t settings_read_coord_data(uint8_t coord_select, float *coord_data)
{
  uint32_t addr = coord_select * (sizeof(float) * N_AXIS + 1) + EEPROM_ADDR_PARAMETERS;
  // if (!(memcpy_from_eeprom_with_checksum((char *)coord_data, addr, sizeof(float) * N_AXIS)))
  // {
  //   // Reset with default zero vector
  //   clear_vector_float(coord_data);
  //   settings_write_coord_data(coord_select, coord_data);
  //   return (false);
  // }
  return (true);
}

// Reads Grbl global settings struct from EEPROM.
uint8_t read_global_settings()
{
  return (true);
}

// A helper method to set settings from command line
uint8_t settings_store_global_setting(uint8_t parameter, float value)
{
  return (STATUS_OK);
}

// Initialize the config subsystem
void settings_init()
{
  return;
}

// Returns step pin mask according to Grbl internal axis indexing.
uint8_t get_step_pin_mask(uint8_t axis_idx)
{
  if (axis_idx == X_AXIS)
  {
    return ((1 << X_STEP_BIT));
  }
  if (axis_idx == Y_AXIS)
  {
    return ((1 << Y_STEP_BIT));
  }
  return ((1 << Z_STEP_BIT));
}

// Returns direction pin mask according to Grbl internal axis indexing.
uint8_t get_direction_pin_mask(uint8_t axis_idx)
{
  if (axis_idx == X_AXIS)
  {
    return ((1 << X_DIRECTION_BIT));
  }
  if (axis_idx == Y_AXIS)
  {
    return ((1 << Y_DIRECTION_BIT));
  }
  return ((1 << Z_DIRECTION_BIT));
}

// Returns limit pin mask according to Grbl internal axis indexing.
uint8_t get_limit_pin_mask(uint8_t axis_idx)
{
  if (axis_idx == X_AXIS)
  {
    return ((1 << X_LIMIT_BIT));
  }
  if (axis_idx == Y_AXIS)
  {
    return ((1 << Y_LIMIT_BIT));
  }
  return ((1 << Z_LIMIT_BIT));
}