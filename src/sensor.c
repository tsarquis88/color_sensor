#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_exti.h"
#include "lpc17xx_timer.h"
#include "../include/utils.h"
#include "../include/sensor.h"

static uint32_t pulsos = 0;
static uint32_t pulsos_acumulados = 0;
static uint8_t flag_sensor = 0;

void TIMER1_IRQHandler() {
	TIM_ClearIntPending(LPC_TIM1, TIM_MR1_INT);
	NVIC_DisableIRQ(TIMER1_IRQn);

	flag_sensor = 1;
	pulsos_acumulados = pulsos_acumulados + pulsos * 2;
	pulsos = 0;

	NVIC_EnableIRQ(TIMER1_IRQn);
}

void EINT0_IRQHandler() {
	EXTI_ClearEXTIFlag(EXTI_EINT0);
	NVIC_DisableIRQ(EINT0_IRQn);

	pulsos++;

	NVIC_EnableIRQ(EINT0_IRQn);
}

void config_sensor_timer() {

	//Sensor
	TIM_TIMERCFG_Type struct_timer1;
	TIM_MATCHCFG_Type struct_match1;

	struct_timer1.PrescaleOption = TIM_PRESCALE_USVAL;
	struct_timer1.PrescaleValue  = 1000;

	struct_match1.MatchChannel = 1;
	struct_match1.IntOnMatch = ENABLE;
	struct_match1.ResetOnMatch = ENABLE;
	struct_match1.StopOnMatch = DISABLE;
	struct_match1.ExtMatchOutputType = TIM_EXTMATCH_NOTHING;
	struct_match1.MatchValue = TIEMPO_MEDIDA;

	TIM_Init(LPC_TIM1, TIM_TIMER_MODE, &struct_timer1);
	TIM_ConfigMatch(LPC_TIM1, &struct_match1);

}

void config_sensor_pinsel(){
	PINSEL_CFG_Type pin_config;
	//Sensor
		//s1 s2
		pin_config.Funcnum = PINSEL_FUNC_0;
		pin_config.Portnum = PINSEL_PORT_0;
		pin_config.Pinnum = PINSEL_PIN_2;
		PINSEL_ConfigPin(&pin_config);
		GPIO_SetDir(PORT_0, PIN_2, OUTPUT);
		pin_config.Pinnum = PINSEL_PIN_3;
		PINSEL_ConfigPin(&pin_config);
		GPIO_SetDir(PORT_0, PIN_3, OUTPUT);
		// led
		pin_config.Pinnum = PINSEL_PIN_4;
		PINSEL_ConfigPin(&pin_config);
		GPIO_SetDir(PORT_0, PIN_4, OUTPUT);
		//out del sensor
		pin_config.Funcnum = PINSEL_FUNC_1;
		pin_config.Portnum = PINSEL_PORT_2;
		pin_config.Pinnum = PINSEL_PIN_10;
		PINSEL_ConfigPin(&pin_config);
		GPIO_SetDir(PORT_2, PIN_10, INPUT);
}

void config_sensor_exti() {
	EXTI_InitTypeDef struct_exti;
	struct_exti.EXTI_Line = EXTI_EINT0;
	struct_exti.EXTI_Mode = EXTI_MODE_EDGE_SENSITIVE;
	struct_exti.EXTI_polarity = EXTI_POLARITY_HIGH_ACTIVE_OR_RISING_EDGE;
	EXTI_Config(&struct_exti);
}

void start_sensor_timer() {
	//Sensor
	TIM_ResetCounter(LPC_TIM1);
	TIM_Cmd(LPC_TIM1, ENABLE);

}

void config_mode_sensor(color_mode_type color) {

	switch(color) {
	case SENSOR_CFG_RED:
		GPIO_ClearValue(PINSEL_PORT_0, PIN_2);
		GPIO_ClearValue(PINSEL_PORT_0, PIN_3);
		break;
	case SENSOR_CFG_GREEN:
		GPIO_SetValue(PINSEL_PORT_0, PIN_2);
		GPIO_SetValue(PINSEL_PORT_0, PIN_3);
		break;
	case SENSOR_CFG_BLUE:
		GPIO_ClearValue(PINSEL_PORT_0, PIN_2);
		GPIO_SetValue(PINSEL_PORT_0, PIN_3);
		break;
	case SENSOR_CFG_CLEAR:
		GPIO_SetValue(PINSEL_PORT_0, PIN_2);
		GPIO_ClearValue(PINSEL_PORT_0, PIN_3);
	default:
		while(1);
	}
}

/********************************************************************//**
 * @brief        Realiza varias mediciones consecutivas
 * @param[in]    color    Configuración de color, puede ser:
 * 							 RED, GREEN, BLUE o CLEAR
 * @return         Frecuencia promedio sensada en Hz
 *********************************************************************/
uint32_t sensar(color_mode_type color) {

	set_pin(PORT_3, PIN_26);
	uint32_t promedio=0;
	pulsos_acumulados = 0;				// reset pulsos_acumulados
	config_mode_sensor(color);				// configuro sensor de acuerdo al color
	for(int i=0; i<MEDIDAS; i++) {		// repito medicion MEDIDAS veces
		flag_sensor = 0;
		start_sensor_timer();					// comienzo timer y contador de pulsos.
		NVIC_EnableIRQ(TIMER1_IRQn);	// se contaran los pulsos hasta que
		NVIC_EnableIRQ(EINT0_IRQn);		// flag sea igual a 1, o sea
										// cuando interrumpa el timer
		while(!flag_sensor);
	}
										// en este punto, ya se hicieron todas las medidas
	NVIC_DisableIRQ(TIMER1_IRQn);		// y se guardaron en pulsos_acumulados
	NVIC_DisableIRQ(EINT0_IRQn);

	promedio = pulsos_acumulados / MEDIDAS;	// calculo de promedio
	clear_pin(PORT_3, PIN_26);

	return promedio;
}

/********************************************************************//**
 * @brief        Hace un sensado para los 3 colores, y convierte a 8 bits por canal
 *
 * @return       Dato de medicion, en formato RGB 8 bits
 *********************************************************************/
uint32_t rgb() {
	uint32_t r;
	uint32_t g;
	uint32_t b;
	uint8_t r_8;
	uint8_t g_8;
	uint8_t b_8;
	uint32_t rgb = 0;
	//Hago el sensado
	GPIO_SetValue(PORT_0, PIN_4);		// prendo led
	r = sensar(SENSOR_CFG_RED);
	g =	sensar(SENSOR_CFG_GREEN);
	b =	sensar(SENSOR_CFG_BLUE);
	GPIO_ClearValue(PORT_0, PIN_4);		// apago led
	//Lo mapeo a 8 bits
	r_8 = map(r, r_min, r_max, 0, 255);
	g_8 = map(g, g_min, g_max, 0, 255);
	b_8 = map(b, b_min, b_max, 0, 255);
	//Unifico para color rgb
	rgb = b_8 | (g_8<<8) | (r_8<<16);
	return rgb;
}
