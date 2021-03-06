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


int task_01_id;
void task_01(void)
{
    unsigned char *rec;
    while(1) {
        rec = recv_msg_queues();
        if (rec) {
            printf("T1:%d\n\r", *rec);
            free(rec);
        }
    }
}

int task_02_id;
void task_02(void)
{
    unsigned char *rec;
    while(1){
        rec = recv_msg_queues();
        if (rec) {
            printf("T2:%d\n\r", *rec);
            free(rec);
        }
    }
}


void task_03(void)
{
    int i = 0;
    while(1) {
      printf("T-3 time:%d\n\r", i++);
      os_delay_ms(2500);
    }
}


void task_04(void)
{
    int i = 0;
    unsigned char *send;
    while(1) {
        send = malloc(4);
        if (send != NULL) {
            i++;
            *send = i;
            if (i%2)
                send_msg_queues(task_01_id, send);
            else
            send_msg_queues(task_02_id, send);
        }
        os_delay_ms(500);
        
    }
}


int main()
{
    nvic_vtor_remapping(APP_START_ADD - 0x08000000);
    sys_clk_init(9);

    uart_init(72, 115200);  // 115200

    info();

        task_init();    //init idle task (head)

        //TODO:creat task
    task_01_id = creat_task(task_01, 512, 10);
    task_02_id = creat_task(task_02, 512, 10);
    creat_task(task_03, 512, 10);
    creat_task(task_04, 512, 10);

    rtos_start();   //start OS


    while(1) 
        ;           //not run here

    return 0;
}


