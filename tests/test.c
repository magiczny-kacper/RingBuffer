#include <criterion/criterion.h>
#include <criterion/logging.h>
#include <criterion/assert.h>
#include <ring.h>
#include <stdint.h>

Test(ring_tests, dummy){
    cr_assert(1, "Hello");
}

Test(ring_tests, init){
    RingBuffer_t myRing;
    uint8_t buffer [256];
    RingInit(&myRing, &buffer[0], 256);
    cr_assert(256 == myRing.size, "Excepted 'size' to be 256");
    cr_assert(buffer == myRing.buffer, "Excepted pointer to be equal");
}

Test(ring_tests, init_with_null){
    RingStatus_t ret;
    RingBuffer_t myRing;
    uint8_t buf[256];
    ret = RingInit(&myRing, NULL, 256);
    cr_assert(ret == NO_PTR, "Excepted %d, got %d",
        NO_PTR, ret);
    ret = RingInit(NULL, &buf[0], 256);
    cr_assert(ret == NO_PTR, "Excepted %d, got %d",
        NO_PTR, ret);
}

Test(ring_tests, init_with_no_size){
    RingStatus_t ret;
    RingBuffer_t myRing;
    uint8_t arr[10];
    ret = RingInit(&myRing, &arr[0], 0);
    cr_assert(NO_DATA == ret, "Excepted %d, got %d",
    NO_DATA, ret);
}

Test(ring_tests, write_byte){
   RingBuffer_t myRing;
   uint8_t arr[10];
   uint8_t testValue = 10;
   RingInit(&myRing, &arr[0], 10);

   cr_assert(OK == RingWriteByte(&myRing, testValue));
   cr_assert(testValue == arr[0]);
}

Test(ring_tests, write_to_null_buf){
   uint8_t testValue = 10;
   cr_assert(NO_PTR == RingWriteByte(NULL, testValue));
}

Test(ring_tests, write_multiple_bytes){
   RingBuffer_t myRing;
   uint8_t arr[10];
   uint8_t sizeNum = 5;
   uint8_t testValues[5] = {1,2,3,4,5};
   RingInit(&myRing, &arr[0], 10);

   cr_assert(OK == RingWriteMultipleBytes(&myRing, &testValues[0], sizeNum));
   cr_assert_arr_eq(testValues, arr, sizeNum);
}

Test(ring_tests, write_multiple_bytes_from_null){
   RingBuffer_t myRing;
   uint8_t arr[10];
   uint8_t sizeNum = 5;
   RingInit(&myRing, &arr[0], 10);

   cr_assert(NO_PTR == RingWriteMultipleBytes(&myRing, NULL, sizeNum));
}

Test(ring_tests, write_multiple_bytes_to_null){
   uint8_t sizeNum = 5;
   uint8_t testValues[5] = {1,2,3,4,5};

   cr_assert(NO_PTR == RingWriteMultipleBytes(NULL, &testValues[0], sizeNum));
}

Test(ring_tests, write_multiple_bytes_no_data){
   RingBuffer_t myRing;
   uint8_t arr[10];
   uint8_t sizeNum = 0;
   uint8_t testValues[5] = {1,2,3,4,5};
   RingInit(&myRing, &arr[0], 10);

   cr_assert(NO_DATA == RingWriteMultipleBytes(&myRing, &testValues[0], sizeNum));
}

Test(ring_tests, write_multiple_bytes_overlap)
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
   cr_assert(OK == RingWriteMultipleBytes(&myRing, &testValues[0], sizeNum));
   cr_assert_arr_eq(&arrRef[0], &arr[0], 10);
}

Test(ring_tests, write_multiple_bytes_no_place)
{
   RingBuffer_t myRing;
   uint8_t arr[10];
   uint8_t sizeNum = 6;
   uint8_t testValues[6] = {1,2,3,4,5,6};
   uint8_t arrRef[10] = {1,2,3,4,5,6,1,2,3,0};
   RingInit(&myRing, &arr[0], 10);
   cr_assert(OK == RingWriteMultipleBytes(&myRing, &testValues[0], sizeNum));
   cr_assert(NO_PLACE == RingWriteMultipleBytes(&myRing, &testValues[0], sizeNum));
   cr_assert(NO_PLACE == RingWriteMultipleBytes(&myRing, &testValues[0], sizeNum - 1));
   cr_assert(NO_PLACE == RingWriteMultipleBytes(&myRing, &testValues[0], sizeNum - 2));
   cr_assert(OK == RingWriteMultipleBytes(&myRing, &testValues[0], sizeNum - 3));
   cr_assert(NO_PLACE == RingWriteMultipleBytes(&myRing, &testValues[0], 1));
   cr_assert_arr_eq(&arrRef[0], &arr[0], 10);
}

Test(ring_tests, read_byte_after_init)
{
   RingBuffer_t myRing;
   uint8_t arr[10];
   uint8_t data;

   RingInit(&myRing, &arr[0], 10);
   cr_assert(NO_DATA == RingReadByte(&myRing, &data));
}

Test(ring_tests, read_bytes_to_empty)
{
   RingBuffer_t myRing;
   uint8_t arr[10];
   uint8_t sizeNum = 6;
   uint8_t testValues[6] = {1,2,3,4,5,6};
   uint8_t data;
   RingInit(&myRing, &arr[0], 10);
   cr_assert(NO_DATA == RingReadByte(&myRing, &data));

   cr_assert(OK == RingWriteMultipleBytes(&myRing, &testValues[0], sizeNum));
   for(uint8_t i = 0; i < sizeNum; i++){
      cr_assert(OK == RingReadByte(&myRing, &data));
      cr_assert(testValues[i] == data);
   }
   cr_assert(NO_DATA == RingReadByte(&myRing, &data));
   //TEST_FAIL_MESSAGE("initial test setup");
}

Test(ring_tests, get_size)
{
   RingBuffer_t myRing;
   uint8_t arr[10];

   RingInit(&myRing, &arr[0], 10);
   cr_assert(10 == RingGetMaxSize(&myRing));
}

Test(ring_tests, get_space)
{
   RingBuffer_t myRing;
   uint8_t arr[10];
   uint8_t data[5] = {1, 1, 1, 1, 1};

   RingInit(&myRing, &arr[0], 10);
   RingWriteMultipleBytes(&myRing, &data[0], 5);
   cr_assert(4 == RingGetSpace(&myRing));
}