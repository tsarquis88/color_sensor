#ifndef SENSOR_H_
#define SENSOR_H_

#define MEDIDAS (uint32_t) 5
#define TIEMPO_MEDIDA (uint32_t) 500		// en ms
/*
 * Los valores siguientes son acorde a la información del fabricante en la datasheet.
 */
#define r_min 2500
#define r_max 11000
#define g_min 2800
#define g_max 12000
#define b_min 3300
#define b_max 14000

typedef enum{
	SENSOR_CFG_RED,
	SENSOR_CFG_GREEN,
	SENSOR_CFG_BLUE,
	SENSOR_CFG_CLEAR
}color_mode_type;

void TIMER1_IRQHandler();
void EINT0_IRQHandler();
void config_sensor_timer();
void config_sensor_exti();
void config_sensor_pinsel();
void start_sensor_timer();
void config_mode_sensor(uint8_t color);
uint32_t sensar(uint8_t color) ;
uint32_t rgb();


#endif /* SENSOR_H_ */
