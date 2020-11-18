;
; Exerc9_SaraAndrade.asm
;
; Created: 01/11/2020 08:27:03
; Author : saraa
;

	ldi	r18,0xFF // Carrega o registrador r18 com 0xFF
	out DDRB,r18 // Copia o valor do registrador r18 para o registrador DDRB

resetando:
	ldi	r16,0x01 // Carrega o registrador r16 com 0x01

loopPrincipal:
	out PORTB,r16 // Copia o valor do registrador r17 para o registrador de I/O PORTB
	call delay1024 // Chama a subrotina de delay de 100ms
    cpi r16,0x80 // Compara o registrador r16 com 0x80 (1 clk)
	breq resetando // Desvia se a comparação for igual (1 ou 2 clk)
	lsl r16 // Multiplica r16 por 2
	jmp loopPrincipal // Reinicia o laço infinito

//***********************************************************************************************
//Subrotina de delay de 100ms
//***********************************************************************************************

delay1024: 
	ldi r17,0 // Reseta o registrador r17 (1 clk)
	ldi r18,0 // Reseta o registrador r18 (1 clk)
	ldi r19,0 // Reseta o registrador r19 (1 clk)
loop_delay1024:
	nop nop nop nop nop nop // (6 clk)
	inc r17 // Incrementa o registrador r17 (1 clk)
	cpi r17,0xFF // Compara o registrador r17 com 0xFF (1 clk)
	brne loop_delay1024 // Desvia se a comparação NÃO for igual ou seja z=0, (1 ou 2 clk)
	nop nop nop nop nop nop // (6 clk)
	inc r18 // Incrementa o registrador r18 (1 clk)
	cpi r18,0xFF // Compara o registrador r18 com 0xFF (1 clk)
	brne loop_delay1024 // Desvia se a comparação NÃO for igual ou seja z=0, (1 ou 2 clk)
	nop nop nop nop nop nop // (6 clk)
	inc r19 // Incrementa o registrador r18 (1 clk)
	cpi r19,3 // Compara o registrador r18 com 3 (1 clk)
	brne loop_delay1024 // Desvia se a comparação NÃO for igual ou seja z=0, (1 ou 2 clk)
	ret 