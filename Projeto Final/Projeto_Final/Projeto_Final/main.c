#include <atmel_start.h>
#include <EEPROM.h>
#include <string.h>

// Variáveis Globais
int8_t seg, min, hrs, pausa, endereco;
int16_t mili;
char tarefas, tarefa_atual;

// Função para escrever horas e minutos na memória EEPROM
void writeStringToEEPROM( const String &strToWrite)
{
	byte len = strToWrite.length();
	EEPROM.write(endereco, len);
	for (int i = 0; i < len; i++)
	{
		EEPROM.write(endereco + 1 + i, strToWrite[i]);
		
	}
	endereco += len;
}

// Função para ler horas e minutos da memória EEPROM
int lerEEPROM(int endereco1, int endereco2)
{
	int valor = 0;
	byte primeiroByte = EEPROM.read(endereco1);
	byte segundoByte = EEPROM.read(endereco2);
	valor = (segundoByte << 8) + primeiroByte;
	return valor;
}

// Interrupção externa 0, captura o Ligar/Desligar
ISR(INT0_vect)
{
	if(pausa == 1)
	{
		atualizaDisplay('l'); // Chamada de função - Mensagem: Ligando o programa
		pausa = 0; // Inicia o Timer
	}
	else
	{
		atualizaDisplay('d'); // Chamada de função - Mensagem: Desligando o programa
		pausa = 1; // Pausa o Timer
	}
}

// Interrupção externa 1, captura o Play/Pause
ISR(INT1_vect)
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

// Interrupção 0 por mudança de pino, captura o Finalizar Tarefa
ISR(PCINT0_vect)
{
	if (tarefa_atual == tarefas)
	{
		// Salvar hrs e min na memória flash
		String horas = to_string(hrs);
		String minutos = to_string(min);
		writeStringToEEPROM (horas);
		writeStringToEEPROM (minutos);
		
		// Finaliza contagem
		atualizaDisplay('x'); // Chamada de função - Mensagem: Dados coletados durante as atividades
	}
	else
	{
		// Salvar hrs e min na memória flash
		String horas = to_string(hrs);
		String minutos = to_string(min);
		writeStringToEEPROM (horas);
		writeStringToEEPROM (minutos);
		
		atualizaDisplay('f'); // Chamada de função - Mensagem: Finalizando tarefa atual
		
		tarefa_atual ++; // Passa para a próxima tarefa
		
		_delay_ms(1000);
		atualizaDisplay(tarefa_atual); // Chamada de função - Mensagem: Atividade atual em andamento
	}
}	

// Interrupção do TC0 a cada 1ms = (64*(249+1))/16MHz
ISR(TIMER0_COMPA_vect)
{
	if (pausa != 1) // Incrementa o timer se a flag pausa for 0
	{
		mili ++; // Incrementa os milissegundos
		
		if (hrs == 0)
		{
			S0 = S1 = S2 = 0; // Seleciona a saída do DEMUX: 00
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
						// Seleciona a saída do DEMUX: 01
						S0 = 1;
						S1 = S2 = 0;
						led1 = 1; // Aciona o primeiro LED
					}
					if (hrs == 2)
					{
						// Seleciona a saída do DEMUX: 02
						S1 = 1;
						S0 = S2 = 0;
						led2 = 1; // Aciona o segundo LED
					}
					if (hrs == 3)
					{
						// Seleciona a saída do DEMUX: 03
						S2 = 0;
						S0 = S1 = 1;
						led3 = 1; // Aciona o terceiro LED
					}
					if (hrs == 4)
					{
						// Seleciona a saída do DEMUX: 04
						S2 = 1;
						S0 = S1 = 0;
						led4 = 1; // Aciona o quarto LED
					}
					if (hrs == 5)
					{
						// Seleciona a saída do DEMUX: 05
						S1 = 0;
						S0 = S2 = 1;
						led5 = 1; // Aciona o quinto LED
					}
					if (hrs == 6)
					{
						// Seleciona a saída do DEMUX: 06
						S0 = 0;
						S1 = S2 = 1;
						led6 = 1; // Aciona o sexto LED
						
						// Finalizar
					}
				}
			}
		}
	}
}

// Função de tratamento de interrupção – Recepção USART
ISR(USART_RX_vect)
{
	tarefas = UDR0; // UDR0 contém o dado recebido via USART
	tarefa_atual = 1; // Inicializa as atividades pela tarefa 1
	
	nokia_lcd_clear();
	nokia_lcd_write_string("--------------", 1);
	nokia_lcd_set_cursor(0, 10);
	nokia_lcd_write_string("Serão realizadas", 1);
	nokia_lcd_set_cursor(0, 20);
	nokia_lcd_write_string(tarefas, 1);
	nokia_lcd_set_cursor(0, 30);
	nokia_lcd_write_string("Atividades", 1);
	nokia_lcd_set_cursor(0, 40);
	nokia_lcd_write_string("--------------", 1);
	nokia_lcd_render();
	
	_delay_ms(1000);
	atualizaDisplay(tarefa_atual); // Chamada de função - Mensagem: Atividade atual em andamento
	
	USART_Transmit(tarefas);
}

int main(void) //-
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	
	// Variáveis inicializadas
	mili = 0;
	seg = 0;
	min = 0;
	hrs = 0;
	pausa = 1;
	
	nokia_lcd_init();
	nokia_lcd_clear();
	nokia_lcd_set_cursor(0, 10);
	nokia_lcd_write_string("--------------", 1);
	nokia_lcd_set_cursor(0, 20);
	nokia_lcd_write_string("Quantas atividades", 1);
	nokia_lcd_set_cursor(0, 30);
	nokia_lcd_write_string("deseja realizar?", 1);
	nokia_lcd_set_cursor(0, 40);
	nokia_lcd_write_string("--------------", 1);
	nokia_lcd_render();
	
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
		nokia_lcd_write_string("Bem-vindo!!", 1);
		nokia_lcd_set_cursor(0, 30);
		nokia_lcd_write_string("--------------", 1);
		nokia_lcd_render();
		
		_delay_ms(1000);
		
		nokia_lcd_clear();
		nokia_lcd_write_string("--------------", 1);
		nokia_lcd_set_cursor(0, 10);
		nokia_lcd_write_string("Atividade", 1);
		nokia_lcd_set_cursor(0, 20);
		nokia_lcd_write_string(tarefa_atual, 1);
		nokia_lcd_set_cursor(0, 30);
		nokia_lcd_write_string("Em Andamento...", 1);
		nokia_lcd_set_cursor(0, 40);
		nokia_lcd_write_string("--------------", 1);
		nokia_lcd_render();
	}
	else if (entrada == 'd') // Mensagem: Desligando o programa
	{
		nokia_lcd_clear();
		nokia_lcd_write_string("--------------", 1);
		nokia_lcd_set_cursor(0, 20);
		nokia_lcd_write_string("Desligando...", 1);
		nokia_lcd_set_cursor(0, 40);
		nokia_lcd_write_string("--------------", 1);
		nokia_lcd_render();
		
		_delay_ms(1000);
		
		nokia_lcd_clear();
		
	}
	else if (entrada == 'f') // Mensagem: Finalizando tarefa atual
	{
		nokia_lcd_clear();
		nokia_lcd_write_string("--------------", 1);
		nokia_lcd_set_cursor(0, 10);
		nokia_lcd_write_string("Atividade", 1);
		nokia_lcd_set_cursor(0, 20);
		nokia_lcd_write_string(tarefa_atual, 1);
		nokia_lcd_set_cursor(0, 30);
		nokia_lcd_write_string("Finalizada!", 1);
		nokia_lcd_set_cursor(0, 40);
		nokia_lcd_write_string("--------------", 1);
		nokia_lcd_render();
	}
	else if (entrada == 'p') // Mensagem: Pausando o programa
	{
		nokia_lcd_clear();
		nokia_lcd_write_string("--------------", 1);
		nokia_lcd_set_cursor(0, 20);
		nokia_lcd_write_string("Atividade Pausada", 1);
		nokia_lcd_set_cursor(0, 40);
		nokia_lcd_write_string("--------------", 1);
		nokia_lcd_render();
		
	}
	else if (entrada == 'x') // Mensagem: Dados coletados durante as atividades
	{
		nokia_lcd_clear();
		
		for(int endereco_leitura = 0; endereco_leitura < endereco; endereco_leitura+=4){
			
			int hora = lerEEPROM(endereco_leitura, endereco_leitura+1);
			int minutos = lerEEPROM(endereco_leitura+2, endereco_leitura+3);
			
			
			//LEMBRAR DE EXIBIR DENTRO DO FOR
			//SALVAR EM UM ARRAY SE FOR EXIBIR FORA
		}
		
		// Pegar informações da memória flash
		nokia_lcd_render();
		
	}
	else // Mensagem: Atividade atual em andamento
	{
		nokia_lcd_clear();
		nokia_lcd_write_string("--------------", 1);
		nokia_lcd_set_cursor(0, 10);
		nokia_lcd_write_string("Atividade", 1);
		nokia_lcd_set_cursor(0, 20);
		nokia_lcd_write_string(entrada, 1);
		nokia_lcd_set_cursor(0, 30);
		nokia_lcd_write_string("Em Andamento...", 1);
		nokia_lcd_set_cursor(0, 40);
		nokia_lcd_write_string("--------------", 1);
		nokia_lcd_render();
	}
}
