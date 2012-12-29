#include "buttons.h"
#include <pic.h>

#include "config.h"


#define BUTTON_COUNTER_MAX     48
#define BUTTON_COUNTER_MAX_MAX 144


volatile char ButtonCounterNext = BUTTON_COUNTER_MAX;
volatile char ButtonCounterUnit = BUTTON_COUNTER_MAX;


bit buttonNextCheck() {
    if (BUTTON_NEXT == 0) {
        if (ButtonCounterNext > 0) { ButtonCounterNext -= 1; }
        return 1;
    } else {
        buttonNextReset();
        return 0;
    }
}


bit buttonUnitCheck() {
    if (BUTTON_UNIT == 0) {
        if (ButtonCounterUnit > 0) { ButtonCounterUnit -= 1; }
        return 1;
    } else {
        buttonUnitReset();
        return 0;
    }
}


void buttonNextReset() {
    ButtonCounterNext = BUTTON_COUNTER_MAX;
}


void buttonUnitReset() {
    ButtonCounterUnit = BUTTON_COUNTER_MAX;
}


bit isButtonNextPressed() {
    return (ButtonCounterNext == 0);
}


bit isButtonUnitPressed() {
    return (ButtonCounterUnit == 0);
}


bit isButtonNextReleased() {
    return (ButtonCounterNext == BUTTON_COUNTER_MAX);
}


bit isButtonUnitReleased() {
    return (ButtonCounterUnit == BUTTON_COUNTER_MAX);
}
