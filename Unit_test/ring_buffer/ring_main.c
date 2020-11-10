#include "unity/fixture/unity_fixture.h"

static void run_all_tests(void);

int main(int argc, const char **argv)
{
   UnityMain(argc, argv, run_all_tests);

   return 0;
}

static void run_all_tests(void)
{
	printf("Ring buffer tests:\n");
	RUN_TEST_GROUP(ring_tests);
}
