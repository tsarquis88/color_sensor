#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_exti.h"
#include "lpc17xx_timer.h"
#include "../include/utils.h"
#include "../include/servo.h"

uint32_t pos_actual;

void TIMER0_IRQHandler() {

	NVIC_DisableIRQ(TIMER0_IRQn);
	if(TIM_GetIntStatus(LPC_TIM0, TIM_MR1_INT) == SET) {
		TIM_ClearIntPending(LPC_TIM0, TIM_MR1_INT);
		clear_pin(PINSEL_PORT_0, PIN_0);
	}

	if(TIM_GetIntStatus(LPC_TIM0, TIM_MR0_INT) == SET){
		TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT);
		set_pin(PINSEL_PORT_0, PIN_0);
	}
	NVIC_EnableIRQ(TIMER0_IRQn);
}

void config_servo_timer(){
		TIM_TIMERCFG_Type timer_config;
		TIM_MATCHCFG_Type match_config0;
		TIM_MATCHCFG_Type match_config1;

		timer_config.PrescaleOption = TIM_PRESCALE_USVAL;
		timer_config.PrescaleValue = 1;
		TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &timer_config);

		match_config0.MatchChannel = 0;
		match_config0.IntOnMatch = ENABLE;
		match_config0.ResetOnMatch = DISABLE;
		match_config0.StopOnMatch = DISABLE;
		match_config0.ExtMatchOutputType= TIM_EXTMATCH_NOTHING;
		match_config0.MatchValue = initial_pos;
		TIM_ConfigMatch(LPC_TIM0, &match_config0);

		match_config1.MatchChannel = 1;
		match_config1.IntOnMatch = ENABLE;
		match_config1.ResetOnMatch = ENABLE;
		match_config1.StopOnMatch = DISABLE;
		match_config1.ExtMatchOutputType= TIM_EXTMATCH_NOTHING;
		match_config1.MatchValue = 20000;
		TIM_ConfigMatch(LPC_TIM0, &match_config1);

}

void config_servo_pinsel() {
	//Servo
	PINSEL_CFG_Type pin_config;
	pin_config.Portnum = PINSEL_PORT_0;
	pin_config.Pinnum = PINSEL_PIN_0;
	pin_config.Pinmode = PINSEL_PINMODE_PULLUP;
	pin_config.Funcnum = PINSEL_FUNC_0;
	pin_config.OpenDrain = PINSEL_PINMODE_NORMAL;
	PINSEL_ConfigPin(&pin_config);
	GPIO_SetDir(PORT_0, PIN_0 , OUTPUT);
}

/********************************************************************//**
 * @brief        Lleva el servo a la posicion indicada por el parámetro
 * @param[in]    pos    posición en grados, debe ser un valor entre 0 y 180
 * @return         None
 *********************************************************************/
void set_servo(uint32_t pos){
	uint32_t aux=pos_actual;
	if(pos > pos_actual){
		while(aux != pos){
			TIM_UpdateMatchValue(LPC_TIM0,0,map(aux,0,180,560,2600));
			retardo_systick(3);
			aux++;
		}
	}else{
		while(aux != pos){
				TIM_UpdateMatchValue(LPC_TIM0,0,map(aux,0,180,560,2600));
				retardo_systick(3);
				aux--;
		}
	}
	pos_actual = pos;
}

void start_servo_timer() {
	//Servo
	TIM_ResetCounter(LPC_TIM0);
	TIM_Cmd(LPC_TIM0, ENABLE);
	NVIC_EnableIRQ(TIMER0_IRQn);
}
