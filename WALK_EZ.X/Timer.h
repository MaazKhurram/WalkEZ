#ifndef TIMER
#define	TIMER

// Device configuration 
#pragma config FOSC = INTOSC    // Oscillator Selection Bits->INTOSC oscillator: I/O function on CLKIN pin
#pragma config WDTE = OFF    // Watchdog Timer Enable->WDT disabled
#pragma config PLLEN = ON    // Phase Lock Loop enable->4x PLL is always enabled

// Library Files
#include <stdbool.h>
#include <xc.h>


/* -------------------- Functions provided in Timer.c -------------------- */

// REQUIRES: Nothing
// PROMISES: Configure Timer1 at 8MHz with pre-scaler of 8 with no gate controls.
//           timer1 ON bit is off.
void ConfigureTimer1();

// REQUIRES: System clock at 32MHz
// PROMISES: Configure Timer2 at system clock with pre-scaler of 0 and post-scaler of 0.
//           timer2 ON bit is off.
void ConfigureTimer2();

// REQUIRES: Nothing
// PROMISES: Configure Timer4 as a one-shot with a 0.5us tick rate.
//           Configure Timer6 as a one-shot with a 484us tick rate.
//           timer4 ON bit is off.
void ConfigureTimer4();

/* ------------------- Macro-Based Function Declarations -------------------- */

#define TIMER1_START()          \
{                               \
    PIR1bits.TMR1IF=0;          \
    TMR1H = 0;                  \
    TMR1L = 0;                  \
    T1CONbits.ON = 1;           \
}

#define TIMER1_WAIT()           \
{                               \
    while(PIR1bits.TMR1IF==0);  \
}

#define TIMER2_START(period)    \
{                               \
    T2PR = period;              \
    PIR1bits.TMR2IF = 0;        \
    T2CONbits.ON = 1;           \
}

#define TIMER2_WAIT()           \
{                               \
    while(!PIR1bits.TMR2IF);    \
    PIR1bits.TMR2IF = 0;        \
}

#define TIMER4_START(period)    \
{                               \
    T4PR = period;              \
    PIR4bits.TMR4IF = 0;        \
    T4CONbits.ON = 1;           \
}

#define TIMER4_WAIT()           \
{                               \
    while(!PIR4bits.TMR4IF){PLAY_AUDIO();}    \
    PIR4bits.TMR4IF = 0;        \
}




#endif	/* LAB2_LIBRARY_H */
