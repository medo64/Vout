volatile char Display[4] = { 0, 0, 0, 0 };

char getSegments(char number) {
    switch (number) {
        case  0: return 0b00111111;
        case  1: return 0b00000110;
        case  2: return 0b01011011;
        case  3: return 0b01001111;
        case  4: return 0b01100110;
        case  5: return 0b01101101;
        case  6: return 0b01111101;
        case  7: return 0b00000111;
        case  8: return 0b01111111;
        case  9: return 0b01101111;
        case 10: return 0b01110111;
        case 11: return 0b01111100;
        case 12: return 0b00111001;
        case 13: return 0b01011110;
        case 14: return 0b01111001;
        case 15: return 0b01110001;
        default: return 0b11111111;
    }
}

void displayValue000(int value, int dotIndex) {
    char tempDisplay[3];
    char d0 = (char)(value / 100);
    char d1 = (char)((value / 10) % 10);
    char d2 = (char)(value % 10);
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

void displayValue(float value, char measureIndex, char measureUnit) {
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

    switch (measureIndex) {
        case 0:  Display[3] = 0b00000001; break;
        case 1:  Display[3] = 0b00000010; break;
        case 2:  Display[3] = 0b00000100; break;
        case 3:  Display[3] = 0b00001000; break;
        default: Display[3] = 0b00001111; break;
    }
    switch (measureUnit) {
        case 0:  Display[3] ^= 0b00100000; break;
        case 1:  Display[3] ^= 0b01000000; break;
        case 2:  Display[3] ^= 0b01100000; break;
        default: Display[3] ^= 0b01110000; break;
    }
}

void displaySplash() {
    wait_adjust(56);

    Display[0] = 0b11111111; Display[1] = 0b11111111; Display[2] = 0b11111111; Display[3] = 0b11111111;
    wait_ms(100);
    Display[0] = 0b01111111; Display[1] = 0b01111111; Display[2] = 0b01111111;
    wait_ms(100);
    Display[0] = 0b00111001; Display[1] = 0b00001001; Display[2] = 0b00001111; Display[3] = 0b00000000;
    wait_ms(100);
    Display[0] = 0b00000000; Display[1] = 0b00000000; Display[2] = 0b00000000;
    wait_ms(100);
    Display[0] = 0b00111001; Display[1] = 0b00001001; Display[2] = 0b00001111;
    wait_ms(100);
    Display[0] = 0b00111001; Display[1] = 0b00000000; Display[2] = 0b00001111;
    wait_ms(100);
    Display[0] = 0b01110000; Display[1] = 0b00000000; Display[2] = 0b01000110;
    wait_ms(100);
    Display[0] = 0b01000000; Display[1] = 0b01000000; Display[2] = 0b01000000;
    wait_ms(100);
    Display[0] = 0b00000000; Display[1] = 0b01000000; Display[2] = 0b00000000;
    wait_ms(100);
    Display[0] = 0b00000000; Display[1] = 0b00000000; Display[2] = 0b00000000;
}
