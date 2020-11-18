/*
 * Exerc6_SaraAndrade.c
 *
 * Created: 11/10/2020 13:49:31
 * Author : saraa
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

int8_t dezena, unid, count;
uint8_t LUT[10] = {0x7E, 0x30, 0x6D, 0x79, 0x33, 0x5B, 0x5F, 0x70, 0x7F, 0x7B};

ISR(INT0_vect) // Interrupção externa 0, Incrementa
{	
	if(count < 95)
	{
		count += 5;
		OCR0A  = (count*256)/100;
	}
	else if(count == 95)
	{
		count +=4;
		OCR0A = (count*256)/100;
	}
}

ISR(INT1_vect) // Interrupção externa 1, Decrementa
{
	if(count > 0 && count != 99)
	{
		count -= 5;
		OCR0A = (count*256)/100;
	}
	else if(count == 99)
	{
		count -=4;
		OCR0A = (count*256)/100;
	}
}

int main(void)
{
	DDRD =	0b11110011; // Todos os pinos da porta D como saídas, exceto os pinos PD2 e PD3
	PORTD = 0b00001100; // Habilita pull-ups dos pinos PD2 e PD3
	
	DDRC =	0xFF; // Habilita Todos os pinos da porta C como saída
	DDRB =	0xFF; // Habilita Todos os pinos da porta B como saída
	
	// Configuração das interrupções
	EICRA = 0b00001010;// Interrupções externas INT0 e INT1 na borda de descida
	EIMSK = 0b00000011;// Habilita as interrupções externas INT0 e INT1
	sei();// Habilita interrupções globais, ativando o bit I do SREG
	
	// Fast PWM, TOP = 0xFF, OC0A habilitado
	TCCR0A = 0b10000011; // PWM não invertido no pino OC0A
	TCCR0B = 0b00000101; // Liga TC0, prescaler = 1024, fpwm = f0sc/(256*prescaler) = 16MHz/(256*1024) = 61Hz
	OCR0A = 0; // Controle do ciclo ativo do PWM 0C0A

	count = 0;
	
	while (1)
	{
		dezena = count / 10;
		unid = count % 10;
		
		PORTC = LUT[dezena];
		PORTB = LUT[unid];
	}
}