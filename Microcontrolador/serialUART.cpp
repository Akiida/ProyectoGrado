#include <stdio.h>
#include "STM32F4xx.h"
#include "pines.h"
#include "serialUART.h"

/*
FUNCIONES ALTERNANTES

--> pinx_0 - 7 --> resgistro bajo  , AF8
--> pinx_8 - 15  --> registro alto , AF8

*/


pin TX4('C',10, 2,0,0,0);//UART4
pin RX4('C',11, 2,0,0,0);

pin TX5('C',12, 2,0,0,0);//UART5
pin RX5('D',2, 2,0,0,0);



SUART::SUART(int puart, int pvel){

	uart = puart;
	vel = pvel;
	
}


void SUART::AddUart(){

	switch(uart){
	
		case 4:
	RCC->APB1ENR |= (1UL << 19); 							 
	UART4->BRR = (16000000)/(vel);     
  UART4->CR1 |= (1UL << 2); //RE
  UART4->CR1 |= (1UL << 3); //TE
	UART4->CR1 |= (1UL << 5); //RXNEIE
	UART4->CR1 |= (1UL << 13); //UE
	NVIC_EnableIRQ(UART4_IRQn);      
	TX4.AddPin();
	RX4.AddPin();
	GPIOC->AFR[1] |= 0x8800;               
		break;
	
	
		case 5:
	RCC->APB1ENR |= (1UL << 20); 										
	UART5->BRR = (16000000)/(vel);     
  UART5->CR1 |= (1UL << 2); //RE
  UART5->CR1 |= (1UL << 3); //TE
	UART5->CR1 |= (1UL << 5); //RXNEIE
	UART5->CR1 |= (1UL << 13); //UE      
	NVIC_EnableIRQ(UART5_IRQn);      
	TX5.AddPin();
  RX5.AddPin();
	GPIOC->AFR[1] |= 0x80000;            
 	GPIOD->AFR[0] |= 0x800;     
		break;
	
	}
	

}


void SUART::enviar(char dato){

	switch(uart){
	
		case 4:
			UART4 -> DR = dato;
			while ((UART4->SR &= 0x80)==0); 
		break;
		
		case 5:
			UART5 -> DR = dato;
			while ((UART5->SR &= 0x80)==0); 
		break;
	
	}

}