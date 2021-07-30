/**
 * @file ring.c
 * @author Kacper Brzostowski (kapibrv97@gmail.com)
 * @link https://github.com/magiczny-kacper
 * @brief Ring buffer library source file.
 * @version 2.0.0
 * @date 2021-02-12
 *
 * @copyright Copyright (c) 2020
 *
 */

/**
 * @copyright GNU General Public License v3.0
 * @{
 */
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "ring.h"

/**< Modulo for operations on array indexes. */
#define MODULO_BUF(value, max) ((value) % (max))

uint32_t RingGetElementsCapacity (RingBuffer_t* buffer) {
    return buffer -> size;
}

uint32_t RingGetSpace (RingBuffer_t* buffer) {
    return buffer -> place;
}

uint32_t RingGetDataCnt (RingBuffer_t* buffer) {
    return buffer -> size - RingGetSpace(buffer);
}

/* DONE: Add null pointer exceptions. */
RingStatus_t RingInit (RingBuffer_t* buffer, void* arrayBuffer, size_t bufferSize, size_t elementSize){
    if(NULL == buffer) return NO_PTR;
    if(NULL == arrayBuffer) return NO_PTR;

    memset(buffer, 0, sizeof(RingBuffer_t));

    if(arrayBuffer == NULL) return NO_PTR;
    if(bufferSize <= 0) return NO_DATA;

    buffer -> buffer = arrayBuffer;
    buffer -> size = bufferSize;
    buffer -> place = buffer -> size - 1;
    buffer -> writePtr = 0;
    buffer -> readPtr = 0;
    buffer -> elementSize = elementSize;
    buffer -> sizeB = buffer -> elementSize * buffer -> size;

    memset(buffer -> buffer, 0, buffer -> sizeB);
    return OK;
}

RingStatus_t RingInitAlloc (RingBuffer_t* buffer, size_t bufferSize, size_t elementSize){
    void* ptr;
    size_t reqSize = elementSize * bufferSize;
    if(NULL == buffer){
        return NO_PTR;
    }
    ptr = malloc(reqSize);
    if(NULL == ptr){
        return NO_PTR;
    }
    return RingInit(buffer, ptr, bufferSize, elementSize);
}

RingStatus_t RingWriteElement (RingBuffer_t* buffer, void* data){
    RingStatus_t retval = OK;

    if(buffer == NULL) return NO_PTR;
    if(data == NULL) return NO_PTR;
    if(buffer -> buffer == NULL) return NO_PTR;

    uint32_t tempHead = buffer -> writePtr;
    uint32_t tempTail = buffer -> readPtr;
    size_t elSize = buffer -> elementSize;
    size_t bufferSize = buffer -> sizeB;

    void* wrPtr;
    wrPtr = buffer -> buffer + tempHead;

    tempHead = MODULO_BUF(tempHead + elSize, bufferSize);
    if(tempHead != tempTail){
        memcpy(wrPtr, data, elSize);
        buffer -> writePtr = tempHead;
        buffer -> place --;
    }else{
        retval = NO_PLACE;
    }
    return retval;
}

RingStatus_t RingWriteElements (RingBuffer_t* buffer, void* data, size_t len){
    RingStatus_t retval = OK;

    if(buffer == NULL) return NO_PTR;
    if(data == NULL) return NO_PTR;
    if(buffer -> buffer == NULL) return NO_PTR;
    if(len <= 0) return NO_DATA;

    uint32_t tempHead = buffer -> writePtr;
    uint32_t tempPlace = buffer -> place;

    size_t elSize = buffer -> elementSize;
    size_t bytesToWrite = elSize * len;
    size_t bufferSize = buffer -> size;
    size_t bufferSizeB = buffer -> sizeB;
    uint8_t* wrPtr;

    wrPtr = buffer -> buffer + tempHead;

    if(tempPlace >= len){
        if(tempHead + bytesToWrite >= bufferSizeB){
            uint32_t temp1, temp2;
            temp1 = bufferSizeB - tempHead;
            memcpy(wrPtr, data, temp1);
            data += temp1;
            temp2 = len - temp1;
            wrPtr = buffer -> buffer;
            memcpy(wrPtr, data, temp2);
        }else{
            memcpy(wrPtr, data, len);
            tempHead = MODULO_BUF(tempHead + len, bufferSize);
        }
        tempPlace -= len;
        buffer -> writePtr = tempHead;
        buffer -> place = tempPlace;
    }else{
        retval = NO_PLACE;
    }

    return retval;
}

RingStatus_t RingReadElement (RingBuffer_t* buffer, void* data){
    RingStatus_t retval = OK;
    uint32_t tempHead = buffer -> writePtr;
    uint32_t tempTail = buffer -> readPtr;
    uint32_t tempPlace = buffer -> place;
    size_t bufferSize = buffer -> size;
    size_t elSize = buffer -> elementSize;
    void* wrPtr;
    wrPtr = buffer -> buffer + tempTail;

    if(tempHead != tempTail){
        memcpy(data, wrPtr, elSize);
        tempTail = MODULO_BUF(tempTail + 1, bufferSize);
        tempPlace++;
        buffer -> readPtr = tempTail;
        buffer -> place = tempPlace;
    }else{
        retval = NO_DATA;
    }
    return retval;
}

RingStatus_t RingReadElements (RingBuffer_t* buffer, void* data, size_t len){
    RingStatus_t retval = OK;
    uint32_t tempHead = buffer -> writePtr;
    uint32_t tempTail = buffer -> readPtr;
    uint32_t tempPlace = buffer -> place;
    uint8_t* wrPtr;
    wrPtr = buffer -> buffer + tempHead;
    if(tempHead != tempTail){
        if(tempTail + len > buffer -> size - 1){
            uint32_t temp1, temp2;
            temp1 = buffer -> size - tempTail;
            memcpy(data, wrPtr, temp1);
            data += temp1;
            temp2 = len - temp1;
            wrPtr = buffer -> buffer;
            memcpy(data, wrPtr, temp2);
            tempTail = temp2;
        }else{
            memcpy(data, wrPtr, len);
            tempTail += len;
        }
        tempPlace += len;
        buffer -> readPtr = tempTail;
        buffer -> place = tempPlace;
    }else{
        retval = NO_DATA;
    }
    return retval;
}

uint32_t RingGetHead (RingBuffer_t* buffer){
    return buffer -> writePtr;
}

uint32_t RingGetTail (RingBuffer_t* buffer){
    return buffer -> readPtr;
}

RingStatus_t RingGetLastElement(RingBuffer_t* buffer, void* element){
    RingStatus_t ret = OK;
    if(buffer && element){
        memcpy(element, buffer -> buffer + buffer -> writePtr, buffer -> elementSize);
    }else{
        ret = NO_PTR;
    }
    return ret;
}

/**
 * @}
 *
 */