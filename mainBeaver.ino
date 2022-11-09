/* TODO: clean up functions
 * TODO: remove servos that don't need to be used (like removing arms from nod)
 * TODO: work on arms wide open function
 * TODO: fix elbows in clap
 * TODO: add way to control servo from serial port
 * TODO: identify range/angles of right and left shoulder
*/

#include <Adafruit_PWMServoDriver.h>
#include <ArduinoJson.h>

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

#ifndef APSSID
#define APSSID "thetekbot"
#define APPSK  "derpypassword"
#endif

const char *ssid = "robotics";
const char *password = APPSK;

bool first = true;

ESP8266WiFiMulti WiFiMulti;
int nullCount = 0;


/* Json */
StaticJsonDocument<512> doc;
/* Adafruit I2C servo driver */
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

/* Enum for each servo */
/* Ex: HEAD is channel 0 */
enum SERVOS {
  HEAD,
  HEADROT,
  LEFTARMROT,
  RIGHTARMROT,
  LEFTARM,
  RIGHTARM,
  LEFTELBOW,
  RIGHTELBOW
};

/* Enums for pulse lengths */
/* Ex: SERVOMAX[HEAD] is the max for the HEAD servo */
static const int SERVOMIN[] = {
  400, /* HEAD */
  200, /* HEADROT */
  250, /* LERFTARMROT */
  250, /* RIGHTARMROT */
  250, /* RIGHTARM */
  250, /* LEFTARM */
  250, /* RIGHTELBOW */
  250  /* LEFTELBOW */
};
static const int SERVOMAX[] = {
  250, /* HEAD */
  450, /* HEADROT */
  400, /* LERFTARMROT */
  400, /* RIGHTARMROT */
  400, /* RIGHTARM */
  400, /* LEFTARM */
  400, /* RIGHTELBOW */
  400  /* LEFTELBOW */
};


/* ESP8266 needs yield if for looping through servo positions
    #ifdef ESP8266
    yield();  // take a breather, required for ESP8266
    #endif
*/

/* Example JSON request
   {
    "leftarm": {
    "pos":90,
    "rot":10
   },
    "rightarm": {
    "pos":90,
    "rot":30
   },
    "head": {
    "pos":10,
    "rot":20
   }
  }
*/
 /** 
 * Beaver shakes head no and moves arms to default.
 * This function takes blah blah blah.
 * @param cycles this is the number of cycles that it will do
 * @param rate this is the speed of the cycles - should be positive and < 5
 * @return this function does not return anything
 */
void shake_no(int cycles, int rate){
      Serial.println("no rec");

    pwm.setPWM(HEADROT, 0, (SERVOMAX[HEADROT]-SERVOMIN[HEADROT])*(51/100.0)+SERVOMIN[HEADROT]);
    pwm.setPWM(LEFTARMROT, 0, (SERVOMAX[LEFTARMROT]-SERVOMIN[LEFTARMROT])*(25/100.0)+SERVOMIN[LEFTARMROT]);
    pwm.setPWM(RIGHTARMROT, 0, (SERVOMAX[RIGHTARMROT]-SERVOMIN[RIGHTARMROT])*(71/100.0)+SERVOMIN[RIGHTARMROT]);
    pwm.setPWM(RIGHTARM, 0, (SERVOMAX[RIGHTARM]-SERVOMIN[RIGHTARM])*(25/100.0)+SERVOMIN[RIGHTARM]);
    pwm.setPWM(LEFTARM, 0, (SERVOMAX[LEFTARM]-SERVOMIN[LEFTARM])*(85/100.0)+SERVOMIN[LEFTARM]);
    
    for(int i = 0; i < cycles; i++)
    {
      for(int j = 0; j <= 100; j++) {
        if(j <= 100) {
              pwm.setPWM(HEADROT, 0, (SERVOMAX[HEADROT]-SERVOMIN[HEADROT])*(j/100.0)+SERVOMIN[HEADROT]);
        }
        delay(rate);
      }
      delay(50);
      for(int j = 100; j >= 0; j--) {
        if(j >= 0) {
              pwm.setPWM(HEADROT, 0, (SERVOMAX[HEADROT]-SERVOMIN[HEADROT])*(j/100.0)+SERVOMIN[HEADROT]);
        }
        delay(rate);
      }
      delay(50);
    }
  }
 /** 
 * Beaver nods yes and moves arms to default.
 * This function takes blah blah blah.
 * @param cycles this is the number of cycles that it will do
 * @param rate this is the speed of the cycles - should be positive and < 5
 * @return this function does not return anything
 */
void nod_yes(int cycles, int rate){
    pwm.setPWM(HEADROT, 0, (SERVOMAX[HEADROT]-SERVOMIN[HEADROT])*(51/100.0)+SERVOMIN[HEADROT]);
    //pwm.setPWM(LEFTARMROT, 0, (SERVOMAX[LEFTARMROT]-SERVOMIN[LEFTARMROT])*(25/100.0)+SERVOMIN[LEFTARMROT]);
    //pwm.setPWM(RIGHTARMROT, 0, (SERVOMAX[RIGHTARMROT]-SERVOMIN[RIGHTARMROT])*(71/100.0)+SERVOMIN[RIGHTARMROT]);
   // pwm.setPWM(RIGHTARM, 0, (SERVOMAX[RIGHTARM]-SERVOMIN[RIGHTARM])*(25/100.0)+SERVOMIN[RIGHTARM]);
    //pwm.setPWM(LEFTARM, 0, (SERVOMAX[LEFTARM]-SERVOMIN[LEFTARM])*(85/100.0)+SERVOMIN[LEFTARM]);
    
    for(int i = 0; i < cycles; i++)
    {
      for(int j = 0; j <= 100; j++) {
        if(j <= 100) {
              pwm.setPWM(HEAD, 0, (SERVOMAX[HEAD]-SERVOMIN[HEAD])*(j/100.0)+SERVOMIN[HEAD]);
        }
        delay(rate);
      }
      delay(50);
      for(int j = 100; j >= 0; j--) {
        if(j >= 0) {
              pwm.setPWM(HEAD, 0, (SERVOMAX[HEAD]-SERVOMIN[HEAD])*(j/100.0)+SERVOMIN[HEAD]);
        }
        delay(rate);
      }
    }

}
 /** 
 * claps hands.
 * This function takes blah blah blah.
 * @param cycles this is the number of cycles that it will do
 * @param rate this is the speed of the cycles - should be positive and < 5
 * @return this function does not return anything
 */
void clap_hands(int cycles, int rate) {
    Serial.println("clap rec");

    /*pwm.setPWM(HEAD, 0, (SERVOMAX[HEAD]-SERVOMIN[HEAD])*(96/100.0)+SERVOMIN[HEAD]);
    pwm.setPWM(HEADROT, 0, (SERVOMAX[HEADROT]-SERVOMIN[HEADROT])*(51/100.0)+SERVOMIN[HEADROT]);*/
    pwm.setPWM(LEFTARMROT, 0, (SERVOMAX[LEFTARMROT]-SERVOMIN[LEFTARMROT])*(25/100.0)+SERVOMIN[LEFTARMROT]);
    pwm.setPWM(RIGHTARMROT, 0, (SERVOMAX[RIGHTARMROT]-SERVOMIN[RIGHTARMROT])*(71/100.0)+SERVOMIN[RIGHTARMROT]);
    pwm.setPWM(RIGHTARM, 0, (SERVOMAX[RIGHTARM]-SERVOMIN[RIGHTARM])*(25/100.0)+SERVOMIN[RIGHTARM]);
    pwm.setPWM(LEFTARM, 0, (SERVOMAX[LEFTARM]-SERVOMIN[LEFTARM])*(85/100.0)+SERVOMIN[LEFTARM]);
    
    for(int i = 0; i < cycles; i++)
    {
      for(int j = 0; j <= 100; j++) {
          pwm.setPWM(RIGHTARMROT, 0, (SERVOMAX[RIGHTARMROT]-SERVOMIN[RIGHTARMROT])*(j/100.0)+SERVOMIN[RIGHTARMROT]);
          pwm.setPWM(LEFTARMROT, 0, SERVOMAX[LEFTARMROT]-(SERVOMAX[LEFTARMROT]-SERVOMIN[LEFTARMROT])*(j/100.0));
          delay(rate);
      }
      delay(10);
      for(int j = 100; j >= 0; j--) {
          pwm.setPWM(RIGHTARMROT, 0, (SERVOMAX[RIGHTARMROT]-SERVOMIN[RIGHTARMROT])*(j/100.0)+SERVOMIN[RIGHTARMROT]);
          pwm.setPWM(LEFTARMROT, 0, SERVOMAX[LEFTARMROT]-(SERVOMAX[LEFTARMROT]-SERVOMIN[LEFTARMROT])*(j/100.0));
          delay(rate);
      }
    }
  }
  /** 
 * setting head to a position
 * This function takes blah blah blah.
 * @param pos this sets the up and down position, value must be between 0(down) and 100 (up)
 * @param rot this sets the left and right position, value must be between 0(right) and 100 (left)
 * @return this function does not return anything
 */
  void set_head(int pos, int rot){
    Serial.println("head rec");
      Serial.println((SERVOMAX[HEAD]-SERVOMIN[HEAD])*(pos/100.0)+SERVOMIN[HEAD]);
      if(pos >= 0 && pos <= 100)
        pwm.setPWM(HEAD, 0, (SERVOMAX[HEAD]-SERVOMIN[HEAD])*(pos/100.0)+SERVOMIN[HEAD]);
      Serial.println((SERVOMAX[HEADROT]-SERVOMIN[HEADROT])*(rot/100.0)+SERVOMIN[HEADROT]);
      if(rot >= 0 && rot <= 100)
        pwm.setPWM(HEADROT, 0, (SERVOMAX[HEADROT]-SERVOMIN[HEADROT])*(rot/100.0)+SERVOMIN[HEADROT]);
  }
  /** 
 * setting position of the left elbow and right elbow
 * This function takes blah blah blah.
 * @param lpos sets the position of the left elbow between 0 (points in) and 100 (points out)
 * @param rpos sets the position of the right elbow between 0 (points in) and 100 (points out)
 * @return this function does not return anything
 */
  void set_elbows(int lpos, int rpos){
    Serial.println("leftelbow rec");
    Serial.println((SERVOMAX[LEFTELBOW]-SERVOMIN[LEFTELBOW])*(lpos/100.0)+SERVOMIN[LEFTELBOW]);
    if(lpos >= 0 && lpos <= 100)
        pwm.setPWM(LEFTELBOW, 0, (SERVOMAX[LEFTELBOW]-SERVOMIN[LEFTELBOW])*(lpos/100.0)+SERVOMIN[LEFTELBOW]);
 
    Serial.println("rightelbow rec");
    Serial.println((SERVOMAX[RIGHTELBOW]-SERVOMIN[RIGHTELBOW])*(rpos/100.0)+SERVOMIN[RIGHTELBOW]);
    if(rpos >= 0 && rpos <= 100)
        pwm.setPWM(RIGHTELBOW, 0, (SERVOMAX[RIGHTELBOW]-SERVOMIN[RIGHTELBOW])*(rpos/100.0)+SERVOMIN[RIGHTELBOW]);
  }
  
  /** 
 * setting position of the left shoulder and right shoulder
 * This function takes blah blah blah.
 * @param lpos sets the position of the left shoulder between 0 (arms up) and 100 (arms down)
 * @param lrot sets the rotation of the left shoulder between 0 (arms rotate in) and 100 (arms rotate out)
 * @param rpos sets the position of the right shoulder between 0 (arms up) and 100 (arms down)
 * @param rrot sets the rotation of the right shoulder between 0 (arms rotate in) and 100 (arms rotate out)
 * @return this function does not return anything
 */
  void set_arms(int lpos,int lrot, int rpos, int rrot)
  {
    Serial.println("leftarm rec");
    Serial.println((SERVOMAX[LEFTARM]-SERVOMIN[LEFTARM])*(lpos/100.0)+SERVOMIN[LEFTARM]);
      if(lpos >= 0 && lpos <= 100)
        pwm.setPWM(LEFTARM, 0, (SERVOMAX[LEFTARM]-SERVOMIN[LEFTARM])*(lpos/100.0)+SERVOMIN[LEFTARM]);

    Serial.println((SERVOMAX[LEFTARMROT]-SERVOMIN[LEFTARMROT])*(lrot/100.0)+SERVOMIN[LEFTARMROT]);
      if(lrot >= 0 && lrot <= 100)
        pwm.setPWM(LEFTARMROT, 0, (SERVOMAX[LEFTARMROT]-SERVOMIN[LEFTARMROT])*(lrot/100.0)+SERVOMIN[LEFTARMROT]);
    
    Serial.println("rightarm rec");
    Serial.println((SERVOMAX[RIGHTARM]-SERVOMIN[RIGHTARM])*(rpos/100.0)+SERVOMIN[RIGHTARM]);
      if(rpos >= 0 && rpos <= 100)
        pwm.setPWM(RIGHTARM, 0, 100-(SERVOMAX[RIGHTARM]-SERVOMIN[RIGHTARM])*(rpos/100.0)+SERVOMIN[RIGHTARM]);

    Serial.println((SERVOMAX[RIGHTARMROT]-SERVOMIN[RIGHTARMROT])*(rrot/100.0)+SERVOMIN[RIGHTARMROT]);
      if(rrot >= 0 && rrot <= 100)
        pwm.setPWM(RIGHTARMROT, 0, 100-(SERVOMAX[RIGHTARMROT]-SERVOMIN[RIGHTARMROT])*(rrot/100.0)+SERVOMIN[RIGHTARMROT]);
  }
   /** 
 * Sets all servos to the default position
 * This function takes blah blah blah.
 * @return this function does not return anything
 */
  void at_rest(){
       Serial.println("Default Pos");
       pwm.setPWM(HEAD, 0, (SERVOMAX[HEAD]-SERVOMIN[HEAD])*(50/100.0)+SERVOMIN[HEAD]);
       pwm.setPWM(HEADROT, 0, (SERVOMAX[HEADROT]-SERVOMIN[HEADROT])*(50/100.0)+SERVOMIN[HEADROT]);
       pwm.setPWM(RIGHTARMROT, 0, (SERVOMAX[RIGHTARMROT]-SERVOMIN[RIGHTARMROT])*(50/100.0)+SERVOMIN[RIGHTARMROT]);
       pwm.setPWM(LEFTARMROT, 0, (SERVOMAX[LEFTARMROT]-SERVOMIN[LEFTARMROT])*(50/100.0)+SERVOMIN[LEFTARMROT]);
       pwm.setPWM(RIGHTARM, 0, (SERVOMAX[RIGHTARM]-SERVOMIN[RIGHTARM])*(50/100.0)+SERVOMIN[RIGHTARM]);
       pwm.setPWM(LEFTARM, 0, (SERVOMAX[LEFTARM]-SERVOMIN[LEFTARM])*(50/100.0)+SERVOMIN[LEFTARM]);
       pwm.setPWM(RIGHTELBOW, 0, (SERVOMAX[RIGHTELBOW]-SERVOMIN[RIGHTELBOW])*(50/100.0)+SERVOMIN[RIGHTELBOW]);
       pwm.setPWM(LEFTELBOW, 0, (SERVOMAX[LEFTELBOW]-SERVOMIN[LEFTELBOW])*(50/100.0)+SERVOMIN[LEFTELBOW]);
    }
 /** 
 * Arms-only version of the Running Man.
 * This function takes blah blah blah.
 * @param cycles this is the number of cycles that it will do
 * @param rate this is the speed of the cycles - should be positive and < 5
 * @return this function does not return anything
 */

void running_man(int cycles, int rate) {
      Serial.println("running man");

    pwm.setPWM(HEADROT, 0, (SERVOMAX[HEADROT]-SERVOMIN[HEADROT])*(51/100.0)+SERVOMIN[HEADROT]);
    pwm.setPWM(LEFTARMROT, 0, (SERVOMAX[LEFTARMROT]-SERVOMIN[LEFTARMROT])*(25/100.0)+SERVOMIN[LEFTARMROT]);
    pwm.setPWM(RIGHTARMROT, 0, (SERVOMAX[RIGHTARMROT]-SERVOMIN[RIGHTARMROT])*(71/100.0)+SERVOMIN[RIGHTARMROT]);
    pwm.setPWM(RIGHTARM, 0, (SERVOMAX[RIGHTARM]-SERVOMIN[RIGHTARM])*(25/100.0)+SERVOMIN[RIGHTARM]);
    pwm.setPWM(LEFTARM, 0, (SERVOMAX[LEFTARM]-SERVOMIN[LEFTARM])*(85/100.0)+SERVOMIN[LEFTARM]);
    
    for(int i = 0; i < cycles; i++)
    {
      for(int j = 0; j <= 100; j++) {
        if(j <= 100) {
              pwm.setPWM(RIGHTARM, 0, (SERVOMAX[RIGHTARM]-SERVOMIN[RIGHTARM])*(j/100.0)+SERVOMIN[RIGHTARM]);
        }
        if(j <= 100) {
              pwm.setPWM(LEFTARM, 0, (SERVOMAX[LEFTARM]-SERVOMIN[LEFTARM])*(j/100.0)+SERVOMIN[LEFTARM]);
        }
        delay(rate);
      }
      delay(50);
      for(int j = 100; j >= 0; j--) {
        if(j >= 0) {
              pwm.setPWM(RIGHTARM, 0, (SERVOMAX[RIGHTARM]-SERVOMIN[RIGHTARM])*(j/100.0)+SERVOMIN[RIGHTARM]);
        }
        if(j >= 0) {
              pwm.setPWM(LEFTARM, 0, (SERVOMAX[LEFTARM]-SERVOMIN[LEFTARM])*(j/100.0)+SERVOMIN[LEFTARM]);
        }
        delay(rate);
      }
    }

}
void setup() {
  Serial.begin(115200);
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);


  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  delay(10);
  for (uint16_t servonum = 0; servonum < 8; servonum++) {
    pwm.setPWM(servonum, 0, SERVOMIN[servonum]);
  }
  at_rest();
  delay(1000);
}
void loop() {

  /* for (int i = 0; i < 100; i++){
       Serial.println("Default Pos");
       pwm.setPWM(HEAD, 0, (SERVOMAX[HEAD]-SERVOMIN[HEAD])*(i/100.0)+SERVOMIN[HEAD]);
       pwm.setPWM(HEADROT, 0, (SERVOMAX[HEADROT]-SERVOMIN[HEADROT])*(i/100.0)+SERVOMIN[HEADROT]);
       pwm.setPWM(LEFTARMROT, 0, (SERVOMAX[LEFTARMROT]-SERVOMIN[LEFTARMROT])*(50/100.0)+SERVOMIN[LEFTARMROT]);
       pwm.setPWM(RIGHTARMROT, 0, (SERVOMAX[RIGHTARMROT]-SERVOMIN[RIGHTARMROT])*(50/100.0)+SERVOMIN[RIGHTARMROT]);
       pwm.setPWM(RIGHTARM, 0, (SERVOMAX[RIGHTARM]-SERVOMIN[RIGHTARM])*(25/100.0)+SERVOMIN[RIGHTARM]);
       pwm.setPWM(LEFTARM, 0, (SERVOMAX[LEFTARM]-SERVOMIN[LEFTARM])*(50/100.0)+SERVOMIN[LEFTARM]);
       delay(50);
    }*/
    //running_man(5, 4);
    //delay(500);
   
    //set_arms(0,0,0,0);
    //delay(2000);
    //set_arms(0,0,0,100); //lpos lrot rpos rrot, 100 rot arm outward; 0 rot inward;
    //delay(10000);
   
    //delay(10000);
    //shake_no(5,4);
    //delay(500);
    //clap_hands(5,4);
    //delay(500);
    //at_rest();
    //delay(1000);
    
}   
