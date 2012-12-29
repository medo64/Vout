#ifndef BUTTONS_H
#define	BUTTONS_H


bit isButtonNextPressed();
bit isButtonUnitPressed();

bit isButtonNextReleased();
bit isButtonUnitReleased();

void buttonNextDetect();
void buttonUnitDetect();

void buttonNextReset();
void buttonUnitReset();

#endif
