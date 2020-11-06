/**
 * @file ring.h
 * @author Kacper Brzostowski (kapibrv97@gmail.com)
 * @brief FIFO ring buffer driver library header.
 * @version 1.1.0
 * @date 2020-06-11
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef SRC_CODE_RING_RING_H_
#define SRC_CODE_RING_RING_H_

#include <stdint.h>

/**
 * @defgroup Ring_Buffer
 * @brief FIFO ring buffer library.
 * @{
 */

#define RING_BUFFER_POINTERS 1
#define RING_BUFFER_ARRAYS 0

#define RING_BUFFER_MODE RING_BUFFER_POINTERS

#ifndef RING_BUFFER_MODE
#define RING_BUFFER_MODE RING_BUFFER_ARRAYS
#endif

#if RING_BUFFER_MODE == RING_BUFFER_ARRAYS
	#define BUFFER_SIZE 256
#endif

#ifndef RING_BUFFER_MODE 
#error Define Ring buffer mode.
#endif

/**
 * @brief Ring buffer status enumerator.
 * 
 */
typedef enum{
	NO_PTR = -3,
	NO_DATA = -2, /**< Returned if there was no data to read in buffer. */
	NO_PLACE = -1, /**< Returned if there was no place to write data in buffer. */
	OK = 1 /**< Returned when write/read was succesfull/ */
} ring_status;

/**
 * @brief Buffer handler structure.
 * 
 */
typedef struct{
	uint32_t size; /**< Size of buffer. */
	uint32_t writePtr; /**< Buffer next write pointer. */
	uint32_t readPtr; /**< Buffer next read pointer. */
	uint32_t place; /**< Place available in buffer. */
#if RING_BUFFER_MODE == RING_BUFFER_ARRAYS
	uint8_t buffer[BUFFER_SIZE];
#elif RING_BUFFER_MODE == RING_BUFFER_POINTERS
	uint8_t* buffer;
#endif
} ringBufferTypeDef;

/*
 * Function that returns size of whole ring buffer.
 *
 * @param buffer 	Pointer to buffer structure, which size has to be returned.
 * @retval 			Size of pointed ring buffer.
 */
uint32_t RingGetMaxSize (ringBufferTypeDef* buffer);

/**
 * @brief Function that returns available space in selected buffer.
 * 
 * @param buffer Pointer to buffer structure, which size has to be returned.
 * @return uint32_t Available size in provided buffer.
 */
uint32_t RingGetSpace (ringBufferTypeDef* buffer);

/**
 * @brief Function that returns count of data available in buffer.
 * 
 * @param buffer Pointer to buffer structure
 * @return uint32_t Data count in buffer.
 */
uint32_t RingGetDataCnt (ringBufferTypeDef* buffer);

/**
 * @brief Function to initialize selected ring buffer.
 * 
 * @param buffer 	Pointer to buffer structure that has to be initialized.
 */
ring_status RingInit (ringBufferTypeDef* buffer
#if RING_BUFFER_MODE == RING_BUFFER_POINTERS
	, uint8_t* arrayBuffer, uint32_t bufferSize
#endif
);

/**
 * @brief Addes one byte to the end of buffer.
 * 
 * @param buffer Pointer to buffer to write.
 * @param data Data to write.
 * @return ring_status Status of write process.
 */
ring_status RingWriteByte (ringBufferTypeDef* buffer, uint8_t data);

/**
 * @brief Writes multiple bytes to buffer.
 * 
 * @param buffer Buffer to write data.
 * @param data Data pointer to write.
 * @param len Length of data.
 * @return ring_status Write status.
 */
ring_status RingWriteMultipleBytes (ringBufferTypeDef* buffer, uint8_t* data, uint32_t len);

/**
 * @brief Reads one byte from buffer.
 * 
 * @param buffer Buffer to read.
 * @param data Pointer to save data.
 * @return ring_status Read status.
 */
ring_status RingReadByte (ringBufferTypeDef* buffer, uint8_t* data);

/**
 * @brief Reads multiple bytes from buffer.
 * 
 * @param buffer Buffer to read data.
 * @param data Pointer to write data.
 * @param len Length of data to read.
 * @return ring_status Read status.
 */
ring_status RingReadMultipleBytes (ringBufferTypeDef* buffer, uint8_t* data, uint32_t len);

/**
 * @brief Returns write pointer of buffer.
 * 
 * @param buffer Buffer to get pointer.
 * @return uint32_t Write pointer.
 */
uint32_t RingGetHead (ringBufferTypeDef* buffer);

/**
 * @brief Returns read pointer of buffer.
 * 
 * @param buffer Buffer to get pointer.
 * @return uint32_t Read pointer.
 */
uint32_t RingGetTail (ringBufferTypeDef* buffer);

/**
 * @brief Gets last element from buffer without taking it from buffer.
 * 
 * @param buffer Buffer to read.
 * @return uint32_t Last byte in buffer.
 */
uint32_t RingGetLastElement(ringBufferTypeDef* buffer);

/**
 * @}
 * 
 */
#endif /* SRC_CODE_RING_RING_H_ */
