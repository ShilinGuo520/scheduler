#include "common.h"
#include "types.h"
#include "leds.h"
#include "uart.h"
#include "clk.h"
#include "glib.h"
#include "timer.h"
#include "rtos.h"
#include "mem.h"

void __MSR_MSP(u32 TopOfMainStack);

void info(void)
{
	printf("\n\rBuild Info:\n\rDate:%s\n\rTime:%s", __DATE__, __TIME__);
	printf("\n\rApp Start Add:%x", APP_START_ADD);
	printf("\n\rFlash Size:%d", FLASH_SIZE);
	printf("\n\rRam Size:%d\n\r", RAM_SIZE);
}

void task_01(void)
{
	while(1) {
		printf("T-1\n\r");
		os_delay_ms(50);
	}
}

void task_02(void)
{
	while(1){
		printf("T-2\n\r");
//		os_delay_ms(50);
	}
}


void task_03(void)
{
	while(1) {
		printf("T-3\n\r");
//		os_delay_ms(25);
	}
}


void task_04(void)
{
	while(1) {
		printf("T-4\n\r");
//		os_delay_ms(100);
	}
}


int main()
{
	nvic_vtor_remapping(APP_START_ADD - 0x08000000);
	sys_clk_init(9);

	uart_init(72, 115200); 	// 115200

	info();

    	task_init();    //init idle task (head)

    	//TODO:creat task
	creat_task(task_01, 512);
	creat_task(task_02, 512);
	creat_task(task_03, 512);
	creat_task(task_04, 512);

	rtos_start();   //start OS


	while(1) 
        ;           //not run here

	return 0;
}


