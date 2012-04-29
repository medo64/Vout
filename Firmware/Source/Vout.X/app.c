#include <p18f23k20.h>
#include "config.h"

#include "init.h"
#include "support.h"


volatile unsigned char Display[4] = { 0, 0, 0, 0 };
volatile unsigned char DisplayIndex = 0;

unsigned char MeasureIndex = 0;
bit MeasureUnit = 0;


float measure(void);
void showMeasureStatus(void);
void writeValue(float value);
void splash(void);


void main(void) {
    init();
    splash();

    char switchNextCounter = 0;
    char switchUnitCounter = 0;
    float value = measure();
    while (1) {
        writeValue(value);
        for (int i=0; i<10; i++) {
            float newValue = measure();
            value = (value + (newValue - value) * 0.13); //to smooth it a little
            value = (int)(value * 1000.0) / 1000.0; //rounding
            if (SWITCH_NEXT == 0) { switchNextCounter += 1; } else { switchNextCounter = 0; }
            if (SWITCH_UNIT == 0) { switchUnitCounter += 1; } else { switchUnitCounter = 0; }
            if (switchNextCounter > 25) {
                switchNextCounter = 0;
                MeasureIndex = (MeasureIndex + 1) % 4;
                showMeasureStatus();
                value = measure();
                break;
            } else if (switchUnitCounter > 25) {
                switchUnitCounter = 0;
                MeasureUnit = !MeasureUnit;
                showMeasureStatus();
                value = measure();
                break;
            }
        }
    }
}

void interrupt isr(void) {
    if(TMR0IE && TMR0IF) { //if enabled and triggered
        LED_C1 = LED_C2 = LED_C3 = LED_C4 = 1;
        LED_A1 = (Display[DisplayIndex] & 0x01) ? 1 : 0;
        LED_A2 = (Display[DisplayIndex] & 0x02) ? 1 : 0;
        LED_A3 = (Display[DisplayIndex] & 0x04) ? 1 : 0;
        LED_A4 = (Display[DisplayIndex] & 0x08) ? 1 : 0;
        LED_A5 = (Display[DisplayIndex] & 0x10) ? 1 : 0;
        LED_A6 = (Display[DisplayIndex] & 0x20) ? 1 : 0;
        LED_A7 = (Display[DisplayIndex] & 0x40) ? 1 : 0;
        LED_A8 = (Display[DisplayIndex] & 0x80) ? 1 : 0;
        switch (DisplayIndex) {
            case 0: LED_C1 = 0; break;
            case 1: LED_C2 = 0; break;
            case 2: LED_C3 = 0; break;
            case 3: LED_C4 = 0; break;
        }
        DisplayIndex = (DisplayIndex + 1) % 4;
        TMR0IF = 0; //clear flag
    }
}

void showMeasureStatus() {
    if (MeasureUnit == 0) {
        Display[0] = 0b00111110;
    } else {
        Display[0] = 0b00110000;
    }
    Display[1] = 0b00000000;
    Display[2] = getSegments(MeasureIndex);
    __delay_ms(100); __delay_ms(100); __delay_ms(100); __delay_ms(100); __delay_ms(100);
}

float measure() {
    switch (MeasureIndex) {
        case 0: return (MeasureUnit == 0) ? getVoltage(ADC_V1) : getCurrent(ADC_I1);
        case 1: return (MeasureUnit == 0) ? getVoltage(ADC_V2) : getCurrent(ADC_I2);
        case 2: return (MeasureUnit == 0) ? getVoltage(ADC_V3) : getCurrent(ADC_I3);
        case 3: return (MeasureUnit == 0) ? getVoltage(ADC_V4) : getCurrent(ADC_I4);
        default: return 1000000;
    }
}

void writeValue000(int value, int dotIndex) {
    char tempDisplay[3];
    tempDisplay[0] = getSegments((char)(value / 100));
    tempDisplay[1] = getSegments((char)((value / 10) % 10));
    tempDisplay[2] = getSegments((char)(value % 10));
    if (dotIndex < 2) {
        tempDisplay[dotIndex] = tempDisplay[dotIndex] | 0b10000000;
    }
    Display[0] = tempDisplay[0];
    Display[1] = tempDisplay[1];
    Display[2] = tempDisplay[2];
}

void writeValue(float value) {
    if (value > 999) {
        Display[0] = 0b00111111;
        Display[1] = 0b00011100;
        Display[2] = 0b00000000;
    } else {
        if (value < 10) {
            writeValue000((int)(value * 100), 0);
        } else if (value < 100) {
            writeValue000((int)(value * 10), 1);
        } else {
            writeValue000((int)value, 2);
        }
    }
}


void splash(void) {
    Display[0] = 0b00111001; Display[1] = 0b00001001; Display[2] = 0b00001111;
    __delay_ms(100);
    Display[0] = 0b00000000; Display[1] = 0b00000000; Display[2] = 0b00000000;
    __delay_ms(100);
    Display[0] = 0b00111001; Display[1] = 0b00001001; Display[2] = 0b00001111;
    __delay_ms(100);
    Display[0] = 0b00111001; Display[1] = 0b00000000; Display[2] = 0b00001111;
    __delay_ms(100);
    Display[0] = 0b01110000; Display[1] = 0b00000000; Display[2] = 0b01000110;
    __delay_ms(100);
    Display[0] = 0b01000000; Display[1] = 0b01000000; Display[2] = 0b01000000;
    __delay_ms(100);
    Display[0] = 0b00000000; Display[1] = 0b01000000; Display[2] = 0b00000000;
    __delay_ms(100);
    Display[0] = 0b00000000; Display[1] = 0b00000000; Display[2] = 0b00000000;
    __delay_ms(100);
}
