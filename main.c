#include "stm32g0xx.h"

uint32_t entrada1=0, entrada2=0;
int btn1ANT=0, btn2ANT=0;
int count=0;

int main(){

	// Config
	RCC->IOPENR=0x3f; // Habilitando o clock
	// Entradas > PC8 PC9 [00]
	GPIOC->MODER &= ~(GPIO_MODER_MODE8 | GPIO_MODER_MODE9);

	// Saídas > PC0 a PC7 e PC10 E PC11 [01 ]
	GPIOC->MODER &= ~(0b1111111111111111);
	GPIOC->MODER |= (0b0101010101010101);

	GPIOC->MODER &= ~(GPIO_MODER_MODE10 | GPIO_MODER_MODE11);
	GPIOC->MODER |= (GPIO_MODER_MODE10_0 | GPIO_MODER_MODE11_0);

	// TIMER TIM14
	RCC->APBENR2 |= RCC_APBENR2_TIM14EN;
	TIM14->PSC = (160-1); // (16MHz / 160 = 100kHz de freq. do timer)
	TIM14->ARR = 20000000; // A cada 20000000 contagens ele estoura (100kHz / 20000000 = 5mHz) => de 200 em 200ms
	TIM14->CR1 |= TIM_CR1_CEN; // Liga o contador

	//  Ação
	while (1){
		entrada1 = (GPIOC->IDR & GPIO_IDR_ID8);
		entrada2 = (GPIOC->IDR & GPIO_IDR_ID9);
		GPIOC->ODR &= ~0b110011111111;
		if (TIM14->SR & TIM_SR_UIF){ // Se estourou o timer
			TIM14->SR &= ~TIM_SR_UIF; // Desativa a flag
			if (entrada1 && btn1ANT == 0){ // Testa o estado do botão
				count++;
				btn1ANT=1;
			}
			else if (entrada1 == 0b0 && btn1ANT == 1){
				btn1ANT=0;
			}
			if (entrada2 && btn2ANT == 0){
				count--;
				btn2ANT=1;
			}
			else if (entrada2 == 0b0 && btn2ANT == 1){
				btn2ANT=0;
			}
		}
		switch(count){ // De acordo com o count, imprime o dígito no display
		case -1:
			count=0;
		case 0:
			GPIOC->ODR |= GPIO_ODR_OD11;
			GPIOC->ODR |= 0b0111111;
			break;
		case 1:
			GPIOC->ODR |= 0b0000110;
			break;
		case 2:
			GPIOC->ODR |= 0b1011011;
			break;
		case 3:
			GPIOC->ODR |= 0b1001111;
			break;
		case 4:
			GPIOC->ODR |= 0b1100110;
			break;
		case 5:
			GPIOC->ODR |= 0b1101101;
			break;
		case 6:
			GPIOC->ODR |= 0b1111101;
			break;
		case 7:
			GPIOC->ODR |= 0b0000111;
			break;
		case 8:
			GPIOC->ODR |= 0b1111111;
			break;
		case 9:
			GPIOC->ODR |= GPIO_ODR_OD10;
			GPIOC->ODR |= 0b1101111;
			break;
		case 10:
			count=9;
			break;
		}
	}
}
