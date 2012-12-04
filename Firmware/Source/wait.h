unsigned char wait_asm_W; //copy of W
unsigned char wait_asm_N; //outer spending block
unsigned char wait_asm_M; //main spending block
unsigned char wait_asm_N_initial = 0;

/* Waits for one millisecond. It requires _XTAL_FREQ to be set. Don't rely on this for precission work. Valid from 32 kHz to 128 MHz. */
void wait_ms_1() {
    if (wait_asm_N_initial == 0) {
        unsigned long initialN = (_XTAL_FREQ / 4000 - 3) / 130; //N = (cycles per millisecond - 3) / (6 + 4m)
        wait_asm_N_initial = (char)initialN;
        if (wait_asm_N_initial == 0) { return; }
    }

    //initialize variables
    wait_asm_W = 0;
    wait_asm_N = wait_asm_N_initial;
    wait_asm_M = 0;

    #asm                                        //6n + 4nm + 3
        BANKSEL _wait_asm_W                     ;1
        MOVWF   _wait_asm_W & 0x7F              ;1

        ;N total of [ 6n + 4nm - 1 ] instructions
        BANKSEL     _wait_asm_M                 ;1*n
        MOVLW       0x1F                        ;1*n  //m=31
        MOVWF       _wait_asm_M & 0x7F          ;1*n

        ;M total of [ 4m - 1 ] instructions
        CLRWDT                                  ;1*m
        DECFSZ      _wait_asm_M & 0x7F, F       ;1*(m-1) + 2
        GOTO        $-2                         ;2*(m-1)

        BANKSEL     _wait_asm_N                 ;1*n
        DECFSZ      _wait_asm_N & 0x7F, F       ;1*(n-1) + 2
        GOTO        $-8                         ;2*(n-1)

        BANKSEL _wait_asm_W                     ;1
        MOVF    _wait_asm_W & 0x7F, W           ;1
    #endasm
}

char wait_adjustmentFactorTens = 10;

/* Sets adjustment factor. Used when interrupts are enabled and time measurement is skewed. */
void wait_adjust(char adjustmentFactorTens) {
    if (adjustmentFactorTens < 10) {
        wait_adjustmentFactorTens = 10;
    } else {
        wait_adjustmentFactorTens = adjustmentFactorTens;
    }
}

/* Waits for defined number of milliseconds. It requires _XTAL_FREQ to be set. Don't rely on this for precission work. */
void wait_ms(short milliseconds) {
    milliseconds = milliseconds * 10 / wait_adjustmentFactorTens;
    if (milliseconds <= 0) { return; }
    for (short i=0; i<milliseconds; i++) {
        wait_ms_1();
    }
}
