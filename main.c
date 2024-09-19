#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"

void PortF_INIT(void)
{
    SYSCTL_RCGCGPIO_R |= 0x20;                                       // This enables clock for the GPIO Port F Register
    while ((SYSCTL_PRGPIO_R & 0x20) == 0) {};
    GPIO_PORTF_LOCK_R = 0x4C4F434B;                                 // This unlocks Port F for working
    GPIO_PORTF_CR_R = 0xFF;                                         // This allows changing control registers of specific GPIO pins
    GPIO_PORTF_DIR_R = 0x0E;                                        // This sets input or output for each pin
    GPIO_PORTF_PUR_R = 0x11;                                        // This sets pull up resistors for each pin
    GPIO_PORTF_DEN_R = 0x1F;                                        // This enables each pin

    GPIO_PORTF_IM_R &= ~0x11;                                       // This sets mask so that control registers for interrupts can be changed
    GPIO_PORTF_IS_R &= ~0x11;                                       // This clears IS PF0, PF4 for enabling edge detection
    GPIO_PORTF_IEV_R |= 0x11;                                       // This sets IEV PF0, PF4 for rising edge detection
    GPIO_PORTF_ICR_R = 0x11;                                        // This sets ICR PF0, PF4 to clear interrupts for it
    GPIO_PORTF_IM_R |= 0x11;                                        // This un-masks the interrupts again

    NVIC_EN0_R = (1 << 30);
}

void GPIOPortFHandler(void){
    if (GPIO_PORTF_RIS_R & 0x01) {                                  // Check if interrupt came from PF0
        GPIO_PORTF_ICR_R = 0x01;                                    // Clear interrupt flag for PF0
        GPIO_PORTF_DATA_R ^= 0x08;                                  // This toggles data in Port F Pin 3
    }
    if (GPIO_PORTF_RIS_R & 0x10) {                                  // Check if interrupt came from PF4
        GPIO_PORTF_ICR_R = 0x10;                                    // Clear interrupt flag for PF4
        GPIO_PORTF_DATA_R ^= 0x08;                                  // This toggles data in Port F Pin 3
    }
}

int main(void)
{
    PortF_INIT();                                                   // This calls Port F initialization
    while (1){
        ;
    }
	return 0;
}



