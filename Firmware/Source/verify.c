#include "verify.h"

#include "config.h"
#include "measure.h"
#include "display.h"


#define VOLTAGE_MIN 4.5
#define VOLTAGE_MAX 13.2
#define VOLTAGE_DIFF_MAX 0.1
#define CURRENT_IDLE_INPUT_MAX  0.025
#define CURRENT_IDLE_OUTPUT_MAX 0.005


void verify() {
    float i1 = getCurrent(ADC_I1);
    float i2 = getCurrent(ADC_I2);
    float i3 = getCurrent(ADC_I3);
    float i4 = getCurrent(ADC_I4);

    float v1 = getVoltage(ADC_V1);
    float v2 = getVoltage(ADC_V2);
    float v3 = getVoltage(ADC_V3);
    float v4 = getVoltage(ADC_V4);
    float v2diff = (v2 > v1) ?  v2 - v1 : v1 - v2;
    float v3diff = (v3 > v1) ?  v3 - v1 : v1 - v3;
    float v4diff = (v4 > v1) ?  v4 - v1 : v1 - v4;

    if (i1 > CURRENT_IDLE_INPUT_MAX) { //E1: idle input current more than 20mA
        displayError(1);
    } else if (i2 > CURRENT_IDLE_OUTPUT_MAX) { //E2: unexpected current draw on output 1
        displayError(2);
    } else if (i3 > CURRENT_IDLE_OUTPUT_MAX) { //E3: unexpected current draw on output 2
        displayError(3);
    } else if (i4 > CURRENT_IDLE_OUTPUT_MAX) { //E4: unexpected current draw on output 3
        displayError(4);
    } else if ((v1 < VOLTAGE_MIN) || (v1 > VOLTAGE_MAX)) { //E5: input voltage is outside of allowed range
        displayError(5);
    } else if (v2diff > VOLTAGE_DIFF_MAX)  { //E6: idle voltage drop is more than 100mV on output 1
        displayError(6);
    } else if (v3diff > VOLTAGE_DIFF_MAX) { //E7: idle voltage drop is more than 100mV on output 2
        displayError(7);
    } else if (v4diff > VOLTAGE_DIFF_MAX) { //E8: idle voltage drop is more than 100mV on output 3
        displayError(8);
    } else { //E0: no error
        displayError(0);
    }
}
