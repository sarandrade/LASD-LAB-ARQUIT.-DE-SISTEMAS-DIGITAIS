// EXEMPLO DE USO DA USART - AVR

#define F_CPU 16000000UL //Frequência de trabalho da CPU
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
//----------------------------------------------------------------------------------

ISR(USART_RX_vect)
{
	char recebido;
	recebido = UDR0;
	if(recebido=='l')
		PORTC = 0b00000001;
	if(recebido=='d')
		PORTC = 0b00000000;
	USART_Transmit(recebido);
}

// ||Função para inicialização da USART||
void USART_Init(unsigned int ubrr)
{
	UBRR0H = (unsigned char)(ubrr>>8); //Ajusta a taxa de transmissão
	UBRR0L = (unsigned char)ubrr;
	UCSR0B = (1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0); //Habilita o transmissor e o receptor
	UCSR0C = (1<<USBS0)|(3<<UCSZ00); //Ajusta o formato do frame: 8 bits de dados e 2 de parada
	
	DDRC = 0xFF; //Define a porta C como saída
}

// ||Função para envio de um frame de 5 a 8bits||
void USART_Transmit(unsigned char data)
{
	while(!( UCSR0A & (1<<UDRE0)));//Espera a limpeza do registr. de transmissão
	UDR0 = data; //Coloca o dado no registrador e o envia
}

// ||Função para recepção de um frame de 5 a 8bits||
unsigned char USART_Receive(void)
{
	while(!(UCSR0A & (1<<RXC0))); //Espera o dado ser recebido
	return UDR0; //Lê o dado recebido e retorna
}

void main(void)
{
	USART_Init(MYUBRR);
	sei();
	while(1)
	{
		//USART_Transmit('u');
		//_delay_ms(200);
		//USART_Transmit('s');
		//_delay_ms(200);
		//USART_Transmit('a');
		//_delay_ms(200);
		//USART_Transmit('r');
		//_delay_ms(200);
		//USART_Transmit('t');
		//_delay_ms(200);
		//USART_Transmit(' ');
		//_delay_ms(200);
		
		//USART_Transmit(USART_Receive());
	}
}
