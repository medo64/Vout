#include "measure.h"
#include "pic.h"

#include "config.h"


const float VREF = 3.3;
const float ADC_N = 1024.0;
const float RATIO_VOLTAGE = 5.7; //5.7:1 ratio (4K7:1K)
const float RATIO_CURRENT = 1.0; //1V is 1A (3V max)


float round(float value) {
    return (int)(value * 1000) / 1000.0;
}


unsigned int getAdc(unsigned char channel) {
    ADCON0 = (channel << 2); //Analog Channel Select bits
    ADON = 1; //ADC is enabled
    __delay_us(10); //to discharge holding cap if there was measurement just before (at least 10us)
    ADGO = 1; //Setting this bit starts an A/D conversion cycle.
    while(ADGO); //A/D conversion cycle in progress.
    ADON = 0; //ADC is disabled and consumes no operating current
    return ADRES;
}



float getVoltage(unsigned char channel) {
    float reading = (float)getAdc(channel);
    return round(reading * VREF / ADC_N * RATIO_VOLTAGE);
}


float getCurrent(unsigned char channel) {
    float reading = (float)getAdc(channel);
    return round(reading * VREF / ADC_N * RATIO_CURRENT);
}
