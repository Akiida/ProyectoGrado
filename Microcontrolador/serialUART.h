#include <stdio.h>
#include "STM32F4xx.h"



class SUART{

	private:
		int uart;
		int vel;
	
	public:
		SUART(int puart, int pvel);
		void AddUart();
		void enviar(char dato);
};