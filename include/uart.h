#ifndef UART_H_
#define UART_H_


#define SIZE 1


void config_uart_pinsel();
void config_uart();
void start_uart();
void stop_uart();
void check(uint8_t*);
uint8_t recibir_cmd();

#endif /* UART_H_ */
