#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "unity/fixture/unity_fixture.h"
#include "ring.h"

TEST_GROUP(ring_tests);

TEST_SETUP(ring_tests)
{
   /* Init before every test */
}

TEST_TEAR_DOWN(ring_tests)
{
   /* Cleanup after every test */
}

TEST(ring_tests, init)
{
   RingBuffer_t myRing;
   uint8_t buffer [256];

   RingInit(&myRing, &buffer[0], 256);

   TEST_ASSERT_EQUAL(256, myRing.size);
   TEST_ASSERT_POINTERS_EQUAL(&buffer, myRing.buffer);
}

TEST(ring_tests, init_with_null)
{
   RingBuffer_t myRing;
   TEST_ASSERT_EQUAL(NO_PTR, RingInit(&myRing, NULL, 256));
}

TEST(ring_tests, init_with_no_size)
{
   RingBuffer_t myRing;
   uint8_t arr[10];
   TEST_ASSERT_EQUAL(NO_DATA, RingInit(&myRing, &arr[0], 0));
}

TEST(ring_tests, write_byte)
{
   RingBuffer_t myRing;
   uint8_t arr[10];
   uint8_t testValue = 10;
   RingInit(&myRing, &arr[0], 10);

   TEST_ASSERT_EQUAL(OK, RingWriteByte(&myRing, testValue));
   TEST_ASSERT_EQUAL(testValue, arr[0]);
}

TEST(ring_tests, write_to_null_buf)
{
   uint8_t testValue = 10;
   TEST_ASSERT_EQUAL(NO_PTR, RingWriteByte(NULL, testValue));
}

TEST(ring_tests, write_multiple_bytes)
{
   RingBuffer_t myRing;
   uint8_t arr[10];
   uint8_t sizeNum = 5;
   uint8_t testValues[5] = {1,2,3,4,5};
   RingInit(&myRing, &arr[0], 10);

   TEST_ASSERT_EQUAL(OK, RingWriteMultipleBytes(&myRing, &testValues[0], sizeNum));
   TEST_ASSERT_EQUAL_UINT8_ARRAY(&testValues[0], &arr[0], 5);
}

TEST(ring_tests, write_multiple_bytes_from_null)
{
   RingBuffer_t myRing;
   uint8_t arr[10];
   uint8_t sizeNum = 5;
   RingInit(&myRing, &arr[0], 10);

   TEST_ASSERT_EQUAL(NO_PTR, RingWriteMultipleBytes(&myRing, NULL, sizeNum));
}

TEST(ring_tests, write_multiple_bytes_to_null)
{
   uint8_t sizeNum = 5;
   uint8_t testValues[5] = {1,2,3,4,5};

   TEST_ASSERT_EQUAL(NO_PTR, RingWriteMultipleBytes(NULL, &testValues[0], sizeNum));
}

TEST(ring_tests, write_multiple_bytes_no_data)
{
   RingBuffer_t myRing;
   uint8_t arr[10];
   uint8_t sizeNum = 0;
   uint8_t testValues[5] = {1,2,3,4,5};
   RingInit(&myRing, &arr[0], 10);

   TEST_ASSERT_EQUAL(NO_DATA, RingWriteMultipleBytes(&myRing, &testValues[0], sizeNum));
}

TEST(ring_tests, write_multiple_bytes_overlap)
{
   RingBuffer_t myRing;
   uint8_t arr[10];
   uint8_t sizeNum = 5;
   uint8_t testValues[5] = {1,2,3,4,5};
   uint8_t arrRef[10] = {3,4,5,0,0,0,0,0,1,2};
   RingInit(&myRing, &arr[0], 10);
   // Sztuczne przesunięcie wskaźnika
   myRing.writePtr = 8;
   myRing.readPtr = 8;
   TEST_ASSERT_EQUAL(OK, RingWriteMultipleBytes(&myRing, &testValues[0], sizeNum));
   TEST_ASSERT_EQUAL_UINT8_ARRAY(&arrRef[0], &arr[0], 10);
}

TEST(ring_tests, write_multiple_bytes_no_place)
{
   RingBuffer_t myRing;
   uint8_t arr[10];
   uint8_t sizeNum = 6;
   uint8_t testValues[6] = {1,2,3,4,5,6};
   uint8_t arrRef[10] = {1,2,3,4,5,6,1,2,3,0};
   RingInit(&myRing, &arr[0], 10);
   TEST_ASSERT_EQUAL(OK, RingWriteMultipleBytes(&myRing, &testValues[0], sizeNum));
   TEST_ASSERT_EQUAL(NO_PLACE, RingWriteMultipleBytes(&myRing, &testValues[0], sizeNum));
   TEST_ASSERT_EQUAL(NO_PLACE, RingWriteMultipleBytes(&myRing, &testValues[0], sizeNum - 1));
   TEST_ASSERT_EQUAL(NO_PLACE, RingWriteMultipleBytes(&myRing, &testValues[0], sizeNum - 2));
   TEST_ASSERT_EQUAL(OK, RingWriteMultipleBytes(&myRing, &testValues[0], sizeNum - 3));
   TEST_ASSERT_EQUAL(NO_PLACE, RingWriteMultipleBytes(&myRing, &testValues[0], 1));
   TEST_ASSERT_EQUAL_UINT8_ARRAY(&arrRef[0], &arr[0], 10);
}

TEST(ring_tests, read_byte_after_init)
{
   RingBuffer_t myRing;
   uint8_t arr[10];
   uint8_t data;
   
   RingInit(&myRing, &arr[0], 10);
   TEST_ASSERT_EQUAL(NO_DATA, RingReadByte(&myRing, &data));
}

TEST(ring_tests, read_bytes_to_empty)
{
   RingBuffer_t myRing;
   uint8_t arr[10];
   uint8_t sizeNum = 6;
   uint8_t testValues[6] = {1,2,3,4,5,6};
   uint8_t data;
   RingInit(&myRing, &arr[0], 10);
   TEST_ASSERT_EQUAL(NO_DATA, RingReadByte(&myRing, &data));

   TEST_ASSERT_EQUAL(OK, RingWriteMultipleBytes(&myRing, &testValues[0], sizeNum));
   for(uint8_t i = 0; i < sizeNum; i++){
      TEST_ASSERT_EQUAL(OK, RingReadByte(&myRing, &data));
      TEST_ASSERT_EQUAL(testValues[i], data);
   }
   TEST_ASSERT_EQUAL(NO_DATA, RingReadByte(&myRing, &data));
}