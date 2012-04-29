#define _XTAL_FREQ 4000000

#define LED_C1 LATC3
#define LED_C2 LATA4
#define LED_C3 LATA3
#define LED_C4 LATB5
#define LED_A1 LATA5
#define LED_A2 LATA7
#define LED_A3 LATA6
#define LED_A4 LATC4
#define LED_A5 LATC5
#define LED_A6 LATC2
#define LED_A7 LATC1
#define LED_A8 LATC0

#define SWITCH_NEXT PORTBbits.RB7
#define SWITCH_UNIT PORTBbits.RB6

#define ADC_V1 12
#define ADC_V2  8
#define ADC_V3  0
#define ADC_V4  2
#define ADC_I1 10
#define ADC_I2  9
#define ADC_I3 11
#define ADC_I4  1


void init(void) {
    //Oscillator
    IRCF2 = 1;   //4 MHz
    IRCF1 = 0;
    IRCF0 = 1;

    //I/O
    ANSEL  = 0b00000111;
    ANSELH = 0b00011111;
    TRISA  = 0b00000111;
    TRISB  = 0b11011111;
    TRISC  = 0b00000000;
    LATA   = 0;
    LATB   = 0;
    LATC   = 0;

    //pull-up for switches
    RBPU = 0;   //PORTB pull-ups are enabled provided that the pin is an input and the corresponding WPUB bit is set.
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