//Biblioteca pulsadores
//Autores Gerard Calder�n y Alex Calizaya
//Utilizar para configurar puerto uart0 y recibir datos

#include "stdint.h"

//Funci�n que configura puerto F, E y B
//Puerto F: Activa Switch1 y Switch2
//Puerto E: Habilita PE0, PE1, PE2, PE3 como entradas
//Puerto B: Habilita PB4, PB5 como salidas
//Par�metros de entrada: Ninguno
//Par�metros de salida: Ninguno
void config_pulsadores(void);

//Funci�n que evalua el estado de los pulsadores SW1, SW2, PE0, PE1,PE2,PE3 al utilizar el men�
//Par�metros de entrada: Niguno
//Par�metros de salida: N�mero de 8 bits sin signo que indica si alg�n si se presion� alg�n pulsador
uint8_t evaluar_pulsadores_menu(void);
