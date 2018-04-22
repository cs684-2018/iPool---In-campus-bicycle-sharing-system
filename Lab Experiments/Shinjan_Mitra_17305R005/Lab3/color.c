
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/debug.h"
#include "driverlib/pwm.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/pin_map.h"
#include "inc/hw_gpio.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "switches.h"
#define DELAY_MS(x) (SysCtlDelay(6700000/500*x))
#define DELAY_US(x) (SysCtlDelay(6700000/500000*x))
#define PWM_FREQUENCY 55

//Datatypes
enum operationMode{Auto,Manual};
enum manualMode{mode1,mode2,mode3};
typedef struct {
    uint32_t r;
    uint32_t g;
    uint32_t b;
} pixel;

//Global variables
pixel ledColor ={255,1,1};
int i=999;
uint8_t transitionDelay=1;
uint8_t ui8PinData=2;
uint32_t ui32Period;
enum operationMode opMode = Auto,prevOpMode = Auto; //Initialized to Auto mode
enum manualMode mMode = mode1;
void switch1PressAction(sw* s);
void switch2PressAction(sw* s);
void switch1LongPressAction(sw* s);
void switch2LongPressAction(sw* s);
void dummy(sw* s);
void changeManualMode(enum manualMode newMode);

sw sw1 = {1,GPIO_PORTF_BASE,GPIO_PIN_4,idle,0,false,0,&switch1PressAction,&switch1LongPressAction,&dummy};
sw sw2 = {2,GPIO_PORTF_BASE,GPIO_PIN_0,idle,0,false,0,&switch2PressAction,&switch2LongPressAction,&dummy};
void changeManualMode(enum manualMode newMode){
    mMode = newMode;
    sw1.swStatus = sw1.swStatus%2;
//    sw2.swStatus = 0;
}

void switch1PressAction(sw* s){
    switch(opMode){
    case Auto:
        transitionDelay= (transitionDelay>1) ? (transitionDelay-1) : transitionDelay;
        if(sw2.swState != idle) //SW2 is also pressed
            opMode = Manual;
        break;

    case Manual:
        // Change modes
        if(sw2.swState == longRelease && sw1.swStatus==1)
            changeManualMode(mode1);
        else if(sw2.swState == longRelease && sw1.swStatus==2)
            changeManualMode(mode2);

        switch(mMode){
        case mode1: ledColor.r= (ledColor.r>100)? ledColor.r-100 : ledColor.r;
        break;
        case mode2: ledColor.b= (ledColor.b>100)? ledColor.b-100 : ledColor.b;
        break;
        case mode3: ledColor.g= (ledColor.g>100)? ledColor.g-100 : ledColor.g;
        break;
        }
        break;


    }
}
void switch2PressAction(sw* s){
    switch(opMode){
    case Auto:
        transitionDelay= (transitionDelay<254) ? (transitionDelay+1) : transitionDelay;
        if(sw1.swState != idle) //SW2 is also pressed
            opMode = Manual;
        break;

    case Manual:
        switch(mMode){
        case mode1: ledColor.r= (ledColor.r<900)? ledColor.r+100 : ledColor.r;
        break;
        case mode2: ledColor.b= (ledColor.b<900)? ledColor.b+100 : ledColor.b;
        break;
        case mode3: ledColor.g= (ledColor.g<900)? ledColor.g+100 : ledColor.g;
        break;
        }
//        i= (i<900)? i+100 : i;
        break;
    }
}
void switch1LongPressAction(sw* s){
    sw1.swStatus=0;
    sw2.swStatus=0;
    switch(opMode){
    case Auto:
        break;
    case Manual:
        if(sw2.swState == longRelease )
            changeManualMode(mode3);
        break;

    }
}
void switch2LongPressAction(sw* s){
    sw1.swStatus=0;
    sw2.swStatus=0;
    switch(opMode){
    case Auto:
        break;
    case Manual:
        if(sw1.swState == longRelease )
            changeManualMode(mode3);
        break;
    }
}
void dummy(sw* s){
    return 0;
}


void pin_config(void)
{

    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;

    ROM_GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0, GPIO_DIR_MODE_IN);
    ROM_GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

}
void timer_config(void){

    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);

    ui32Period = (SysCtlClockGet() / 10) / 2;
    TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period -1);

    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    IntMasterEnable();
    TimerEnable(TIMER0_BASE, TIMER_A);
}

bool opModeChanged(){
    if(prevOpMode != opMode){
        prevOpMode = opMode;
        return true;
    }
    else
        return false;
}

void color(){

    volatile uint32_t ui32Load;
    volatile uint32_t ui32PWMClock;
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
    ROM_SysCtlPWMClockSet(SYSCTL_PWMDIV_64);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    ROM_GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
    ROM_GPIOPinConfigure(GPIO_PF1_M1PWM5);
    ROM_GPIOPinConfigure(GPIO_PF2_M1PWM6);
    ROM_GPIOPinConfigure(GPIO_PF3_M1PWM7);

    timer_config();
    pin_config();

    ui32PWMClock = SysCtlClockGet() / 64;
    ui32Load = (ui32PWMClock / PWM_FREQUENCY) - 1;

    // Pin R
    PWMGenConfigure(PWM1_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN);
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, ui32Load);
    ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, 999 * ui32Load / 1000);
    ROM_PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT, true);
    ROM_PWMGenEnable(PWM1_BASE, PWM_GEN_2);

    // Pin G
    PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN);
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, ui32Load);
    ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, 1 * ui32Load / 1000);
    ROM_PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT, true);
    ROM_PWMGenEnable(PWM1_BASE, PWM_GEN_3);


    // Pin B
    ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, 1 * ui32Load / 1000);
    ROM_PWMOutputState(PWM1_BASE, PWM_OUT_7_BIT, true);

    while(1)
    {
        switch(opMode){
        case Auto:
            // Auto mode
            for(ledColor.g=1;ledColor.g<999;ledColor.g++){
                // Increase G
                if(opModeChanged())goto END_AUTO;
                ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, ledColor.g * ui32Load / 1000);
                DELAY_MS(transitionDelay);
            }
            for(ledColor.r=999;ledColor.r>1;ledColor.r--){
                // Decrease R
                if(opModeChanged())goto END_AUTO;
                ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, ledColor.r* ui32Load / 1000);
                DELAY_MS(transitionDelay);
            }
            for(ledColor.b=1;ledColor.b<999;ledColor.b++){
                // Increase B
                if(opModeChanged())goto END_AUTO;
                ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, ledColor.b* ui32Load / 1000);
                DELAY_MS(transitionDelay);
            }
            for(ledColor.g=999;ledColor.g>1;ledColor.g--){
                // Decrease G
                if(opModeChanged())goto END_AUTO;
                ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, ledColor.g * ui32Load / 1000);
                DELAY_MS(transitionDelay);
            }
            for(ledColor.r=1;ledColor.r<999;ledColor.r++){
                // Increase R
                if(opModeChanged())goto END_AUTO;
                ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, ledColor.r* ui32Load / 1000);
                DELAY_MS(transitionDelay);
            }
            for(ledColor.b=999;ledColor.b>1;ledColor.b--){
                // Decrease B
                if(opModeChanged())goto END_AUTO;
                ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, ledColor.b* ui32Load / 1000);
                DELAY_MS(transitionDelay);
            }
            END_AUTO:
            break;

        case Manual:
            switch(mMode){
                case mode1: // Red
                    ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, ledColor.r * ui32Load / 1000);
                    break;
                case mode2: // Blue
                    ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, ledColor.b * ui32Load / 1000);
                    break;
                case mode3: // Green
                    ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, ledColor.g * ui32Load / 1000);
                    break;
            }
            break;
        }


    }
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

