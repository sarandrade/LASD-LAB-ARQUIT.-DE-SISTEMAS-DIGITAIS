;
; assembly1.asm
;
; Created: 30/10/2020 22:27:18
; Author : rafael
;

//***********************************************************************************************
//Exemplo 1 - Contador de 8bits
//***********************************************************************************************
/*	ldi	r16,0		//Carrega o registrador r16 com 0

loopPrincipal:
    inc r16			//Incrementa r16 = r16 + 1
    rjmp loopPrincipal //Reinicia o laço infinito*/

//***********************************************************************************************
//Exemplo 2 - Onda quadrada no pino PB0. Instruções básicas
//***********************************************************************************************
/*	ldi	r16,1		//Carrega o registrador r16 com 1
	ldi	r17,0		//Carrega o registrador r16 com 0
    out DDRB,r16	//Copia o valor do registrador r16 para o registrador de I/O DDRB

loopPrincipal:
    out PORTB,r16	//Copia o valor do registrador r16 para o registrador de I/O PORTB
	out PORTB,r17	//Copia o valor do registrador r17 para o registrador de I/O PORTB
    rjmp loopPrincipal*/

//***********************************************************************************************
//Exemplo 3 - Onda quadrada no pino PB0. Com delay
//***********************************************************************************************
	sbi DDRB,0	//Seta o bit zero de DDRB. Define PB0 como saída
loopPrincipal:
	sbi PORTB,0	//PB0 em nível alto
	call delay1024 //Chama a subrotina de delay de 64us
	cbi PORTB,0 //PB0 em nível baixo
	call delay1024 //Chama a subrotina de delay de 64us
	rjmp loopPrincipal //Reinicia o laço infinito

//***********************************************************************************************
//Subrotina de delay de 64us
//***********************************************************************************************
delay1024: //Delay de aprox 4*255 = 1020 clk >>>> se clk=16MHz, 64us 
	ldi r17,0 //Reseta o registrador r17 (1 clk)
loop_delay1024:
	//nop nop nop nop
	inc r17 //Incrementa o registrador r17 (1 clk)
	cpi r17,0xFF //Compara o registrador r17 com 0xFF (1 clk)
	brne loop_delay1024 //Desvia se a comparação NÃO for igual ou seja z=0, (1 ou 2 clk)
	ret

