/* Nokia 5110 LCD AVR Library example
 *
 * Copyright (C) 2015 Sergey Denisov.
 * Written by Sergey Denisov aka LittleBuster (DenisovS21@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version 3
 * of the Licence, or (at your option) any later version.
 *
 * Original library written by SkewPL, http://skew.tk
 */

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#include "nokia5110.h"

int main(void)
{
    nokia_lcd_init(); //Inicia o LCD
    nokia_lcd_clear(); //Limpa o LCD
    nokia_lcd_write_string("UFCG 2020",1); //Escreve um texto do tamanho 1
    nokia_lcd_set_cursor(0, 10); //Muda o cursos para a posição 0,10 ou seja, pula uma linha
    nokia_lcd_write_string("LASD", 3); //Escreve um texto do tamanho 3
	nokia_lcd_set_cursor(0, 40); //Muda o cursos para a posição 0,40
	nokia_lcd_write_string("Prof. Rafael",1);//Escreve um texto do tamanho 1
    nokia_lcd_render(); //Atualiza a tela do display com o conteúdo do buffer

    while(1)
	{
        _delay_ms(1000);
    }
}