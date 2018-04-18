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
//#define MOTOR_POSITIVE D6
//#define MOTOR_NEGATIVE D7
#include <qrcode.h>
#include <SSD1306.h>

byte message=0;
bool state=false, lastState = false;
unsigned long lastRead = 0;
int counter =1, x1=0, x0=0;

SSD1306  display(0x3c, D3, D5);
QRcode qrcode (&display);

void drawProgressBarDemo() {
  int progress = (counter / 5) % 100;
  // draw the progress bar
  display.drawProgressBar(0, 32, 120, 10, progress);

  // draw the percentage as String
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 15, String(progress) + "%");
}

void setup() {
  // put your setup code here, to run once:
  pinMode(PHOTXPIN,INPUT);
  Serial.begin(9600);
  Serial.println("");
//  attachInterrupt(digitalPinToInterrupt(PHOTXPIN), countBits, CHANGE);
  Serial.println("Starting...");

  display.init();
  display.clear();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_24);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 15, "iPool");
  display.display();
//  display.setTextAlignment(TEXT_ALIGN_CENTER);
//  display.setFont(ArialMT_Plain_16);
//  display.drawString(0, 10, "iPool");
//  for(int i=0;i<100;i++){
//    counter ++;
//    drawProgressBarDemo();
//    delay(10);
//  }
  lastRead = micros();
  display.setFont(ArialMT_Plain_16);
}

void loop() {
  while(micros()-lastRead <=50000);
  lastRead = micros();  
  state = bool(digitalRead(PHOTXPIN));
  message = (message << 1) | state;
  if(message == 0x99){
    Serial.println("UNLOCK");
    display.clear();
    qrcode.init();
    // create qrcode
    qrcode.create("UNLOCKED");
//    unlock();
  }
  if(message == 0xAA){
    Serial.println("LOCK");
    display.clear();
//    lock();
    qrcode.init();
    // create qrcode
    qrcode.create("LOCKED");
  }
//  if(state){
//    x1++;
//  }
//  else{
//    x0++;
//  }
}
void countBits(){
  if(state){
    Serial.print("Ones:");
    Serial.println(x1);
  }
  else{
    Serial.print("Zeros:");
    Serial.println(x0);
  }
  state = !state;
  x1=0;x0=0;
}
//void lock(){
//  digitalWrite(MOTOR_POSITIVE, HIGH);
//  digitalWrite(MOTOR_NEGATIVE, LOW);
//  delay(250);
//  digitalWrite(MOTOR_POSITIVE, LOW);
//  digitalWrite(MOTOR_NEGATIVE, LOW);
//}
//void unlock(){
//  digitalWrite(MOTOR_POSITIVE, LOW);
//  digitalWrite(MOTOR_NEGATIVE, HIGH);
//  delay(250);
//  digitalWrite(MOTOR_POSITIVE, LOW);
//  digitalWrite(MOTOR_NEGATIVE, LOW);
//}

