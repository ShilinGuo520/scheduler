#include "common.h"
#include "types.h"

#define uint32 u32

#define BIT(shift)                     (1UL << (shift))

#define SYSTICK_CSR_COUNTFLAG           BIT(16)                                                                                
#define SYSTICK_CSR_CLKSOURCE           BIT(2)                                                                                 
#define SYSTICK_CSR_CLKSOURCE_EXTERNAL  0
#define SYSTICK_CSR_CLKSOURCE_CORE      BIT(2)                                                                                 
#define SYSTICK_CSR_TICKINT             BIT(1)                                                                                 
#define SYSTICK_CSR_TICKINT_PEND        BIT(1)                                                                                 
#define SYSTICK_CSR_TICKINT_NO_PEND     0
#define SYSTICK_CSR_ENABLE              BIT(0)                                                                                 
#define SYSTICK_CSR_ENABLE_MULTISHOT    BIT(0)                                                                                 
#define SYSTICK_CSR_ENABLE_DISABLED     0                                                                                      

/* Calibration value register */                                                                                               

#define SYSTICK_CVR_NOREF               BIT(31)                                                                                
#define SYSTICK_CVR_SKEW                BIT(30)
#define SYSTICK_CVR_TENMS               0xFFFFFF

#define SYSTICK_RELOAD_VAL 71999	//1K 0.1K

/** SysTick register map type */
typedef struct systick_reg_map {
    volatile uint32 CSR;            /**< Control and status register */
    volatile uint32 RVR;            /**< Reload value register */
    volatile uint32 CNT;            /**< Current value register ("count") */
    volatile uint32 CVR;            /**< Calibration value register */
} systick_reg_map;

/** SysTick register map base pointer */
#define SYSTICK_BASE                    ((struct systick_reg_map*)0xE000E010)


extern void systick_init(uint32 reload_val);
extern void systick_disable();
extern void systick_enable();
extern void systick_attach_callback(void (*callback)(void));

