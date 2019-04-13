#include <pic.h>
#include <pic16f1778.h>
#include "Config.h"
#include "Timer.h"

#define OutputPin PORTBbits.RB0 //trig
#define InputPin PORTBbits.RB1  //echo
#define Button PORTBbits.RB2    //button   

typedef struct sine_node{
    short item;
    struct sine_node* next; 
} Node;

#define PLAY_AUDIO()                        \
{                                           \
    if(audio_output){                       \
        DAC1REFL = current->item;           \
        DACLDbits.DAC1LD = 1;               \
        current = current->next;            \
        TIMER2_WAIT();                      \
    }                                       \
}                             


void __interrupt () playSineWave(void);


static short audio_output; 
static char ArrayIndex = 0;
static const Node Sine_Sample[]={
    {(short)128,(Node*)&Sine_Sample[1]},{(short)144,(Node*)&Sine_Sample[2]},
    {(short)160,(Node*)&Sine_Sample[3]},{(short)176,(Node*)&Sine_Sample[4]},
    {(short)191,(Node*)&Sine_Sample[5]},{(short)205,(Node*)&Sine_Sample[6]},
    {(short)218,(Node*)&Sine_Sample[7]},{(short)229,(Node*)&Sine_Sample[8]},
    {(short)238,(Node*)&Sine_Sample[9]},{(short)245,(Node*)&Sine_Sample[10]},
    {(short)251,(Node*)&Sine_Sample[11]},{(short)254,(Node*)&Sine_Sample[12]},
    {(short)255,(Node*)&Sine_Sample[13]},{(short)254,(Node*)&Sine_Sample[14]},
    {(short)251,(Node*)&Sine_Sample[15]},{(short)245,(Node*)&Sine_Sample[16]},
    {(short)238,(Node*)&Sine_Sample[17]},{(short)229,(Node*)&Sine_Sample[18]},
    {(short)218,(Node*)&Sine_Sample[19]},{(short)205,(Node*)&Sine_Sample[20]},
    {(short)191,(Node*)&Sine_Sample[21]},{(short)176,(Node*)&Sine_Sample[22]},
    {(short)160,(Node*)&Sine_Sample[23]},{(short)144,(Node*)&Sine_Sample[24]},
    {(short)128,(Node*)&Sine_Sample[25]},{(short)111,(Node*)&Sine_Sample[26]},
    {(short)95,(Node*)&Sine_Sample[27]},{(short)79,(Node*)&Sine_Sample[28]},
    {(short)64,(Node*)&Sine_Sample[29]},{(short)50,(Node*)&Sine_Sample[30]},
    {(short)37,(Node*)&Sine_Sample[31]},{(short)26,(Node*)&Sine_Sample[32]},
    {(short)17,(Node*)&Sine_Sample[33]},{(short)10,(Node*)&Sine_Sample[34]},
    {(short)4,(Node*)&Sine_Sample[35]},{(short)1,(Node*)&Sine_Sample[36]},
    {(short)0,(Node*)&Sine_Sample[37]},{(short)1,(Node*)&Sine_Sample[38]},
    {(short)4,(Node*)&Sine_Sample[39]},{(short)10,(Node*)&Sine_Sample[40]},
    {(short)17,(Node*)&Sine_Sample[41]},{(short)26,(Node*)&Sine_Sample[42]},
    {(short)37,(Node*)&Sine_Sample[43]},{(short)50,(Node*)&Sine_Sample[44]},
    {(short)64,(Node*)&Sine_Sample[45]},{(short)79,(Node*)&Sine_Sample[46]},
    {(short)95,(Node*)&Sine_Sample[47]},{(short)111,(Node*)&Sine_Sample[48]},
    {(short)128,(Node*)&Sine_Sample[0]}
};


static Node* current = (Node*)&Sine_Sample[0];
void main(void) {
   
    // Set the system clock speed to 32MHz and wait for the ready flag.
    OSCCON = 0xF4;
    while(OSCSTATbits.HFIOFR == 0);
    //Initialize the Digital to Analog Converter
    TRISBbits.TRISB0=0;
    TRISBbits.TRISB1=1;
    TRISBbits.TRISB2=1;
    ANSELBbits.ANSB0=0;
    ANSELBbits.ANSB1=0;
    ANSELBbits.ANSB2=0;
    
    TRISA = 251;
    DAC1CON0 = 160;
    DAC1REFH = 1;
    long unsigned int duration=0;
    
    ConfigureTimer1();
    ConfigureTimer2();
    ConfigureTimer4();
    
    
    //INTCONbits.GIE=1;
    //INTCONbits.PEIE=1;
    //PIE1bits.TMR2IE=1;
    TIMER2_START(1);
    /*
    while(1)
    {
        OutputPin = 0;
        while(PIR1bits.TMR2IF==0);
        PIR1bits.TMR2IF=0;
        OutputPin = 1;
        while(PIR1bits.TMR2IF==0);
        PIR1bits.TMR2IF=0;
        OutputPin=0;
        __nop();
    }
    
    while(1){
        //PLAY_AUDIO();
        while(PIR1bits.TMR2IF==0);
        DAC1REFL = current->item;               
        DACLDbits.DAC1LD = 1;                   
        current = current->next;
        PIR1bits.TMR2IF=0;
    }
    */
    audio_output=0;
    volatile char hold=0;
    volatile char button_counter=0;
    while(1)
    {
        
        TIMER1_START();
        if(button_counter>0)
        {
            
            button_counter=0;
            //acquire new distance/audio
            OutputPin = 0;
            TIMER4_START(20);
            TIMER4_WAIT();

            OutputPin = 1;
            TIMER4_START(20);
            TIMER4_WAIT();

            OutputPin = 0;
            __nop();
            while(InputPin == 0){};
            TIMER1_START();
            
            while(InputPin == 1&&PIR1bits.TMR1IF==0){};
            
            if(PIR1bits.TMR1IF==1)
                continue;
            duration = TMR1H;
            duration = (duration<<8)+TMR1L;
            
            
            long unsigned int timer2p=(duration << 4)+(duration<<2);
            timer2p = timer2p>>10;
            if(timer2p<80)
            {
                TIMER2_START((char)timer2p);
                audio_output=1;
            }
            else
            {
                PIR1bits.TMR2IF=1;
                audio_output=0;
            }
            
            //play for some time.
            int play_count=10;
            while(play_count>0)
            {
                TIMER1_START();
                while(PIR1bits.TMR1IF==0)
                {
                    PLAY_AUDIO();
                }
                play_count--;
            }
        }
        else
        {
            if(Button==1 && hold==0)
            {
                hold=1;
                button_counter=1;
            }
            else if(Button==1 && hold==1)
            {
                continue;
            }
            else
            {
                hold=0;
                button_counter=0;
            }
        }
        
        TIMER1_WAIT();
    }
    
    //INTCONbits.GIE=1;
    //INTCONbits.PEIE=1;
    //PIE1bits.TMR2IE=1;
    while(1)
    {
            OutputPin = 0;
            TIMER4_START(20);
            TIMER4_WAIT();

            OutputPin = 1;
            TIMER4_START(20);
            TIMER4_WAIT();

            OutputPin = 0;
            __nop();
            while(InputPin == 0)
            {
                PLAY_AUDIO();
            }

            TIMER1_START();
            while((PIR1bits.TMR1IF==0) && (InputPin == 1) )
            {
                PLAY_AUDIO();
            }

            duration = TMR1H;
            duration = (duration<<8)+TMR1L;
            
            
            long unsigned int timer2p=(duration << 4)+(duration<<2);
            timer2p = timer2p>>10;
            if(timer2p<80)
            {
                TIMER2_START((char)timer2p);
                audio_output=1;
            }
            else
            {
                PIR1bits.TMR2IF=1;
                audio_output=0;
            }

            while(PIR1bits.TMR1IF==0)
            {
                PLAY_AUDIO();
            }
        
    }
      
}
 
void __interrupt () playSineWave(void)
{
    if(audio_output){
        DAC1REFL = current->item;
        DACLDbits.DAC1LD = 1;
        current = current->next;
        TIMER2_WAIT();
    }
}