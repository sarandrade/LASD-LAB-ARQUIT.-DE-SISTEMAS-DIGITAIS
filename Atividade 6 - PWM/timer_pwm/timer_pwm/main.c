/*
 * timer_pwm.c
 *
 * Created: 09/10/2020 11:35:59
 * Author : rafae
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	//GPIO
	DDRD =	0xFF; //Todos os pinos da porta D como sa�das PD6
	
	//fast PWM, TOP = 0xFF, OC0A e OC0B habilitados
	TCCR0A = 0b10100011; //PWM n�o invertido nos pinos OC0A e OC0B
	TCCR0B = 0b00000011; //liga TC0, prescaler = 64, fpwm = f0sc/(256*prescaler) = 16MHz/(256*64) = 976Hz
	OCR0A = 192; //controle do ciclo ativo do PWM 0C0A - duty = 192/256 = 75%
	OCR0B = 64; //controle do ciclo ativo do PWM OC0B - duty = 64/256 =  25%
	
	while (1)
	{
		//OCR0A+=10;
		//_delay_ms(50);
	}
}

