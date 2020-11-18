/*
 * Exercício 2 – SoftPWM.c
 *
 * Created: 15/09/2020 14:39:16
 * Author : saraa
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRB = 0b00100000; // Habilita o pino PB5 como saída
	float dutyCycle = 0.01;
	
	while(1)
	{
		int T = 32;
		int tHigh = dutyCycle*T;

		PORTB = 0b00100000;// Pino PB5 em nível alto
		for (int i = 0; i < tHigh; i++)
		{
			_delay_ms(1);
		}

		PORTB = 0b00000000;// Pino PB5 em nível baixo
		for (int j = tHigh; j < T; j++)
		{
			_delay_ms(1);
		}
		
		if (dutyCycle < 1)
		{
			dutyCycle += 0.01;
		} 
		else
		{
			dutyCycle = 0.01;
		}
	}
}

