#ifndef platform_h
#define platform_h

//#define ARDUINO
#define SIMULATOR
//#define RASPI

#ifndef ARDUINO
#define PSTR(s) ((const char *)(s))
#endif

#ifdef RASPI
#include "raspi.h"
#endif

#include "platform/limits.h"
#include "platform/eeprom.h"
#include "platform/coolant_control.h"
#include "platform/pin_map.h"
#include "platform/serial.h"
#include "platform/spindle_control.h"

#endif
