/*

 * Author: Texas Instruments

 * Editted by: Parin Chheda
          ERTS Lab, CSE Department, IIT Bombay

 * Description: This code structure will assist you in completing Lab 1
 * Filename: lab-1.c

 * Functions: setup(), config(), main()


 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/hw_ints.h"
#include <time.h>

#define DELAY_HALF_SEC 6700000
//Globals for part 1
uint8_t ui8PinData=2;
bool SW1Activation=0, SW1PreviousActivation=0;
uint8_t toggleDelayFactor=1;

// Globals for part 2

bool SW2Activation=0, SW2PreviousActivation=0;
uint8_t SW2Status=0;

//Globals for part 3
bool LEDON=0;

/*

 * Function Name: setup()

 * Input: none

 * Output: none

 * Description: Set crystal frequency,enable GPIO Peripherals and unlock Port F pin 0 (PF0)

 * Example Call: setup();

 */
void setup(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    //unlock PF0 based on requirement

}

/*

 * Function Name: pin_config_part_1()

 * Input: none

 * Output: none

 * Description: Set Port F Pin 1, Pin 2, Pin 3 as output. On this pin Red, Blue and Green LEDs are connected.
			   Set Port F Pin 0 and 4 as input, enable pull up on both these pins.

 * Example Call: pin_config();

 */

void pin_config_part_1(void)
{

    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);

}
/*

 * Function Name: pin_config_part_2()

 * Input: none

 * Output: none

 * Description: Set Port F Pin 1, Pin 2, Pin 3 as output. On this pin Red, Blue and Green LEDs are connected.
               Set Port F Pin 0 and 4 as input, enable pull up on both these pins.

 * Example Call: pin_config();

 */

void pin_config_part_2(void)
{
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;   // unlock the GPIOCR register for port F
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) = 0x01;      // Free up pin 0
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK)= 0 ;

    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);

}
void pin_config_part_3(void)
{
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;   // unlock the GPIOCR register for port F
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) = 0x01;      // Free up pin 0
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK)= 0 ;

    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0|GPIO_PIN_4);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0|GPIO_PIN_4, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);

}

void part_1(void){

    setup();
    pin_config_part_1();

    while(1)
    {
        /*
        Refer to helper document for writing logic for different parts of the lab-1 problem statement

         */
        SW1PreviousActivation = SW1Activation;
        SW1Activation = !GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4); // Since SW1 is pull-up, negation of value is status, i.e. if pin in HIGH then switch is not pressed

        if(SW1Activation && !SW1PreviousActivation){
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1| GPIO_PIN_2| GPIO_PIN_3, ui8PinData);
        }
        else if(!SW1Activation && SW1PreviousActivation){
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x00);
            if(ui8PinData==8) {ui8PinData=2;} else {ui8PinData=ui8PinData*2;}
        }

    }
}

void part_2(void){
    setup();
    pin_config_part_2();

    while(1)
    {
        /*
             Refer to helper document for writing logic for different parts of the lab-1 problem statement

         */
        SW2PreviousActivation = SW2Activation;
        SW2Activation = !GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0); // Since SW2 is pull-up, negation of value is status, i.e. if pin in HIGH then switch is not pressed

        if(SW2Activation && !SW2PreviousActivation){
            SW2Status++;
        }

    }
}
void part_3(void){
    setup();
    pin_config_part_3();

    while(1)
    {
        /*
             Refer to helper document for writing logic for different parts of the lab-1 problem statement

         */
        if(!LEDON){
            //            Turn in on now
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1| GPIO_PIN_2| GPIO_PIN_3, ui8PinData);

        }
        else{
            //            Turn it off now
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x00);
        }
        LEDON=!LEDON;
        int delay = toggleDelayFactor*DELAY_HALF_SEC;
        int i;
        for(i=0;i<delay;i+=1000){
            //         Read switch values
            SW1PreviousActivation = SW1Activation;
            SW1Activation = !GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4); // Since SW1 is pull-up, negation of value is status, i.e. if pin in HIGH then switch is not pressed
            SW2PreviousActivation = SW2Activation;
            SW2Activation = !GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0); // Since SW2 is pull-up, negation of value is status, i.e. if pin in HIGH then switch is not pressed

            //         Adjust factors
            if(SW1Activation && !SW1PreviousActivation){
                if(toggleDelayFactor==4) {toggleDelayFactor=1;} else {toggleDelayFactor=toggleDelayFactor*2;}
            }
            if(SW2Activation && !SW2PreviousActivation){
                if(ui8PinData==8) {ui8PinData=2;} else {ui8PinData=ui8PinData*2;}
            }
            SysCtlDelay(1000);
        }





    }
}
int main(void)
{

    part_3();

}
