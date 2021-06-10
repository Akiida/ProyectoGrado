#include <stdio.h>
#include "STM32F4xx.h"
#include "Pines.h"
#include "Time.h"

TIM_TypeDef* tmr[14] = {TIM1, TIM2, TIM3, TIM4, TIM5, TIM6, TIM7, TIM8, TIM9, TIM10, TIM11, TIM12, TIM13, TIM14};


Tim :: Tim(int pnumero,int pdireccion,double pperiodo){

	numero = pnumero;
	direccion = pdireccion;
	periodo = pperiodo;

}

void Tim ::iniciar(){

	tmr[numero-1] ->PSC=15;
	tmr[numero-1]-> ARR= periodo/0.000001;
	tmr[numero-1]->DIER|=0X1;
	tmr[numero-1]->CR1|=0X1;
	if(direccion == 1){
		tmr[numero-1]->CR1|= (1UL<< 4);
	}
}
	