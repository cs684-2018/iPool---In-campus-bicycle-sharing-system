/*
 * Lab 5 : Sample Code
 * Course : CS-684
 * Created on: Jan 27, 2018
 * Author: Lab staff ERTS LAB
 * This sample code will help you in interfacing glcd
 */

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/debug.h"
#include "driverlib/pin_map.h"
#include "inc/hw_gpio.h"
#include "driverlib/rom.h"
#include "driverlib/adc.h"
#include "glcd.h"
#include "logo.h"
#include "mickey.h"
#include "leopard.h"

#define DELAY_MS(x) SysCtlDelay(6700000/500*x)

volatile uint32_t ui32JoyStickValue[2];
uint32_t ui32ADC0Value[4];
uint32_t left_right_val[4];
uint32_t up_down_val[4];

volatile uint32_t position_avg[2];
volatile uint32_t temp[2];
uint32_t prev_x=0,prev_y=0;
void display_image(const unsigned char* image){
    int j=0,p=0,i=0;
    while(p<8){
        // Set the page
        glcd_setpage(p);

        for(i=0;i<128;i++){
            // Select columns from 0 to 127
            glcd_setcolumn(i);

            // Send the hex values to GLCD
            glcd_data(image[j]);
            j++;
        }

        // Increment page number
        p++;
    }
}
void display_box(uint32_t x, uint32_t y){

    int i=0;
    if(x== prev_x && y == prev_y) return;

    // clear prev box
    glcd_setpage(prev_y);
    glcd_setcolumn(prev_x); //Scale between 0 to 120 not 128

    //Clear 8x8 box
    for(i=0;i<8;i++){
        glcd_setcolumn(prev_x+i); //Scale between 0 to 120 not 128
        glcd_data(0x00);
    }
    glcd_setpage(y);


    //Print 8x8 box
    for(i=0;i<8;i++){
        glcd_setcolumn(x+i); //Scale between 0 to 120 not 128
        glcd_data(0xFF);
    }
    prev_x=x;prev_y=y;
}
void pin_config(){

    SysCtlClockSet(SYSCTL_SYSDIV_2|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_25MHZ);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

     SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
     SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC1);

     ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);
     ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_CH6);
     ADCSequenceStepConfigure(ADC0_BASE, 1, 1, ADC_CTL_CH6);
     ADCSequenceStepConfigure(ADC0_BASE, 1, 2, ADC_CTL_CH6);
     ADCSequenceStepConfigure(ADC0_BASE,1,3,ADC_CTL_CH6|ADC_CTL_IE|ADC_CTL_END);
     ADCSequenceEnable(ADC0_BASE, 1);

     ADCSequenceConfigure(ADC1_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);
     ADCSequenceStepConfigure(ADC1_BASE, 1, 0, ADC_CTL_CH7);
     ADCSequenceStepConfigure(ADC1_BASE, 1, 1, ADC_CTL_CH7);
     ADCSequenceStepConfigure(ADC1_BASE, 1, 2, ADC_CTL_CH7);
     ADCSequenceStepConfigure(ADC1_BASE,1,3,ADC_CTL_CH7|ADC_CTL_IE|ADC_CTL_END);
     ADCSequenceEnable(ADC1_BASE, 1);

    /* Unlock pin PF0 */
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK)= GPIO_LOCK_KEY;    // unlocking sw2 switch
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK)= 0;

    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_4); //This is disabling the speaker
    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, 0x10);

    /* Configure PE5 (RST), PE0 to PE3 (D0 to D3) and PB4 to PB7(D4 to D7) as output pins */
    //GLCD D0-D3
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    //GLCD D4-D7
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);

    /* Configure RS as output */
    //GLCD RS
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_6);
    //GLCD Enable PIN
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0);

    /* Configure CS1 or CS2 as output */
    //GLCD CS
    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_3);
    //GLCD Reset
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_5);

    /*This ensures buzzer remains OFF, since PC4 when logic 0 turns ON buzzer */
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_4);
    GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_4,16);

}
void part1(){
    pin_config();
    display_image(mickey);
}
void part2(){
    pin_config();
    //ADC stuff
//    ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);
//    ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_CH7|ADC_CTL_IE);
//    ADCSequenceStepConfigure(ADC0_BASE, 1, 1, ADC_CTL_CH6|ADC_CTL_IE|ADC_CTL_END);
//    ADCSequenceEnable(ADC0_BASE, 1);

    /*initialize glcd*/
    glcd_init();
    glcd_cleardisplay();


    while(1)
    {


        ADCIntClear(ADC0_BASE, 1);
        ADCProcessorTrigger(ADC0_BASE, 1);
        ADCIntClear(ADC1_BASE, 1);
        ADCProcessorTrigger(ADC1_BASE, 1);
        while(!ADCIntStatus(ADC0_BASE, 1, false))
        {
        }
        while(!ADCIntStatus(ADC1_BASE, 1, false))
        {
        }
        ADCSequenceDataGet(ADC0_BASE, 1, left_right_val);
        ADCSequenceDataGet(ADC1_BASE, 1, up_down_val);

        position_avg[0] = (left_right_val[0] + left_right_val[1] + left_right_val[2] + left_right_val[3] + 2)/4;
        temp[0]=position_avg[0];
        position_avg[1] = (up_down_val[0] + up_down_val[1] + up_down_val[2] + up_down_val[3] + 2)/4;
        temp[1]=position_avg[1];

        ui32JoyStickValue[0]= 120 - (temp[0] / 34) ;
        ui32JoyStickValue[1]= 7 - (temp[1] / 512);
//        ui32JoyStickValue[0] = ui32ADC0Value[1]/34;
//        ui32JoyStickValue[1] = ui32ADC0Value[0]/512;
        display_box(ui32JoyStickValue[0], ui32JoyStickValue[1]);
        DELAY_MS(10);

    }
}
void readJoystickPos(){
    ADCIntClear(ADC0_BASE, 1);
    ADCProcessorTrigger(ADC0_BASE, 1);
    ADCIntClear(ADC1_BASE, 1);
    ADCProcessorTrigger(ADC1_BASE, 1);
    while(!ADCIntStatus(ADC0_BASE, 1, false))
    {
    }
    while(!ADCIntStatus(ADC1_BASE, 1, false))
    {
    }
    ADCSequenceDataGet(ADC0_BASE, 1, left_right_val);
    ADCSequenceDataGet(ADC1_BASE, 1, up_down_val);

    position_avg[0] = (left_right_val[0] + left_right_val[1] + left_right_val[2] + left_right_val[3] + 2)/4;
    temp[0]=position_avg[0];
    position_avg[1] = (up_down_val[0] + up_down_val[1] + up_down_val[2] + up_down_val[3] + 2)/4;
    temp[1]=position_avg[1];

    ui32JoyStickValue[0]= 127 - (temp[0] / 34) ;
    ui32JoyStickValue[1]= 7 - (temp[1] / 512);
}
part3(){
    pin_config();

    while(1){
        readJoystickPos();
        display_image(one);
        DELAY_MS((ui32JoyStickValue[0]+1)*5);
//        glcd_cleardisplay();

        readJoystickPos();
        display_image(two);
        DELAY_MS((ui32JoyStickValue[0]+1)*5);
//        glcd_cleardisplay();

        readJoystickPos();
        display_image(three);
        DELAY_MS((ui32JoyStickValue[0]+1)*5);
//        glcd_cleardisplay();

        readJoystickPos();
        display_image(four);
        DELAY_MS((ui32JoyStickValue[0]+1)*5);
//        glcd_cleardisplay();

        readJoystickPos();
        display_image(five);
        DELAY_MS((ui32JoyStickValue[0]+1)*5);
//        glcd_cleardisplay();

        readJoystickPos();
        display_image(six);
        DELAY_MS((ui32JoyStickValue[0]+1)*5);
//        glcd_cleardisplay();

        readJoystickPos();
        display_image(seven);
        DELAY_MS((ui32JoyStickValue[0]+1)*5);
//        glcd_cleardisplay();

        readJoystickPos();
        display_image(eight);
        DELAY_MS((ui32JoyStickValue[0]+1)*5);
//        glcd_cleardisplay();
    }
}
int main()
{
    part3();

}
