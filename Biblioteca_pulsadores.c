#include "Biblioteca_pulsadores.h"
#include "stdint.h"
#include "tm4c123gh6pm.h"

void config_pulsadores(void){ //Se configura puertos de entrada y salida
	
	//Configuración puerto F
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;					//Se activa la señal de reloj del puerto F
	while( (SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R5)==0) {}//Se espera a que se active 
	
	GPIO_PORTF_LOCK_R = 0x4C4F434B; 									//Se desbloquea el puerto F
	GPIO_PORTF_CR_R |= 0x01; 													//Se habilita el SW2
	GPIO_PORTF_AFSEL_R &= ~0x11;											//No se utilizan funciones alternas
	GPIO_PORTF_DEN_R |= 0x11;													//Se habilita de PF0 y PF4 como digital
		
	GPIO_PORTF_DIR_R &= ~0x11; 												//PF0 y PF4 como entradas
	GPIO_PORTF_PUR_R |= 0x11; 												//Se activan resistencias pull up para PF0 Y PF4
		
	//Configuración puerto E
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R4;					//Se activa la señal de reloj del puerto F
	while( (SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R4)==0) {}//Se espera a que se active 
	
	GPIO_PORTE_AFSEL_R &= ~0x0F;											//No se utilizan funciones alternas
	GPIO_PORTE_DEN_R |= 0x0F;													//Se habilita de PE0 a PE3 como digital
	GPIO_PORTE_AMSEL_R &= ~0x0F;											//No se utilizan funciones analógicas	
	GPIO_PORTE_DIR_R &= ~0x0F; 												//PF0 y PF3 como entradas
	GPIO_PORTE_PCTL_R &= ~0x02; 											// Se deshabilita funciones alternas
		
	//Configuración puerto B
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;					//Se activa la señal de reloj del puerto B
	while( (SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R1)==0) {}//Se espera a que se active 
	
	GPIO_PORTB_AFSEL_R &= ~0x30;											//No se utilizan funciones alternas
	GPIO_PORTB_DEN_R |= 0x30;													//Se habilita de PE0 a PE3 como digital
	GPIO_PORTB_AMSEL_R &= ~0x30;											//No se utilizan funciones analógicas	
	GPIO_PORTB_DIR_R |= 0x30; 												//PB4 y PB5 como salidas
	GPIO_PORTB_PCTL_R &= ~0x30;												// Se deshabilita funciones alternas
	GPIO_PORTB_DATA_R &= ~0x30;											// Apagamos leds
}

uint8_t evaluar_pulsadores_menu(){	//Función que evalúa los pulsadores
	if((GPIO_PORTE_DATA_R & 0x01)==0x01){								
			while((GPIO_PORTE_DATA_R & 0x01)==0x01){}
				return 1;
	}
	
	if((GPIO_PORTE_DATA_R & 0x02)==0x02){								
			while((GPIO_PORTE_DATA_R & 0x02)==0x02){}
				return 2;
	}
	
	if((GPIO_PORTE_DATA_R & 0x04)==0x04){								
			while((GPIO_PORTE_DATA_R & 0x04)==0x04){}
				return 3;
	}
	
	if((GPIO_PORTE_DATA_R & 0x08)==0x08){								
			while((GPIO_PORTE_DATA_R & 0x08)==0x08){}
				return 4;
	}
	return 0;
}
