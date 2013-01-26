#include "verify.h"
#include <pic.h>

#include "config.h"
#include "measure.h"
#include "display.h"


#define VOLTAGE_MIN 4.5
#define VOLTAGE_MAX 13.2
#define VOLTAGE_DIFF_MAX 0.1
#define CURRENT_IDLE_INPUT_MAX  0.03
#define CURRENT_IDLE_OUTPUT_MAX 0.01


void displayError(unsigned char errorNumber, float value, unsigned char measureIndex, unsigned char measureUnit) {
    displayVerifyError(errorNumber);
    __delay_ms(500);
    displayValue(value, measureIndex, measureUnit);
    __delay_ms(1000);
    displayVerifyError(errorNumber);
}

void verify() {
    __delay_ms(500);

    float i1 = getRawCurrent(ADC_I1);
    float i2 = getRawCurrent(ADC_I2);
    float i3 = getRawCurrent(ADC_I3);
    float i4 = getRawCurrent(ADC_I4);

    float v1 = getRawVoltage(ADC_V1);
    float v2 = getRawVoltage(ADC_V2);
    float v3 = getRawVoltage(ADC_V3);
    float v4 = getRawVoltage(ADC_V4);
    float v2diff = (v2 > v1) ?  v2 - v1 : v1 - v2;
    float v3diff = (v3 > v1) ?  v3 - v1 : v1 - v3;
    float v4diff = (v4 > v1) ?  v4 - v1 : v1 - v4;

    if (i1 > CURRENT_IDLE_INPUT_MAX) { //E1: idle input current more than 20mA
        displayError(1, i1 * 1000, 0, 1);
    } else if (i2 > CURRENT_IDLE_OUTPUT_MAX) { //E2: unexpected current draw on output 1
        displayError(2, i2 * 1000, 1, 1);
    } else if (i3 > CURRENT_IDLE_OUTPUT_MAX) { //E3: unexpected current draw on output 2
        displayError(3, i3 * 1000, 2, 1);
    } else if (i4 > CURRENT_IDLE_OUTPUT_MAX) { //E4: unexpected current draw on output 3
        displayError(4, i4 * 1000, 3, 1);
    } else if ((v1 < VOLTAGE_MIN) || (v1 > VOLTAGE_MAX)) { //E5: input voltage is outside of allowed range
        displayError(5, v1, 0, 0);
    } else if (v2diff > VOLTAGE_DIFF_MAX)  { //E6: idle voltage drop is more than 100mV on output 1
        displayError(6, v2diff * 1000, 1, 0);
    } else if (v3diff > VOLTAGE_DIFF_MAX) { //E7: idle voltage drop is more than 100mV on output 2
        displayError(7, v3diff * 1000, 2, 0);
    } else if (v4diff > VOLTAGE_DIFF_MAX) { //E8: idle voltage drop is more than 100mV on output 3
        displayError(8, v4diff * 1000, 3, 0);
    } else { //E0: no error
        displayVerifyError(0);
    }
}
