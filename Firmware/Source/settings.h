#define _SETTINGS_FLASH_RAW { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } //reserving space because erase block is block 32-word (64-bytes)
#define _SETTINGS_FLASH_LOCATION 0x0400
#define _SETTINGS_FLASH_RAW_BYTES 64

#define _SETTINGS_STORE_EMPTY { 0, 0 }
#define _SETTINGS_STORE_BYTES 2

#define _SETTINGS_REENABLE_GIE_AFTER_WRITE     1

const unsigned char _SETTINGS_FLASH[] @ _SETTINGS_FLASH_LOCATION = _SETTINGS_FLASH_RAW;

unsigned char _settings_asm_FlashLocationH = (_SETTINGS_FLASH_LOCATION >> 8) && 0xFF;
unsigned char _settings_asm_FlashLocationL =  _SETTINGS_FLASH_LOCATION && 0xFF;
unsigned char _settings_asm_FlashDataH = 0;
unsigned char _settings_asm_FlashDataL = 0;


void _settings_write(int index, unsigned char data) {
    unsigned char newProgram[] = _SETTINGS_STORE_EMPTY;
    for (int i=0; i<_SETTINGS_STORE_BYTES; i++) {
        newProgram[i] = _SETTINGS_FLASH[i];
    }
    newProgram[index] = data;

    GIE = 0;

    CFGS = 0; //access Flash program memory
    WREN = 1; //enable write to memory

    //erase row
    #asm //load starting address
        BANKSEL (__settings_asm_FlashLocationH)
        MOVF (__settings_asm_FlashLocationH & 0x7F), W
        BANKSEL PMADRH
        MOVWF (PMADRH & 0x7F)
        BANKSEL (__settings_asm_FlashLocationL)
        MOVF (__settings_asm_FlashLocationL & 0x7F), W
        BANKSEL PMADRH
        MOVWF (PMADRL & 0x7F)
    #endasm
    FREE = 1; //enable block Erase operation
    #asm //erase block
        BANKSEL PMCON2
        MOVLW 055h
        MOVWF (PMCON2 & 0x7F)  ;write 55h
        MOVLW 0AAh
        MOVWF (PMCON2 & 0x7F)  ;write AAh
    #endasm
    WR = 1;
    #asm //mandatory wait
        NOP
        NOP
    #endasm
    
    //load latches
    #asm //load starting address
        BANKSEL (__settings_asm_FlashLocationH)
        MOVF (__settings_asm_FlashLocationH & 0x7F), W
        BANKSEL PMADRH
        MOVWF (PMADRH & 0x7F)
        BANKSEL (__settings_asm_FlashLocationL)
        MOVF (__settings_asm_FlashLocationL & 0x7F), W
        BANKSEL PMADRH
        MOVWF (PMADRL & 0x7F)
    #endasm
    LWLO = 1; //only load write latches
    for (int i=0; i<_SETTINGS_FLASH_RAW_BYTES; i+=2) {
        if ((i+0)<_SETTINGS_STORE_BYTES) { _settings_asm_FlashDataL = newProgram[i+0]; } else { _settings_asm_FlashDataL = 0; }
        if ((i+1)<_SETTINGS_STORE_BYTES) { _settings_asm_FlashDataH = newProgram[i+1]; } else { _settings_asm_FlashDataH = 0; }
        #asm //load data
            BANKSEL (__settings_asm_FlashDataL)
            MOVF (__settings_asm_FlashDataL & 0x7F), W
            BANKSEL PMDATL
            MOVWF (PMDATL & 0x7F)
            BANKSEL (__settings_asm_FlashDataH)
            MOVF (__settings_asm_FlashDataH & 0x7F), W
            BANKSEL PMDATH
            MOVWF (PMDATH & 0x7F)
        #endasm
        if (i+2>=_SETTINGS_FLASH_RAW_BYTES) { continue; } //we are on last word so there is no need to write it
        #asm //unlock sequence
            BANKSEL PMCON2
            MOVLW 055h
            MOVWF (PMCON2 & 0x7F)  ;write 55h
            MOVLW 0AAh
            MOVWF (PMCON2 & 0x7F)  ;write AAh
        #endasm
        WR = 1; //begin write
        #asm //mandatory wait
            NOP
            NOP
        #endasm
        #asm //increase pointer
            BANKSEL PMADRL
            INCF (PMADRL & 0x7F), F
        #endasm
    }

    //write to flash
    LWLO = 0; //memory write
    #asm //unlock sequence
        BANKSEL PMCON2
        MOVLW 055h
        MOVWF (PMCON2 & 0x7F)  ;write 55h
        MOVLW 0AAh
        MOVWF (PMCON2 & 0x7F)  ;write AAh
    #endasm
    WR = 1; //begin write
    #asm //mandatory wait
        NOP
        NOP
    #endasm

    WREN = 0; //disable write to memory
    if (_SETTINGS_REENABLE_GIE_AFTER_WRITE) { GIE = 1; }
}


#define _SETTINGS_INDEX_MEASURE_INDEX 0

unsigned char  settings_getMeasureIndex() {
    return _SETTINGS_FLASH[_SETTINGS_INDEX_MEASURE_INDEX] % 4;
}

void settings_setMeasureIndex(char data) {
    _settings_write(_SETTINGS_INDEX_MEASURE_INDEX, data);
}


#define _SETTINGS_INDEX_MEASURE_UNIT 1

unsigned char  settings_getMeasureUnit() {
    return _SETTINGS_FLASH[_SETTINGS_INDEX_MEASURE_UNIT] % 3;
}

void settings_setMeasureUnit(char data) {
    _settings_write(_SETTINGS_INDEX_MEASURE_UNIT, data);
}
