#include "settings.h"

#include "pic.h"


#define FLASH_RAW { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } //reserving space because erase block is block 32-word
#define FLASH_RAW_LOCATION 0x0400
#define FLASH_RAW_COUNT 32

#define STORE_EMPTY { 0, 0 }
#define STORE_COUNT 2

#define OFFSET_MEASURE_INDEX 0
#define OFFSET_MEASURE_UNIT  1

const unsigned char FLASH[] @ FLASH_RAW_LOCATION = FLASH_RAW;


void settings_write(int index, unsigned char data) {
    unsigned char newProgram[] = STORE_EMPTY;
    for (int i=0; i<STORE_COUNT; i++) {
        newProgram[i] = FLASH[i];
    }
    newProgram[index] = data;

    GIE = 0; //disable interrupts
    WREN = 1; //enable write to memory
    CFGS = 0; //access Flash program memory

    //erase row
    PMADR = FLASH_RAW_LOCATION;
    FREE = 1; //enable block Erase operation
    #asm //unlock flash
        BANKSEL PMCON2
        MOVLW 055h
        MOVWF (PMCON2 & 0x7F)   ;write 55h
        MOVLW 0AAh
        MOVWF (PMCON2 & 0x7F)   ;write AAh
        BSF (PMCON1 & 0x7F), 1  ;WR=1
        NOP
        NOP
    #endasm

    //write data
    LWLO = 0; //immediate write
    for (int i=0; i<STORE_COUNT; i++) {
        PMADR = FLASH_RAW_LOCATION + i;
        PMDATH = 0;
        PMDATL = newProgram[i];
        #asm //unlock flash
            BANKSEL PMCON2
            MOVLW 055h
            MOVWF (PMCON2 & 0x7F)   ;write 55h
            MOVLW 0AAh
            MOVWF (PMCON2 & 0x7F)   ;write AAh
            BSF (PMCON1 & 0x7F), 1  ;WR=1
            NOP
            NOP
        #endasm
    }

    WREN = 0; //disable write to memory
    GIE = 1; //enable interrupts
}



unsigned char  getMeasureIndex() {
    return FLASH[OFFSET_MEASURE_INDEX] % 4;
}


void setMeasureIndex(unsigned char data) {
    settings_write(OFFSET_MEASURE_INDEX, data);
}


unsigned char  getMeasureUnit() {
    return FLASH[OFFSET_MEASURE_UNIT] % 3;
}


void setMeasureUnit(unsigned char data) {
    settings_write(OFFSET_MEASURE_UNIT, data);
}
