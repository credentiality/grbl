/*
  stepper.c - stepper motor driver: executes motion plans using stepper motors
  Part of Grbl

  Copyright (c) 2009-2011 Simen Svale Skogsrud
  Copyright (c) 2011-2012 Sungeun K. Jeon
  
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

#include <stdint.h>

void st_wake_up() 
{
}

void st_go_idle() 
{
}

inline static uint8_t iterate_trapezoid_cycle_counter() 
{
}          

void st_reset()
{
}

void st_init()
{
}

static uint32_t config_step_timer(uint32_t cycles)
{
}

static void set_step_events_per_minute(uint32_t steps_per_minute) 
{
}

void st_cycle_start() 
{
}

void st_feed_hold() 
{
}

void st_cycle_reinitialize()
{
}
