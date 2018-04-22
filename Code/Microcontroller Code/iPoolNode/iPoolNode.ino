/* *********************************************************************************
 * iPoolNode device module
 * dependency library :
 *   ESP8266 QRCode
 *   ESP8266 Oled Driver for SSD1306 display by Daniel Eichborn, Fabrice Weinberg
 *
 * SDA --> D3
 * SCL --> D5
 * PHOTXPIN --> D0
***********************************************************************************/

#define PHOTXPIN D2
#define PULSE_FREQ_HZ 20
#define READ_PER_PULSE 10
#define BITS_TO_READ 8
#define DEVICE_ID 40

#include <qrcode.h>
#include <SSD1306.h>
#include <Servo.h>

bool message[BITS_TO_READ];
bool state=false, lastState = false, readingFlag=false;
unsigned long lastRead = 0;
int counter =1, x1=0, x0=0, pos=0;
bool lockCode[8] = {1,0,0,1,1,0,0,1};
bool unlockCode[8] = {1,0,1,0,1,0,1,0};


SSD1306  display(0x3c, D3, D5);
QRcode qrcode (&display);
Servo myservo;  // create servo object to control a servo


void setup() {
  Serial.begin(9600);
  Serial.println("");
  Serial.println("Starting...");

//  attachInterrupt(digitalPinToInterrupt(PHOTXPIN), countBits, CHANGE);
  pinMode(PHOTXPIN,INPUT);
  myservo.attach(D4); 
  lock();
  display.init();
  display.clear();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_24);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 15, "iPool");
  display.display();
  lastRead = micros();
  display.setFont(ArialMT_Plain_16);
}
bool readPulse(){
  int x1=0,x0=0;
  bool x=false;
  for(int i=0;i<READ_PER_PULSE;i++){
    while(micros()-lastRead <=1000000/(PULSE_FREQ_HZ*READ_PER_PULSE));
    lastRead = micros();  
    state = bool(digitalRead(PHOTXPIN));
    Serial.print(state);
    if(state)
      x1++;
    else
      x0++;
  }
  x = x0>=x1 ? false : true;
  return x;
}
bool checkMessage(bool message[],int size){
  byte rideId =0, hash = 0;
  
  for(int i =size/2-1;i>=0;i--){
    rideId = (rideId << 1) | message[i];
  }
  for(int i =size-1;i>=size/2;i--){
    hash = (hash << 1) | message[i];
  }
  Serial.println();
  Serial.println(rideId);
  Serial.println(hash);
  Serial.println(rideId ^ 0xA | 0x8);
  Serial.println("End check");
  if ((rideId ^ 0xA | 0x8 )== hash)
    return true;
  return false;
}
void loop() {
  while(micros()-lastRead <=1000000/(PULSE_FREQ_HZ*READ_PER_PULSE));
  lastRead = micros();  
  state = bool(digitalRead(PHOTXPIN));
  
  
  if(state){ //received 1. start reading next 20 bits
    Serial.println("START READING");
    for(int j=BITS_TO_READ-1;j>=0;j--){
      counter = readPulse();
      Serial.print("Read:");
      Serial.println(counter);
      message[j]= counter;
    }
    for(int j=BITS_TO_READ-1;j>=0;j--)
      Serial.print(message[j]);
    if (checkMessage(message,8)){
      qrcode.init();
      qrcode.create("1,40");
      unlock();
    }
    else if (compareBitset(message,lockCode,8)){
//      checkMe/ssage(message,8);
      qrcode.init();
      qrcode.create("0,40");
      lock();
    }
    delay(500);
  }
}
void printBitset(bool message[],size_t size){
 for(int j=0;j<size;j++)
    Serial.print(message[j]);
}
bool compareBitset(bool message1[],bool message2[], size_t size){
  for(int i=0;i<size;i++){
    if(message1[i] != message2[i])
      return false;
  }
  return true;
}
void unlock(){
  Serial.println("unlocking");
  for (pos = 0; pos <= 120; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}
void lock(){
  
  Serial.println("locking");
  for (pos = 120; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}

