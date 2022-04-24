/*
 * File:   PreLab8.c
 * Author: jorge
 *
 * Created on 19 de abril de 2022, 11:25 AM
 */
// PIC16F887 Configuration Bit Settings
// 'C' source line config statements
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
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

#include <xc.h>
#include <stdint.h>             // int8_t, unit8_t
#define _XTAL_FREQ 4000000

void __interrupt() isr (void){
    if(PIR1bits.ADIF){              // Interrupci�n por ADC
        if(ADCON0bits.CHS == 0){    // Interrupci�n por AN0
            PORTC = ADRESH;         // Mostrar los bits m�s signifiativos
        }
        PIR1bits.ADIF = 0;          // Limpiar la bandera de ADC
    }
    return;
}

void setup(void){
    ANSEL = 0b00000001;         // AN0 como entrada anal�gica
    ANSELH = 0;                 // I/O digitales
    
    TRISA = 0b00000001;         // RA0 como entrada anal�gica y dem�s como salidas
    TRISC = 0x00;               // PORTC como salida
    TRISD = 0x00;               // PORTD como salida
    PORTA = 0x00;               // Se limpia PORTA
    PORTC = 0x00;               // Se limpia PORTC
    PORTD = 0x00;               // Se limpia PORTD
    
    // Configuraci�n de interrupciones
    PIR1bits.ADIF = 0;          // Limpiar la bandera de ADC
    PIE1bits.ADIE = 1;          // Habilitar interrupciones de ADC
    INTCONbits.PEIE = 1;        // Habilitar interrupciones de perif�ricos
    INTCONbits.GIE = 1;         // Habilitar interrupciones globales
    
    // Configuraci�n del oscilador
    OSCCONbits.IRCF = 0b0110;   // 4MHz
    OSCCONbits.SCS = 1;         // Oscilador interno
    
    // Configuraci�n ADC
    ADCON0bits.ADCS = 0b00000001;   // FOSC/8
    ADCON1bits.VCFG0 = 0;           // VDD
    ADCON1bits.VCFG1 = 0;           // VSS
    ADCON0bits.CHS = 0b0000;        // Selecciona el AN0
    ADCON1bits.ADFM = 0;            // Justificador a la izquierda
    ADCON0bits.ADON = 1;            // Habilitar modulo ADC
    __delay_us(40);                 // Sample time
    
    return;
}

void main(void){
    setup();
    while(1){
        if (ADCON0bits.GO == 0){
            ADCON0bits.GO = 1;      // On
        }        
    }
    return;
}
