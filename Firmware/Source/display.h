#ifndef DISPLAY_H
#define DISPLAY_H


volatile unsigned char Display[4] = { 0, 0, 0, 0 };

unsigned char getSegments(unsigned char number);
void displayValue(float value, unsigned char measureIndex, unsigned char measureUnit);


#endif
