# RingBuffer
![Build Status](https://github.com/magiczny-kacper/RingBuffer/workflows/BuildWrokflow/badge.svg)

<br>
My ring buffer implementation, for embedde applications. Tested on STM32L1, STM32F3, STM32F4 and PIC18F platforms. Includes unit tests.<br/>
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
* `RingWriteMultipleBytes` - used to write one or more bytes to buffer. First parameter is pointer to buffer handler, second is pointer to data to be written, and the third is length of data to write. </br>
Both functions return `OK` if write was succesfull, or `NO_PLACE` if there was not enough space in buffer, or `NO_PTR` if null pointer was given as an argument.
# To do:
- [ ] Add makefile
- [x] Add makefile for unit tests
- [x] Add unit tests files 
- [x] Generate doxygen docs 
- [ ] Provide examples