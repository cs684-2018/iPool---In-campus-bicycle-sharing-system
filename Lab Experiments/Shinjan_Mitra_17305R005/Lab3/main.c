
/*
 * Lab 3/main.c
 *
 *  Created on: 23-Jan-2018
 *      Author: shinjan
 */

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
#define PWM_FREQUENCY 55
#define DELAY_MS(x) (SysCtlDelay(6700000/500*x))

void servo();
void color();

#define DELAY_MS(x) (SysCtlDelay(6700000/500*x))
#define DELAY_US(x) (SysCtlDelay(6700000/500000*x))


//void switch1PressAction(sw* s){
//    transitionDelay= (transitionDelay>1) ? (transitionDelay-1) : transitionDelay;
//}
//void switch2PressAction(sw* s){
//    transitionDelay = (transitionDelay<245) ? (transitionDelay+1) : transitionDelay;
//}
//void dummy(sw* s){
//    return 0;
//}
//
//sw sw1 = {1,GPIO_PORTF_BASE,GPIO_PIN_4,idle,0,false,0,&switch1PressAction,&dummy,&switch1PressAction};
//sw sw2 = {2,GPIO_PORTF_BASE,GPIO_PIN_0,idle,0,false,0,&switch2PressAction,&dummy,&switch2PressAction};

int main(void)

{
    servo();
}

