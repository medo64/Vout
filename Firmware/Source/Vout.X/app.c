#include <p18f23k20.h>
#include "config.h"

#include "wait.h"

#include "display.h"
#include "support.h"

#define BUTTON_COUNTER_MAX     48
#define BUTTON_COUNTER_MAX_MAX 144

volatile char DisplayIndex = 0;
volatile char ButtonCounterNext = BUTTON_COUNTER_MAX;
volatile char ButtonCounterUnit = BUTTON_COUNTER_MAX;

char MeasureIndex = 0;
char MeasureUnit = 0;


#define bitSet(var, bitno)              ((var) |= 1UL << (bitno))
#define bitClear(var, bitno)            ((var) &= ~(1UL << (bitno)))
#define cathodeBit(var, bitno, state)   if (state) { bitClear(var, bitno); } else { bitSet(var, bitno); }

float measure(void);
bit buttonCheck(void);


void main(void) {
    init();
    displaySplash();

    float value = measure();
    while (1) {
        displayValue(value, MeasureIndex, MeasureUnit);
        for (int i=0; i<10; i++) {
            float newValue = measure();
            value = value + (newValue - value) * 0.23; //to smooth it a little
            value = round(value);
            if (buttonCheck()) {
                value = measure();
                break;
            }
        }
    }
}

void interrupt isr(void) {
    if(TMR0IE && TMR0IF) { //if enabled and triggered
        unsigned char latA = LATA;
        unsigned char latB = LATB;
        unsigned char latC = LATC;

        bitSet(latC, 4); //LED_A1
        bitSet(latA, 3); //LED_A2
        bitSet(latA, 4); //LED_A3
        bitSet(latB, 5); //LED_A4
        cathodeBit(latC, 3, Display[DisplayIndex] & 0x01); //LED_C1
        cathodeBit(latC, 0, Display[DisplayIndex] & 0x02); //LED_C2
        cathodeBit(latC, 1, Display[DisplayIndex] & 0x04); //LED_C3
        cathodeBit(latA, 6, Display[DisplayIndex] & 0x08); //LED_C4
        cathodeBit(latC, 5, Display[DisplayIndex] & 0x10); //LED_C5
        cathodeBit(latC, 2, Display[DisplayIndex] & 0x20); //LED_C6
        cathodeBit(latA, 5, Display[DisplayIndex] & 0x40); //LED_C7
        cathodeBit(latA, 7, Display[DisplayIndex] & 0x80); //LED_C8

        switch (DisplayIndex) { //turn off
            case 0: bitSet(TRISB, 5); break;
            case 1: bitSet(TRISC, 4); break;
            case 2: bitSet(TRISA, 3); break;
            case 3: bitSet(TRISA, 4); break;
        }

        //set cathode
        LATA = latA;
        LATB = latB;
        LATC = latC;

        switch (DisplayIndex) { //turn on
            case 0: bitClear(TRISC, 4); break;
            case 1: bitClear(TRISA, 3); break;
            case 2: bitClear(TRISA, 4); break;
            case 3: bitClear(TRISB, 5); break;
        }

        DisplayIndex = (DisplayIndex + 1) % 4;

        if (BUTTON_NEXT == 0) {
            if (ButtonCounterNext > 0) { ButtonCounterNext -= 1; }
        } else {
            ButtonCounterNext = BUTTON_COUNTER_MAX;
        }
        if (BUTTON_UNIT == 0) {
            if (ButtonCounterUnit > 0) { ButtonCounterUnit -= 1; }
        } else {
            ButtonCounterUnit = BUTTON_COUNTER_MAX;
        }

        TMR0IF = 0; //clear flag
    }
}

bit buttonCheck() {
    if (ButtonCounterNext == 0) {
        MeasureIndex = (MeasureIndex + 1) % 4;
        ButtonCounterNext = BUTTON_COUNTER_MAX_MAX;
        return 1;
    }

    if (ButtonCounterUnit == 0) {
        MeasureUnit = (MeasureUnit + 1) % 3;
        ButtonCounterUnit = BUTTON_COUNTER_MAX_MAX;
        return 1;
    }

    return 0;
}

float measure() {
    switch (MeasureIndex) {
        case 0:
            switch (MeasureUnit) {
                case 0:  return getVoltage(ADC_V1);
                case 1:  return getCurrent(ADC_I1);
                case 2:  return getVoltage(ADC_V1) * getCurrent(ADC_I1);
                default: return 1000000;
            }
        case 1:
            switch (MeasureUnit) {
                case 0:  return getVoltage(ADC_V2);
                case 1:  return getCurrent(ADC_I2);
                case 2:  return getVoltage(ADC_V2) * getCurrent(ADC_I2);
                default: return 1000000;
            }
        case 2:
            switch (MeasureUnit) {
                case 0:  return getVoltage(ADC_V3);
                case 1:  return getCurrent(ADC_I3);
                case 2:  return getVoltage(ADC_V3) * getCurrent(ADC_I3);
                default: return 1000000;
            }
        case 3:
            switch (MeasureUnit) {
                case 0:  return getVoltage(ADC_V4);
                case 1:  return getCurrent(ADC_I4);
                case 2:  return getVoltage(ADC_V4) * getCurrent(ADC_I4);
                default: return 1000000;
            }
    }
    return 1000000;
}
