#define _SETTINGS_FLASH_RAW { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } //reserving space because erase block is block 32-word
#define _SETTINGS_FLASH_RAW_LOCATION 0x0400
#define _SETTINGS_FLASH_RAW_COUNT 32

#define _SETTINGS_STORE_EMPTY { 0, 0 }
#define _SETTINGS_STORE_COUNT 2

#define _SETTINGS_REENABLE_GIE_AFTER_WRITE     1

const unsigned char _SETTINGS_FLASH[] @ _SETTINGS_FLASH_RAW_LOCATION = _SETTINGS_FLASH_RAW;

unsigned char _settings_asm_FlashLocationH = (_SETTINGS_FLASH_RAW_LOCATION >> 8) && 0xFF;
unsigned char _settings_asm_FlashLocationL =  _SETTINGS_FLASH_RAW_LOCATION && 0xFF;
unsigned char _settings_asm_FlashDataH = 0;
unsigned char _settings_asm_FlashDataL = 0;


void _settings_write(int index, unsigned char data) {
    unsigned char newProgram[] = _SETTINGS_STORE_EMPTY;
    for (int i=0; i<_SETTINGS_STORE_COUNT; i++) {
        newProgram[i] = _SETTINGS_FLASH[i];
    }
    newProgram[index] = data;

    GIE = 0;

    //erase row
    PMADR = _SETTINGS_FLASH_RAW_LOCATION;
    CFGS = 0; //access Flash program memory
    FREE = 1; //enable block Erase operation
    WREN = 1; //enable write to memory

    #asm //unlock flash
        BANKSEL PMCON2
        MOVLW 055h
        MOVWF (PMCON2 & 0x7F)  ;write 55h
        MOVLW 0AAh
        MOVWF (PMCON2 & 0x7F)  ;write AAh
        BSF (PMCON1 & 0x7F), 1  ;WR
        NOP
        NOP
    #endasm

    for (int i=0; i<_SETTINGS_STORE_COUNT; i++) {
        //write data
        WREN = 1; //enable write to memory
        CFGS = 0; //access Flash program memory
        LWLO = 0; //write directly
        PMADR = _SETTINGS_FLASH_RAW_LOCATION + i;
        PMDATH = 0;
        PMDATL = newProgram[i];

        #asm //unlock flash
            BANKSEL PMCON2
            MOVLW 055h
            MOVWF (PMCON2 & 0x7F)  ;write 55h
            MOVLW 0AAh
            MOVWF (PMCON2 & 0x7F)  ;write AAh
            BSF (PMCON1 & 0x7F), 1  ;WR
            NOP
            NOP
        #endasm
    }

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
