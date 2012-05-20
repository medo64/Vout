#include <p18f23k20.h>
#include "config.h"

#include "init.h"
#include "support.h"

#define SWITCH_COUNTER_MAX     48;
#define SWITCH_COUNTER_MAX_MAX 128;

volatile char Display[4] = { 0, 0, 0, 0 };
volatile char DisplayIndex = 0;
volatile char SwitchCounterNext = SWITCH_COUNTER_MAX;
volatile char SwitchCounterUnit = SWITCH_COUNTER_MAX;

char MeasureIndex = 0;
char MeasureUnit = 0;

float measure(void);
void splash(void);
bit switchCheck(void);
void displayValue(float value, char measureIndex, char measureUnit);


void main(void) {
    init();
    splash();

    float value = measure();
    while (1) {
        displayValue(value, MeasureIndex, MeasureUnit);
        for (int i=0; i<10; i++) {
            float newValue = measure();
            value = value + (newValue - value) * 0.23; //to smooth it a little
            value = round(value);
            if (switchCheck()) {
                value = measure();
                break;
            }
        }
    }
}

void interrupt isr(void) {
    if(TMR0IE && TMR0IF) { //if enabled and triggered
        LED_A1 = LED_A2 = LED_A3 = LED_A4 = 0;
        LED_C1 = LED_C2 = LED_C3 = LED_C4 = LED_C5 = LED_C6 = LED_C7 = LED_C8 = 0;

        LED_C1 = (Display[DisplayIndex] & 0x01) ? 0 : 1;
        LED_C2 = (Display[DisplayIndex] & 0x02) ? 0 : 1;
        LED_C3 = (Display[DisplayIndex] & 0x04) ? 0 : 1;
        LED_C4 = (Display[DisplayIndex] & 0x08) ? 0 : 1;
        LED_C5 = (Display[DisplayIndex] & 0x10) ? 0 : 1;
        LED_C6 = (Display[DisplayIndex] & 0x20) ? 0 : 1;
        LED_C7 = (Display[DisplayIndex] & 0x40) ? 0 : 1;
        LED_C8 = (Display[DisplayIndex] & 0x80) ? 0 : 1;
        switch (DisplayIndex) {
            case 0: LED_A1 = 1; break;
            case 1: LED_A2 = 1; break;
            case 2: LED_A3 = 1; break;
            case 3: LED_A4 = 1; break;
        }
        DisplayIndex = (DisplayIndex + 1) % 4;

        if (SWITCH_NEXT == 0) {
            if (SwitchCounterNext > 0) { SwitchCounterNext -= 1; }
        } else {
            SwitchCounterNext = SWITCH_COUNTER_MAX;
        }
        if (SWITCH_UNIT == 0) {
            if (SwitchCounterUnit > 0) { SwitchCounterUnit -= 1; }
        } else {
            SwitchCounterUnit = SWITCH_COUNTER_MAX;
        }

        TMR0IF = 0; //clear flag
    }
}

bit switchCheck() {
    if (SwitchCounterNext == 0) {
        MeasureIndex = (MeasureIndex + 1) % 4;
        SwitchCounterNext = SWITCH_COUNTER_MAX_MAX;
        return 1;
    }

    if (SwitchCounterUnit == 0) {
        MeasureUnit = (MeasureUnit + 1) % 3;
        SwitchCounterUnit = SWITCH_COUNTER_MAX_MAX;
        return 1;
    }

    return 0;
}

float measure() {
    switch (MeasureIndex) {
        case 0:
            switch (MeasureUnit) {
                case 0: return getVoltage(ADC_V1);
                case 1: return getCurrent(ADC_I1);
                case 2: return getVoltage(ADC_V1) * getCurrent(ADC_I1);
            }
        case 1:
            switch (MeasureUnit) {
                case 0: return getVoltage(ADC_V2);
                case 1: return getCurrent(ADC_I2);
                case 2: return getVoltage(ADC_V2) * getCurrent(ADC_I2);
            }
        case 2:
            switch (MeasureUnit) {
                case 0: return getVoltage(ADC_V3);
                case 1: return getCurrent(ADC_I3);
                case 2: return getVoltage(ADC_V3) * getCurrent(ADC_I3);
            }
        case 3:
            switch (MeasureUnit) {
                case 0: return getVoltage(ADC_V4);
                case 1: return getCurrent(ADC_I4);
                case 2: return getVoltage(ADC_V4) * getCurrent(ADC_I4);
            }
    }
    return 1000000;
}

void displayValue000(int value, int dotIndex) {
    char tempDisplay[3];
    char d0 = (char)(value / 100);
    char d1 = (char)((value / 10) % 10);
    char d2 = (char)(value % 10);
    tempDisplay[0] = getSegments(d0);
    tempDisplay[1] = getSegments(d1);
    tempDisplay[2] = getSegments(d2);
    if (dotIndex < 2) {
        if ((dotIndex == 0) && (d0 == 0)) {
            tempDisplay[0] = 0b10000000;
        } else {
            tempDisplay[dotIndex] = tempDisplay[dotIndex] | 0b10000000;
        }
    }
    Display[0] = tempDisplay[0];
    Display[1] = tempDisplay[1];
    Display[2] = tempDisplay[2];
}

void displayValue(float value, char measureIndex, char measureUnit) {
    if (value > 999) {
        Display[0] = 0b00111111;
        Display[1] = 0b00011100;
        Display[2] = 0b00000000;
    } else {
        if (value < 10) {
            displayValue000((int)(value * 100), 0);
        } else if (value < 100) {
            displayValue000((int)(value * 10), 1);
        } else {
            displayValue000((int)value, 2);
        }
    }

    switch (measureIndex) {
        case 0: Display[3] = 0b00000001; break;
        case 1: Display[3] = 0b00000010; break;
        case 2: Display[3] = 0b00000100; break;
        case 3: Display[3] = 0b00001000; break;
        default: Display[3] = 0b00001111; break;
    }
    switch (measureUnit) {
        case 0: Display[3] ^= 0b01000000; break;
        case 1: Display[3] ^= 0b10000000; break;
        case 2: Display[3] ^= 0b11000000; break;
        case 3: Display[3] ^= 0b11110000; break;
    }
}

void splash(void) {
    Display[0] = 0b11111111; Display[1] = 0b11111111; Display[2] = 0b11111111; Display[3] = 0b11111111;
    __delay_ms(100);    
    Display[0] = 0b01111111; Display[1] = 0b01111111; Display[2] = 0b01111111;
    __delay_ms(100);
    Display[0] = 0b00111001; Display[1] = 0b00001001; Display[2] = 0b00001111; Display[3] = 0b00000000;
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
    Display[3] = 0b01000001;
}
