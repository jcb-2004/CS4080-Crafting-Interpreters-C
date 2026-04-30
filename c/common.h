#ifndef clox_common_h
#define clox_common_h

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

//Comment out these two lines to disable debug mode/printing each instruction
//#define DEBUG_PRINT_CODE
//#define DEBUG_TRACE_EXECUTION

//Comment out this line to disable garbage collector stress debug mode - it makes the GC run every time it possibly can to sniff out bugs
//#define DEBUG_STRESS_GC

//Comment out this line to disable garbage collector logging/debugging - it makes all dynamic memory modifications print to console
//#define DEBUG_LOG_GC

#define UINT8_COUNT (UINT8_MAX + 1)

#endif