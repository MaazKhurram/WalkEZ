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
    DAC1REFL = current->item;               \
    DACLDbits.DAC1LD = 1;                   \
    current = current->next;                \
    TIMER2_WAIT();                          \
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
static Node* current;

void main(void) {
    int test = 1;
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
    
    const Node Sine_Sample[201]={
        {(short)512,(Node*)&Sine_Sample[1]},{(short)528,(Node*)&Sine_Sample[2]},{(short)544,(Node*)&Sine_Sample[3]},{(short)560,(Node*)&Sine_Sample[4]},{(short)576,(Node*)&Sine_Sample[5]},{(short)592,(Node*)&Sine_Sample[6]},{(short)607,(Node*)&Sine_Sample[7]},{(short)623,(Node*)&Sine_Sample[8]},{(short)639,(Node*)&Sine_Sample[9]},{(short)654,(Node*)&Sine_Sample[10]},{(short)670,(Node*)&Sine_Sample[11]},{(short)685,(Node*)&Sine_Sample[12]},{(short)700,(Node*)&Sine_Sample[13]},{(short)715,(Node*)&Sine_Sample[14]},{(short)729,(Node*)&Sine_Sample[15]},{(short)744,(Node*)&Sine_Sample[16]},{(short)758,(Node*)&Sine_Sample[17]},{(short)772,(Node*)&Sine_Sample[18]},{(short)786,(Node*)&Sine_Sample[19]},{(short)799,(Node*)&Sine_Sample[20]},{(short)812,(Node*)&Sine_Sample[21]},{(short)825,(Node*)&Sine_Sample[22]},{(short)838,(Node*)&Sine_Sample[23]},{(short)850,(Node*)&Sine_Sample[24]},{(short)862,(Node*)&Sine_Sample[25]},{(short)873,(Node*)&Sine_Sample[26]},{(short)884,(Node*)&Sine_Sample[27]},{(short)895,(Node*)&Sine_Sample[28]},{(short)906,(Node*)&Sine_Sample[29]},{(short)916,(Node*)&Sine_Sample[30]},{(short)925,(Node*)&Sine_Sample[31]},{(short)935,(Node*)&Sine_Sample[32]},{(short)943,(Node*)&Sine_Sample[33]},{(short)952,(Node*)&Sine_Sample[34]},{(short)960,(Node*)&Sine_Sample[35]},{(short)967,(Node*)&Sine_Sample[36]},{(short)974,(Node*)&Sine_Sample[37]},{(short)981,(Node*)&Sine_Sample[38]},{(short)987,(Node*)&Sine_Sample[39]},{(short)993,(Node*)&Sine_Sample[40]},{(short)998,(Node*)&Sine_Sample[41]},{(short)1003,(Node*)&Sine_Sample[42]},{(short)1007,(Node*)&Sine_Sample[43]},{(short)1011,(Node*)&Sine_Sample[44]},{(short)1014,(Node*)&Sine_Sample[45]},{(short)1017,(Node*)&Sine_Sample[46]},{(short)1019,(Node*)&Sine_Sample[47]},{(short)1021,(Node*)&Sine_Sample[48]},{(short)1022,(Node*)&Sine_Sample[49]},{(short)1023,(Node*)&Sine_Sample[50]},{(short)1023,(Node*)&Sine_Sample[51]},{(short)1023,(Node*)&Sine_Sample[52]},{(short)1022,(Node*)&Sine_Sample[53]},{(short)1021,(Node*)&Sine_Sample[54]},{(short)1019,(Node*)&Sine_Sample[55]},{(short)1017,(Node*)&Sine_Sample[56]},{(short)1014,(Node*)&Sine_Sample[57]},{(short)1011,(Node*)&Sine_Sample[58]},{(short)1007,(Node*)&Sine_Sample[59]},{(short)1003,(Node*)&Sine_Sample[60]},{(short)998,(Node*)&Sine_Sample[61]},{(short)993,(Node*)&Sine_Sample[62]},{(short)987,(Node*)&Sine_Sample[63]},{(short)981,(Node*)&Sine_Sample[64]},{(short)974,(Node*)&Sine_Sample[65]},{(short)967,(Node*)&Sine_Sample[66]},{(short)960,(Node*)&Sine_Sample[67]},{(short)952,(Node*)&Sine_Sample[68]},{(short)943,(Node*)&Sine_Sample[69]},{(short)935,(Node*)&Sine_Sample[70]},{(short)925,(Node*)&Sine_Sample[71]},{(short)916,(Node*)&Sine_Sample[72]},{(short)906,(Node*)&Sine_Sample[73]},{(short)895,(Node*)&Sine_Sample[74]},{(short)884,(Node*)&Sine_Sample[75]},{(short)873,(Node*)&Sine_Sample[76]},{(short)862,(Node*)&Sine_Sample[77]},{(short)850,(Node*)&Sine_Sample[78]},{(short)838,(Node*)&Sine_Sample[79]},{(short)825,(Node*)&Sine_Sample[80]},{(short)812,(Node*)&Sine_Sample[81]},{(short)799,(Node*)&Sine_Sample[82]},{(short)786,(Node*)&Sine_Sample[83]},{(short)772,(Node*)&Sine_Sample[84]},{(short)758,(Node*)&Sine_Sample[85]},{(short)744,(Node*)&Sine_Sample[86]},{(short)729,(Node*)&Sine_Sample[87]},{(short)715,(Node*)&Sine_Sample[88]},{(short)700,(Node*)&Sine_Sample[89]},{(short)685,(Node*)&Sine_Sample[90]},{(short)670,(Node*)&Sine_Sample[91]},{(short)654,(Node*)&Sine_Sample[92]},{(short)639,(Node*)&Sine_Sample[93]},{(short)623,(Node*)&Sine_Sample[94]},{(short)607,(Node*)&Sine_Sample[95]},{(short)592,(Node*)&Sine_Sample[96]},{(short)576,(Node*)&Sine_Sample[97]},{(short)560,(Node*)&Sine_Sample[98]},{(short)544,(Node*)&Sine_Sample[99]},{(short)528,(Node*)&Sine_Sample[100]},{(short)512,(Node*)&Sine_Sample[101]},{(short)495,(Node*)&Sine_Sample[102]},{(short)479,(Node*)&Sine_Sample[103]},{(short)463,(Node*)&Sine_Sample[104]},{(short)447,(Node*)&Sine_Sample[105]},{(short)431,(Node*)&Sine_Sample[106]},{(short)416,(Node*)&Sine_Sample[107]},{(short)400,(Node*)&Sine_Sample[108]},{(short)384,(Node*)&Sine_Sample[109]},{(short)369,(Node*)&Sine_Sample[110]},{(short)353,(Node*)&Sine_Sample[111]},{(short)338,(Node*)&Sine_Sample[112]},{(short)323,(Node*)&Sine_Sample[113]},{(short)308,(Node*)&Sine_Sample[114]},{(short)294,(Node*)&Sine_Sample[115]},{(short)279,(Node*)&Sine_Sample[116]},{(short)265,(Node*)&Sine_Sample[117]},{(short)251,(Node*)&Sine_Sample[118]},{(short)237,(Node*)&Sine_Sample[119]},{(short)224,(Node*)&Sine_Sample[120]},{(short)211,(Node*)&Sine_Sample[121]},{(short)198,(Node*)&Sine_Sample[122]},{(short)185,(Node*)&Sine_Sample[123]},{(short)173,(Node*)&Sine_Sample[124]},{(short)161,(Node*)&Sine_Sample[125]},{(short)150,(Node*)&Sine_Sample[126]},{(short)139,(Node*)&Sine_Sample[127]},{(short)128,(Node*)&Sine_Sample[128]},{(short)117,(Node*)&Sine_Sample[129]},{(short)107,(Node*)&Sine_Sample[130]},{(short)98,(Node*)&Sine_Sample[131]},{(short)88,(Node*)&Sine_Sample[132]},{(short)80,(Node*)&Sine_Sample[133]},{(short)71,(Node*)&Sine_Sample[134]},{(short)63,(Node*)&Sine_Sample[135]},{(short)56,(Node*)&Sine_Sample[136]},{(short)49,(Node*)&Sine_Sample[137]},{(short)42,(Node*)&Sine_Sample[138]},{(short)36,(Node*)&Sine_Sample[139]},{(short)30,(Node*)&Sine_Sample[140]},{(short)25,(Node*)&Sine_Sample[141]},{(short)20,(Node*)&Sine_Sample[142]},{(short)16,(Node*)&Sine_Sample[143]},{(short)12,(Node*)&Sine_Sample[144]},{(short)9,(Node*)&Sine_Sample[145]},{(short)6,(Node*)&Sine_Sample[146]},{(short)4,(Node*)&Sine_Sample[147]},{(short)2,(Node*)&Sine_Sample[148]},{(short)1,(Node*)&Sine_Sample[149]},{(short)0,(Node*)&Sine_Sample[150]},{(short)0,(Node*)&Sine_Sample[151]},{(short)0,(Node*)&Sine_Sample[152]},{(short)1,(Node*)&Sine_Sample[153]},{(short)2,(Node*)&Sine_Sample[154]},{(short)4,(Node*)&Sine_Sample[155]},{(short)6,(Node*)&Sine_Sample[156]},{(short)9,(Node*)&Sine_Sample[157]},{(short)12,(Node*)&Sine_Sample[158]},{(short)16,(Node*)&Sine_Sample[159]},{(short)20,(Node*)&Sine_Sample[160]},{(short)25,(Node*)&Sine_Sample[161]},{(short)30,(Node*)&Sine_Sample[162]},{(short)36,(Node*)&Sine_Sample[163]},{(short)42,(Node*)&Sine_Sample[164]},{(short)49,(Node*)&Sine_Sample[165]},{(short)56,(Node*)&Sine_Sample[166]},{(short)63,(Node*)&Sine_Sample[167]},{(short)71,(Node*)&Sine_Sample[168]},{(short)80,(Node*)&Sine_Sample[169]},{(short)88,(Node*)&Sine_Sample[170]},{(short)98,(Node*)&Sine_Sample[171]},{(short)107,(Node*)&Sine_Sample[172]},{(short)117,(Node*)&Sine_Sample[173]},{(short)128,(Node*)&Sine_Sample[174]},{(short)139,(Node*)&Sine_Sample[175]},{(short)150,(Node*)&Sine_Sample[176]},{(short)161,(Node*)&Sine_Sample[177]},{(short)173,(Node*)&Sine_Sample[178]},{(short)185,(Node*)&Sine_Sample[179]},{(short)198,(Node*)&Sine_Sample[180]},{(short)211,(Node*)&Sine_Sample[181]},{(short)224,(Node*)&Sine_Sample[182]},{(short)237,(Node*)&Sine_Sample[183]},{(short)251,(Node*)&Sine_Sample[184]},{(short)265,(Node*)&Sine_Sample[185]},{(short)279,(Node*)&Sine_Sample[186]},{(short)294,(Node*)&Sine_Sample[187]},{(short)308,(Node*)&Sine_Sample[188]},{(short)323,(Node*)&Sine_Sample[189]},{(short)338,(Node*)&Sine_Sample[190]},{(short)353,(Node*)&Sine_Sample[191]},{(short)369,(Node*)&Sine_Sample[192]},{(short)384,(Node*)&Sine_Sample[193]},{(short)400,(Node*)&Sine_Sample[194]},{(short)416,(Node*)&Sine_Sample[195]},{(short)431,(Node*)&Sine_Sample[196]},{(short)447,(Node*)&Sine_Sample[197]},{(short)463,(Node*)&Sine_Sample[198]},{(short)479,(Node*)&Sine_Sample[199]},{(short)495,(Node*)&Sine_Sample[200]},{(short)512,(Node*)&Sine_Sample[0]}

};
    
    current = (Node*)&Sine_Sample[0];
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

    long unsigned int duration=0;
    
    ConfigureTimer1();
    ConfigureTimer2();
    ConfigureTimer4();
    
    /*
    INTCONbits.GIE=1;
    INTCONbits.PEIE=1;
    PIE1bits.TMR2IE=1;
    TIMER2_START(1);
    
    OutputPin = 0;
    while(PIR1bits.TMR2IF==0);
    PIR1bits.TMR2IF=0;
    OutputPin = 1;
    while(PIR1bits.TMR2IF==0);
    PIR1bits.TMR2IF=0;
    OutputPin=0;
    __nop();
     
    while(1){
        TIMER2_WAIT();
    __nop();
    }
    */
    audio_output=0;
    
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
            timer2p =timer2p>>10;
            if(timer2p<255)
            {
                TIMER2_START((char)timer2p);
            }
            else
            {
                PIR1bits.TMR2IF=1;
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
    
}