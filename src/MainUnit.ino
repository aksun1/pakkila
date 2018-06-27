
#include "OneButton.h"



// LED-nauhat:

    const byte lightTV_L = 11;
    const byte lightTV_R = 10;
    const byte lightBED_L = 9;
    const byte lightBED_R = 6;
    
    const byte lightSTAIRS = 21;

// Painonappien indikaattorivalot:

    const byte indicTV_L = 7;
    const byte indicTV_R = 8;
    const byte indicBED_L = 12;
    const byte indicBED_R = 13;
    byte lightIndicators[52];
 


// Painonapit:
    
    const byte buttonTV_Lp = 2;
    const byte buttonTV_Rp = 3;
    const byte buttonBED_Lp = 4;
    const byte buttonBED_Rp = 5;
    bool pinStates[52];
    byte lightDimState[52];

    const byte dimmerTV=A2;
    const byte dimmerBED=A1;

OneButton buttonTV_L(buttonTV_Lp, true);
OneButton buttonTV_R(buttonTV_Rp, true);
OneButton buttonBED_L(buttonBED_Lp, true);
OneButton buttonBED_R(buttonBED_Rp, true);



unsigned long BED_R_timer;
boolean BED_R_pressed;
byte longPressTime=200; // +300 ms kuten .setPressTicks

int logScale[256] = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255};
  
void lights(byte pin,int level = 256) {
  if(level != 256) {
    /*Serial.print("lights level ");
    Serial.print(level);
    Serial.print(" on pin ");
    Serial.println(pin);*/
    analogWrite(pin,level);
    if(level==0){
      pinStates[pin]=0;
      if(lightIndicators[pin]) {
        lights(lightIndicators[pin],255);
      }
    }
    else {
      pinStates[pin]=1;
      if(lightIndicators[pin]) {
        lights(lightIndicators[pin],0);
      }
    }
  }
  else {
   if(pinStates[pin]) {
    Serial.print("lights off pin");
    Serial.println(pin);
    analogWrite(pin,0);
    pinStates[pin]=0;
   }
   else {
    Serial.print("lights on pin");
    Serial.print(lightDimState[pin]);
    Serial.println(pin);
    if(logScale[lightDimState[pin]]) {
      analogWrite(pin,logScale[lightDimState[pin]]);
    }
    else {
      analogWrite(pin,255);
    }
    pinStates[pin]=1;
   }
   if(lightIndicators[pin]) {
    lights(lightIndicators[pin]);
   }
  }
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  lightIndicators[lightTV_L] = indicTV_L;
  lightIndicators[lightTV_R] = indicTV_R;
  lightIndicators[lightBED_L] = indicBED_L;
  lightIndicators[lightBED_R] = indicBED_R;

  // buttons are initialized in the library
  
  pinMode(lightTV_L,OUTPUT);
  pinMode(lightTV_R,OUTPUT);
  pinMode(lightBED_L,OUTPUT);
  pinMode(lightBED_R,OUTPUT);

  
  lights(indicTV_L);
  lights(indicTV_R);
  lights(indicBED_L);
  lights(indicBED_R);

  buttonTV_L.attachClick(TV_L_click);
  buttonTV_R.attachClick(TV_R_click);
  buttonBED_L.attachClick(BED_L_click);
  buttonBED_R.attachClick(BED_R_click);
  buttonBED_R.attachLongPressStop(BED_R_pressStart);
  buttonBED_R.attachDuringLongPress(BED_R_pressing);
  buttonBED_R.setPressTicks(300);

}
void TV_L_click() {
  lights(lightTV_L);
}
void TV_R_click() {
  lights(lightTV_R);
}
void BED_L_click() {
  lights(lightTV_L);
  lights(lightTV_R);
}
void BED_R_click() {
  lights(lightBED_L);
  lights(lightBED_R);
}
void BED_R_pressStart() {
  BED_R_timer = millis();
  BED_R_pressed=false;
}
void BED_R_pressing() {
  if(!BED_R_pressed) {
    if(millis()-BED_R_timer >= longPressTime) {
      lights(lightBED_L);
      BED_R_pressed=true;  
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  buttonTV_L.tick();
  buttonTV_R.tick();
  buttonBED_L.tick();
  buttonBED_R.tick();
  runDimmer(dimmerTV,1,lightTV_R,lightTV_L);
  //runDimmer(dimmerBED,1,lightBED_R,lightBED_L);
}

void runDimmer(byte dimmer, byte raI, byte light1, byte light2){
  int reading = analogRead(dimmer);
  static int maxValue, minValue =500;


  if(reading > maxValue) {
    maxValue = reading;
  }
  if(reading > 50 && minValue-reading<20 && reading < minValue) {
    minValue = reading;
  }
  
  byte val = map( runningAverage(constrain(reading,minValue,maxValue),raI), minValue, maxValue, 0, 255);
   if(abs(lightDimState[light1]-val) > 2) {
      Serial.print(minValue);
      Serial.print(",");
      Serial.print(maxValue);
      Serial.print(",");
      Serial.println(reading);
      dim(light1,val);
      dim(light2,val);
   }
}
void dim(byte pin, byte level) {
    lights(pin,logScale[level]);
  lightDimState[pin] = level;
}