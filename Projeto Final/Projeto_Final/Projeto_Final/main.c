/*
 * Projeto_Final.c
 *
 * Created: 29/11/2020 20:03:28
 * Author : saraa
 */ 

#define F_CPU 16000000UL // Frequ�ncia de trabalho da CPU
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1
#define tam_vetor 2

#include <avr/io.h>
//#include <EEPROM.h>
#include <string.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "nokia5110.h"

// Vari�veis Globais
int8_t seg, min, hrs, pausa, endereco;
int8_t pcint0_int = 0;
int8_t count = 0;
int16_t mili;
char tarefas, tarefa_atual;
char snum[tam_vetor];

// Fun��o para escrever horas e minutos na mem�ria EEPROM
//void writeStringToEEPROM( const String &strToWrite)
//{
	//byte len = strToWrite.length();
	//EEPROM.write(endereco, len);
	//for (int i = 0; i < len; i++)
	//{
		//EEPROM.write(endereco + 1 + i, strToWrite[i]);
		//
	//}
	//endereco += len;
//}

// Fun��o para ler horas e minutos da mem�ria EEPROM
//int lerEEPROM(int endereco1, int endereco2)
//{
	//int valor = 0;
	//byte primeiroByte = EEPROM.read(endereco1);
	//byte segundoByte = EEPROM.read(endereco2);
	//valor = (segundoByte << 8) + primeiroByte;
	//return valor;
//}

// Interrup��o externa 0, captura o Ligar/Desligar
ISR(INT0_vect)
{
	if(pausa == 1)
	{
		atualizaDisplay('l'); // Chamada de fun��o - Mensagem: Ligando o programa
		pausa = 0; // Inicia o Timer
	}
	else
	{
		atualizaDisplay('d'); // Chamada de fun��o - Mensagem: Desligando o programa
		pausa = 1; // Pausa o Timer
	}
}

// Interrup��o externa 1, captura o Play/Pause
ISR(INT1_vect)
{
	if(pausa == 1)
	{
		atualizaDisplay(tarefa_atual); // Chamada de fun��o - Mensagem: Atividade atual em andamento
		pausa = 0; // D� play no Timer
	}
	else
	{
		atualizaDisplay('p'); // Chamada de fun��o - Mensagem: Pausando o programa
		pausa = 1; // Pausa o Timer
	}
}

// Interrup��o 0 por mudan�a de pino, captura o Finalizar Tarefa
ISR(PCINT0_vect)
{
	if (pcint0_int == 0)
	{
		if (tarefa_atual == tarefas)
		{
			// Salvar hrs e min na mem�ria flash
			//String horas = to_string(hrs);
			//String minutos = to_string(min);
			//writeStringToEEPROM (horas);
			//writeStringToEEPROM (minutos);
			
			// Finaliza contagem
			atualizaDisplay('x'); // Chamada de fun��o - Mensagem: Dados coletados durante as atividades
		}
		else
		{
			// Salvar hrs e min na mem�ria flash
			//String horas = to_string(hrs);
			//String minutos = to_string(min);
			//writeStringToEEPROM (horas);
			//writeStringToEEPROM (minutos);
			
			atualizaDisplay('f'); // Chamada de fun��o - Mensagem: Finalizando tarefa atual
			
			tarefa_atual ++; // Passa para a pr�xima tarefa
			
			_delay_ms(1000);
			atualizaDisplay(tarefa_atual); // Chamada de fun��o - Mensagem: Atividade atual em andamento
		}
		
		pcint0_int ++;
	}
	else
	{
		pcint0_int = 0;
	}
}

// Interrup��o do TC0 a cada 1ms = (64*(249+1))/16MHz
ISR(TIMER0_COMPA_vect)
{
	if (pausa != 1) // Incrementa o timer se a flag pausa for 0
	{
		mili ++; // Incrementa os milissegundos
		
		if (hrs == 0)
		{	
			// Seleciona a sa�da do DEMUX: 00
			PORTC &= 0b0111111; // S0 = PC6 = 0
			PORTD &= 0b11101111; // S1 = PD4 = 0
			PORTB &= 0b10111111; // S2 = PB6 = 0
		}
		if (mili == 1000)
		{
			mili = 0;
			seg ++; // Incrementa os segundos
			
			if (seg == 60)
			{
				seg = 0;
				min ++; // Incrementa os minutos
				
				if (min == 5)
				{
					OCR0A = 0.05*256;
				}
				else if (min == 15)
				{
					OCR0A = 0.25*256;
				}
				else if (min == 30)
				{
					OCR0A = 0.50*256;
				}
				else if (min ==45)
				{
					OCR0A = 0.75*256;
				}
				else if (min ==60)
				{
					OCR0A = 0.98*256;
					
					min = 0;
					hrs ++; // Incrementa as horas
					
					if (hrs == 1)
					{
						// Seleciona a sa�da do DEMUX: 01
						PORTC |= 0b1000000; // S0 = PC6 = 1
						PORTD &= 0b11101111; // S1 = PD4 = 0
						PORTB &= 0b10111111; // S2 = PB6 = 0
						
						PORTC |= 0b0000001; // Aciona o primeiro LED (PC0)
					}
					if (hrs == 2)
					{
						// Seleciona a sa�da do DEMUX: 02
						PORTC &= 0b0111111; // S0 = PC6 = 0
						PORTD |= 0b00010000;  // S1 = PD4 = 1
						PORTB &= 0b10111111; // S2 = PB6 = 0
						
						PORTC |= 0b0000010; // Aciona o segundo LED (PC1)								
					}
					if (hrs == 3)
					{
						// Seleciona a sa�da do DEMUX: 03
						PORTC |= 0b1000000; // S0 = PC6 = 1
						PORTD |= 0b00010000; // S1 = PD4 = 1
						PORTB &= 0b10111111; // S2 = PB6 = 0
						
						PORTC |= 0b0000100; // Aciona o terceiro LED (PC2)										
					}
					if (hrs == 4)
					{
						// Seleciona a sa�da do DEMUX: 04
						PORTC &= 0b0111111; // S0 = PC6 = 0
						PORTD &= 0b11101111; // S1 = PD4 = 0
						PORTB |= 0b01000000; // S2 = PB6 = 1
						
						PORTC |= 0b0001000; // Aciona o quarto LED (PC3)												
					}
					if (hrs == 5)
					{
						// Seleciona a sa�da do DEMUX: 05
						PORTC &= 0b0111111; // S0 = PC6 = 0
						PORTD |= 0b00010000; // S1 = PD4 = 1
						PORTB |= 0b01000000; // S2 = PB6 = 1
						
						PORTC |= 0b0010000; // Aciona o quinto LED (PC4)
					}
					if (hrs == 6)
					{
						// Seleciona a sa�da do DEMUX: 06
						PORTC |= 0b1000000; // S0 = PC6 = 1
						PORTD &= 0b11101111; // S1 = PD4 = 0
						PORTB |= 0b01000000; // S2 = PB6 = 1
						
						PORTC |= 0b0100000; // Aciona o sexto LED (PC5)				
						
						// Finalizar
					}
				}
			}
		}
	}
}

// Fun��o de tratamento de interrup��o � Recep��o USART
ISR(USART_RX_vect)
{
	tarefas = UDR0 - 48; // UDR0 cont�m o dado recebido via USART
	tarefa_atual = 1; // Inicializa as atividades pela tarefa 1
	
	nokia_lcd_clear();
	nokia_lcd_write_string("--------------", 1);
	nokia_lcd_set_cursor(0, 10);
	nokia_lcd_write_string("   Total de", 1);
	itoa(tarefas, snum, 10); // Fun�a� que converte tarefas (int) em string
	nokia_lcd_set_cursor(35, 20);
	nokia_lcd_write_string(snum, 1);
	nokia_lcd_set_cursor(0, 30);
	nokia_lcd_write_string("   Tarefas", 1);
	nokia_lcd_set_cursor(0, 40);
	nokia_lcd_write_string("--------------", 1);
	nokia_lcd_render();
	
	USART_Transmit(tarefas);
}

// Fun��o para inicializa��o da USART
void USART_Init(unsigned int ubrr)
{
	UBRR0H = (unsigned char)(ubrr>>8); // Ajusta a taxa de transmiss�o
	UBRR0L = (unsigned char)ubrr;
	UCSR0B = (1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0); // Habilita o transmissor e o receptor
	UCSR0C = (1<<USBS0)|(3<<UCSZ00); // Ajusta o formato do frame: 8 bits de dados e 2 de parada
	
	DDRC = 0xFF; // Define todos os pinos da porta C como sa�da
}

// Fun��o para envio de um frame de 5 a 8 bits
void USART_Transmit(unsigned char data)
{
	while(!( UCSR0A & (1<<UDRE0)));// Espera a limpeza do registr. de transmiss�o
	UDR0 = data; // Coloca o dado no registrador e o envia
}

// Fun��o para recep��o de um frame de 5 a 8 bits
unsigned char USART_Receive(void)
{
	while(!(UCSR0A & (1<<RXC0))); // Espera o dado ser recebido
	return UDR0; // L� o dado recebido e retorna
}

int main(void) //-
{
	DDRB = 0b11111110; // Define todos os pinos da porta B como sa�da (exceto B0)
	PORTB = 0b00000001; // Habilita pull-up do pino PB0
	DDRD = 0b11110000; // Define os pinos da porta D: D0-D3 como entradas; D4-D7 como sa�da
	PORTD = 0b00001100; // Habilita pull-ups dos pinos PD2 e PD3
	
	// Vari�veis inicializadas
	mili = 0;
	seg = 0;
	min = 0;
	hrs = 0;
	pausa = 1;
	
	USART_Init(MYUBRR);
	
	// Fast PWM, TOP = 0xFF, OC0A habilitado
	TCCR0A = 0b10000011; // PWM n�o invertido no pino OC0A
	TCCR0B = 0b00000101; // Liga TC0, prescaler = 1024, fpwm = f0sc/(256*prescaler) = 16MHz/(256*1024) = 61Hz
	OCR0A = 0; // Controle do ciclo ativo do PWM 0C0A
	
	// Configura��o das Interrup��es Externas
	EICRA = 0b00001010;// Interrup��es externas INT0 e INT1 na borda de descida
	EIMSK = 0b00000011;// Habilita as interrup��es externas INT0 e INT1
	
	// Configura��o da Interrup��o 0 por mudan�a de pino
	PCICR = 0b00000001; // Enable pin change interrupt 0
	PCMSK0 = 0b00000001; // Pin change enable mask 0
	
	// Configura��o dos Timers
	TCCR0A = 0b00000010; // Habilita modo CTC do TC0
	TCCR0B = 0b00000011; // Liga TC0 com prescaler = 64
	OCR0A = 249;		 // Ajusta o comparador para o TC0 contar at� 249
	TIMSK0 = 0b00000010; // Habilita a interrup��o na igualdade de compara��o com OCR0A. A interrup��o ocorre a cada 1ms = (64*(249+1))/16MHz
	
	sei(); // Habilita interrup��es globais, ativando o bit I do SREG
	
	nokia_lcd_init(); // Inicializa o LCD
	nokia_lcd_clear();
	nokia_lcd_write_string("--------------", 1);
	nokia_lcd_set_cursor(0, 10);
	nokia_lcd_write_string("Digite quantas", 1);
	nokia_lcd_set_cursor(0, 20);
	nokia_lcd_write_string(" tarefas vai", 1);
	nokia_lcd_set_cursor(0, 30);
	nokia_lcd_write_string("   realizar", 1);
	nokia_lcd_set_cursor(0, 40);
	nokia_lcd_write_string("--------------", 1);
	nokia_lcd_render();
	
	while (1)
	{
	}
}

// Fun��o para atualiza��o do display PCD8544-7
void atualizaDisplay(char entrada){
	
	if (entrada == 'l') // Mensagem: Ligando o programa
	{
		nokia_lcd_clear();
		nokia_lcd_write_string("--------------", 1);
		nokia_lcd_set_cursor(0, 20);
		nokia_lcd_write_string("  Bem-vindo!!", 1);
		nokia_lcd_set_cursor(0, 40);
		nokia_lcd_write_string("--------------", 1);
		nokia_lcd_render();
		
		_delay_ms(1000);
		
		nokia_lcd_clear();
		nokia_lcd_write_string("--------------", 1);
		nokia_lcd_set_cursor(0, 10);
		nokia_lcd_write_string("   Tarefa", 1);
		nokia_lcd_set_cursor(35, 20);
		itoa(tarefa_atual, snum, 10); // Fun�a� que converte tarefa_atual (int) em string
		nokia_lcd_write_string(snum, 1);
		nokia_lcd_set_cursor(0, 30);
		nokia_lcd_write_string(" Em Andamento", 1);
		nokia_lcd_set_cursor(0, 40);
		nokia_lcd_write_string("--------------", 1);
		nokia_lcd_render();
	}
	else if (entrada == 'd') // Mensagem: Desligando o programa
	{
		nokia_lcd_clear();
		nokia_lcd_write_string("--------------", 1);
		nokia_lcd_set_cursor(0, 20);
		nokia_lcd_write_string(" Desligando...", 1);
		nokia_lcd_set_cursor(0, 40);
		nokia_lcd_write_string("--------------", 1);
		nokia_lcd_render();
		
		_delay_ms(1000);
		
		nokia_lcd_clear();
		nokia_lcd_render();
		
	}
	else if (entrada == 'f') // Mensagem: Finalizando tarefa atual
	{
		nokia_lcd_clear();
		nokia_lcd_write_string("--------------", 1);
		nokia_lcd_set_cursor(0, 10);
		nokia_lcd_write_string("   Tarefa", 1);
		nokia_lcd_set_cursor(35, 20);
		itoa(tarefa_atual, snum, 10); // Fun�a� que converte tarefa_atual (int) em string
		nokia_lcd_write_string(snum, 1);
		nokia_lcd_set_cursor(0, 30);
		nokia_lcd_write_string("  Finalizada!", 1);
		nokia_lcd_set_cursor(0, 40);
		nokia_lcd_write_string("--------------", 1);
		nokia_lcd_render();
	}
	else if (entrada == 'p') // Mensagem: Pausando o programa
	{
		nokia_lcd_clear();
		nokia_lcd_write_string("--------------", 1);
		nokia_lcd_set_cursor(0, 20);
		nokia_lcd_write_string("Tarefa Pausada", 1);
		nokia_lcd_set_cursor(0, 40);
		nokia_lcd_write_string("--------------", 1);
		nokia_lcd_render();
		
	}
	else if (entrada == 'x') // Mensagem: Dados coletados durante as atividades
	{
		nokia_lcd_clear();
		nokia_lcd_write_string(" T | hrs | min", 1);
		nokia_lcd_render();
		int tarefa = 1;
		int cursor = 20;
		
		// Ler informa��es da mem�ria EEPROM
		//for(int endereco_leitura = 0; endereco_leitura < endereco; endereco_leitura += 4)
		//{
			//int hora = lerEEPROM(endereco_leitura, endereco_leitura+1);
			//int minutos = lerEEPROM(endereco_leitura+2, endereco_leitura+3);
			//nokia_lcd_set_cursor(0, cursor);
			//nokia_lcd_write_string(tarefa, 1);
			//nokia_lcd_set_cursor(25, cursor);
			//nokia_lcd_write_string(hora, 1);
			//nokia_lcd_set_cursor(35, cursor);
			//nokia_lcd_write_string(minutos, 1);
			//nokia_lcd_render();
			//
			//_delay_ms(10000);
			//
			//tarefa ++;
			//cursor += 10;
			//
			//if (cursor == 50)
			//{
				//cursor = 20;
			//}
		//}
	}
	else // Mensagem: Atividade atual em andamento
	{
		nokia_lcd_clear();
		nokia_lcd_write_string("--------------", 1);
		nokia_lcd_set_cursor(0, 10);
		nokia_lcd_write_string("   Tarefa", 1);
		nokia_lcd_set_cursor(35, 20);
		itoa(entrada, snum, 10); // Fun�a� que converte entrada (int) em string
		nokia_lcd_write_string(snum, 1);
		nokia_lcd_set_cursor(0, 30);
		nokia_lcd_write_string(" Em Andamento", 1);
		nokia_lcd_set_cursor(0, 40);
		nokia_lcd_write_string("--------------", 1);
		nokia_lcd_render();
	}
}

