unsigned char getSegments(char number) {
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

float round(float value) {
    return (int)(value * 1000) / 1000.0;
}

unsigned int getAdc(unsigned char channel) {
    ADCON0 = (channel << 2); //Analog Channel Select bits
    ADON = 1; //ADC is enabled
    __delay_us(20); //to discharge holding cap if there was measurement just before (at least 5us)
    GODONE = 1; //Setting this bit starts an A/D conversion cycle.
    while(GODONE); //A/D conversion cycle in progress.
    ADON = 0; //ADC is disabled and consumes no operating current
    return ADRES;
}

float getVoltage(unsigned char channel) {
    float reading = (float)getAdc(channel);
    return round(reading * 3.3 / 1024.0 * 11.0); //11:1 ratio (10K:1K)
}

float getCurrent(unsigned char channel) {
    float reading = (float)getAdc(channel);
    return round(reading * 3.3 / 1024.0 * 1.0); //1V is 1A (3V max)
}
