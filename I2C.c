/* 
 * File:   I2C.c
 * Author: paola
 *
 * Created on 17 de agosto de 2022, 9:47
 */


#include "I2C.h"

void I2C_Master_Init(const unsigned long c)
{
    SSPCON = 0b00101000;
    SSPCON2 = 0;
    SSPADD = (_XTAL_FREQ/(4*c))-1;
    SSPSTAT = 0;
    TRISCbits.TRISC3 = 1;
    TRISCbits.TRISC4 = 1;
}

// Funcion de espera: Mientras se esté iniciada una comunicación, 
// esté habilitado una recepción, esté habilitado una parada
// esté habilitado un reinicio de la comunicación, esté iniciada
// una comunicación o se este transmitiendo, el I2C PID se esparará
// antes de realizar algún trabajo. 

void I2C_Master_Wait()
{
    while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));
}

//FUNCION DE INICIO DE LA COMUNICACION I2C PIC
void I2C_Master_Start()
{
    I2C_Master_Wait();
    SSPCON2bits.SEN = 1; // el módulo  da inicio
    //hace el start
    //representa la S 
}

void I2C_Master_RepeatedStart()
{
    I2C_Master_Wait();
    SSPCON2bits.RSEN = 1;
}

void I2C_Master_Stop()
{
    I2C_Master_Wait();
    SSPCON2bits.PEN = 1;
}

void I2C_Master_Write(unsigned d)
{
    I2C_Master_Wait();
    SSPBUF = d;             
}

// Funcion de recepción de datos enviados por el esclavo al maestro
//esta funcion es para leer los datos que están en el esclavo
unsigned short I2C_Master_Read(unsigned short a)
{
    unsigned short temp;
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    SSPCON2bits.RCEN = 1;
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    temp = SSPBUF;
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    if(a == 1){
        SSPCON2bits.ACKDT = 0;
    }else{
        SSPCON2bits.ACKDT = 1;
    }
    SSPCON2bits.ACKEN = 1;  //iniciar secuencia de Acknowledge
    return temp;            //regresar valor del dato leído
}

//FUNCION PARA INICIALIZAR I2C ESCLAVO
void I2C_Slave_Init(uint8_t address)
{ 
    SSPADD  = address;
    SSPCON  = 0x36;
    SSPSTAT = 0x80;
    SSPCON2 = 0x01;
    TRISC3  = 1;
    TRISC4  = 1;
    SSPIF   = 0;
    SSPIE   = 1;
    PEIE    = 1;
    GIE     = 1;
    
}
