#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include "main.h"
#include "logic.h"

static bool programmingMode = false;
int programmingBeepsCount = 0;
int programmingBeeps[RFID_PROGRAMMING_BEEPS_COUNT] = { 0 };
bool programmingStart = false;
TickType_t programmingStartMs = 0;
int programmingTouchCardCount = 0;
TickType_t programmingTouchCardLastMs = 0;
TickType_t lastTickMs = 0;

static void RFID_TEST_Delay(TickType_t milliseconds);

void RFID_Reset(void)
{
	programmingMode = false;
	programmingBeepsCount = 0;
	for (int i = 0; i < RFID_PROGRAMMING_BEEPS_COUNT; i++)
		programmingBeeps[i] = 0;
	programmingStart = false;
	programmingStartMs = 0;
	programmingTouchCardCount = 0;
	programmingTouchCardLastMs = 0;
	lastTickMs = 0;
}

bool RFID_IsProgrammingMode(TickType_t milliseconds)
{
	RFID_TEST_Delay(milliseconds);
	lastTickMs = milliseconds;

	return programmingMode;
}

void RFID_CardRead(uint8_t serial_numer[], TickType_t milliseconds)
{
	RFID_TEST_Delay(milliseconds);
	lastTickMs = milliseconds;

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

void RFID_TaskTick10ms(TickType_t milliseconds)
{
	// printf("tick %d ms\r\n", milliseconds);

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

void RFID_TEST_Delay(TickType_t milliseconds)
{
#ifdef TEST
	while (lastTickMs <= milliseconds)
	{
		// printf("delay %d ms\r\n", lastTickMs);
		RFID_TaskTick10ms(milliseconds);
		lastTickMs += RFID_TICK_LONG_MS;
	}
#endif
}

void RFID_PrintDebug(void)
{
	printf("programmingTouchCardCount %d\r\n", programmingTouchCardCount);
}
