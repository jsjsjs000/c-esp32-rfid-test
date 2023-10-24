#include <stdbool.h>
#include <stdio.h>
#include "logic.h"

static bool programmingMode = false;
int programmingBeepsCount = 0;
int programmingBeeps[RFID_PROGRAMMING_BEEPS_COUNT] = { 0 };
bool programmingStart = false;
int programmingStartMs = 0;
int programmingTouchCardCount = 0;
int programmingTouchCardLastMs = 0;

void RfidReset(void)
{
	programmingMode = false;
	programmingBeepsCount = 0;
	for (int i = 0; i < RFID_PROGRAMMING_BEEPS_COUNT; i++)
		programmingBeeps[i] = 0;
	programmingStart = false;
	programmingStartMs = 0;
	programmingTouchCardCount = 0;
	programmingTouchCardLastMs = 0;
}

bool RfidIsProgrammingMode(uint32_t milliseconds)
{
	return programmingMode;
}

void RfidCardRead(uint8_t serial_numer[], uint32_t milliseconds)
{
	if (!programmingStart)
	{
		programmingStartMs = milliseconds;
		programmingStart = true;
	}

		/// buzzer
	if (programmingBeepsCount == 0 || (programmingBeepsCount > 0 &&
			milliseconds > programmingBeeps[programmingBeepsCount - 1] + RFID_PROGRAMMING_CYCLE_LONG))
		programmingBeeps[programmingBeepsCount++] = milliseconds;

		/// touch card
	if (programmingTouchCardCount > 0 &&
			milliseconds > programmingTouchCardLastMs + 2 * RFID_PROGRAMMING_CYCLE_LONG)
	{
		programmingStart = 0;
		programmingStartMs = 0;
		programmingTouchCardCount = 0;
		programmingTouchCardLastMs = 0;
	}
	else if (milliseconds > programmingTouchCardLastMs + RFID_PROGRAMMING_CYCLE_LONG)
	{
		programmingTouchCardCount++;
		programmingTouchCardLastMs = milliseconds;
	}
	else if (programmingTouchCardCount == 0)
	{
		programmingTouchCardCount++;
		programmingTouchCardLastMs = milliseconds;
	}

		/// programming mode
	if (programmingTouchCardCount == 3)
		programmingMode = true;

	// if (sizeof(serial_numer) != 5)
	// 	return false;
}

void RfidTaskTick10ms(uint32_t milliseconds)
{
	// printf("%d ms\r\n", milliseconds);

	if (programmingStart && milliseconds - programmingStartMs > RFID_PROGRAMMING_CYCLE_LONG)
	{
		programmingStart = false;
		programmingStartMs = 0;
	}

	if (!programmingMode && programmingTouchCardCount == 3 &&
			milliseconds >= programmingTouchCardLastMs + RFID_PROGRAMMING_CYCLE_LONG)
	{
		programmingMode = true;
	}
}

void print_debug(void)
{
	printf("programmingTouchCardCount %d\r\n", programmingTouchCardCount);
}
