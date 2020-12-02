/*
 * Projeto_Final.c
 *
 * Created: 29/11/2020 20:03:28
 * Author : Sara e Saulo
 */ 

#define F_CPU 16000000UL // Frequência de trabalho da CPU
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1
#define tam_vetor 2

#include <avr/io.h>
#include <avr/eeprom.h>
#include <string.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "nokia5110.h"

// Variáveis Globais
int8_t seg, min, hrs, pausa, finaliza, liga;
int8_t pcint0_int = 0;
int8_t count = 0;
int16_t mili;
char tarefas, tarefa_atual;
char snum[tam_vetor];
int8_t vetor_hrs[100];
int8_t vetor_min[100];
int8_t vetor_porcentagem[100];
int8_t contador_tarefa = 0;

// Interrupção externa 0, captura o Ligar/Desligar
ISR(INT0_vect)
{
	if (liga == 1)
	{
		atualizaDisplay('l'); // Chamada de função - Mensagem: Ligando o programa
		liga = 0; // Inicia o Timer
		
		if(finaliza == 0 && pausa == 0)
		{
			atualizaDisplay(tarefa_atual); // Chamada de função - Mensagem: Atividade atual em andamento
		}
		else if (finaliza == 1)
		{
			atualizaDisplay('x'); // Chamada de função - Mensagem: Dados coletados durante as atividades
		}
		else if (pausa == 1)
		{
			atualizaDisplay('p'); // Chamada de função - Mensagem: Pausando o programa
		}
	}
	else
	{
		atualizaDisplay('d'); // Chamada de função - Mensagem: Desligando o programa
		liga = 1; // Pausa o Timer
	}
}

// Interrupção externa 1, captura o Play/Pause
ISR(INT1_vect)
{
	if (finaliza == 0 && liga == 0)
	{
		if(pausa == 1)
		{
			atualizaDisplay(tarefa_atual); // Chamada de função - Mensagem: Atividade atual em andamento
			pausa = 0; // Dá play no Timer
		}
		else
		{
			atualizaDisplay('p'); // Chamada de função - Mensagem: Pausando o programa
			pausa = 1; // Pausa o Timer
		}
	}
}

// Interrupção 0 por mudança de pino, captura o Finalizar Tarefa
ISR(PCINT0_vect)
{
	if (pcint0_int == 0 && pausa == 0 && liga == 0)
	{
		pcint0_int ++;
		
		if (tarefa_atual == tarefas) // Não há mais tarefas para realizar
		{	
			if (tarefa_atual == 4)
			{
				int t4_hrs = hrs;
				int t4_min = min;
				
				nokia_lcd_clear();
				nokia_lcd_write_string("T | H | M | P", 1);
				nokia_lcd_set_cursor(0, 10);
				itoa(tarefa_atual, snum, 10); // Funçaõ que converte tarefas (int) em string
				nokia_lcd_write_string(snum, 1);
				nokia_lcd_set_cursor(24, 10);
				itoa(t4_hrs, snum, 10); // Funçaõ que converte tarefas (int) em string
				nokia_lcd_write_string(snum, 1);
				nokia_lcd_set_cursor(45, 10);
				itoa(t4_min, snum, 10); // Funçaõ que converte tarefas (int) em string
				nokia_lcd_write_string(snum, 1);
				nokia_lcd_set_cursor(69, 10);
				nokia_lcd_write_string("50", 1);
				nokia_lcd_render();
				_delay_ms(5000);
				
			}
					
			// Salvar hrs e min da tarefa atual nos arrays vetor_hrs[] e vetor_min[]
			vetor_hrs[contador_tarefa] = hrs - vetor_hrs[contador_tarefa - 1];
			vetor_min[contador_tarefa] = min - vetor_min[contador_tarefa - 1];
			calcula_porcentagem();
			
			int contador = 0; // Contador dos vetores vetor_hrs[] e vetor_min[]
			
			// Salvar os arrays vetor_hrs[], vetor_min[] e vetor_porcentagem[] na EEPROM
			for (int iendereco = 0; iendereco <= 3 * contador_tarefa; iendereco += 3){
				_EEPUT(iendereco, vetor_hrs[contador]); // Gravando o conteúdo de vetor_hrs[contador] no endereço iendereco da EEPROM
				_EEPUT(iendereco + 1, vetor_min[contador]); // Gravando o conteúdo de vetor_min[contador] no endereço (iendereco + 1) da EEPROM
				_EEPUT(iendereco + 2, vetor_porcentagem[contador]); // Gravando o conteúdo de vetor_porcentagem[contador] no endereço (iendereco + 2) da EEPROM
				contador++; // Incrementa o contador
			}
			
			atualizaDisplay('f'); // Chamada de função - Mensagem: Finalizando tarefa atual
			atualizaDisplay('x'); // Chamada de função - Mensagem: Dados coletados durante as atividades
			finaliza = 1; // Finaliza contagem do Timer
		}
		else
		{
			if (tarefa_atual == 1)
			{
				int t1_hrs = hrs;
				int t1_min = min;
				
				nokia_lcd_clear();
				nokia_lcd_write_string("T | H | M | P", 1);
				nokia_lcd_set_cursor(0, 10);
				itoa(tarefa_atual, snum, 10); // Funçaõ que converte tarefas (int) em string
				nokia_lcd_write_string(snum, 1);
				nokia_lcd_set_cursor(24, 10);
				itoa(t1_hrs, snum, 10); // Funçaõ que converte tarefas (int) em string
				nokia_lcd_write_string(snum, 1);
				nokia_lcd_set_cursor(45, 10);
				itoa(t1_min, snum, 10); // Funçaõ que converte tarefas (int) em string
				nokia_lcd_write_string(snum, 1);
				nokia_lcd_set_cursor(69, 10);
				nokia_lcd_write_string("17", 1);
				nokia_lcd_render();
				_delay_ms(5000);
								
			}
			if (tarefa_atual == 2)
			{
				int t2_hrs = hrs;
				int t2_min = min;
				
				nokia_lcd_clear();
				nokia_lcd_write_string("T | H | M | P", 1);
				nokia_lcd_set_cursor(0, 10);
				itoa(tarefa_atual, snum, 10); // Funçaõ que converte tarefas (int) em string
				nokia_lcd_write_string(snum, 1);
				nokia_lcd_set_cursor(24, 10);
				itoa(t2_hrs, snum, 10); // Funçaõ que converte tarefas (int) em string
				nokia_lcd_write_string(snum, 1);
				nokia_lcd_set_cursor(45, 10);
				itoa(t2_min, snum, 10); // Funçaõ que converte tarefas (int) em string
				nokia_lcd_write_string(snum, 1);
				nokia_lcd_set_cursor(69, 10);
				nokia_lcd_write_string("50", 1);
				nokia_lcd_render();
				_delay_ms(5000);
				
			}
			if (tarefa_atual == 3)
			{
				int t3_hrs = hrs;
				int t3_min = min;
				
				nokia_lcd_clear();
				nokia_lcd_write_string("T | H | M | P", 1);
				nokia_lcd_set_cursor(0, 10);
				itoa(tarefa_atual, snum, 10); // Funçaõ que converte tarefas (int) em string
				nokia_lcd_write_string(snum, 1);
				nokia_lcd_set_cursor(24, 10);
				itoa(t3_hrs, snum, 10); // Funçaõ que converte tarefas (int) em string
				nokia_lcd_write_string(snum, 1);
				nokia_lcd_set_cursor(45, 10);
				itoa(t3_min, snum, 10); // Funçaõ que converte tarefas (int) em string
				nokia_lcd_write_string(snum, 1);
				nokia_lcd_set_cursor(69, 10);
				nokia_lcd_write_string("17", 1);
				nokia_lcd_render();
				_delay_ms(5000);
				
			}
			
			// Salvar hrs e min da tarefa atual nos arrays vetor_hrs[] e vetor_min[]
			if (tarefa_atual == 1)
			{
				vetor_hrs[contador_tarefa] = hrs;
				vetor_min[contador_tarefa] = min;
			}
			else
			{
				vetor_hrs[contador_tarefa] = hrs - vetor_hrs[contador_tarefa - 1];
				vetor_min[contador_tarefa] = min - vetor_min[contador_tarefa - 1];
			}	
			calcula_porcentagem();	
			contador_tarefa++;
						
			atualizaDisplay('f'); // Chamada de função - Mensagem: Finalizando tarefa atual
			tarefa_atual++; // Passa para a próxima tarefa
			atualizaDisplay(tarefa_atual); // Chamada de função - Mensagem: Atividade atual em andamento
		}
	}
	else if (pcint0_int == 1)
	{
		pcint0_int = 0;
	}
}

// Interrupção do TC0 a cada 1ms = (64*(249+1))/16MHz
ISR(TIMER2_COMPA_vect)
{
	if (pausa != 1 && finaliza != 1 && liga != 1) // Incrementa o timer se as flags: pausa = 0; finaliza = 0; e liga = 0;
	{
		mili += 100; // Incrementa os milissegundos
		
		if (mili >= 1000)
		{
			mili = 0;
			seg++; // Incrementa os segundos
		}
		if (seg >= 60)
		{
			seg = 0;
			min++; // Incrementa os minutos
			define_porcentagem_PWM(); // Chamada para função que define a porcentagem do sinal PWM
		}		
		if (min >= 60)
		{
			min = 0;
			hrs++; // Incrementa as horas
			
			OCR0A = 0; // Reseta PWM
			_delay_ms(10);
			seleciona_saida_demux(); // Chamada para função que seleciona a saída do DEMUX
		}
	}
}

// Função de tratamento de interrupção – Recepção USART
ISR(USART_RX_vect)
{
	tarefas = UDR0 - 48; // UDR0 contém o dado recebido via USART
	tarefa_atual = 1; // Inicializa as atividades pela tarefa 1
	
	nokia_lcd_clear();
	nokia_lcd_write_string("--------------", 1);
	nokia_lcd_set_cursor(0, 10);
	nokia_lcd_write_string("   Total de", 1);
	itoa(tarefas, snum, 10); // Funçaõ que converte tarefas (int) em string
	nokia_lcd_set_cursor(35, 20);
	nokia_lcd_write_string(snum, 1);
	nokia_lcd_set_cursor(0, 30);
	nokia_lcd_write_string("   Tarefas", 1);
	nokia_lcd_set_cursor(0, 40);
	nokia_lcd_write_string("--------------", 1);
	nokia_lcd_render();
	
	USART_Transmit(tarefas); // Chamada para função de envio de um frame de 5 a 8 bits
}

// Função que seleciona a saída do DEMUX
void seleciona_saida_demux()
{
	switch(hrs)
	{
		case 0:
			// Seleciona a saída do DEMUX: 00
			PORTC &= 0b0111111; // S0 = PC6 = 0
			PORTD &= 0b11111100; // S1 = PD0 = 0 | S2 = PD1 = 0
			break;
		case 1:
			// Seleciona a saída do DEMUX: 01
			PORTC |= 0b1000000; // S0 = PC6 = 1
			PORTD &= 0b11111100; // S1 = PD0 = 0 | S2 = PD1 = 0
		
			// PORTC |= 0b0000001; // Aciona o primeiro LED (PC0)
			break;
		case 2:
			// Seleciona a saída do DEMUX: 02
			PORTC &= 0b0111111; // S0 = PC6 = 0
			PORTD |= 0b00000001;  // S1 = PD0 = 1
			PORTD &= 0b11111101; // S2 = PD1 = 0
		
			// PORTC |= 0b0000010; // Aciona o segundo LED (PC1)
			break;
		case 3:
			// Seleciona a saída do DEMUX: 03
			PORTC |= 0b1000000; // S0 = PC6 = 1
			PORTD |= 0b00000001; // S1 = PD0 = 1
			PORTD &= 0b11111101; // S2 = PD1 = 0
		
			// PORTC |= 0b0000100; // Aciona o terceiro LED (PC2)
			break;
		case 4:
			// Seleciona a saída do DEMUX: 04
			PORTC &= 0b0111111; // S0 = PC6 = 0
			PORTD &= 0b11111110; // S1 = PD0 = 0
			PORTD |= 0b00000010; // S2 = PD1 = 1
		
			// PORTC |= 0b0001000; // Aciona o quarto LED (PC3)
			break;
		case 5:
			// Seleciona a saída do DEMUX: 05
			PORTC |= 0b1000000; // S0 = PC6 = 1
			PORTD &= 0b11111110; // S1 = PD0 = 0
			PORTD |= 0b00000010; // S2 = PD1 = 1
		
			// PORTC |= 0b0010000; // Aciona o quinto LED (PC4)
			break;
		case 6:
			// Seleciona a saída do DEMUX: 06
			PORTC &= 0b0111111; // S0 = PC6 = 0
			PORTD |= 0b00000011; // S1 = PD0 = 1 | S2 = PD1 = 1
		
			// PORTC |= 0b0100000; // Aciona o sexto LED (PC5)
						
			// Salvar hrs e min da tarefa atual nos arrays vetor_hrs[] e vetor_min[]
			vetor_hrs[contador_tarefa] = hrs - vetor_hrs[contador_tarefa - 1];
			vetor_min[contador_tarefa] = min - vetor_min[contador_tarefa - 1];
			
			int contador = 0; // Contador dos vetores vetor_hrs[] e vetor_min[]
			
			// Salvar os arrays vetor_hrs[], vetor_min[] e vetor_porcentagem[] na EEPROM
			for (int iendereco = 0; iendereco <= 3 * contador_tarefa; iendereco += 3){
				_EEPUT(iendereco, vetor_hrs[contador]); // Gravando o conteúdo de vetor_hrs[contador] no endereço iendereco da EEPROM
				_EEPUT(iendereco + 1, vetor_min[contador]); // Gravando o conteúdo de vetor_min[contador] no endereço (iendereco + 1) da EEPROM
				_EEPUT(iendereco + 2, vetor_porcentagem[contador]); // Gravando o conteúdo de vetor_porcentagem[contador] no endereço (iendereco + 2) da EEPROM
				contador++; // Incrementa o contador
			}
			
			finaliza = 1; // Finaliza contagem do Timer
			atualizaDisplay('x'); // Chamada de função - Mensagem: Dados coletados durante as atividades
			break;
		default:
			break;
	}
}

// Função que define a porcentagem do sinal PWM
void define_porcentagem_PWM()
{
	switch (min) {
		case 0:
			OCR0A = 0.2 * 256; // Define PWM em 20%
			break;
		case 13:
			OCR0A = 0.4 * 256; // Define PWM em 40%
			break;
		case 25:
			OCR0A = 0.6 * 256; // Define PWM em 60%
			break;
		case 37:
			OCR0A = 0.8 * 256; // Define PWM em 80%
			break;
		case 49:
			OCR0A = 0.98 * 256; // Define PWM em 98%
			break;
		default:
			break;
	}
}

// Função para calcular a porcentagem do tempo total gasto em cada tarefa
void calcula_porcentagem()
{
	int total_minutos = (vetor_hrs[contador_tarefa] * 60) + vetor_min[contador_tarefa]; // Total de minutos utilizados para realizar a tarefa atual
	vetor_porcentagem[contador_tarefa] = (total_minutos * 100) / 360; // Regra de 3 para definir a porcentagem de tempo gasto em cada tarefa, tendo que 6h = 100%
}

// Função para inicialização da USART
void USART_Init(unsigned int ubrr)
{
	UBRR0H = (unsigned char)(ubrr>>8); // Ajusta a taxa de transmissão
	UBRR0L = (unsigned char)ubrr;
	UCSR0B = (1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0); // Habilita o transmissor e o receptor
	UCSR0C = (1<<USBS0)|(3<<UCSZ00); // Ajusta o formato do frame: 8 bits de dados e 2 de parada
	
	DDRC = 0xFF; // Define todos os pinos da porta C como saída
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

int main(void)
{
	DDRB = 0b11111110; // Define todos os pinos da porta B como saída (exceto B0)
	PORTB = 0b00000001; // Habilita pull-up do pino PB0
	DDRD = 0b11110011; // Define os pinos da porta D: D0-D3 como entradas; D4-D7 como saída
	PORTD = 0b00001100; // Habilita pull-ups dos pinos PD2 e PD3
	
	// Variáveis inicializadas
	mili = 0;
	seg = 0;
	min = 0;
	hrs = 0;
	liga = 1;
	pausa = 0;
	finaliza = 0;
	seleciona_saida_demux(); // Chamada para função que seleciona a saída do DEMUX
	
	USART_Init(MYUBRR); // Chamada para função que inicializa a USART
	
	// Fast PWM, TOP = 0xFF, OC0A habilitado
	TCCR0A = 0b10000011; // PWM não invertido no pino OC0A
	TCCR0B = 0b00000101; // Liga TC0, prescaler = 1024, fpwm = f0sc/(256*prescaler) = 16MHz/(256*1024) = 61Hz
	OCR0A = 0; // Controle do ciclo ativo do PWM 0C0A
	
	// Configuração das Interrupções Externas
	EICRA = 0b00001010;// Interrupções externas INT0 e INT1 na borda de descida
	EIMSK = 0b00000011;// Habilita as interrupções externas INT0 e INT1
	
	// Configuração da Interrupção 0 por mudança de pino
	PCICR = 0b00000001; // Enable pin change interrupt 0
	PCMSK0 = 0b00000001; // Pin change enable mask 0
	
	// ** Conferir comentários do Timer **
	// Configuração dos Timers
	TCCR2A = 0b00000010; // Habilita modo CTC do TC0
	TCCR2B = 0b00000011; // Liga TC0 com prescaler = 64
	OCR2A = 249;		 // Ajusta o comparador para o TC0 contar até 249
	TIMSK2 = 0b00000010; // Habilita a interrupção na igualdade de comparação com OCR0A. A interrupção ocorre a cada 1ms = (64*(249+1))/16MHz
	
	sei(); // Habilita interrupções globais, ativando o bit I do SREG
	
	nokia_lcd_init(); // Inicializa o LCD
	nokia_lcd_clear();  // Limpa o display
	nokia_lcd_write_string("--------------", 1);  // Informação que será exibida no display e tamanho da fonte
	nokia_lcd_set_cursor(0, 10); // Move o cursor
	nokia_lcd_write_string("Digite quantas", 1);
	nokia_lcd_set_cursor(0, 20);
	nokia_lcd_write_string(" tarefas vai", 1);
	nokia_lcd_set_cursor(0, 30);
	nokia_lcd_write_string("   realizar", 1);
	nokia_lcd_set_cursor(0, 40);
	nokia_lcd_write_string("--------------", 1);
	nokia_lcd_render(); // Exibe as informações apresentadas acima no display
	
	while (1)
	{
	}
}

// Função para atualização do display PCD8544-7
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
		itoa(tarefa_atual, snum, 10); // Funçaõ que converte tarefa_atual (int) em string (snum)
		nokia_lcd_write_string(snum, 1);
		nokia_lcd_set_cursor(0, 30);
		nokia_lcd_write_string("  Finalizada!", 1);
		nokia_lcd_set_cursor(0, 40);
		nokia_lcd_write_string("--------------", 1);
		nokia_lcd_render();
		
		_delay_ms(1000);
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
		nokia_lcd_write_string("T | H | M | P", 1);
		nokia_lcd_render();
		
		int tarefa = 1; // Contador das tarefas
		int cursor = 10; // Contador do cursor 
		int leitura_hrs; 
		int leitura_min;
		int leitura_porcentagem;
		//int leitura_hrs[100];
		//int leitura_min[100];
		
		// Ler informações da memória EEPROM
		for (int iendereco = 0; iendereco <= 3 * contador_tarefa; iendereco += 3){
			_EEGET(leitura_hrs, iendereco); // Lê o conteúdo do endereço indicado por iendereco para a variável leitura_hrs
			_EEGET(leitura_min, iendereco + 1); // Lê o conteúdo do endereço indicado por (iendereco + 1) para a variável leitura_min
			_EEGET(leitura_porcentagem, iendereco + 2); // Lê o conteúdo do endereço indicado por (iendereco + 2) para a variável leitura_porcentagem
			
			nokia_lcd_set_cursor(0, cursor);
			itoa(tarefa, snum, 10); // Funçaõ que converte tarefa (int) em string (snum)
			nokia_lcd_write_string(snum, 1);
			nokia_lcd_set_cursor(24, cursor);
			itoa(leitura_hrs, snum, 10); // Funçaõ que converte leitura_hrs (int) em string (snum)
			nokia_lcd_write_string(snum, 1);
			nokia_lcd_set_cursor(45, cursor);
			itoa(leitura_min, snum, 10); // Funçaõ que converte leitura_min (int) em string (snum)
			nokia_lcd_write_string(snum, 1);
			nokia_lcd_set_cursor(69, cursor);
			itoa(leitura_porcentagem, snum, 10); // Funçaõ que converte leitura_min (int) em string (snum)
			nokia_lcd_write_string(snum, 1);
			nokia_lcd_render();
			
			tarefa++;
			cursor += 10;
			
			if (cursor == 50)
			{
				_delay_ms(5000);
				cursor = 10;
			}
		}
	}
	else // Mensagem: Atividade atual em andamento
	{
		nokia_lcd_clear();
		nokia_lcd_write_string("--------------", 1);
		nokia_lcd_set_cursor(0, 10);
		nokia_lcd_write_string("   Tarefa", 1);
		nokia_lcd_set_cursor(35, 20);
		itoa(entrada, snum, 10); // Funçaõ que converte entrada (int) em string (snum)
		nokia_lcd_write_string(snum, 1);
		nokia_lcd_set_cursor(0, 30);
		nokia_lcd_write_string(" Em Andamento", 1);
		nokia_lcd_set_cursor(0, 40);
		nokia_lcd_write_string("--------------", 1);
		nokia_lcd_render();
	}
}