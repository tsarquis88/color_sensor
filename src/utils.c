#include <math.h>
#include "../include/utils.h"
#include "LPC17xx.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_systick.h"
#include "lpc17xx_spi.h"
#include "lpc17xx_pinsel.h"

static uint32_t flag_systick;
static uint32_t counter_systick=0;

void SysTick_Handler(){
	counter_systick++;
	if(counter_systick == 20){
		counter_systick=0;
		flag_systick = 1;
		SYSTICK_Cmd(DISABLE);
		SYSTICK_IntCmd(DISABLE);
	}
}

void set_pin(uint32_t port_num, uint32_t pin_num) {

	GPIO_ClearValue(port_num, pin_num);
}

void clear_pin(uint32_t port_num, uint32_t pin_num) {

	GPIO_SetValue(port_num, pin_num);
}

void write_pin(uint32_t port_num, uint32_t pin_num,uint32_t value){
	if(value == LOW ){
		GPIO_ClearValue(port_num, pin_num);
	}
	else{
		GPIO_SetValue(port_num, pin_num);
	}
}

uint32_t map(uint32_t x, uint32_t in_min, uint32_t in_max, uint32_t out_min, uint32_t out_max) {
  return (uint32_t)  (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void send_spi(uint16_t data){
	uint32_t stat = 0;
	SPI_SendData(LPC_SPI, data);
	while (!((stat = LPC_SPI->SPSR) & SPI_SPSR_SPIF));
}


float constrain(float value, float value_min, float value_max)
{
      if(value >= value_max)
      {
           return value_max;
      }
      else if(value <= value_min)
      {
           return value_min;
      }
      else
      {
           return value;
      }
}




void retardo_systick(uint32_t value_in_10msec){

	SYSTICK_InternalInit(value_in_10msec);
	SYSTICK_Cmd(ENABLE);
	SYSTICK_IntCmd(ENABLE);
	flag_systick = 0;
	while(flag_systick == 0){
	}
}

uint32_t distance(uint32_t c1,uint32_t c2){
	uint8_t r1= (uint8_t) (c1>>16);
	uint8_t r2= (uint8_t) (c2>>16);
	uint8_t g1= (uint8_t) (c1>>8);
	uint8_t g2= (uint8_t) (c2>>8);
	uint8_t b1= (uint8_t) (c1>>0);
	uint8_t b2= (uint8_t) (c2>>0);
	return sqrt( pow(r2-r1, 2) + pow(g2-g1, 2) + pow(b2-b1, 2) );
}

void config_leds_pinsel() {
	PINSEL_CFG_Type pin_config;

	// LED LPC ROJO
	pin_config.Funcnum = PINSEL_FUNC_0;
	pin_config.Pinnum = PINSEL_PIN_22;
	PINSEL_ConfigPin(&pin_config);
	GPIO_SetDir(PORT_0, PIN_22, OUTPUT);
	GPIO_SetValue(PORT_0, PIN_22);

	// LED LPC VERDE
	pin_config.Portnum = PINSEL_PORT_3;
	pin_config.Pinnum = PINSEL_PIN_25;
	PINSEL_ConfigPin(&pin_config);
	GPIO_SetDir(PORT_3, PIN_25, OUTPUT);
	GPIO_SetValue(PORT_3, PIN_25);

	// LED LPC AZUL
	pin_config.Pinnum = PINSEL_PIN_26;
	PINSEL_ConfigPin(&pin_config);
	GPIO_SetDir(PORT_3, PIN_26, OUTPUT);
	GPIO_SetValue(PORT_3, PIN_26);
}
