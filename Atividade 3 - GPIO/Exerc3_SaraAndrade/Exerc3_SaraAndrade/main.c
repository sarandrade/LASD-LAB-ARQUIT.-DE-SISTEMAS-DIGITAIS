/*
 * Exerc3_SaraAndrade.c
 *
 * Created: 21/09/2020 20:35:06
 * Author : saraa
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
	DDRB  = 0b11111111; //Habilita os pinos PB0..6 como saídas
	DDRD  &= ~(1<<0);	//DDRD  &= 0b11111110; Habilita o pino PD0 como entrada
	PORTD |= (1<<0);	//PORTD |= 0b00000001; Habilita o resistor de pull-up do pino PD0

	int8_t cont = 0, cont_4 = 8;
	uint8_t num = 0;
	uint8_t LED[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
	uint8_t LED_3[8] = {0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0xff};

	while (1)
	{   
		if (!(PIND & (1<<0))) //Retorna 1 se PD0 for 1 ... e 0 se PD0 for 0
		{
			while(!(PIND & (1<<0)));//Trava o programa até que o botão seja liberado
			num++;
		}
		if (num==5)
			num = 0;
		
		switch (num)
		{
		case 0:
			while((PIND & (1<<0)))
			{
				PORTB = LED[cont];
				cont++;
				if(cont == 8)
				{
					cont = 0;
				}
				_delay_ms(100);
			}
			break;
		case 1:
			while((PIND & (1<<0)))
			{
				cont--;
				if(cont == -1)
				{
					cont = 7;
				}
				PORTB = LED[cont];
				_delay_ms(100);		
			}
			break;
		case 2:
				while(cont != 8 && (PIND & (1<<0)))
				{
					PORTB = LED[cont];
					cont++;
					_delay_ms(100);
				}
				cont--;
				while(cont != -1 && (PIND & (1<<0)))
				{
					PORTB = LED[cont];
					cont--;
					_delay_ms(100);
				}
			break;
		case 3:
			cont = 0;
			while((PIND & (1<<0)))
			{
				PORTB = LED_3[cont];
				cont++;
				if(cont == 8)
				{
					cont = 0;
				}
				_delay_ms(100);
			}
			break;
		case 4:
			while(cont != cont_4 && (PIND & (1<<0)))
			{
				PORTB = LED[cont];
				cont++;
				_delay_ms(100);
			}
			cont_4--;
			if(cont_4 == 1)
			{
				cont_4 = 8;
			}
			cont--;
			while(cont != -1 && (PIND & (1<<0)))
			{
				PORTB = LED[cont];
				cont--;
				_delay_ms(100);
			}
			break;
		}
	}
}