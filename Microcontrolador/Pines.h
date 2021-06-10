#include <stdio.h>
#include "STM32F4xx.h"

class pin{

	private:
		
		char puerto;
		int pn; 
		int mode;
		int type;
		int speed;
		int pupd;
	
	
	public:
		pin(char ppuerto,int ppn ,unsigned int  moder, unsigned int typer, unsigned int speedr, unsigned int pupdr);
		void AddPin();
		void SetPinState(int state);
		bool GetPinState();

};