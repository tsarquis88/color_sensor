#ifndef UTILS_H_
#define UTILS_H_

#include <stdint.h>
/*
 * Port definition for GPIO functions
 */
#define PORT_0 (uint8_t) 0
#define PORT_1 (uint8_t) 1
#define PORT_2 (uint8_t) 2
#define PORT_3 (uint8_t) 3

/*
 * Pin definition for GPIO functions
 */
#define PIN_0 ( (int32_t) (1<<0) )
#define PIN_1 ( (int32_t) (1<<1) )
#define PIN_2 ( (int32_t) (1<<2) )
#define PIN_3 ( (int32_t) (1<<3) )
#define PIN_4 ( (int32_t) (1<<4) )
#define PIN_5 ( (int32_t) (1<<5) )
#define PIN_6 ( (int32_t) (1<<6) )
#define PIN_7 ( (int32_t) (1<<7) )
#define PIN_8 ( (int32_t) (1<<8) )
#define PIN_9 ( (int32_t) (1<<9) )
#define PIN_10 ( (int32_t) (1<<10) )
#define PIN_11 ( (int32_t) (1<<11) )
#define PIN_12 ( (int32_t) (1<<12) )
#define PIN_13 ( (int32_t) (1<<13) )
#define PIN_14 ( (int32_t) (1<<14) )
#define PIN_15 ( (int32_t) (1<<15) )
#define PIN_16 ( (int32_t) (1<<16) )
#define PIN_17 ( (int32_t) (1<<17) )
#define PIN_18 ( (int32_t) (1<<18) )
#define PIN_19 ( (int32_t) (1<<19) )
#define PIN_20 ( (int32_t) (1<<20) )
#define PIN_21 ( (int32_t) (1<<21) )
#define PIN_22 ( (int32_t) (1<<22) )
#define PIN_23 ( (int32_t) (1<<23) )
#define PIN_24 ( (int32_t) (1<<24) )
#define PIN_25 ( (int32_t) (1<<25) )
#define PIN_26 ( (int32_t) (1<<26) )
#define PIN_27 ( (int32_t) (1<<27) )
#define PIN_28 ( (int32_t) (1<<28) )
#define PIN_29 ( (int32_t) (1<<29) )
#define PIN_30 ( (int32_t) (1<<30) )
#define PIN_31 ( (int32_t) (1<<31) )

#define HIGH (uint8_t) 1
#define LOW (uint8_t) 0


/*
 * IO definition for GPIO functions
 */

#define OUTPUT (int32_t) 1
#define INPUT (int32_t) 0

/*
 * Public functions
 */

void SysTick_Handler();
void set_pin(uint32_t port_num, uint32_t pin_num);
void clear_pin(uint32_t port_num, uint32_t pin_num);
void write_pin(uint32_t port_num, uint32_t pin_num,uint32_t value);
void send_spi(uint16_t);
uint32_t map(uint32_t x, uint32_t in_min, uint32_t in_max, uint32_t out_min, uint32_t out_max);
uint32_t distance(uint32_t c1,uint32_t c2);
void retardo_systick(uint32_t value_in_10msec);
float constrain(float value, float value_min, float value_max);
void config_leds_pinsel();

#endif /* UTILS_H_ */
