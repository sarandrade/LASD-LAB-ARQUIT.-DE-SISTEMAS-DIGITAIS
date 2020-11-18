/*
 * GPIO_7seg.c
 *
 * Created: 20/09/2020 04:00:04
 * Author : rafae
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
	DDRC  = 0b11111111; //Habilita os pinos PC0..6 como saídas
	DDRD  &= ~(1<<0);	//DDRD  &= 0b11111110; Habilita o pino PD0 como entrada
	PORTD |= (1<<0);	//PORTD |= 0b00000001; Habilita o resistor de pull-up do pino PD0

	uint8_t num = 0, out = 0;
	uint8_t LUT[10] = {0x7E, 0x30, 0x6D, 0x79, 0x33, 0x5B, 0x5F, 0x70, 0x7F, 0x7B};

	//Switch
	//Program Memory Usage 	:	1200 bytes   3,7 % Full
	//Data Memory Usage 		:	0 bytes   0,0 % Full
	
	//LUT
	//Program Memory Usage 	:	1134 bytes   3,5 % Full
	//Data Memory Usage 		:	10 bytes   0,5 % Full

	while (1)
	{      // 0bxxxxxxxx &  0b00000001 = 0b0000000x
		if (!(PIND & (1<<0))) //Retorna 1 se PD0 for 1 ... e 0 se PD0 for 0
		{
			while(!(PIND & (1<<0)));//Trava o programa até que o botão seja liberado
			num++;
		}
		if (num==10)
			num = 0;
		
		_delay_ms(100);
		
		/*switch (num)
		{
		case 0:
			out =  0x7E;
			break;
		case 1:
			out =  0x30;
			break;
		case 2:
			out =  0x6D;
			break;
		case 3:
			out =  0x79;
			break;
		case 4:
			out =  0x33;
			break;
		case 5:
			out =  0x5B;
			break;
		case 6:
			out =  0x5F;
			break;
		case 7:
			out =  0x70;
			break;
		case 8:
			out =  0x7F;
			break;
		case 9:
			out =  0x7B;
			break;
		}
		
		PORTC = out;*/
		PORTC = LUT[num];//out;
		
		
	}
}

