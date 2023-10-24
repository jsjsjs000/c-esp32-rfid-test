#pragma once
#include <stdint.h>
#include <stdbool.h>

#define RFID_PROGRAMMING_MODE_MASTER_COUNT        3
#define RFID_PROGRAMMING_CYCLE_LONG               1000
#define RFID_PROGRAMMING_BEEP_CONFIRMATION_LONG   50
#define RFID_PROGRAMMING_BEEP_CONFIRMATION_DELAY  100
#define RFID_PROGRAMMING_BEEPS_COUNT              10

extern int programmingBeepsCount;
extern int programmingBeeps[10];

extern void RfidReset(void);
extern bool RfidIsProgrammingMode(uint32_t milliseconds);
extern void RfidCardRead(uint8_t serial_numer[], uint32_t milliseconds);
extern void RfidTaskTick10ms(uint32_t milliseconds);
extern void print_debug(void);
