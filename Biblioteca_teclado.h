//Biblioteca teclado
//Autores Gerard Calder�n y Alex Calizaya
//Utilizar para configurar puerto uart0 y recibir datos

#include "stdint.h"

//Funci�n que configura el uart0. 9600 baudios, 1 bit de parada, sin paridad y Fifo habilitado
//Par�metros de entrada: Ninguno
//Par�metros de salida: Ninguno

void config_uart0( void );

//Funci�n que recibe un dato por el uart0
//Par�metros de entrada: Niguno
//Par�metros de salida: Dato recibido de 8 bits sin signo
uint8_t rxcar_uart0(void);

