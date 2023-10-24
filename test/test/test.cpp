#include <thread>
#include <catch.hpp>

extern "C"
{
#include "logic.h"
}

uint8_t rfid_master[] = { 0x31, 0x0a, 0x24, 0x67, 0xb1 };
uint8_t rfid_user1[] = { 0x32, 0xba, 0x24, 0x87, 0xb2 };
uint8_t rfid_user2[] = { 0x33, 0xfa, 0x24, 0x97, 0xb3 };

void thread_loop(void)
{
	for (int ticks = 0; ticks < 10 * 1000; ticks += 10)
		RfidTaskTick10ms(ticks);
}

void test_init()
{
	std::thread thread(thread_loop);
	thread.join();
	RfidReset();
}

TEST_CASE("Master RFID card", "[master_rfid_card]")
{
	THEN("Case A - action: none")
	{
		test_init();
		int tolerance = 100;
		int master_touch1 = 1000;				// milliseconds
		RfidCardRead(rfid_master, master_touch1);
		REQUIRE_FALSE(RfidIsProgrammingMode(master_touch1 + tolerance));
		REQUIRE_FALSE(RfidIsProgrammingMode(
				master_touch1 + RFID_PROGRAMMING_MODE_MASTER_COUNT * RFID_PROGRAMMING_CYCLE_LONG + tolerance));
		REQUIRE(programmingBeepsCount == 1);
		REQUIRE(programmingBeeps[0] == master_touch1);
	}

	THEN("Case B - action: none")
	{
		test_init();
		int tolerance = 100;
		int master_touch1 = 1000;				// milliseconds
		int master_touch2 = 1000 + 200;
		RfidCardRead(rfid_master, master_touch1);
		RfidCardRead(rfid_master, master_touch2);
		REQUIRE_FALSE(RfidIsProgrammingMode(master_touch1 + tolerance));
		REQUIRE_FALSE(RfidIsProgrammingMode(master_touch2 + tolerance));
		REQUIRE_FALSE(RfidIsProgrammingMode(
				master_touch2 + RFID_PROGRAMMING_MODE_MASTER_COUNT * RFID_PROGRAMMING_CYCLE_LONG + tolerance));
		REQUIRE(programmingBeepsCount == 1);
		REQUIRE(programmingBeeps[0] == master_touch1);
	}

	THEN("Case C - action: none")
	{
		test_init();
		int tolerance = 100;
		int master_touch1 = 1000;				// milliseconds
		int master_touch2 = 1000 + 1200;
		int master_touch3 = 1000 + 1200 + 3100;
		RfidCardRead(rfid_master, master_touch1);
		RfidCardRead(rfid_master, master_touch2);
		RfidCardRead(rfid_master, master_touch3);
		REQUIRE_FALSE(RfidIsProgrammingMode(master_touch1 + tolerance));
		REQUIRE_FALSE(RfidIsProgrammingMode(master_touch2 + tolerance));
		REQUIRE_FALSE(RfidIsProgrammingMode(master_touch3 + tolerance));
		REQUIRE_FALSE(RfidIsProgrammingMode(
				master_touch3 + RFID_PROGRAMMING_MODE_MASTER_COUNT * RFID_PROGRAMMING_CYCLE_LONG + tolerance));
		REQUIRE(programmingBeepsCount == 3);
		REQUIRE(programmingBeeps[0] == master_touch1);
		REQUIRE(programmingBeeps[1] == master_touch2);
		REQUIRE(programmingBeeps[2] == master_touch3);
	}

	THEN("Case D - action: programming mode - adding user cards")
	{
		test_init();
		int tolerance = 100;
		int master_touch1 = 1000;				// milliseconds
		int master_touch2 = 1000 + 1200;
		int master_touch3 = 1000 + 1200 + 1200;
		RfidCardRead(rfid_master, master_touch1);
		RfidCardRead(rfid_master, master_touch2);
		RfidCardRead(rfid_master, master_touch3);
		REQUIRE_FALSE(RfidIsProgrammingMode(master_touch1 + tolerance)); // $$ czas w sprawdzaniu?
		REQUIRE_FALSE(RfidIsProgrammingMode(master_touch2 + tolerance));
		REQUIRE_FALSE(RfidIsProgrammingMode(master_touch3 + tolerance));
		REQUIRE(RfidIsProgrammingMode(master_touch3 + RFID_PROGRAMMING_CYCLE_LONG + tolerance));
		REQUIRE(programmingBeepsCount == 6);
		REQUIRE(programmingBeeps[0] == master_touch1);
		REQUIRE(programmingBeeps[1] == master_touch2);
		REQUIRE(programmingBeeps[2] == master_touch3);
		REQUIRE(programmingBeeps[3] == master_touch3 + RFID_PROGRAMMING_CYCLE_LONG);
		REQUIRE(programmingBeeps[4] == master_touch3 + RFID_PROGRAMMING_CYCLE_LONG + RFID_PROGRAMMING_BEEP_CONFIRMATION_DELAY);
		REQUIRE(programmingBeeps[5] == master_touch3 + RFID_PROGRAMMING_CYCLE_LONG + 2 * RFID_PROGRAMMING_BEEP_CONFIRMATION_DELAY);
	}
/*
	THEN("Case E - action: none")
	{
		test_init();
		int tolerance = 100;
		int master_touch1 = 1000;				// milliseconds
		int user_touch1 = 1000 + 500;
		int master_touch2 = 1000 + 500 + 600;
		int master_touch3 = 1000 + 500 + 600 + 1200;
		RfidCardRead(rfid_master, master_touch1);
		RfidCardRead(rfid_master, user_touch1);
		RfidCardRead(rfid_master, master_touch2);
		RfidCardRead(rfid_master, master_touch3);
		REQUIRE_FALSE(RfidIsProgrammingMode(master_touch1 + tolerance));
		REQUIRE_FALSE(RfidIsProgrammingMode(user_touch1 + tolerance));
		REQUIRE_FALSE(RfidIsProgrammingMode(master_touch2 + tolerance));
		REQUIRE_FALSE(RfidIsProgrammingMode(master_touch3 + tolerance));
		REQUIRE_FALSE(RfidIsProgrammingMode(master_touch3 + RFID_PROGRAMMING_CYCLE_LONG + tolerance));
		REQUIRE(programmingBeepsCount == 4);
		REQUIRE(programmingBeeps[0] == master_touch1);
		REQUIRE(programmingBeeps[1] == user_touch1);
		REQUIRE(programmingBeeps[2] == master_touch2);
		REQUIRE(programmingBeeps[3] == master_touch3);
	}

	THEN("Case F - action: programming mode - adding user cards")
	{
		test_init();
		int tolerance = 100;
		int master_touch1 = 1000;				// milliseconds
		int master_touch2 = 1000 + 250;
		int master_touch3 = 1000 + 500;
		int master_touch4 = 1000 + 750;
		int master_touch5 = 1000 + 1000 + 100;
		int master_touch6 = 1000 + 1000 + 250;
		int master_touch7 = 1000 + 1000 + 500;
		int master_touch8 = 1000 + 1000 + 750;
		int master_touch9 = 1000 + 1000 + 1000 + 100;
		int master_touch10 = 1000 + 1000 + 1000 + 250;
		int master_touch11 = 1000 + 1000 + 1000 + 500;
		int master_touch12 = 1000 + 1000 + 1000 + 750;
		RfidCardRead(rfid_master, master_touch1);
		RfidCardRead(rfid_master, master_touch2);
		RfidCardRead(rfid_master, master_touch3);
		RfidCardRead(rfid_master, master_touch4);
		RfidCardRead(rfid_master, master_touch5);
		RfidCardRead(rfid_master, master_touch6);
		RfidCardRead(rfid_master, master_touch7);
		RfidCardRead(rfid_master, master_touch8);
		RfidCardRead(rfid_master, master_touch9);
		RfidCardRead(rfid_master, master_touch10);
		RfidCardRead(rfid_master, master_touch11);
		RfidCardRead(rfid_master, master_touch12);
		REQUIRE_FALSE(RfidIsProgrammingMode(master_touch1 + tolerance));
		REQUIRE_FALSE(RfidIsProgrammingMode(master_touch5 + tolerance));
		REQUIRE_FALSE(RfidIsProgrammingMode(master_touch9 + tolerance));
		REQUIRE_FALSE(RfidIsProgrammingMode(master_touch12 + tolerance));
		REQUIRE(RfidIsProgrammingMode(master_touch12 + RFID_PROGRAMMING_CYCLE_LONG + tolerance));
		REQUIRE(programmingBeepsCount == 3);
		REQUIRE(programmingBeeps[0] == master_touch1);
		REQUIRE(programmingBeeps[1] == master_touch5);
		REQUIRE(programmingBeeps[2] == master_touch9);
		REQUIRE(programmingBeeps[3] == master_touch9 + RFID_PROGRAMMING_CYCLE_LONG);
		REQUIRE(programmingBeeps[4] == master_touch9 + RFID_PROGRAMMING_CYCLE_LONG + RFID_PROGRAMMING_BEEP_CONFIRMATION_DELAY);
		REQUIRE(programmingBeeps[5] == master_touch9 + RFID_PROGRAMMING_CYCLE_LONG + 2 * RFID_PROGRAMMING_BEEP_CONFIRMATION_DELAY);
	}

	THEN("Case G - action: none")
	{
		test_init();
		int tolerance = 100;
		int master_touch1 = 1000;				// milliseconds
		int master_touch2 = 1000 + 250;
		int master_touch3 = 1000 + 500;
		int master_touch4 = 1000 + 750;
		int master_touch5 = 1000 + 1000 + 100;
		int master_touch6 = 1000 + 1000 + 250;
		int master_touch7 = 1000 + 1000 + 500;
		int master_touch8 = 1000 + 1000 + 750;
		int master_touch9 = 1000 + 1000 + 1000 + 100;
		int master_touch10 = 1000 + 1000 + 1000 + 250;
		int master_touch11 = 1000 + 1000 + 1000 + 500;
		int master_touch12 = 1000 + 1000 + 1000 + 750;
		int master_touch13 = 1000 + 1000 + 1000 + 1000 + 100;
		RfidCardRead(rfid_master, master_touch1);
		RfidCardRead(rfid_master, master_touch2);
		RfidCardRead(rfid_master, master_touch3);
		RfidCardRead(rfid_master, master_touch4);
		RfidCardRead(rfid_master, master_touch5);
		RfidCardRead(rfid_master, master_touch6);
		RfidCardRead(rfid_master, master_touch7);
		RfidCardRead(rfid_master, master_touch8);
		RfidCardRead(rfid_master, master_touch9);
		RfidCardRead(rfid_master, master_touch10);
		RfidCardRead(rfid_master, master_touch11);
		RfidCardRead(rfid_master, master_touch12);
		RfidCardRead(rfid_master, master_touch13);
		REQUIRE_FALSE(RfidIsProgrammingMode(master_touch1 + tolerance));
		REQUIRE_FALSE(RfidIsProgrammingMode(master_touch5 + tolerance));
		REQUIRE_FALSE(RfidIsProgrammingMode(master_touch9 + tolerance));
		REQUIRE_FALSE(RfidIsProgrammingMode(master_touch13 + tolerance));
		REQUIRE_FALSE(RfidIsProgrammingMode(master_touch13 + RFID_PROGRAMMING_CYCLE_LONG + tolerance));
		REQUIRE(programmingBeepsCount == 3);
		REQUIRE(programmingBeeps[0] == master_touch1);
		REQUIRE(programmingBeeps[1] == master_touch5);
		REQUIRE(programmingBeeps[2] == master_touch9);
	}

	THEN("Case E - action: none")
	{
		test_init();
		int tolerance = 100;
		int master_touch1 = 1000;				// milliseconds
		int master_touch2 = 1000 + 1000 + 200;
		int master_touch3 = 1000 + 1000 + 1000 + 200;
		int user_touch1 = 1000 + 1000 + 1000 + 500;
		RfidCardRead(rfid_master, master_touch1);
		RfidCardRead(rfid_master, master_touch2);
		RfidCardRead(rfid_master, master_touch3);
		RfidCardRead(rfid_master, user_touch1);
		REQUIRE_FALSE(RfidIsProgrammingMode(master_touch1 + tolerance));
		REQUIRE_FALSE(RfidIsProgrammingMode(master_touch2 + tolerance));
		REQUIRE_FALSE(RfidIsProgrammingMode(master_touch3 + tolerance));
		REQUIRE_FALSE(RfidIsProgrammingMode(user_touch1 + tolerance));
		REQUIRE_FALSE(RfidIsProgrammingMode(user_touch1 + RFID_PROGRAMMING_CYCLE_LONG + tolerance));
		REQUIRE(programmingBeepsCount == 4);
		REQUIRE(programmingBeeps[0] == master_touch1);
		REQUIRE(programmingBeeps[1] == master_touch2);
		REQUIRE(programmingBeeps[2] == master_touch3);
		REQUIRE(programmingBeeps[3] == user_touch1);
	}
*/

	// THEN("Touch master card 1 time then later touch master card - nothing to do")
	// {
	// 	test_init();
	// 	RfidCardRead(rfid_master, 1000);
	// 	REQUIRE_FALSE(RfidIsProgrammingMode(1000 + 100));
	// 	RfidCardRead(rfid_master, 1000 + 3000);
	// 	REQUIRE_FALSE(RfidIsProgrammingMode(1000 + 3000 + 100));
	// }

	// THEN("Touch master card 2 time then later touch master card - nothing to do")
	// {
	// 	test_init();
	// 	RfidCardRead(rfid_master, 1000);
	// 	REQUIRE_FALSE(RfidIsProgrammingMode(1000 + 100));
	// 	RfidCardRead(rfid_master, 1000 + 1000);
	// 	REQUIRE_FALSE(RfidIsProgrammingMode(1000 + 1000 + 100));
	// 	RfidCardRead(rfid_master, 1000 + 1000 + 3000);
	// 	REQUIRE_FALSE(RfidIsProgrammingMode(1000 + 1000 + 3000 + 100));
	// }
}

/*
SCENARIO("") {
  GIVEN("") {
    init();
    WHEN("") {
      THEN("") {
        FAIL("error");
        REQUIRE("");
      }
      THEN("") { }
    }
  }
}
*/
