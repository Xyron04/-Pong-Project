//Biblioteca pulsadores
//Autores Gerard Calderón y Alex Calizaya
//Utilizar para configurar puerto uart0 y recibir datos

#include "stdint.h"

//Función que configura puerto F, E y B
//Puerto F: Activa Switch1 y Switch2
//Puerto E: Habilita PE0, PE1, PE2, PE3 como entradas
//Puerto B: Habilita PB4, PB5 como salidas
//Parámetros de entrada: Ninguno
//Parámetros de salida: Ninguno
void config_pulsadores(void);

//Función que evalua el estado de los pulsadores SW1, SW2, PE0, PE1,PE2,PE3 al utilizar el menú
//Parámetros de entrada: Niguno
//Parámetros de salida: Número de 8 bits sin signo que indica si algún si se presionó algún pulsador
uint8_t evaluar_pulsadores_menu(void);
