/* 
 * File:   main_master.c
 * Author: paola
 *
 * Created on 17 de agosto de 2022, 9:47
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


//Librerias
#include <xc.h>
#include <pic16f887.h>
#include "I2C.h"
#include "LCD.h"

//Variables
#define _XTAL_FREQ 8000000
#define RS RC0
#define EN RC1
#define D0 RD0
#define D1 RD1
#define D2 RD2
#define D3 RD3
#define D4 RD4
#define D5 RD5
#define D6 RD6
#define D7 RD7

//Funciones
void setup(void);

//Main
void main(void)
{
    setup();
    unsigned int a;
    TRISD = 0x00;
    TRISC = 0x00;
    Lcd_Init();
    
    while(1){
        I2C_Master_Start();
        I2C_Master_Write(0x50);
        I2C_Master_Write(PORTB);
        I2C_Master_Stop();
        __delay_ms(200);
        /*
        I2C_Master_Start();
        I2C_Master_Write(0x51);
        PORTD = I2C_Master_Read(0);
        __delay_ms(200);
        PORTB++;
        */
        Lcd_Clear();
        Lcd_Set_Cursor(1,1);
        Lcd_Write_String("Paola Ayala P."); //mientras tanto usare este dato
        //para verificar el funcionamiento de la LCD
        Lcd_Set_Cursor(2,1);
        Lcd_Write_String("Carne 19091");
        __delay_ms(2000);
       
        
    }
    return;
}


void setup(void)
{
    ANSEL = 0;
    ANSELH = 0;
    TRISB = 0;
    TRISC = 0;
    TRISD = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    I2C_Master_Init(100000); // Inicia la comunicaci�n
}



