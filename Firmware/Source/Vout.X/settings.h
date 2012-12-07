#define _SETTINGS_FLASH_RAW { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } //reserving space because erase block is block 32-word (64-bytes)
#define _SETTINGS_FLASH_LOCATION 0x0400

#define _SETTINGS_STORE_EMPTY { 0, 0 }
#define _SETTINGS_STORE_BYTES 2

#define _SETTINGS_REENABLE_GIE_AFTER_WRITE     1

const unsigned char _SETTINGS_FLASH[] @ _SETTINGS_FLASH_LOCATION = _SETTINGS_FLASH_RAW;

void _settings_write(int index, unsigned char data) {
    unsigned char newProgram[] = _SETTINGS_STORE_EMPTY;
    for (int i=0; i<_SETTINGS_STORE_BYTES; i++) {
        newProgram[i] = _SETTINGS_FLASH[i];
    }
    newProgram[index] = data;

    GIE = 0;

    EEPGD = 1; //point to Flash program memory
    CFGS = 0; //access Flash program memory
    WREN = 1; //enable write to memory

    TBLPTR = _SETTINGS_FLASH_LOCATION;
    FREE = 1; //enable block Erase operation
    #asm //erase block
        MOVLW 55h
        MOVWF EECON2 ; write 55h
        MOVLW 0AAh
        MOVWF EECON2 ; write 0AAh
    #endasm
    WR = 1; //start erase (CPU stall)

    TBLPTR = _SETTINGS_FLASH_LOCATION;
    for (int i=0; i<_SETTINGS_STORE_BYTES; i++) {
        TABLAT = newProgram[i];
        asm("TBLWT*+");
        #asm
            MOVLW 55h
            MOVWF EECON2 ; write 55h
            MOVLW 0AAh
            MOVWF EECON2 ; write 0AAh
        #endasm
        WR = 1; //start program (CPU stall)
    }
    WREN = 0; //disable write to memory

    if (_SETTINGS_REENABLE_GIE_AFTER_WRITE) { GIE = 1; }
}


#define _SETTINGS_INDEX_MEASURE_INDEX 0

char  settings_getMeasureIndex() {
    return _SETTINGS_FLASH[_SETTINGS_INDEX_MEASURE_INDEX] % 4;
}

void settings_setMeasureIndex(char data) {
    _settings_write(_SETTINGS_INDEX_MEASURE_INDEX, data);
}


#define _SETTINGS_INDEX_MEASURE_UNIT 1

char  settings_getMeasureUnit() {
    return _SETTINGS_FLASH[_SETTINGS_INDEX_MEASURE_UNIT] % 3;
}

void settings_setMeasureUnit(char data) {
    _settings_write(_SETTINGS_INDEX_MEASURE_UNIT, data);
}
