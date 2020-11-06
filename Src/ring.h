/**
 * @file ring.h
 * @author Kacper Brzostowski (kapibrv97@gmail.com)
 * @link https://github.com/magiczny-kacper
 * @brief FIFO ring buffer driver library header.
 * @version 1.1.0
 * @date 2020-10-11
 * 
 * @copyright GNU General Public License v3.0
 * 
 */

/*! \mainpage My Personal Index Page
 *
 * \section intro_sec Introduction
 *
 * This is my implementation of ring buffer. It was primary created as a part of bigger project, but I decided to share it with everyone. <br>
 * It was created for use with STM32L151 peripherals (UART, SPI). Then I used it on STM32F4, STM32F3, and PIC18F platforms without problems. <br>
 * It implements two types of work. <br>
 * One is opration on an array pointer, which gives an ability to create multiple buffers with different size,
 * but the user must provide an array for buffer. <br>
 * Second is with fixed size array, where user only has to create buffer structure, bute every 
 * buffer have the same size.
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

/**< Ring buffer will oprate on array pointer. */
#define RING_BUFFER_POINTERS 1

/**< Ring buffer will operate on fixed size array. */
#define RING_BUFFER_ARRAYS 0

/**< Selected ring buffer mode. */
#define RING_BUFFER_MODE RING_BUFFER_POINTERS

#ifndef RING_BUFFER_MODE
#define RING_BUFFER_MODE RING_BUFFER_ARRAYS
#endif

#if RING_BUFFER_MODE == RING_BUFFER_ARRAYS
	/**< Fixed buffer size when @ref RING_BUFFER_MODE is set to @ref RING_BUFFER_ARRAYS */
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
	NO_PTR = -3, /**< Returned if null pointer was given as parameter. */
	NO_DATA = -2, /**< Returned if there was no data to read in buffer. */
	NO_PLACE = -1, /**< Returned if there was no place to write data in buffer. */
	OK = 1 /**< Returned when write/read was succesfull/ */
} RingStatus_t;

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
	uint8_t buffer[BUFFER_SIZE]; /**< Actual buffer. */
#elif RING_BUFFER_MODE == RING_BUFFER_POINTERS
	uint8_t* buffer; /**< Pointer to array holding ring buffer. */
#endif
} RingBuffer_t;

/**
 * Function that returns size of whole ring buffer.
 *
 * @param buffer 	Pointer to buffer structure, which size has to be returned.
 * @retval 			Size of pointed ring buffer.
 */
uint32_t RingGetMaxSize (RingBuffer_t* buffer);

/**
 * @brief Function that returns available space in selected buffer.
 * 
 * @param buffer Pointer to buffer structure, which size has to be returned.
 * @return uint32_t Available size in provided buffer.
 */
uint32_t RingGetSpace (RingBuffer_t* buffer);

/**
 * @brief Function that returns count of data available in buffer.
 * 
 * @param buffer Pointer to buffer structure
 * @return uint32_t Data count in buffer.
 */
uint32_t RingGetDataCnt (RingBuffer_t* buffer);

/**
 * @brief Function to initialize selected ring buffer.
 * 
 * @param buffer 	Pointer to buffer structure that has to be initialized.
 * @return @ref RingStatus_t
 */
RingStatus_t RingInit (RingBuffer_t* buffer
#if RING_BUFFER_MODE == RING_BUFFER_POINTERS
	, uint8_t* arrayBuffer, uint32_t bufferSize
#endif
);

/**
 * @brief Addes one byte to the end of buffer.
 * 
 * @param buffer Pointer to buffer to write.
 * @param data Data to write.
 * @return RingStatus_t Status of write process.
 */
RingStatus_t RingWriteByte (RingBuffer_t* buffer, uint8_t data);

/**
 * @brief Writes multiple bytes to buffer.
 * 
 * @param buffer Buffer to write data.
 * @param data Data pointer to write.
 * @param len Length of data.
 * @return RingStatus_t Write status.
 */
RingStatus_t RingWriteMultipleBytes (RingBuffer_t* buffer, uint8_t* data, uint32_t len);

/**
 * @brief Reads one byte from buffer.
 * 
 * @param buffer Buffer to read.
 * @param data Pointer to save data.
 * @return RingStatus_t Read status.
 */
RingStatus_t RingReadByte (RingBuffer_t* buffer, uint8_t* data);

/**
 * @brief Reads multiple bytes from buffer.
 * 
 * @param buffer Buffer to read data.
 * @param data Pointer to write data.
 * @param len Length of data to read.
 * @return RingStatus_t Read status.
 */
RingStatus_t RingReadMultipleBytes (RingBuffer_t* buffer, uint8_t* data, uint32_t len);

/**
 * @brief Returns write pointer of buffer.
 * 
 * @param buffer Buffer to get pointer.
 * @return uint32_t Write pointer.
 */
uint32_t RingGetHead (RingBuffer_t* buffer);

/**
 * @brief Returns read pointer of buffer.
 * 
 * @param buffer Buffer to get pointer.
 * @return uint32_t Read pointer.
 */
uint32_t RingGetTail (RingBuffer_t* buffer);

/**
 * @brief Gets last element from buffer without taking it from buffer.
 * 
 * @param buffer Buffer to read.
 * @return uint32_t Last byte in buffer.
 */
uint32_t RingGetLastElement(RingBuffer_t* buffer);

/**
 * @}
 * 
 */
#endif /* SRC_CODE_RING_RING_H_ */
