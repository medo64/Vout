volatile unsigned char Display[4] = { 0, 0, 0, 0 };

const unsigned char SEGMENTS_DIGIT[] = { 0b00111111, 0b00000110, 0b01011011, 0b01001111,
                                         0b01100110, 0b01101101, 0b01111101, 0b00000111,
                                         0b01111111, 0b01101111, 0b01110111, 0b01111100,
                                         0b00111001, 0b01011110, 0b01111001, 0b01110001 };

unsigned char getSegments(unsigned char number) {
    if (number < 16) {
        return SEGMENTS_DIGIT[number];
    } else {
        return 0b00000000;
    }
}

void displayValue000(int value, unsigned char dotIndex) {
    unsigned char tempDisplay[3];
    unsigned char d0 = (unsigned char)(value / 100);
    unsigned char d1 = (unsigned char)((value / 10) % 10);
    unsigned char d2 = (unsigned char)(value % 10);
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

void displayValue(float value, unsigned char measureIndex, unsigned char measureUnit) {
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

    Display[3] = 0b00000001 << measureIndex;
    switch (measureUnit) {
        case 0:  Display[3] ^= 0b00100000; break;
        case 1:  Display[3] ^= 0b01000000; break;
        case 2:  Display[3] ^= 0b01100000; break;
        default: Display[3] ^= 0b01110000; break;
    }
}

void displaySplash() {
    Display[0] = 0b11111111; Display[1] = 0b11111111; Display[2] = 0b11111111; Display[3] = 0b11111111;
    for (int i=0; i<25000; i++) { asm("nop"); } //100ms (25000 nops + interrupt handling)
}