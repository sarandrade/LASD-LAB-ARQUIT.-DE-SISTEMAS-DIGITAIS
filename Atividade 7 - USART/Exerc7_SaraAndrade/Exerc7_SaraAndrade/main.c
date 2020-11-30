/*
* Exerc7_SaraAndrade.c
*
* Created: 19/10/2020 13:54:53
* Author : saraa
*/

#define F_CPU 16000000UL // Frequência de trabalho da CPU
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "nokia5110.h"

char sala, quarto;

ISR(USART_RX_vect)
{
	char recebido;
	recebido = UDR0;
	
	// Luz do Quarto
	if(recebido == 'l')
	{
		sala = 'l';
		PORTC = 0b00000001;
	}
	if(recebido == 'd')
	{
		sala = 'd';
		PORTC = 0b00000000;
	}
	
	// Luz da Sala
	if(recebido == '0')
	{
		quarto = '0';
		OCR0A  = 0;
	}
	if(recebido == '1')
	{
		quarto = '1';
		OCR0A = 0.25*256;
	}
	if(recebido == '2')
	{
		quarto = '2';
		OCR0A = 0.5*256;
	}
	if(recebido == '3')
	{
		quarto = '3';
		OCR0A = 0.75*256;
	}
	if(recebido == '4')
	{
		quarto = '4';
		OCR0A = 254;
	}
	
	if(recebido == 'c'){
		nokia_lcd_set_cursor(0, 40);
		nokia_lcd_write_string("Cort.:Fechando", 1);
		nokia_lcd_render();
		for (int i = 0; i < 16; i++)
		{
			PORTC = 0b00010000;
			_delay_ms(100);
			PORTC = 0b00001000;
			_delay_ms(100);
			PORTC = 0b00000100;
			_delay_ms(100);
			PORTC = 0b00000010;
			_delay_ms(100);
		}
		PORTC = 0b00010000;
		_delay_ms(100);
	}
	if(recebido == 's'){
		nokia_lcd_set_cursor(0, 40);
		nokia_lcd_write_string("Cort.: Abrindo", 1);
		nokia_lcd_render();
		for (int i = 0; i < 16; i++)
		{
			PORTC = 0b00000010;
			_delay_ms(100);
			PORTC = 0b00000100;
			_delay_ms(100);
			PORTC = 0b00001000;
			_delay_ms(100);
			PORTC = 0b00010000;
			_delay_ms(100);
		}
	}
	
	atualizaDisplay(recebido);
	
	USART_Transmit(recebido);
}

ISR(INT0_vect) // Interrupção externa 0, Sensor de Presença
{
	nokia_lcd_clear();
	nokia_lcd_set_cursor(20, 20);
	nokia_lcd_write_string("ALARME!!", 3);
	nokia_lcd_render();
	
	_delay_ms(3000);
	
	nokia_lcd_clear();
	nokia_lcd_write_string("CasaLASD", 1);
	nokia_lcd_set_cursor(0, 10);
	nokia_lcd_write_string("--------------", 1);
	nokia_lcd_set_cursor(0, 20);
	atualizaDisplay(quarto);
	atualizaDisplay(sala);
}

// Função para inicialização da USART
void USART_Init(unsigned int ubrr)
{
	UBRR0H = (unsigned char)(ubrr>>8); //Ajusta a taxa de transmissão
	UBRR0L = (unsigned char)ubrr;
	UCSR0B = (1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0); //Habilita o transmissor e o receptor
	UCSR0C = (1<<USBS0)|(3<<UCSZ00); //Ajusta o formato do frame: 8 bits de dados e 2 de parada
	
	DDRC = 0xFF; //Define a porta C como saída
}

// Função para envio de um frame de 5 a 8 bits
void USART_Transmit(unsigned char data)
{
	while(!( UCSR0A & (1<<UDRE0)));// Espera a limpeza do registr. de transmissão
	UDR0 = data; // Coloca o dado no registrador e o envia
}

// Função para recepção de um frame de 5 a 8 bits
unsigned char USART_Receive(void)
{
	while(!(UCSR0A & (1<<RXC0))); // Espera o dado ser recebido
	return UDR0; // Lê o dado recebido e retorna
}

void main(void)
{
	DDRD =	0b11111011; // Todos os pinos da porta D como saídas, exceto o pino PD2
	PORTD = 0b00000100; // Habilita pull-up do pino PD2
	
	DDRC =	0b11111111; // Todos os pinos da porta C como saídas
	PORTC = 0b00000000;
	
	//Configuração das interrupções
	EICRA = 0b00000010;// Interrupção externa INT0 na borda de descida
	EIMSK = 0b00000001;// Habilita a interrupção externas INT0
	
	// Fast PWM, TOP = 0xFF, OC0A habilitado
	TCCR0A = 0b10000011; // PWM não invertido no pino OC0A
	TCCR0B = 0b00000101; // Liga TC0, prescaler = 1024, fpwm = f0sc/(256*prescaler) = 16MHz/(256*1024) = 61Hz
	OCR0A = 0; // Controle do ciclo ativo do PWM 0C0A
	
	USART_Init(MYUBRR);
	sei();// Habilita interrupções globais, ativando o bit I do SREG
	
	nokia_lcd_init();
	nokia_lcd_clear();
	nokia_lcd_write_string("CasaLASD", 1);
	nokia_lcd_set_cursor(0, 10);
	nokia_lcd_write_string("--------------", 1);
	nokia_lcd_set_cursor(0, 20);
	nokia_lcd_write_string("L. Quarto: OFF", 1);
	nokia_lcd_set_cursor(0, 30);
	nokia_lcd_write_string("L. Sala: Niv 0", 1);
	nokia_lcd_set_cursor(0, 40);
	nokia_lcd_write_string("Cort.: Fechada", 1);
	nokia_lcd_render();
	
	while(1)
	{

	}
}

void atualizaDisplay(char entrada){
	switch(entrada){
		case('l'):
		nokia_lcd_set_cursor(0, 20);
		nokia_lcd_write_string("L. Quarto: ON ", 1);
		break;
		case('d'):
		nokia_lcd_set_cursor(0, 20);
		nokia_lcd_write_string("L. Quarto: OFF", 1);
		break;
		// Luz da Sala
		case('0'):
		nokia_lcd_set_cursor(0, 30);
		nokia_lcd_write_string("L. Sala: Niv 0", 1);
		break;
		case('1'):
		nokia_lcd_set_cursor(0, 30);
		nokia_lcd_write_string("L. Sala: Niv 1", 1);
		break;
		case('2'):
		nokia_lcd_set_cursor(0, 30);
		nokia_lcd_write_string("L. Sala: Niv 2", 1);
		break;
		case('3'):
		nokia_lcd_set_cursor(0, 30);
		nokia_lcd_write_string("L. Sala: Niv 3", 1);
		break;
		case('4'):
		nokia_lcd_set_cursor(0, 30);
		nokia_lcd_write_string("L. Sala: Niv 4", 1);
		break;
		case('s'):
		nokia_lcd_set_cursor(0, 40);
		nokia_lcd_write_string("Cort.: Aberta ", 1);
		break;
		case('c'):
		nokia_lcd_set_cursor(0, 40);
		nokia_lcd_write_string("Cort.: Fechada", 1);
		break;
	}
	nokia_lcd_render();
}