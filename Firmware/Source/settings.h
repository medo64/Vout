#ifndef SETTINGS_H
#define SETTINGS_H


unsigned unsigned char getMeasureIndex();
void setMeasureIndex(unsigned char value);

unsigned unsigned char getMeasureUnit();
void setMeasureUnit(unsigned char value);

signed char getCurrentAdcOffset(unsigned char channel);
void setCurrentAdcOffset(unsigned char channel, signed char value);


#endif
