#include <stdio.h>
#include "STM32F4xx.h"
#include "Pines.h"

int port(char x);
GPIO_TypeDef* io(int y);
GPIO_TypeDef* gpio[11] = {GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOH, GPIOI};


pin::pin(char ppuerto,int ppn ,unsigned int moder, unsigned int typer, unsigned int speedr, unsigned int pupdr){
	
	puerto = ppuerto;
	pn = ppn;
	mode = moder;
	type = typer;
	speed = speedr;
	pupd = pupdr;

}

void pin::AddPin(){ 
	
	RCC -> AHB1ENR &= ~(1UL << port(puerto));
	RCC -> AHB1ENR |= (1UL << port(puerto));
	
	io(port(puerto)) -> MODER &= ~(3UL<<2*pn);
	if(mode != 0)
	io(port(puerto)) -> MODER |= (mode<<2*pn);
	
	
	io(port(puerto)) -> OTYPER &= ~(3UL<<pn);
	if(type != 0)
	io(port(puerto)) -> OTYPER |= (type<<pn);
	
	
	io(port(puerto)) -> OSPEEDR &= ~(3UL<<2*pn);
	if(speed != 0)
	io(port(puerto)) -> OSPEEDR |= (speed<<2*pn);
	
	io(port(puerto)) -> PUPDR &= ~(3UL<<2*pn);
	if(pupd != 0)
	io(port(puerto)) -> PUPDR |= (pupd<<2*pn);

}

void pin::SetPinState(int state){
		
	switch (state){		
		case 0:
			io(port(puerto)) -> ODR &= ~(1UL<<pn);
			break;
		case 1:
			io(port(puerto)) -> ODR |= (1UL<<pn);
			break;
	
	}
	
}

bool pin::GetPinState(){
	
	return (io(port(puerto)) -> IDR & (1UL<<pn));	
	
}


int port(char x){ //funcion para asignar valores a puerto
	
	for(int a = 65; a <= 75; a++ ){	
		if(x == a){
			return a - 65;
		}
	}
	
}

GPIO_TypeDef* io(int y){ // funcion para los gpio

	for(int a = 0; a <= 10; a++){
		if(a == y){
			return gpio[a];
		}
	}

}