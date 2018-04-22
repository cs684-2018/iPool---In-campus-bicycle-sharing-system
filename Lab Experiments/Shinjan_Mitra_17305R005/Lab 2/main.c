#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "inc/hw_gpio.h"
#include "switches.h"
//#include "inc/hw_ints.h"
/**
 * main.c
 */
#define DELAY_MS(x) (SysCtlDelay(6700000/500*x))
#define DELAY_US(x) (SysCtlDelay(6700000/500000*x))

uint8_t ui8PinData=2;

void switch1PressAction(sw* s){
    if(ui8PinData==8) {ui8PinData=2;} else {ui8PinData=ui8PinData*2;}
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ui8PinData);

}
void dummy(sw* s){

}
void switch1LongPressAction(sw* s){

    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ui8PinData);
//    Delay for a bit
    DELAY_MS(500);
//    Turn off LED
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);
//    Delay
    DELAY_MS(500);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ui8PinData);
}

void switch2PressAction(sw* s){
    s->swStatus++;
}
sw sw1 = {1,GPIO_PORTF_BASE,GPIO_PIN_4,idle,0,false,0,&switch1PressAction,&dummy,&switch1PressAction};
sw sw2 = {2,GPIO_PORTF_BASE,GPIO_PIN_0,idle,0,false,0,&switch2PressAction,&dummy,&switch2PressAction};

void pin_config(void)
{

    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;   // unlock the GPIOCR register for port F
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) = 0x01;      // Free up pin 0
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK)= 0 ;

    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0|GPIO_PIN_4);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0|GPIO_PIN_4, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);

}
int main(void)
{
    uint32_t ui32Period;
    sw1.swState = idle;

    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    pin_config();

    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);

    ui32Period = (SysCtlClockGet() / 10) / 2;
    TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period -1);

    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    IntMasterEnable();
    TimerEnable(TIMER0_BASE, TIMER_A);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ui8PinData);
    while(1){
        // Turn on the LED
//        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ui8PinData);
        // Delay for a bit
//        DELAY_MS(500);
        // Turn off LED
//        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);
        // Delay
//        DELAY_MS(500);

    }
    return 0;
}


void Timer0IntHandler(void)
{
    // Clear the timer interrupt
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

//    Change SW1 state after looking at key press
    transition(&sw1);

//    Change SW2 state after looking at key press
    transition(&sw2);
}
