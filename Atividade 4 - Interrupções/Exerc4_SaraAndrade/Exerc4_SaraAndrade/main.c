/*
 * Exerc4_SaraAndrade.c
 *
 * Created: 28/09/2020 09:34:06
 * Author : saraa
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int8_t dezena, unid, pausa; // Variáveis globais
uint8_t LUT[10] = {0x7E, 0x30, 0x6D, 0x79, 0x33, 0x5B, 0x5F, 0x70, 0x7F, 0x7B};
	
ISR(INT0_vect) // Interrupção externa 0, captura o start/pause
{	
	if(pausa == 1)
	{
		pausa = 0;
	}
	else
	{
		pausa = 1;
	}
}

ISR(INT1_vect) // Interrupção externa 1, captura o reset
{
	dezena = 0; 
	unid = 0;
	PORTC = LUT[dezena];
	PORTB = LUT[unid];
}

int main(void)
{
	DDRD =	0x00; // Todos os pinos da porta D como entradas
	PORTD = 0b00001100; // Habilita pull-ups dos pinos PD2 e PD3
	
	DDRC =	0xFF; // Habilita Todos os pinos da porta C como saída
	DDRB =	0xFF; // Habilita Todos os pinos da porta B como saída
	
	//Configuração das interrupções
	EICRA = 0b00001010;// Interrupções externas INT0 e INT1 na borda de descida
	EIMSK = 0b00000011;// Habilita as interrupções externas INT0 e INT1
	sei();// Habilita interrupções globais, ativando o bit I do SREG
	
	pausa = 1;
	dezena = 0;
	unid = 0;
	while (1)
	{
		PORTC = LUT[dezena];
		PORTB = LUT[unid];
		
		if(pausa != 1)
		{
			if(unid < 9)
			{
				unid++;
			}
			else
			{
				unid = 0;
				
				if(dezena < 9)
				{
					dezena++;
				}
				else
				{
					dezena = 0;
				}
			}
			_delay_ms(1000);
		}
	}
}

