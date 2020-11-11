# RingBuffer
(https://github.com/magiczny-kacper/RingBuffer/workflows/BuildWrokflow/badge.svg)
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

### Short description
My ring buffer implementation, for embedde applications. Tested on STM32L1, STM32F3, STM32F4 and PIC18F platforms. Includes unit tests.
* Src - folder containing source code.
* Unit_test - Unit tests source files.
* Docs - Documentation files.
* unity - Unit tests framework.

# Usage
This part explains basic usage of this ring buffer library.
## Initialization
If user uses fixed size arrays mode, function `RingInit` takes only one argument, which is pointer to `RingBuffer_t` structure. <br/>
If variable arrays mode is used, user should declare array of type `uint8_t`, with desired size. This array and its size should be given as parameters to `RingInit` function. <br/>
If inputs parameters given are valid, function should return `OK`. Now the buffer is ready to use. <br/>
Note: there could be more than one buffer declared.
## Writing to buffer
There are two functions used to write data to buffer. These are:
* `RingWriteByte` - used to write only one byte to buffer. As parameters, function takse pointer to buffer handler, and as second argument, one byte of data to be written.
* `RingWriteMultipleBytes` - used to write one or more bytes to buffer. First parameter is pointer to buffer handler, second is pointer to data to be written, and the third is length of data to write. </br></br>
Both functions return `OK` if write was succesfull, or `NO_PLACE` if there was not enough space in buffer, or `NO_PTR` if null pointer was given as an argument.
## Reading from buffer
As before, to read from buffer, there are two functions provided.
* `RingReadByte` - used to read one byte from given buffer. 
* `RingReadMultipleBytes` - used to read one or more bytes from buffer.
## Additional functions
* `RingGetHead` - returns next array index to write.
* `RingGetTail` - returns index of the next element from array that will be read.
* `RingGetMaxSize` - returns buffer size in bytes.
* `RingGetSpace` - retuns free spce in buffer.
* `RingGetDataCnt` - returns number of unread bytes in buffer.
* `RingGetLastElement` - returns last element from buffer without taking it from buffer. Could be usable for example when using as UART receive buffer, for waiting if received data string was terminated with specific value.
# To do:
- [x] Add makefile for unit tests
- [x] Add unit tests files 
- [x] Generate doxygen docs 
- [x] Provide examples