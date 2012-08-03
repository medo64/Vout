#define _XTAL_FREQ 4000000

#define BUTTON_NEXT PORTBbits.RB7
#define BUTTON_UNIT PORTBbits.RB6

#define ADC_V1 11
#define ADC_I1  9
#define ADC_V2 10
#define ADC_I2  8
#define ADC_V3  0
#define ADC_I3 12
#define ADC_V4  2
#define ADC_I4  1


void init(void) {
    //Oscillator
    IRCF2 = 1;   //4 MHz
    IRCF1 = 0;
    IRCF0 = 1;

    //I/O
    ANSEL  = 0b00000111;
    ANSELH = 0b00011111;
    TRISA  = 0b00011111;   // c8 c4 c7 a3 a2 v4 i4 v3
    TRISB  = 0b11111111;   // b1 b2 a4 i3 i2 v2 i1 v1
    TRISC  = 0b10010000;   // rx tx c5 a1 c1 c6 c3 c2
    LATA   = 0;
    LATB   = 0;
    LATC   = 0;

    //pull-up for buttons
    RBPU = 0;            //PORTB pull-ups are enabled provided that the pin is an input and the corresponding WPUB bit is set.
    WPUB = 0b11000000;   //RB6, RB7

    //ADC
    ADFM  = 1;   //Right justified
    ACQT2 = 1;   //16 Tad
    ACQT1 = 1;
    ACQT0 = 0;
    ADCS2 = 1;   //Fosc/16
    ADCS1 = 0;
    ADCS0 = 1;
    VCFG1 = 0;   //Negative voltage reference supplied internally by VSS
    VCFG0 = 0;   //Positive voltage reference supplied internally by VDD

    //Timer
    T08BIT = 1;   //Timer0 is configured as an 8-bit timer/counter
    T0CS   = 0;   //Internal instruction cycle clock (CLKOUT)
    PSA    = 0;   //Timer0 prescaler is assigned. Timer0 clock input comes from prescaler output.
    T0PS2  = 0;   //1:8 prescale value
    T0PS1  = 1;
    T0PS0  = 0;
    TMR0IE = 1;   //Enable TIMER0 Interrupt
    TMR0ON = 1;   //Enables Timer0
    GIE    = 1;   //Enables all unmasked interrupts
}
