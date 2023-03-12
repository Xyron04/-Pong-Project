//Biblioteca teclado
//Autores Gerard Calderón y Alex Calizaya
//Utilizar para configurar puerto uart0 y recibir datos

#include "stdint.h"

//Función que configura el uart0. 9600 baudios, 1 bit de parada, sin paridad y Fifo habilitado
//Parámetros de entrada: Ninguno
//Parámetros de salida: Ninguno

void config_uart0( void );

//Función que recibe un dato por el uart0
//Parámetros de entrada: Niguno
//Parámetros de salida: Dato recibido de 8 bits sin signo
uint8_t rxcar_uart0(void);

