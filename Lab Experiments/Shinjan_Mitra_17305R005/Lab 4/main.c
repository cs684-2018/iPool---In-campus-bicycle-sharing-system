#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"

#define DELAY_MS(x) SysCtlDelay(6700000/500*x)
#define DELAY_US(x) SysCtlDelay(6700000/500000*x)

char message[10];
char xdelim[2] = {88,120};
char ydelim[2] = {89,121};
int i;
volatile uint32_t ui32JoyStickValue[2];

void sendCoordsViaUART(uint32_t x, uint32_t y){
    i=0;
    sprintf(message, "%c%c%c%c%c%c",xdelim[0], xdelim[1], x, ydelim[0], ydelim[1], y);
    while(message[i]!='\0'){
        UARTCharPut(UART0_BASE, message[i++]);
    }

}
int main(void){
    uint32_t ui32ADC1Value[4];


    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);

    // Peripheral enable
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC1);

    // Pin configure
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_4);
    GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, 16);

    //ADC stuff
    ADCSequenceConfigure(ADC1_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC1_BASE, 1, 0, ADC_CTL_CH7|ADC_CTL_IE);
    ADCSequenceStepConfigure(ADC1_BASE, 1, 1, ADC_CTL_CH6|ADC_CTL_IE|ADC_CTL_END);
    ADCSequenceEnable(ADC1_BASE, 1);

    //UART stuff
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 9600, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    while(1)
    {
        ADCIntClear(ADC1_BASE, 1);
        ADCProcessorTrigger(ADC1_BASE, 1);

        while(!ADCIntStatus(ADC1_BASE, 1, false))
        {
        }
        ADCSequenceDataGet(ADC1_BASE, 1, ui32ADC1Value);
        ui32JoyStickValue[0] = ui32ADC1Value[0]/32;
        ui32JoyStickValue[1] = 127 - ui32ADC1Value[1]/32;
        sendCoordsViaUART(ui32JoyStickValue[0], ui32JoyStickValue[1]);

        DELAY_MS(10);

    }
}
