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

int main()
{
	int ti = 0;
	nvic_vtor_remapping(APP_START_ADD - 0x08000000);
	sys_clk_init(9);

	uart_init(72, 115200); 	// 115200
	led_init(1);

	timer_init(100,719); // 10K 

	info();

	rtos_start();

	while(1) {
		set_time_out(100);
		while(get_time_out())
			;
		led_on(1);
		set_time_out(100);
        while(get_time_out())
            ;
		led_off(1);
	}

	return 0;
}


