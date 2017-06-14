
# scheduler

stm32f1xx scheduler


#RTOS#

##Overview##

主要功能：
==
1.	task独立调度

2.	task通信

3.	task主动挂起

4.	支持优先级（可配置）

5.	动态内存管理

6.	动态创建task

7.	超小代码尺寸

8.	临界代码保护


##Sub-module##
==
#Mem#

内存管理单元，主要实现了对动态内存分配和释放，对堆的管理
相关函数：
malloc()
free()
堆尺寸20K供支配


#Driver#

与SOC片内硬件资源相关驱动：
主要包括timer、uart、gpio、clk、nvic


#Glib#

需要用到的c库函数的重新实现
主要包括printf、memcpy、memset、strlen、strcmp


#Rtos#

操作系统部分
主要包括：
1.	task线程调度
2.	task挂起
3.	task消息传递
4.	task创建、初始化
5.	task优先级管理


#Common#
应用程序


###Use guidelines###


