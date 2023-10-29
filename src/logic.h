#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "main.h"

#define RFID_PROGRAMMING_MODE_MASTER_COUNT        3
#define RFID_PROGRAMMING_CYCLE_LONG               1000
#define RFID_PROGRAMMING_BEEP_CONFIRMATION_LONG   50
#define RFID_PROGRAMMING_BEEP_CONFIRMATION_DELAY  100
#define RFID_PROGRAMMING_BEEPS_COUNT              10
#define RFID_TICK_LONG_MS                         10

extern int programmingBeepsCount;
extern int programmingBeeps[10];

extern void RFID_Reset(void);
extern bool RFID_IsProgrammingMode(TickType_t milliseconds);
extern void RFID_CardRead(uint8_t serial_numer[], TickType_t milliseconds);
extern void RFID_TaskTick10ms(TickType_t milliseconds);
extern void RFID_PrintDebug(void);
