/*
  system.c - Handles system level commands and real-time processes
  Part of Grbl

  Copyright (c) 2014-2016 Sungeun K. Jeon for Gnea Research LLC

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

void system_init()
{
  CONTROL_DDR &= ~(CONTROL_MASK); // Configure as input pins
#ifdef DISABLE_CONTROL_PIN_PULL_UP
  CONTROL_PORT &= ~(CONTROL_MASK); // Normal low operation. Requires external pull-down.
#else
  CONTROL_PORT |= CONTROL_MASK; // Enable internal pull-up resistors. Normal high operation.
#endif
  CONTROL_PCMSK |= CONTROL_MASK; // Enable specific pins of the Pin Change Interrupt
  PCICR |= (1 << CONTROL_INT);   // Enable Pin Change Interrupt
}

// Returns control pin state as a uint8 bitfield. Each bit indicates the input pin state, where
// triggered is 1 and not triggered is 0. Invert mask is applied. Bitfield organization is
// defined by the CONTROL_PIN_INDEX in the header file.
uint8_t system_control_get_state()
{
  uint8_t control_state = 0;
  uint8_t pin = (CONTROL_PIN & CONTROL_MASK) ^ CONTROL_MASK;
#ifdef INVERT_CONTROL_PIN_MASK
  pin ^= INVERT_CONTROL_PIN_MASK;
#endif
  if (pin)
  {
#ifdef ENABLE_SAFETY_DOOR_INPUT_PIN
    if (bit_istrue(pin, (1 << CONTROL_SAFETY_DOOR_BIT)))
    {
      control_state |= CONTROL_PIN_INDEX_SAFETY_DOOR;
    }
#else
    if (bit_istrue(pin, (1 << CONTROL_FEED_HOLD_BIT)))
    {
      control_state |= CONTROL_PIN_INDEX_FEED_HOLD;
    }
#endif
    if (bit_istrue(pin, (1 << CONTROL_RESET_BIT)))
    {
      control_state |= CONTROL_PIN_INDEX_RESET;
    }
    if (bit_istrue(pin, (1 << CONTROL_CYCLE_START_BIT)))
    {
      control_state |= CONTROL_PIN_INDEX_CYCLE_START;
    }
  }
  return (control_state);
}

// Pin change interrupt for pin-out commands, i.e. cycle start, feed hold, and reset. Sets
// only the realtime command execute variable to have the main program execute these when
// its ready. This works exactly like the character-based realtime commands when picked off
// directly from the incoming serial data stream.
/*
ISR(CONTROL_INT_vect)
{
  uint8_t pin = system_control_get_state();
  if (pin)
  {
    if (bit_istrue(pin, CONTROL_PIN_INDEX_RESET))
    {
      mc_reset();
    }
    if (bit_istrue(pin, CONTROL_PIN_INDEX_CYCLE_START))
    {
      bit_true(sys_rt_exec_state, EXEC_CYCLE_START);
    }
#ifndef ENABLE_SAFETY_DOOR_INPUT_PIN
    if (bit_istrue(pin, CONTROL_PIN_INDEX_FEED_HOLD))
    {
      bit_true(sys_rt_exec_state, EXEC_FEED_HOLD);
#else
    if (bit_istrue(pin, CONTROL_PIN_INDEX_SAFETY_DOOR))
    {
      bit_true(sys_rt_exec_state, EXEC_SAFETY_DOOR);
#endif
    }
  }
}
*/

// Returns if safety door is ajar(T) or closed(F), based on pin state.
uint8_t system_check_safety_door_ajar()
{
#ifdef ENABLE_SAFETY_DOOR_INPUT_PIN
  return (system_control_get_state() & CONTROL_PIN_INDEX_SAFETY_DOOR);
#else
  return (false); // Input pin not enabled, so just return that it's closed.
#endif
}

// Executes user startup script, if stored.
void system_execute_startup(char *line)
{
  uint8_t n;
  for (n = 0; n < N_STARTUP_LINE; n++)
  {
    if (!(settings_read_startup_line(n, line)))
    {
      line[0] = 0;
      report_execute_startup_message(line, STATUS_SETTING_READ_FAIL);
    }
    else
    {
      if (line[0] != 0)
      {
        uint8_t status_code = gc_execute_line(line);
        report_execute_startup_message(line, status_code);
      }
    }
  }
}

// Directs and executes one line of formatted input from protocol_process. While mostly
// incoming streaming g-code blocks, this also executes Grbl internal commands, such as
// settings, initiating the homing cycle, and toggling switch states. This differs from
// the realtime command module by being susceptible to when Grbl is ready to execute the
// next line during a cycle, so for switches like block delete, the switch only effects
// the lines that are processed afterward, not necessarily real-time during a cycle,
// since there are motions already stored in the buffer. However, this 'lag' should not
// be an issue, since these commands are not typically used during a cycle.
uint8_t system_execute_line(char *line)
{
  return (STATUS_OK); // If '$' command makes it to here, then everything's ok.
}

void system_flag_wco_change()
{
  return;
}

// Returns machine position of axis 'idx'. Must be sent a 'step' array.
// NOTE: If motor steps and machine position are not in the same coordinate frame, this function
//   serves as a central place to compute the transformation.
float system_convert_axis_steps_to_mpos(int32_t *steps, uint8_t idx)
{
  float pos;
#ifdef COREXY
  if (idx == X_AXIS)
  {
    pos = (float)system_convert_corexy_to_x_axis_steps(steps) / settings.steps_per_mm[idx];
  }
  else if (idx == Y_AXIS)
  {
    pos = (float)system_convert_corexy_to_y_axis_steps(steps) / settings.steps_per_mm[idx];
  }
  else
  {
    pos = steps[idx] / settings.steps_per_mm[idx];
  }
#else
  pos = steps[idx] / settings.steps_per_mm[idx];
#endif
  return (pos);
}

void system_convert_array_steps_to_mpos(float *position, int32_t *steps)
{
  uint8_t idx;
  for (idx = 0; idx < N_AXIS; idx++)
  {
    position[idx] = system_convert_axis_steps_to_mpos(steps, idx);
  }
  return;
}

// CoreXY calculation only. Returns x or y-axis "steps" based on CoreXY motor steps.
#ifdef COREXY
int32_t system_convert_corexy_to_x_axis_steps(int32_t *steps)
{
  return ((steps[A_MOTOR] + steps[B_MOTOR]) / 2);
}
int32_t system_convert_corexy_to_y_axis_steps(int32_t *steps)
{
  return ((steps[A_MOTOR] - steps[B_MOTOR]) / 2);
}
#endif

// Checks and reports if target array exceeds machine travel limits.
uint8_t system_check_travel_limits(float *target)
{
  uint8_t idx;
  for (idx = 0; idx < N_AXIS; idx++)
  {
#ifdef HOMING_FORCE_SET_ORIGIN
    // When homing forced set origin is enabled, soft limits checks need to account for directionality.
    // NOTE: max_travel is stored as negative
    if (bit_istrue(settings.homing_dir_mask, bit(idx)))
    {
      if (target[idx] < 0 || target[idx] > -settings.max_travel[idx])
      {
        return (true);
      }
    }
    else
    {
      if (target[idx] > 0 || target[idx] < settings.max_travel[idx])
      {
        return (true);
      }
    }
#else
    // NOTE: max_travel is stored as negative
    if (target[idx] > 0 || target[idx] < settings.max_travel[idx])
    {
      return (true);
    }
#endif
  }
  return (false);
}

// Special handlers for setting and clearing Grbl's real-time execution flags.
void system_set_exec_state_flag(uint8_t mask)
{
  uint8_t sreg = SREG;
  // cli();
  sys_rt_exec_state |= (mask);
  SREG = sreg;
}

void system_clear_exec_state_flag(uint8_t mask)
{
  uint8_t sreg = SREG;
  // cli();
  sys_rt_exec_state &= ~(mask);
  SREG = sreg;
}

void system_set_exec_alarm(uint8_t code)
{
  uint8_t sreg = SREG;
  // cli();
  sys_rt_exec_alarm = code;
  SREG = sreg;
}

void system_clear_exec_alarm()
{
  uint8_t sreg = SREG;
  // cli();
  sys_rt_exec_alarm = 0;
  SREG = sreg;
}

void system_set_exec_motion_override_flag(uint8_t mask)
{
  uint8_t sreg = SREG;
  // cli();
  sys_rt_exec_motion_override |= (mask);
  SREG = sreg;
}

void system_set_exec_accessory_override_flag(uint8_t mask)
{
  uint8_t sreg = SREG;
  // cli();
  sys_rt_exec_accessory_override |= (mask);
  SREG = sreg;
}

void system_clear_exec_motion_overrides()
{
  uint8_t sreg = SREG;
  // cli();
  sys_rt_exec_motion_override = 0;
  SREG = sreg;
}

void system_clear_exec_accessory_overrides()
{
  uint8_t sreg = SREG;
  // cli();
  sys_rt_exec_accessory_override = 0;
  SREG = sreg;
}
