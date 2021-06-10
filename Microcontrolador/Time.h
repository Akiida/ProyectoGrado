#include <stdio.h>
#include "STM32F4xx.h"

class Tim{

	
	private:
		
		int numero;
		int direccion;
		double periodo;
		
	public:
		
		Tim(int pnumero,int pdireccion,double pperiodo);
		void iniciar(); 
};