
# scheduler

> * Support: stm32f1xx
> * bootloader: https://github.com/ShilinGuo520/bootloader


## Overview

The main function:
* Independent scheduling
* Task message
* Task state management(Hang ready)
* Support task priority
* Dynamic memory management
* Dynamically create and delete tasks
* Ultra Small Code Size (5K)
* Critical code protection

## Sub-module

### Mem
* Memory management unit, the main realization of the dynamic memory allocation and release, the heap management
* Related functions:malloc()、free()  
* Configurable memory pool size (default 20K)

### Driver
* And SOC chip hardware-related drivers:
* Mainly include:timer、uart、gpio、clk、nvic

### Glib
* The reused implementation of the c library function referenced
* Mainly include:printf、memcpy、memset、strlen、strcmp

### Rtos
* Operating system section
* Mainly include:
    * Task scheduling
    * Task state management(Hang ready)
    * Task message transmission(send recv)
    * Task create and delete
    * Task priority management

### Common
* User application
* Demo(main.c)
