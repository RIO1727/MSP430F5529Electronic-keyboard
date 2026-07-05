#include "system.h"

void SetClock_8MHz(void)
{
    UCSCTL3 = SELREF_2;
    UCSCTL4 = SELA__REFOCLK | SELS__DCOCLK | SELM__DCOCLK;

    __bis_SR_register(SCG0);
    UCSCTL0 = 0x0000;
    UCSCTL1 = DCORSEL_5;
    UCSCTL2 = FLLD_0 + 243;
    __bic_SR_register(SCG0);

    __delay_cycles(250000);
}

// 20msÏû¶¶ÑÓÊ± (ÊÊÅä8MHz)
void delay_debounce(void)
{
    unsigned int i;
    for(i = 20000; i > 0; i--);
}
