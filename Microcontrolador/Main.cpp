#include <stdio.h>
#include "STM32F4xx.h"
#include "Pines.h"
#include "Time.h"
#include "serialUART.h"
int prueba = 0;
float prueba2 = 0;
//Serial
SUART uart4(4, 9600);
char entrada[5] = {"0"};
bool estadoAux = 0;
int contadorAux = 0;
char datoEntrada = 0;
float referencia[4] = {0,0,0,0};
float referenciaFalsa[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
bool giro[4] = {0,0,0,0};
bool giroFalso[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
bool estado = 0;
int NumPulsos1 = 0;
int NumPulsos2 = 0;
int NumPulsos3 = 0;
int NumPulsos4 = 0;
double Vrpm1 = 0;
double Vrpm2 = 0;
double Vrpm3 = 0;
double Vrpm4 = 0;
int Vrpm1E = 0;
int Vrpm2E = 0;
int Vrpm3E = 0;
int Vrpm4E = 0;
double frec1 = 0;
double frec2 = 0;
double frec3 = 0;
double frec4 = 0;
double velocidad1=0;
double velocidad2 = 0;
double velocidad3 = 0;
double velocidad4 = 0;
//Entrada para interrupcion
pin Entrada1('C', 0, 0, 0, 2, 2);
pin Entrada2('C', 1, 0, 0, 2, 2);
pin Entrada3('C', 2, 0, 0, 2, 2);
pin Entrada4('C', 4, 0, 0, 2, 2);
//timmer 7
Tim tm7(7,0,0.1);
float obser = 0;
//Para Control
//COntrol
float uk11 = 0;
float ek11 = 0;
float ek1 = 0;
float uk12 = 0;
float ek12 = 0;
float ek2 = 0;
float uk13 = 0;
float ek13 = 0;
float ek3 = 0;
float uk14 = 0;
float ek14 = 0;
float ek4 = 0;
float q1[4] = {100, 100, 100, 100};
float q2[4] = {-60, -60, -60, -60};
extern "C"{
 void UART4_IRQHandler(void){  
   if (UART4->SR & 0x20) {
    datoEntrada = UART4->DR;
    prueba = datoEntrada;
    if(prueba>=50){
       giroFalso[contadorAux] = 0;
       referenciaFalsa[contadorAux] = (datoEntrada-50)*(1/100.0)*2.0;
    }else{
      referenciaFalsa[contadorAux] = (datoEntrada)*(1/100.0)*2.0;
      giroFalso[contadorAux] = 1;
    }
    contadorAux++;
     if(datoEntrada == 95){
     if(contadorAux == 5){
      for(int k = 0; k<=3; k++){
       giro[k] = giroFalso[k];
       referencia[k] = referenciaFalsa[k];
      }
     }
      contadorAux = 0;
     }
   } 
 }
 void EXTI0_IRQHandler(void){//inicio
 EXTI -> PR |= 0;
 NumPulsos1++;
}//fin
void EXTI1_IRQHandler(void){//inicio
 EXTI -> PR |= 0;
 NumPulsos2++;
}//fin
  void EXTI2_IRQHandler(void){//inicio
 EXTI -> PR |= 0;
 NumPulsos3++;
 obser = NumPulsos3;
}//fin
  void EXTI4_IRQHandler(void){//inicio
 EXTI -> PR |= 0;
 NumPulsos4++;
}//fin
void TIM7_IRQHandler(void){
 TIM7->SR &= ~(1<<0);
 Vrpm1 = NumPulsos1*60.0*4.0/(4741.44*2);
 frec1 = NumPulsos1;
 NumPulsos1 = 0;
 Vrpm2 = NumPulsos2*60.0*4.0/(4741.44*2);
 frec2 = NumPulsos2;
 NumPulsos2 = 0;
 Vrpm3 = NumPulsos3*60.0*4.0/(4741.44*2);
 frec3 = NumPulsos3;
 NumPulsos3 = 0;
 Vrpm4 = NumPulsos4*60.0*4.0/(4741.44*2);
 frec4 = NumPulsos4;
 NumPulsos4 = 0;
   ek1 = referencia[0] - Vrpm1;
   velocidad1 = uk11 +ek1*q1[0] + ek11*(q2[0]);
   ek11 = ek1;
   uk11 = velocidad1;
     if (velocidad1>500)
       velocidad1 = 500;
     if (velocidad1<0)
       velocidad1 = 0;
 ek2 = referencia[1] - Vrpm2;
   velocidad2 = uk12 +ek2*q1[1] + ek12*(q2[1]);
   ek12 = ek2;
   uk12 = velocidad2;
     if (velocidad2>500)
       velocidad2 = 500;
     if (velocidad2<0)
       velocidad2 = 0;
 ek3 = referencia[2] - Vrpm3;
   velocidad3 = uk13 +ek3*q1[2] + ek13*(q2[2]);
   ek13 = ek3;
   uk13 = velocidad3;
     if (velocidad3>500)
       velocidad3 = 500;
     if (velocidad3<0)
       velocidad3 = 0;
 ek4 = referencia[3] - Vrpm4;
   velocidad4 = uk14 +ek4*q1[3] + ek14*(q2[3]);
   ek14 = ek4;
   uk14 = velocidad4;
     if (velocidad4>500)
       velocidad4 = 500;
     if (velocidad4<0)
       velocidad4 = 0;
  if(referencia[0]==0)
   velocidad1 = 0;
  if(referencia[1]==0)
   velocidad2 = 0;
  if(referencia[2]==0)
   velocidad3 = 0;
  if(referencia[3]==0)
   velocidad4 = 0;
//         //Enviar valores encoder
//      
Vrpm1E = Vrpm1*100;
   Vrpm2E = Vrpm2*100;
   Vrpm3E = Vrpm3*100;
   Vrpm4E = Vrpm4*100; 
   if(giro[0] == 0){
    Vrpm1E = Vrpm1E/2 + 50;
   }else{
    Vrpm1E = Vrpm1E/2;
   }
   if(giro[1] == 0){
    Vrpm2E = Vrpm2E/2 + 50;
   }else{
    Vrpm2E = Vrpm2E/2;
   }
   if(giro[2] == 0){
    Vrpm3E = Vrpm3E/2 + 50;
   }else{
    Vrpm3E = Vrpm3E/2;
   }
   if(giro[3] == 0){
    Vrpm4E = Vrpm4E/2 + 50;
   }else{
    Vrpm4E = Vrpm4E/2;
   }
   uart4.enviar(Vrpm1E);
   uart4.enviar(Vrpm2E);
   uart4.enviar(Vrpm3E);
   uart4.enviar(Vrpm4E);
   uart4.enviar('\r');
   uart4.enviar('\n');
//      
//      //Fin envio datos
}
}
int main(){
//Serial
 uart4.AddUart();
 pin ENA('A',0,2,0,0,0);
 pin ENB('A',1,2,0,0,0);
  pin ENAA('A',2,2,0,0,0);
 pin ENBB('A',3,2,0,0,0);
  ENA.AddPin();
 ENB.AddPin();
 ENAA.AddPin();
 ENBB.AddPin();
RCC->APB1ENR |=0X21; //TIMER 2-3
 GPIOA->AFR[0]|=0X1111;
 TIM2->PSC =20;
TIM2->ARR=500;
TIM2->DIER =0X1;
TIM2->CR1=0X1;
 TIM2->CCMR1 = 0X6060;
 TIM2->CCMR2 = 0X6060;
TIM2->CCER=0X1111;
TIM2->EGR|=0x1;
TIM2->CCR1=0;//ccr1 / arr = cicluo util
TIM2->CCR2=0;//ccr2 / arr = cicluo util
TIM2->CCR3=0;//ccr3 / arr = cicluo util
TIM2->CCR4=0;//ccr4 / arr = cicluo util
// 
// 
// ///FINPWM
//Sentido de Giro
pin IN1('D',13,1,0,1,3);//pin IN1
pin IN2('D',11,1,0,1,3);//pin IN2
pin IN3('D',9,1,0,1,3);//pin IN3
pin IN4('B',15,1,0,1,3);//pin IN4
pin IN11('B',3,1,0,1,3);//pin IN1
pin IN22('D',6,1,0,1,3);//pin IN2
pin IN33('D',4,1,0,1,3);//pin IN3
pin IN44('D',2,1,0,1,3);//pin IN4
IN1.AddPin();
IN2.AddPin();
IN3.AddPin();
IN4.AddPin();
 IN11.AddPin();
 IN22.AddPin();
IN33.AddPin();
IN44.AddPin();
 //habilitar timmer 5
tm7.iniciar();
NVIC_EnableIRQ(TIM7_IRQn);
 //Entrada - interrupcion
Entrada1.AddPin();
Entrada2.AddPin();
Entrada3.AddPin();
Entrada4.AddPin();
//habilitando syscfg n APB2 
RCC -> APB2ENR |= (1UL<<14); //0x4000
 //habilitando EXTI boton en SYSCFG
SYSCFG -> EXTICR[0] |= (0X222); // C0,C1,C2
SYSCFG -> EXTICR[1] |= (0X2); // C4
//IMR 
EXTI -> IMR |= (1UL<<0);
EXTI -> IMR |= (1UL<<1);
EXTI -> IMR |= (1UL<<2);
EXTI -> IMR |= (1UL<<4);
//FLANCO BAJADA
EXTI -> FTSR |= (1UL<<0);
EXTI -> FTSR |= (1UL<<1);
EXTI -> FTSR |= (1UL<<2);
EXTI -> FTSR |= (1UL<<4);
//NVIC 
NVIC_EnableIRQ(EXTI0_IRQn);
NVIC_EnableIRQ(EXTI1_IRQn);
NVIC_EnableIRQ(EXTI2_IRQn);
NVIC_EnableIRQ(EXTI4_IRQn);
while(true){
TIM2->CCR1=velocidad1;//ccr1 / arr = cicluo util
TIM2->CCR2=velocidad2;//ccr1 / arr = cicluo util
TIM2->CCR3=velocidad3;//ccr1 / arr = cicluo util
TIM2->CCR4=velocidad4;//ccr1 / arr = cicluo util
 //Giro1
 IN1.SetPinState(giro[0]);
 IN2.SetPinState(!giro[0]);
 //Giro2
 IN3.SetPinState(giro[1]);
 IN4.SetPinState(!giro[1]);
 //Giro3
 IN11.SetPinState(!giro[2]);
 IN22.SetPinState(giro[2]);
 //Giro4
 IN33.SetPinState(giro[3]);
 IN44.SetPinState(!giro[3]);
}
}
