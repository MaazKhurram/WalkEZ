#include <pic.h>
#include <pic16f1778.h>
#include "Config.h"
#include "Timer.h"

#define OutputPin PORTBbits.RB0 //trig RC2
#define InputPin PORTBbits.RB1  //echo RC3
#define MAX_SAMPLES 201


typedef struct sine_node{
    short item;
    struct sine_node* next; 
} Node;
/*
#define PLAY_AUDIO()                        \
{                                           \
    if (ArrayIndex == 200)                  \
        ArrayIndex = 0;                     \
    audio_output = array200[ArrayIndex++];  \
    write2DAC(audio_output);                \
    TIMER2_WAIT();                          \
}                                           
*/
#define PLAY_AUDIO()                        \
{                                           \
    if(audio_output){                        \
    DAC1REFL = current->item;               \
    DACLDbits.DAC1LD = 1;                   \
    current = current->next;                \
    TIMER2_WAIT();                          \
    }                                       \
}                             


short SineArray(void);
//REQUIRES:
//          WriteValue is between 0 - 1023
//PROMISES:
//          writes value of WriteValue to the DAC1REFH and DAC1REFL such that 
//          WriteValue is written to the output of DAC1
void write2DAC(short WriteValue);
void __interrupt () playSineWave(void);


static short audio_output; 
static char ArrayIndex = 0;
static const Node Sine_Sample[201]={
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
    
    const short array200[] = 
        {512,528,544,560,576,592,607,623,639,654,670,685,700,715,
        729,744,758,772,786,799,812,825,838,850,862,873,884,895,
        906,916,925,935,943,952,960,967,974,981,987,993,998,1003,
        1007,1011,1014,1017,1019,1021,1022,1023,1023,1023,1022,1021,
        1019,1017,1014,1011,1007,1003,998,993,987,981,974,967,960,952,
        943,935,925,916,906,895,884,873,862,850,838,825,812,799,786,772,
        758,744,729,715,700,685,670,654,639,623,607,592,576,560,544,528,
        512,495,479,463,447,431,416,400,384,369,353,338,323,308,294,279,
        265,251,237,224,211,198,185,173,161,150,139,128,117,107,98,88,80,
        71,63,56,49,42,36,30,25,20,16,12,9,6,4,2,1,0,0,0,1,2,4,6,9,12,16,20,
        25,30,36,42,49,56,63,71,80,88,98,107,117,128,139,150,161,173,185,198,
        211,224,237,251,265,279,294,308,323,338,353,369,384,400,416,431,447,
        463,479,495,512};
    
    
    // Set the system clock speed to 32MHz and wait for the ready flag.
    OSCCON = 0xF4;
    while(OSCSTATbits.HFIOFR == 0);
    //Initialize the Digital to Analog Converter
    TRISBbits.TRISB0=0;
    TRISBbits.TRISB1=1;
    ANSELBbits.ANSB0=0;
    ANSELBbits.ANSB1=0;

    TRISA = 251;
    DAC1CON0 = 160;
    DAC1REFH = 0;
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


void write2DAC(short WriteValue)
{
    DAC1REFH = WriteValue>>8;
    short temp = WriteValue<<8;
    DAC1REFL = temp>>8;
    DACLDbits.DAC1LD = 1;
}
 

void __interrupt () playSineWave(void)
{
    DAC1REFL = current->item;
    DACLDbits.DAC1LD = 1;
    current = current->next;
    TIMER2_WAIT();
}