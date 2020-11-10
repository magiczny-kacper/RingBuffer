#include "unity/fixture/unity_fixture.h"

TEST_GROUP_RUNNER(ring_tests)
{
   /* Test cases to run */
   RUN_TEST_CASE(ring_tests, init);
   RUN_TEST_CASE(ring_tests, init_with_null);
   RUN_TEST_CASE(ring_tests, init_with_no_size);
   RUN_TEST_CASE(ring_tests, write_byte);
   RUN_TEST_CASE(ring_tests, write_to_null_buf);
   RUN_TEST_CASE(ring_tests, write_multiple_bytes);
   RUN_TEST_CASE(ring_tests, write_multiple_bytes_from_null);
   RUN_TEST_CASE(ring_tests, write_multiple_bytes_to_null);
   RUN_TEST_CASE(ring_tests, write_multiple_bytes_no_data);
   RUN_TEST_CASE(ring_tests, write_multiple_bytes_overlap);
   RUN_TEST_CASE(ring_tests, write_multiple_bytes_no_place);
   RUN_TEST_CASE(ring_tests, read_byte_after_init);
   RUN_TEST_CASE(ring_tests, read_bytes_to_empty);
}
