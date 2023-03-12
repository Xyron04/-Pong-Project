#include "Biblioteca_teclado.h"
#include "stdint.h"
#include "tm4c123gh6pm.h"

void config_uart0( void )
{
  uint32_t temp;

  SYSCTL_RCGC1_R |= SYSCTL_RCGC1_UART0; // Se activa el reloj del UART
  temp = SYSCTL_RCGC2_R;                // Espera de unos ciclos de reloj 
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA; // Se activa el reloj del puerto A 
  // PA0 (U0Rx) PA1( U0Tx)  	
  temp = SYSCTL_RCGC2_R;                // Espera de unos ciclos de reloj
  UART0_CTL_R &= ~ UART_CTL_UARTEN;     // Se desactiva el UART 
  UART0_IBRD_R = (UART0_IBRD_R & ~UART_IBRD_DIVINT_M)|104; // Se configura DIVINT 
  // 16MHz/(16*9600) 
  // Parte entera 
  UART0_FBRD_R = (UART0_FBRD_R & ~UART_FBRD_DIVFRAC_M)|11; //Se configura DIVFRAC
  // Parte fraccionaria*64 
  UART0_LCRH_R = ((UART0_LCRH_R & ~0x000000FF)|(UART_LCRH_WLEN_8)|(UART_LCRH_FEN));  
  // Se configuran los bits de datos, 1 bit de parada, sin paridad y habilita el
  //FIFO 
  UART0_CTL_R |= UART_CTL_UARTEN;       // Se habilita el UART 
  // Desactivamos modo analógico en PA0 y PA1 
  GPIO_PORTA_AMSEL_R &= ~(0x03); 
  // Conectamos UART0 a PA0 y PA1 
  GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R&0xFFFFFF00)|0x00000011; 
  // Activamos funciones alternas en PA0 y PA1 
  GPIO_PORTA_AFSEL_R |= 0x03; 
  // Activamos funciones digitales en PA0 y PA1 
  GPIO_PORTA_DEN_R |= 0x03;
}

uint8_t rxcar_uart0(void)
{
  uint8_t temp; 
  if((UART0_FR_R & UART_FR_RXFE)==0) // Se espera que llegue un dato
		temp= UART0_DR_R&0xFF;                  // Se toman solo 8 bits
  return temp; 
}
