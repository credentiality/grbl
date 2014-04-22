#ifndef platform_h
#define platform_h

//#define ARDUINO
#define SIMULATOR

#ifdef SIMULATOR
#define PSTR(s) ((const char *)(s))
#endif

#include "platform/stepper.h"
#include "platform/limits.h"
#include "platform/eeprom.h"
#include "platform/coolant_control.h"
#include "platform/print.h"
#include "platform/pin_map.h"
#include "platform/serial.h"
#include "platform/spindle_control.h"
#include "platform/protocol.h"

#endif
