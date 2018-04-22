/*
 * switches.h
 *
 *  Created on: 24-Jan-2018
 *      Author: shinjan
 */

#ifndef SWITCHES_H_
#define SWITCHES_H_

#define LONG_PRESS_WAIT_MS 100
enum state{idle,press,release,longRelease};
struct sw;
typedef void (*swPressAction)(struct sw* s);

typedef struct {
    int id;
    uint32_t port;
    uint8_t pin;
    enum state swState;
    int swStatus;
    bool activation;
    int pressCounter;
    swPressAction pressAction;
    swPressAction longPressAction;
    swPressAction longPressReleaseAction;
} sw;

unsigned char detectKeyPress(sw* s)
{
    s->activation= !GPIOPinRead(s->port, s->pin  ); // Since SW1 is pull-up, negation of value is status, i.e. if pin in HIGH then switch is not pressed
    if(s->activation ){

        s->pressCounter = s->pressCounter ? s->pressCounter : 1;
    }
    else {
        s->pressCounter=0;
    }
    return s->pressCounter;
}

void transition(sw* s){
    switch(s->swState){
    case idle:
        if(detectKeyPress(s)){
            s->swState = press;
            s->pressCounter++;
        }
        else
            s->swState = idle;
        break;
    case press:
        if(detectKeyPress(s)){
            s->pressCounter++;
            s->swState = release;
        }
        else{
            s->swState = idle;
            s->pressAction(s);
        }
        break;
    case release:
        if(detectKeyPress(s) && s->pressCounter<LONG_PRESS_WAIT_MS/10){
            s->pressCounter++;
            s->swState = release;
        }
        else if(detectKeyPress(s) && s->pressCounter>=LONG_PRESS_WAIT_MS/10){
            s->pressCounter++;
            s->swState = longRelease;
            s->longPressAction(s);
        }
        else{
            s->swState = idle;
            s->pressAction(s);
        }
        break;
    case longRelease:
        if(detectKeyPress(s)){
            s->swState = longRelease;
        }
        else{
            s->swState = idle;
            s->longPressReleaseAction(s);
        }
        break;
    }
}
#endif /* SWITCHES_H_ */
