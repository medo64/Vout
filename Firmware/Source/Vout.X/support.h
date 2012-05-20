float round(float value) {
    return (int)(value * 1000) / 1000.0;
}

unsigned int getAdc(unsigned char channel) {
    ADCON0 = (channel << 2); //Analog Channel Select bits
    ADON = 1; //ADC is enabled
    __delay_us(10); //to discharge holding cap if there was measurement just before (at least 5us)
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
