#ifndef MEASURE_H
#define MEASURE_H


float round(float value);

float getVoltage(unsigned char channel);
float getCurrent(unsigned char channel);

unsigned int getRawAdc(unsigned char channel);
float getRawVoltage(unsigned char channel);
float getRawCurrent(unsigned char channel);


#endif
