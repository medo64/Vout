#pragma config FOSC    = INTIO67                                                //Internal oscillator block, port function on RA6 and RA7
#pragma config FCMEN   = OFF                                                    //Fail-Safe Clock Monitor disabled
#pragma config IESO    = OFF                                                    //Oscillator Switchover mode disabled

#pragma config PWRT    = ON                                                     //PWRT enabled
#pragma config BOREN   = ON                                                     //Brown-out Reset enabled and controlled by software (SBOREN is enabled)
#pragma config BORV    = 18                                                     //VBOR set to 1.8 V nominal

#pragma config WDTEN   = OFF                                                    //WDT is controlled by SWDTEN bit of the WDTCON register
#pragma config WDTPS   = 32768                                                  //1:32768

#pragma config CCP2MX  = PORTC                                                  //CCP2 input/output is multiplexed with RC1
#pragma config PBADEN  = ON                                                     //PORTB<4:0> pins are configured as analog input channels on Reset
#pragma config LPT1OSC = OFF                                                    //Timer1 configured for higher power operation
#pragma config HFOFST  = OFF                                                    //The system clock is held off until the HFINTOSC is stable.
#pragma config MCLRE   = OFF                                                    //RE3 input pin enabled; MCLR disabled

#pragma config STVREN  = ON                                                     //Stack full/underflow will cause Reset
#pragma config LVP     = OFF                                                    //Single-Supply ICSP disabled
#pragma config XINST   = OFF                                                    //Instruction set extension and Indexed Addressing mode disabled (Legacy mode)

#pragma config CP0     = OFF                                                    //Block 0 (000200-000FFFh) not code-protected
#pragma config CP1     = OFF                                                    //Block 1 (001000-001FFFh) not code-protected

#pragma config CPB     = OFF                                                    //Boot block (000000-0001FFh) not code-protected
#pragma config CPD     = OFF                                                    //Data EEPROM not code-protected

#pragma config WRT0    = OFF                                                    //Block 0 (000200-000FFFh) not write-protected
#pragma config WRT1    = OFF                                                    //Block 1 (001000-001FFFh) not write-protected

#pragma config WRTC    = OFF                                                    //Configuration registers (300000-3000FFh) not write-protected
#pragma config WRTB    = OFF                                                    //Boot Block (000000-0001FFh) not write-protected
#pragma config WRTD    = OFF                                                    //Data EEPROM not write-protected

#pragma config EBTR0   = OFF                                                    //Block 0 (000200-000FFFh) not protected from table reads executed in other blocks
#pragma config EBTR1   = OFF                                                    //Block 1 (001000-001FFFh) not protected from table reads executed in other blocks

#pragma config EBTRB   = OFF                                                    //Boot Block (000000-0001FFh) not protected from table reads executed in other blocks
