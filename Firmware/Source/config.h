#ifndef APP_H
#define APP_H


#pragma config FOSC     = INTOSC
#pragma config WDTE     = OFF
#pragma config PWRTE    = ON
#pragma config MCLRE    = OFF
#pragma config CP       = OFF
#pragma config BOREN    = ON
#pragma config CLKOUTEN = OFF
#pragma config IESO     = OFF
#pragma config FCMEN    = OFF

#pragma config WRT      = OFF
#pragma config VCAPEN   = OFF
#pragma config STVREN   = ON
#pragma config BORV     = LO
#pragma config LPBOR    = OFF
#pragma config LVP      = OFF


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


void init();
void splash();

#endif
