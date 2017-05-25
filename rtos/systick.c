#include "systick.h"

static void (*systick_user_callback)(void);

void systick_init(uint32 reload_val) 
{
    SYSTICK_BASE->RVR = reload_val;
    systick_enable();
}

/**
 * Clock the system timer with the core clock, but don't turn it
 * on or enable interrupt.
 */
void systick_disable() 
{
    SYSTICK_BASE->CSR = SYSTICK_CSR_CLKSOURCE_CORE;
}

/**
 * Clock the system timer with the core clock and turn it on;
 * interrupt every 1 ms, for systick_timer_millis.
 */
void systick_enable() 
{
    /* re-enables init registers without changing reload val */
    SYSTICK_BASE->CSR = (SYSTICK_CSR_CLKSOURCE_CORE   |
                         SYSTICK_CSR_ENABLE           |
                         SYSTICK_CSR_TICKINT_PEND);
}

/**
 * @brief Attach a callback to be called from the SysTick exception handler.
 *
 * To detach a callback, call this function again with a null argument.
 */
void systick_attach_callback(void (*callback)(void)) 
{
    systick_user_callback = callback;
}

/*
 * SysTick ISR
 */

void SysTick_Handler_temp(void) 
{
    if (systick_user_callback) {
        systick_user_callback();
    }
}


