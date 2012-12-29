#ifndef BUTTONS_H
#define	BUTTONS_H


bit buttonNextCheck();
bit buttonUnitCheck();

void buttonNextReset();
void buttonUnitReset();

bit isButtonNextPressed();
bit isButtonUnitPressed();

bit isButtonNextReleased();
bit isButtonUnitReleased();

#endif
