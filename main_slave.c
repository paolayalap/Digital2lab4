/* 
 * File:   main_slave.c
 * Author: paola
 *
 * Created on 17 de agosto de 2022, 9:50
 */

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)
// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)
// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

//DEFINICION E IMPORTACION DE LIBRERIAS
#include <xc.h>
#include <stdint.h>
#include "I2C.h"
#include "ADC.h"
#include <pic16f887.h>


//DEFINICION DE VARIABLES
#define _XTAL FREQ 8000000
uint8_t z;
uint8_t dato;

//DECLARACION DE FUNCIONES
//Definicon de funciones para que se puedan colocar después del main de lo contrario hay que colocarlos todas las funciones antes del main
void setup(void);


//CODIGO DE INTERRUPCION
void __interrupt() isr(void){
	if(PIR1bits.SSPIF == 1){
	SSPCONbits.CKP = 0;

		if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL))
		{
			z = SSPBUF;
			SSPCONbits.SSPOV 	= 0;	//Read the previos file
			SSPCONbits.WCOL 	= 0;	//clear the overflow
			SSPCONbits.CKP 		= 1;	//enables SCL
		}

		if (!SSPSTATbits.D_nA && !SSPSTATbits.R_nW)
		{
			//__delay_us(7);	
			z = SSPBUF;				//lectura de SSPBUF
			// __delay_us(2);
			PIR1bits.SSPIF = 0;		//limpia bandera
			SSPCONbits.CKP = 1;		//habilita entrada
			while(!SSPSTATbits.BF);	//espera a que la entrada
			PORTD = SSPBUF;			//guarda dato en PORTD
			__delay_us(250);

			/*
			si se mandan múltiples datos se puede poner esta parte
			PORTD = SSPBUF;			//guarda dato en PORTD
			__delay_us(250);
			varias veces
			o mejor dicho, se coloca la cantidad de veces 
			que I2C_Master_Write se repite en el while del main del master
			*/

		}

		//este else if sirve para leer lo que se tiene en el esclavo
		//y se vincula con el segundo párrafo del while
		else if(SSPSTATbits.D_nA && SSPSTATbits.R_nW) // esperando un 0
		{
			z = SSPBUF;
			BF = 0;
			SSPBUF = PORTB;
			SSPCONbits.CKP = 1;
			__delay_us(250);
			while(SSPSTATbits.BF);
			//si quisiera mandar otro dato coloco este código debajo del while
			//BF = 0;
			//SSPBUF = PORTB;
			//while(SSPSTATbits.BF);
		}

		PIR1bits.SSPIF = 0;
	}

}


//MAIN
void main(void){
	setup();
    
	//Loop infinito
	while(1){
		PORTB = ~PORTB;
		__delay_ms(500);
	}
	return;
}


//FUNCION DE INICIALIZACION
void setup(void){
	ANSEL = 0;
	ANSELH = 0;

	TRISB = 0;
	TRISD = 0;

	PORTB = 0;
	PORTD = 0;
	I2C_Slave_Init(0x50);
}
