/*
 * GPIO.c
 *
 * Created: 19/09/2020 18:28:09
 * Author : rafae
 */ 

//#define F_CPU 16000000UL
#include <avr/io.h>
//#include <util/delay.h>


int main(void)
{
	DDRC  |= 0b11111111; //Habilita os pinos PC0..7 como saídas
	DDRD  &= 0b00000000; //Habilita os pinos PD0..7 como entradas
	PORTD |= 0b00000001; //Desabilita os resistores de pull-up da porta D

	while (1)
	{
		PORTC = PIND;	
	}
}

