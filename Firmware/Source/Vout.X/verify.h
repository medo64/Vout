#define VERIFY_VOLTAGE_MULTIPLIER 10
#define VERIFY_VOLTAGE_MIN 45
#define VERIFY_VOLTAGE_MAX 132
#define VERIFY_CURRENT_IDLE_INPUT_MAX  0.025
#define VERIFY_CURRENT_IDLE_OUTPUT_MAX 0.005

void verify() {
    float i1 = getCurrent(ADC_I1);
    float i2 = getCurrent(ADC_I2);
    float i3 = getCurrent(ADC_I3);
    float i4 = getCurrent(ADC_I4);

    int v1 = (int)(getVoltage(ADC_V1) * VERIFY_VOLTAGE_MULTIPLIER);
    int v2diff = v1 - (int)(getVoltage(ADC_V2) * VERIFY_VOLTAGE_MULTIPLIER);
    int v3diff = v1 - (int)(getVoltage(ADC_V3) * VERIFY_VOLTAGE_MULTIPLIER);
    int v4diff = v1 - (int)(getVoltage(ADC_V4) * VERIFY_VOLTAGE_MULTIPLIER);

    if (i1 > VERIFY_CURRENT_IDLE_INPUT_MAX) { //E1: idle input current more than 20mA
        Display[0] = getSegments(0x0E);
        Display[1] = getSegments(0xFF);
        Display[2] = getSegments(0x01);
    } else if (i2 > VERIFY_CURRENT_IDLE_OUTPUT_MAX) { //E2: unexpected current draw on output 1
        Display[0] = getSegments(0x0E);
        Display[1] = getSegments(0xFF);
        Display[2] = getSegments(0x02);
    } else if (i3 > VERIFY_CURRENT_IDLE_OUTPUT_MAX) { //E3: unexpected current draw on output 2
        Display[0] = getSegments(0x0E);
        Display[1] = getSegments(0xFF);
        Display[2] = getSegments(0x03);
    } else if (i4 > VERIFY_CURRENT_IDLE_OUTPUT_MAX) { //E4: unexpected current draw on output 3
        Display[0] = getSegments(0x0E);
        Display[1] = getSegments(0xFF);
        Display[2] = getSegments(0x04);
    } else if ((v1 < VERIFY_VOLTAGE_MIN) || (v1 > VERIFY_VOLTAGE_MAX)) { //E5: input voltage is outside of allowed range
        Display[0] = getSegments(0x0E);
        Display[1] = getSegments(0xFF);
        Display[2] = getSegments(0x05);
    } else if (v2diff != 0)  { //E6: idle voltage drop is more than 100mV on output 1
        Display[0] = getSegments(0x0E);
        Display[1] = getSegments(0xFF);
        Display[2] = getSegments(0x06);
    } else if (v3diff != 0) { //E7: idle voltage drop is more than 100mV on output 2
        Display[0] = getSegments(0x0E);
        Display[1] = getSegments(0xFF);
        Display[2] = getSegments(0x07);
    } else if (v4diff != 0) { //E8: idle voltage drop is more than 100mV on output 3
        Display[0] = getSegments(0x0E);
        Display[1] = getSegments(0xFF);
        Display[2] = getSegments(0x08);
    } else { //E0: no error
        Display[0] = getSegments(0xFF);
        Display[1] = getSegments(0x00);
        Display[2] = getSegments(0xFF);
    }
}
