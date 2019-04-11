#include "Timer.h"


void ConfigureTimer1()
{
    //Timer1 control: set Timer1 source = FOSC/4, prescale = 8, 
    //Secondary oscillator off,no sync to external clock, disable timer1
    T1CON = 0x34;  //0011_0100
    //gate control: disabled
    //
    T1GCON =0x50; //0101_000
    
    //PIE1bits.TMR1IE=1;
    //INTCONbits.PEIE = 1;
    //INTCONbits.GIE = 1;
}

void ConfigureTimer2()
{
    T2PR = 0;
    // T2CLKCON: TIMER2 CLOCK SELECTION REGISTER (Page 306))
    // Bits 8:4 = 0b00000   These bits are not implemented
    // Bits 3:0 = 0b0010    Clock source is system clock
    T2CLKCON = 0x02;

    // T2CON: TIMERx CONTROL REGISTER (Page 307)
    // Bit  7   = 0b1       Timer is on.
    // Bits 6:4 = 0b000     Pres-caler is 1:1
    // Bits 3:0 = 0b0000    Posts-caler is 1:1
    T2CON = 0xD0;
    PIR1bits.TMR2IF = 0; 
    return;
}

void ConfigureTimer4()
{    
    // TxCLKCON: TIMERx CLOCK SELECTION REGISTER (Page 306)
    T4CLKCON = 0x02;        // Timer4: Clock source is HFOSC (32MHz)
    //T6CLKCON = 0x05;        // Timer6: Clock source is MFOSC (500kHz)
    
    // TxCON: TIMERx CONTROL REGISTER (Page 307)
    T4CON = 0x30;           // Timer4: Pre-scaler = 32, Post-scaler = 1
    //T6CON = 0x50;           // Timer6: Pre-scaler = 2, Post-scaler = 15
    
    // T2HLT: TxHLT: TIMERx HARDWARE LIMIT CONTROL REGISTER (Page 308)
    T4HLT = 0x08;           // Timer4: One-shot mode. (No auto-restart)
    //T6HLT = 0x08;           // Timer6: One-shot mode. (No auto-restart)
    
    return;
}
 