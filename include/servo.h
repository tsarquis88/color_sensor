#ifndef SERVO_H_
#define SERVO_H_

#include <stdint.h>

#define initial_pos 560
#define final_pos 560

void TIMER0_IRQHandler();
void start_servo_timer();
void config_servo_timer();
void config_servo_pinsel();
void set_servo(uint32_t pos);

#endif /* SERVO_H_ */
