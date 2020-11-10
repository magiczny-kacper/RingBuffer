# RingBuffer
![Build Status](https://github.com/magiczny-kacper/RingBuffer/workflows/BuildWrokflow/badge.svg)

<br>
My ring buffer implementation, for embedde applications. Tested on STM32L1, STM32F3, STM32F4 and PIC18F platforms. Includes unit tests.<br/>
* Src - folder containing source code.
* Unit_test - Unit tests source files.
* Docs - Documentation files.
* unity - Unit tests framework.

# Usage
## Initialization
If user uses fixed size arrays mode, function 'RingInit' takes only one argument, which is pointer to 'RingBuffer_t' structure. <br/>
If variable arrays mode is used, user should declare array of type uint8_t, with desired size. This array and its size should be given as parameters to 'RingInit' function. <br/>
If inputs parameters given are valid, function should return 'OK'. Now the buffer is ready to use. <br/>
Note: there could be more than one buffer declared.

# To do:
- [ ] Add makefile
- [x] Add makefile for unit tests
- [x] Add unit tests files 
- [x] Generate doxygen docs 
- [ ] Provide examples