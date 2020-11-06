/**
 * @file ring.c
 * @author Kacper Brzostowski (kapibrv97@gmail.com)
 * @brief Ring buffer library source file.
 * @version 1.1.0
 * @date 2020-08-04
 * 
 * @copyright Copyright (c) 2020
 * 
 */

/**
 * @addtogroup Ring_Buffer
 * @{
 */
#include <stdint.h>
#include <string.h>
#include "../../utils/tools.h"
#include "ring.h"

uint32_t RingGetMaxSize (ringBufferTypeDef* buffer){
	return buffer -> size;
}

uint32_t RingGetSpace (ringBufferTypeDef* buffer){
	return buffer -> place;
}

uint32_t RingGetDataCnt (ringBufferTypeDef* buffer){
	return buffer -> size - RingGetSpace(buffer);
}

ring_status RingInit (ringBufferTypeDef* buffer
#if RING_BUFFER_MODE == RING_BUFFER_POINTERS
		, uint8_t* arrayBuffer, uint32_t bufferSize
#endif
){
	memset(buffer, 0, sizeof(ringBufferTypeDef));
#if RING_BUFFER_MODE == RING_BUFFER_POINTERS
	if(arrayBuffer == NULL) return NO_PTR;
	if(bufferSize <= 0) return NO_DATA;
	buffer -> buffer = arrayBuffer;
	buffer -> size = bufferSize;
	memset(buffer -> buffer, 0, buffer -> size);
#elif RING_BUFFER_MODE == RING_BUFFER_ARRAYS
	buffer -> size = BUFFER_SIZE;
#endif
	buffer -> place = buffer -> size - 1;
	buffer -> writePtr = 0;
	buffer -> readPtr = 0;
	return OK;
}

ring_status RingWriteByte (ringBufferTypeDef* buffer, uint8_t data){
	ring_status retval = OK;
	if(buffer == NULL) return NO_PTR;
	uint32_t tempHead = buffer -> writePtr;
	uint32_t tempTail = buffer -> readPtr;

#if RING_BUFFER_MODE == RING_BUFFER_POINTERS
	uint8_t* wrPtr;
	if(buffer -> buffer == NULL) return NO_PTR;
	wrPtr = buffer -> buffer + tempHead;
#endif

	tempHead = MODULO_BUF(tempHead + 1, buffer -> size);
	if(tempHead != tempTail){

#if RING_BUFFER_MODE == RING_BUFFER_POINTERS
		*wrPtr = data;
#elif RING_BUFFER_MODE == RING_BUFFER_ARRAYS
		buffer -> buffer[tempHead - 1] = data;
#endif

		buffer -> writePtr = tempHead;
		buffer -> place --;
	}else{
		retval = NO_PLACE;
	}
	return retval;
}

ring_status RingWriteMultipleBytes (ringBufferTypeDef* buffer, uint8_t* data, uint32_t len){
	ring_status retval = OK;
	if(buffer == NULL) return NO_PTR;
	if(data == NULL) return NO_PTR;
	if(len <= 0) return NO_DATA;
	uint32_t tempHead = buffer -> writePtr;
	uint32_t tempPlace = buffer -> place;

#if RING_BUFFER_MODE == RING_BUFFER_POINTERS
	uint8_t* wrPtr;
	if(buffer -> buffer == NULL) return NO_PTR;
	wrPtr = buffer -> buffer + tempHead;
#endif

	if(tempPlace >= len){
		if(tempHead + len >= buffer -> size){
			uint32_t temp1, temp2;
#if RING_BUFFER_MODE == RING_BUFFER_POINTERS
			temp1 = buffer -> size - tempHead;
			memcpy(wrPtr, data, temp1);
			data += temp1;
			temp2 = len - temp1;
			wrPtr = buffer -> buffer;
			memcpy(wrPtr, data, temp2);
#elif RING_BUFFER_MODE == RING_BUFFER_ARRAYS
			temp1 = buffer -> size - tempHead;
			memcpy(&buffer -> buffer[tempHead], data, temp1);
			data += temp1;
			temp2 = len - temp1;
			memcpy(&buffer -> buffer, data, temp2);
			tempHead = temp2;
#endif	
		}else{

#if RING_BUFFER_MODE == RING_BUFFER_POINTERS
			memcpy(wrPtr, data, len);
#elif RING_BUFFER_MODE == RING_BUFFER_ARRAYS	
			memcpy(&buffer -> buffer[tempHead], data, len);
#endif
			tempHead = MODULO_BUF(tempHead + len, buffer -> size);
		}
		tempPlace -= len;
		buffer -> writePtr = tempHead;
		buffer -> place = tempPlace;
	}else{
		retval = NO_PLACE;
	}

	return retval;
}

ring_status RingReadByte (ringBufferTypeDef* buffer, uint8_t* data){
	ring_status retval = OK;
	uint32_t tempHead = buffer -> writePtr;
	uint32_t tempTail = buffer -> readPtr;
	uint32_t tempPlace = buffer -> place;
#if RING_BUFFER_MODE == RING_BUFFER_POINTERS
	uint8_t* wrPtr;
	wrPtr = buffer -> buffer + tempTail;
#endif

	if(tempHead != tempTail){
#if RING_BUFFER_MODE == RING_BUFFER_POINTERS
		*data = *wrPtr;
#elif RING_BUFFER_MODE == RING_BUFFER_ARRAYS
		*data = buffer -> buffer[tempTail];
#endif
		tempTail = MODULO_BUF(tempTail + 1, buffer -> size);
		tempPlace++;
		buffer -> readPtr = tempTail;
		buffer -> place = tempPlace;
	}else{
		retval = NO_DATA;
	}
	return retval;
}

ring_status RingReadMultipleBytes (ringBufferTypeDef* buffer, uint8_t* data, uint32_t len){
	ring_status retval = OK;
	uint32_t tempHead = buffer -> writePtr;
	uint32_t tempTail = buffer -> readPtr;
	uint32_t tempPlace = buffer -> place;
#if RING_BUFFER_MODE == RING_BUFFER_POINTERS
	uint8_t* wrPtr;
	wrPtr = buffer -> buffer + tempHead;
#endif
	if(tempHead != tempTail){
		if(tempTail + len > buffer -> size - 1){
			uint32_t temp1, temp2;
#if RING_BUFFER_MODE == RING_BUFFER_POINTERS
			temp1 = buffer -> size - tempTail;
			memcpy(data, wrPtr, temp1);
			data += temp1;
			temp2 = len - temp1;
			wrPtr = buffer -> buffer;
			memcpy(data, wrPtr, temp2);
			tempTail = temp2;
#elif RING_BUFFER_MODE == RING_BUFFER_ARRAYS
			temp1 = buffer -> size - tempTail;
			memcpy(data, &buffer -> buffer[tempTail], temp1);
			data += temp1;
			temp2 = len - temp1;
			memcpy(data, buffer -> buffer, temp2);
			tempTail = temp2;
#endif
		}else{
#if RING_BUFFER_MODE == RING_BUFFER_POINTERS
			memcpy(data, wrPtr, len);
#elif RING_BUFFER_MODE == RING_BUFFER_ARRAYS
			memcpy(data, &buffer -> buffer[tempTail], len);
#endif
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

uint32_t RingGetHead (ringBufferTypeDef* buffer){
	return buffer -> writePtr;
}

uint32_t RingGetTail (ringBufferTypeDef* buffer){
	return buffer -> readPtr;
}

uint32_t RingGetLastElement(ringBufferTypeDef* buffer){
#if RING_BUFFER_MODE == RING_BUFFER_POINTERS
	uint8_t data;
	data = *(buffer -> buffer + buffer -> writePtr);
	return data;
#elif RING_BUFFER_MODE == RING_BUFFER_ARRAYS
	return (buffer -> buffer [buffer -> writePtr]);
#endif
}

/**
 * @}
 * 
 */