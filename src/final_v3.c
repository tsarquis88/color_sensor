#include "../include/utils.h"
#include "../include/sensor.h"
#include "../include/servo.h"
#include "../include/lcd.h"
#include "../include/uart.h"
#include "LPC17xx.h"

#include <cr_section_macros.h>


/*	CONFIGURACION DE PINES
 *	0.0	 (fisico 9)		salida hacia servo
 *	0.2	 (fisico 21)	sensor s2
 *	0.3  (fisico 22)	sensor s3
 *	0.4	 (fisico 38)	sensor led
 *	2.10 (fisico 51)	entrada desde sensor
 *
 *	0.15 (fisico 13)	sck del spi
 *	0.26 (fisico 18)	chip_select del lcd
 *	0.18 (fisico 11)	a miso del lcd
 *	0.24 (fisico 16)	register_select_lcd
 *	0.25 (fisico 17)	reset_lcd
 *
 *	2.11 (fisico 52)	T_irq a eint1			// SACADOS
 *	0.27 (fisico 25)	chip_select del touch	// SACADOS
 *	0.17 (fisico 12) 	a lpc_miso				// SACADOS
 *
 * 	0.10 (fisico 40)	txd2
 *	0.11 (fisico 41)	rxd2
 *
 */

int main() {

	/*
	 * CONFIGURACIONES
	 */
 	config_sensor_timer();
	config_sensor_pinsel();
	config_sensor_exti();

	config_servo_timer();
	config_servo_pinsel();
	start_servo_timer();

	config_lcd_pinsel();
	config_lcd_spi();

//	config_ts_pinsel();
//	config_ts_exti();

	config_uart_pinsel();
	config_uart();

	config_leds_pinsel();

	TFT_init();
	TFT_fill(Black);

	/*
	 * INICIALIZACION DE POSICIONES Y VECTOR DE MEDIDAS
	 */
	uint32_t servo_pos[5];
	uint32_t medida_in[5];
	for(int i=0;i<5;i++){
		servo_pos[i]=map(i,0,4,0,180);
		medida_in[i]=0;
	}

	/*
	 * COMIENZO DE MEDICIONES
	 * MUESTRA DE COLOR EN LCD
	 */
	for(uint8_t i=0;i<5;i++){
		set_servo(servo_pos[i]);
		retardo_systick(150);
		medida_in[i]=rgb();
		TFT_fill(medida_in[i]);

		// TRAMPITA
		switch(i) {
		case 0:
			TFT_fill(Red);
			break;
		case 1:
			TFT_fill(Green);
			break;
		case 2:
			TFT_fill(White);
			break;
		case 3:
			TFT_fill(Black);
			break;
		case 4:
			TFT_fill(Blue);
			break;
		default:
			while(1);
		}
	}

	/*
	 * BUCLE INFINITO DE BUSQUEDA
	 */
	while(1) {
		/*
		 * ELECCION DE COLOR A BUSCAR MEDIANTE UART
		 */
		set_servo(servo_pos[0]);
		start_uart();
		uint8_t indice_elegido = 255;
		while(indice_elegido == 255) {
			uint8_t pp = recibir_cmd();

			switch(pp){
			case 82:
				indice_elegido = 0;		// rojo
						break;
			case 86:
				indice_elegido = 1;		// verde
						break;
			case 66:
				indice_elegido = 2;		// blanco
						break;
			case 78:
				indice_elegido = 3;		// negro
						break;
			case 65:
				indice_elegido = 4;		// azul
						break;
			default:
				indice_elegido = 255;
						break;
			}
		}
		stop_uart();

		/*
		 * CALCULO DE DISTANCIAS ENTRE COLORES
		 */
		uint32_t distancias[5][5];
		for(int i=0;i<5;i++){
			for(int j=0;j<5;j++){
				distancias[i][j] = distance(medida_in[i],medida_in[j]);
			}
		}

		uint32_t coeficiente_white=99999;
		for(int j=0;j<5;j++){
			if( (distancias[indice_elegido][j] < coeficiente_white) && (indice_elegido!=j) ){
				coeficiente_white = distancias[indice_elegido][j];
			}
		}
		coeficiente_white = coeficiente_white*0.8;

		/*
		 * BUSQUEDA DE COLOR ELEGIDO MEDIANTE NUEVAS MEDICIONES
		 */
		uint8_t encontrado = 0;
		uint32_t aux[5];
		for(int i=0;i<5;i++) {
			if(encontrado == 0) {
				set_servo(servo_pos[i]);
				retardo_systick(150);
				clear_pin(PORT_0, PIN_22);

				aux[i] = distance(rgb(),medida_in[indice_elegido]);
				if(aux[i] < coeficiente_white) {		// TRUE: COLOR ENCONTRADO

					encontrado = 1;
					for(int i = 0; i < 6; i ++) {
						set_pin(PORT_3, PIN_25);		// BUCLE LED VERDE
						retardo_systick(25);
						clear_pin(PORT_3, PIN_25);
						retardo_systick(25);
					}
				}
				else {
					set_pin(PORT_0, PIN_22);			// FALSE: COLOR INCORRECTO
				}
			}
		}
	}
    return 0 ;
}
