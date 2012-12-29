#ifndef DISPLAY_H
#define DISPLAY_H


volatile unsigned char Display[4] = { 0, 0, 0, 0 };

void displayValue(float value, unsigned char measureIndex, unsigned char measureUnit);
void displayError(unsigned char errorNumber);


#endif
