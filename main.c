#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "Nokia5110.h"
#include "Biblioteca_teclado.h"
#include "Biblioteca_pulsadores.h"

uint16_t contador_general= 1 , contador_bola_barra2=1, contador_barra1 = 1;
uint8_t opc, modo=0, control=0, dificultad=1, velocidad_bola_barra2, velocidad_barra1;
int8_t ball_x1, ball_y1, ball_x2, ball_y2;
uint8_t barra1_x1, barra1_x2, barra1_y1, barra1_y2;
uint8_t barra2_x1, barra2_x2, barra2_y1, barra2_y2;

/*Función de configuración del UART*/

void ConfiguraTimer_1ms(void){ //Se configura el systick para 1ms por ciclo
     /*Inhabilitamos el módulo SysTick*/
     NVIC_ST_CTRL_R &= ~NVIC_ST_CTRL_ENABLE;
     /* TiempoDeseado=1ms; FreqCPU=50MHZ
      * valor que debe ir en el registro RELOAD=X
      * X=(TiempoDeseado)*(FreqCPU) -1
      * X= [[1*10^(-3)] * [16*10^(6)]] -1 = 15999
      *159999 en hexadecimal es 000270FF*/
     NVIC_ST_RELOAD_R = (NVIC_ST_RELOAD_R&0xFF000000)|0x00003E7F;

     // Iniciamos el contador con cero (escribiendo cualquier valor)
     NVIC_ST_CURRENT_R &= ~(0x00FFFFFF);
     // Habilitamos el módulo SysTick
     NVIC_ST_CTRL_R |= (NVIC_ST_CTRL_CLK_SRC | NVIC_ST_CTRL_ENABLE);
}


void menus(void){ //Funcion de menús: agrupa la portada principal, los menús de modo, control y de dificultad
	menu_inicio();//Muestra el inicio a través del bitmap
	while(1){
		if((GPIO_PORTF_DATA_R & 0x11)==0x10){									//Se espera a que se presione SW2
			while((GPIO_PORTF_DATA_R & 0x11) == 0x10);		//Se espera a que se suelte para salir
			break;
		}
	}
	
	menu_modo(modo); //Muestra el menú modo a través del bitmap
	
	while(1){
		opc = evaluar_pulsadores_menu();//Evalúa los pulsadores para cambiar de poscisión el cuadro que señala 1J o 2J
		if(opc != 0){
			if(modo == 0){
				modo = modo+1;//Modo se coloca a 1 para señalar 2J a través de la función menu_modo
			}
			else{
				modo = 0;//Modo se coloca a 0 para señalar 1J a través de la función menu_modo
			}
			menu_modo(modo);//Señala la opción elegida según reciba la variable modo
		}
		
		if((GPIO_PORTF_DATA_R & 0x11)==0x10){									//Se espera a que se presione SW2
			while((GPIO_PORTF_DATA_R & 0x11) == 0x10);
			break;
		}		
	}
	
	Nokia5110_Clear(); //Se limpia la pantalla
	menu_control(control); //Se muestra el menú para elegir control
	
	while(1){
		opc = evaluar_pulsadores_menu();//Se evaluan pulsadores
		if(opc != 0){																					
			if(control == 0){
				control ++;
			}
			else{
				control = 0;
			}
			control = control+1;
			menu_control(control);//Recibe la variable control para mostrar qué control se desea elegir
		}
		
		if((GPIO_PORTF_DATA_R & 0x11)==0x10){									//Se espera a que se presione SW2
			while((GPIO_PORTF_DATA_R & 0x11) == 0x10);		//Se espera a que se suelte SW2 para continuar
			break;
		}		

	}
	
	Nokia5110_Clear();	//Se limpia la pantalla
	menu_dificultad(1); //Se muestra la pantalla de la dificultad seleccionandose por defecto la dificultad fácil
	
	
	
	while(1){
		opc = evaluar_pulsadores_menu();//Se evalúa si se presionan los pulsadores
		if(opc != 0){																					
			if((opc == 2) || (opc == 4)){//Si se usa el Key 2 y 4, se sube
				dificultad = dificultad -1;
			}
			else{//Se se usa el Key 1 y 3, se baja
				dificultad = dificultad +1;
			}
			if(dificultad < 1){//Si se pulsa Key 2 o 4 estando en fácil, pasa a difícil
				dificultad = 3;
			}
			if(dificultad > 3){// Si se pulsa Key 1 y 3 estando en difícil, pasa a fácil
				dificultad = 1;
			}
			
			menu_dificultad(dificultad);//Muestra los cambios de dificultad en pantalla
		}
		
		if((GPIO_PORTF_DATA_R & 0x11)==0x10){									//Se espera a que se presione SW2
			while((GPIO_PORTF_DATA_R & 0x11) == 0x10);					//Se espera a que se deje de pulsar
			break;
		}			
	}
	Nokia5110_Clear();// Se limpia la pantalla
}


void actualizarContador(){//Actualiza los contadores cuando pase 1ms
	//preguntamos si paso 1ms
	if((NVIC_ST_CTRL_R & NVIC_ST_CTRL_COUNT)){ 
		//cada vez que pase 1ms, el contador de tiempo se incrementa
    contador_bola_barra2++;
		contador_barra1++;
		contador_general++;
	}
}

void score(uint8_t score1, uint8_t score2){//Recibe el marcador durante el juego
	
	const unsigned char score_1[] = {score1+0x30};//Se convierte en código ASCCI
	const unsigned char score_2[] = {score2+0x30};
	
	clearFilledRectangle(32, 4, 35,  9);
	clearFilledRectangle(52, 4, 55,  9);
	
	text( 32 , 4 , score_1, 0);//Se escribe el marcador en la pantalla para el J1 o CPU
	text( 52 , 4 , score_2, 0);//Se escribe el marcador en la pantalla para el J2 o Jugador vs CPU
	
}

void inicio_de_juego(void){//Función que reinicia el juego en las posiciones iniciales

	barra1_x1=3;
	barra1_x2=5;
	barra1_y1=19;
	barra1_y2=29;
		
	barra2_x1=80;
	barra2_x2=82;
	barra2_y1=19;
	barra2_y2=29;
	
	ball_x1=41;
	ball_y1=22;
	ball_x2=44;
	ball_y2=25;
	
	if(modo == 0){//Configuración para el modo vs CPU
		velocidad_bola_barra2 = 60;
		if(dificultad == 1){
			velocidad_barra1 = 120;
		}
		else if(dificultad == 2){
			velocidad_barra1 = 80;
		}
		else{
			velocidad_barra1 = 60;
		}
	}
	else{//Configuración para el modo J1 vs J2
		if(dificultad == 1){//Config para la dificultad ´facil
			velocidad_bola_barra2 = 80;
			velocidad_barra1 = 80;
		}
		else if(dificultad == 2){//Config para la dificultad media
			velocidad_barra1 = 60;
			velocidad_bola_barra2 = 60;
		}
		else{//Config para la dificultad dificil
			velocidad_barra1 = 40;
			velocidad_bola_barra2 = 40;
		}
	}
}

int8_t evaluar_colision_x(){//Evalúa la colision con las barras
	if((ball_x1 == 3) && (ball_y1 >= barra1_y1-1) && (ball_y1 <= barra1_y2+1)){//Evalúa si colisiona con la barra de la izqu
		return 1;
	}
	else if((ball_x2 == 82) && (ball_y1 >= barra2_y1-1) && (ball_y1 <= barra2_y2+1)){//Evalúa si colisiona con la barra de la der
		return -1;
	}
	else{//Si no golpea, retorna a 0
		return 0;
	}
}

int8_t evaluar_colision_y(){//Evalúa la colisión contra la pared
	if((ball_y1 == 1) || (ball_y2 == 47)){
		return 1;
	}
	else{
		return 0;
	}
}


void Movimiento_J1(void){ //Función que mueve la barra de la izquierda
	if((GPIO_PORTE_DATA_R & 0x03) == 0x02){
				if(barra1_y1>=1){
					barra1_y1--;
					barra1_y2--;
				}
			}
	if((GPIO_PORTE_DATA_R & 0x03) == 0x01 ){
				if(barra1_y2<=47){
					barra1_y1++;
					barra1_y2++;
				}
			}
}

void Movimiento_J2(void){ //Función que mueve la barra de la derecha
	uint8_t tecla;
	
	if(control == 0){		
		if((GPIO_PORTE_DATA_R & 0x0C) == 0x08 ){
			if(barra2_y1>=1){
				barra2_y1--;
				barra2_y2--;
			}
			}
		if((GPIO_PORTE_DATA_R & 0x0C) == 0x04){
			if(barra2_y2<=47){
				barra2_y1++;
				barra2_y2++;
			}
		}
	}
	else{
		tecla = rxcar_uart0();
				
		if(tecla==0x57 || tecla==0x77){
			if(barra2_y1>=1){
					barra2_y1--;
					barra2_y2--;
				}
		}
		if(tecla==0x53 || tecla==0x73){
			if(barra2_y2<=47){
					barra2_y1++;
					barra2_y2++;
			}
		}
	}
}

void Movimiento_IA (int8_t direccion){ //Función que se encarga del movimeinto de la CPU
	uint8_t posicion_media = (barra1_y1 + barra1_y2)/2;
	if((ball_y1 < posicion_media)&&( barra1_y1 >= 1)&&(direccion < 0)){
		barra1_y1--;
		barra1_y2--;
	}
	if((ball_y1 > posicion_media)&&( barra1_y1 <= 47)&&(direccion < 0)){
		barra1_y1++;
		barra1_y2++;
	}
	if((posicion_media < 24)&&(direccion > 0)){
		barra1_y1++;
		barra1_y2++;
	}
	if((posicion_media > 24)&&(direccion > 0)){
		barra1_y1--;
		barra1_y2--;
	}
}

void pausar(){//Se pausa el juego si se presiona SW1, se deja de pausar si se suelta SW1
	uint8_t i=1;
	
	if((GPIO_PORTF_DATA_R & 0x11)==0x01){
		while(i==1){
			if((GPIO_PORTF_DATA_R & 0x11)==0x11)
				i=0;
		}	
	}
}

void prender_led(uint8_t jugador){ //Función que prende los leds PB4 y PB5 dependiendo del gol anotado
	
	uint16_t contador=500, tiempo=0;
	
	if(jugador==1)
		GPIO_PORTB_DATA_R |= 0x10;
	if(jugador==2)
		GPIO_PORTB_DATA_R |= 0x20;
	
	while(contador>tiempo){
		if((NVIC_ST_CTRL_R & NVIC_ST_CTRL_COUNT))
			tiempo++;
	}
	tiempo=0;
	
	GPIO_PORTB_DATA_R &= ~(0x30);
}

void conmutar_leds(){ //Funcion que se encarga de conmutar 5 veces los leds (ocurre cuando hay un ganador)
	uint8_t contador=5, tiempo=0;
	while(contador>tiempo){
		prender_led(1);
		prender_led(2);
		tiempo++;
	}
}

uint8_t fin_partida(uint8_t ganador, uint8_t modo){	//Función que se encarga del efecto del término de la partida
	uint8_t tecla;
	
	Nokia5110_Clear();//Se limpia la pantalla
	
	if(ganador == 1 && modo == 0){
		gana_cpu();//Se pinta en la pantalla PERDISTE
	}
	else if(ganador == 2 && modo == 0){
		gana_jugador();//Se pinta en la pantalla GANASTE
	}
	else if(ganador == 1 && modo == 1){
		ganador_j1();//Se pinta en la pantalla GANA J1
	}
	else if(ganador == 2 && modo == 1){
		ganador_j2();//Se pinta en la pantalla GANA J2
	}
	conmutar_leds(); //Una vez mostrado el mensaje, se conmutan los leds
	contador_general = 0;//Se reinician todos los contadores
	contador_bola_barra2 = 0;
	contador_barra1 = 0;
	while(1){//Se espera a que se presione un SW
		tecla = rxcar_uart0();
		if((GPIO_PORTF_DATA_R & 0x11)==0x10 || tecla==0x0D){									//Si se presiona SW2, se vuelve a jugar con la misma config
			while((GPIO_PORTF_DATA_R & 0x11) == 0x10);
			return 1;
		}
		if((GPIO_PORTF_DATA_R & 0x11)==0x01 || tecla==0x1B){									//Si se presiona SW1, se regresa al menú principal
			while((GPIO_PORTF_DATA_R & 0x11) == 0x01);
			return 2;
		}
	}
}

int main(){	//Programa principal
	uint8_t score_1 = 0, score_2 = 0, nueva_partida=0;
	int8_t direccion_x = 1, direccion_y = 1, temp;

	config_uart0();
	Nokia5110_Init();//Se configura la pantalla Nokia
  Nokia5110_Clear();//Se limpia la pantalla
	ConfiguraTimer_1ms();//Se configura el systick a 1 ms
	config_pulsadores();	//Se configuran los pulsadores (Incluye leds)
	
	while(1){
		menus();	//Muestra los menús para configurar la partida
		
		inicio_de_juego();	//Inicia el juego en las posicones por defecto
		
		while(1){
			actualizarContador();	//Se espera el ciclo de 1 ms
			
			if(contador_general >= 20){//Se tiene un pequeño retardo
				if(contador_barra1 >= velocidad_barra1){
					clearFilledRectangle(barra1_x1, barra1_y1, barra1_x2,  barra1_y2);//Se limpia la posicion anterior de la barra
					
					if(modo == 0){//Si el modo de juego es para un jugador, se activa la IA
						Movimiento_IA(direccion_x);
					}
					else{//Si no, se controla manualmente la barra
						Movimiento_J1();
					}
					
					drawFilledRectangle(barra1_x1, barra1_y1, barra1_x2, barra1_y2);//Se dibuja la barra 1
					contador_barra1 = 0;
				}
				
				if(contador_bola_barra2 >= velocidad_bola_barra2){
					
					clearFilledRectangle(barra2_x1, barra2_y1, barra2_x2,  barra2_y2);
					clearFilledRectangle(ball_x1, ball_y1, ball_x2, ball_y2);
					
					Movimiento_J2();	//Movimiento de la barra 2
					
					drawFilledRectangle(barra2_x1, barra2_y1, barra2_x2, barra2_y2);//Se dibuja la barra 2
			
					temp = evaluar_colision_x();	//se evalúa si hubo alguna colisión
					if(temp != 0){//Si colisiona
						direccion_x = temp;
						drawFilledRectangle(barra1_x1, barra1_y1, barra1_x2, barra1_y2);
					}
				
					ball_x1= ball_x1+direccion_x; ball_x2 = ball_x2+direccion_x;//Se reescribe la bola segun lo recibido por evaluar colision
					ball_y1= ball_y1+direccion_y; ball_y2 = ball_y2+direccion_y;
					
					if(evaluar_colision_y()){//Se pregunta si se colisiona con als paredes
						direccion_y = -direccion_y;//Si es así, cambia de dirección
					}
					if(ball_x2 > 84){//Si llega a la meta de la derecha
						score_1 ++;				// Marca J1
						prender_led(1);
						Nokia5110_Clear();
						clearBuffer();
						if(score_1 == 10){ // GANA J1
							nueva_partida = fin_partida(1, modo);
						}
						else{
							inicio_de_juego();//Si aún no se llega al puntaje, se reinician las posiciones para seguir jugando
						}
					}
					else if(ball_x1 < 0){//Si llega a la meta de la izquierda
						score_2 ++;		// Marca J2
						prender_led(2);
						Nokia5110_Clear();
						clearBuffer();
						if(score_2 == 10){ //Gana J2
							nueva_partida = fin_partida(2, modo);
						}
						else{
							inicio_de_juego();//Si aún no se llega al puntaje, se reinician las posiciones para seguir jugando
						}
					}
					else{//Si no se marca, se sigue moviendo el balón
						drawFilledRectangle(ball_x1, ball_y1, ball_x2, ball_y2);//Se sigue dibujando el balón
						score(score_1, score_2);//Se dibuja el score
						copyToScreen();//Se manda todo a la pantalla para pintarla
					}
					contador_bola_barra2 = 0;
					
					if(nueva_partida == 1){//Se reinicia el juego
						Nokia5110_Clear();
						inicio_de_juego();
						score_1 = 0;
						score_2 = 0;
						
						nueva_partida=0;
					}
					if(nueva_partida == 2 ){//Se vuelve al menú
						nueva_partida=0;
						score_1 = 0;
						score_2 = 0;
						break;
					}
				}
				copyToScreen();//Se pinta en la pantalla
				pausar();
				contador_general = 0;//Se reinicia el contador
			}	
		}
	}
}

