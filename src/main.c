#include "stm32f4xx.h"

#define doisHz 7;
#define seteHz 2;

#define saida 01;

int frequencia = 2;
int count = 0;

int main(void)
{
	void timer14hz(void);
	void toggleC(long int pin);
	int contaAte();

  RCC->AHB1ENR=0x00000087;  // inicializa o clock;

  RCC->APB2ENR|=0x20000;  // inicializa o clock do TIMER10;
  TIM10->PSC=1999;  // coloca o PSC para ;
  TIM10->ARR=570;  // coloca o ARR para ;
  TIM10->CR1|=0x0005;  // habilita a contagem do TIMER10;
	TIM10->SR&=~(0x0001);  // "resetar" o cron�metro, botando zero no bit em quest�o;

  GPIOA->MODER=0x28000000;  // configura a fun��o de Debugger com o ARM;

  GPIOC->MODER=0X00000001; // PC0 para modo saida; PC13 ja esta como entrada (0x00);


  while (1) {
	if (((GPIOC->IDR)&0x00002000) == 0x0) {  // testa se o o pino da posi��o 13 � n�vel l�gico baixo;
		for (int i = 0; i < 10000; i++);
		if (((GPIOC->IDR)&0x00002000) == 0x0) {
			if (frequencia == 2) {
				frequencia = 7;
			} else {
				frequencia = 2;
			}
		}
	}
	timer14hz();
	if (count >= contaAte()) {
		count = 0;
		toggleC(0x00000001);  // inverte o PC0;
	}
  }
}

int contaAte() {
	if (frequencia == 2) {
		return doisHz;
	} else {
		return seteHz;
	}
}

void timer14hz (void) {
	if((TIM10->SR & 0x0001) == 0x0001) {  // verifica se o cron�metro foi "setado" (acabou a contagem);
		// enquanto o cron�metro n�o for "setado para 1", a contagem ir� continuar dentro desse while;
		TIM10->SR&=~(0x0001);  // "resetar" o cron�metro, botando zero no bit em quest�o;
		count++;
	}
}

void toggleC(long int pin) {
	GPIOC->ODR ^=  pin;
}

