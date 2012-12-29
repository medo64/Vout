#include "calibrate.h"

#include "config.h"
#include "display.h"
#include "measure.h"
#include "settings.h"


void calibrate() {
    unsigned int i2 = getRawAdc(ADC_I2);
    unsigned int i3 = getRawAdc(ADC_I3);
    unsigned int i4 = getRawAdc(ADC_I4);

    if (i2 > 5) { //C2: too much current drawn on output 1
        displayCalibrateError(2);
    } else if (i3 > 5) { //C3: too much current drawn on output 2
        displayCalibrateError(3);
    } else if (i4 > 5) { //C3: too much current drawn on output 3
        displayCalibrateError(4);
    } else {
        unsigned int i1 = (i2 + i3 + i4) / 3; //pic draws current from input so it is not possible to calculate it directly
        setCurrentAdcOffset(ADC_I1, (signed char)(-i1));
        setCurrentAdcOffset(ADC_I2, (signed char)(-i2));
        setCurrentAdcOffset(ADC_I3, (signed char)(-i3));
        setCurrentAdcOffset(ADC_I4, (signed char)(-i4));
        displayCalibrateError(0);
    }
}
