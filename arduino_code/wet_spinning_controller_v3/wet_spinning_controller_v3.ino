
/*
Motor driver - Motor connection
BLK-A-1A
GRN-C-1B
RED-B-2A
BLU-D-2B

Jingwen Mar 2023
*/

// OLED Display - Kaitlyn 9/26

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//define modes for different speed settings
#define MODE_RESET 0
#define MODE_REG 1
#define MODE_THERMO 2
#define MODE_CHARCOAL 3

/************SPEED SETTINGS*************/
#define EXTRUDER_DELAY_0 1
#define BOBBIN_DELAY_0 3

#define EXTRUDER_DELAY_1 8
#define BOBBIN_DELAY_1 3

#define EXTRUDER_DELAY_2 12
#define BOBBIN_DELAY_2 5

#define EXTRUDER_DELAY_3 12
#define BOBBIN_DELAY_3 6
/***************************************/

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino nano every:       A4(SDA), A5(SCL)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3D ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define BAUD (9600)
// Motor 1, main extruder
#define EN_PIN 6    //driver enable pin, active low
#define DIR_PIN 10  //driver direction pin
#define STEP_PIN 9  //drive step pin

#define SW_PIN 3 //switch input for direction

//motor 2
#define EN_PIN_2 17    //driver enable pin, active low
#define DIR_PIN_2 21  //driver direction pin
#define STEP_PIN_2 20  //drive step pin

int current_mode = 1; //start with regular mode
int prev_mode = 1; 

int settingExtruderDelay, settingBobbinDelay;

bool prevDirection = 1; //1: forward direction, 0: backward direction
bool currentDirection = 1; ///1: forward direction, 0: backward direction

int prevExtrudeDelay; //extrusion speed delay
int tempExtrudeDelay;

unsigned long previousExtrudeMillis = 0;  // will store last extrusion stepper motor was updated
unsigned long extrudeInterval;  // interval at which to blink (milliseconds)
bool extrudeState = LOW; //the status of the step pin

unsigned long previousBobbinMillis = 0;  // will store last bobbin stepper motor was updated
unsigned long bobbinInterval;  // interval at which to blink (milliseconds)
bool bobbinState = LOW; //the status of the step pin

bool dir; //direction display flag
bool sp; //speed display flag
bool md; //mode display flag


void setup() {
  Serial.begin(BAUD);

  delay(2000);
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  display.setTextSize(2);             // 2:2 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.println("EcoThreads"); // forward
  display.print("V3");
  display.display();
  delay(1000);
  
  pinMode(EN_PIN, OUTPUT);      // Enable
  pinMode(STEP_PIN, OUTPUT);    // Step
  pinMode(DIR_PIN, OUTPUT);     // Dir
  pinMode(SW_PIN, INPUT);       // Direction switch
  digitalWrite(EN_PIN, LOW);    // Set Enable low

  pinMode(EN_PIN_2, OUTPUT);      // Enable
  pinMode(STEP_PIN_2, OUTPUT);    // Step
  pinMode(DIR_PIN_2, OUTPUT);     // Dir
  digitalWrite(EN_PIN_2, HIGH);    // Set Enable low
  digitalWrite(DIR_PIN_2,HIGH); //HIGH: collect, LOW: release

  currentDirection = digitalRead(SW_PIN); //read initial direction switch
  digitalWrite(DIR_PIN, currentDirection);  // Set Dir high, forward
  prevDirection = currentDirection;

  checkMode();
  prevExtrudeDelay = settingExtruderDelay; 
  tempExtrudeDelay = prevExtrudeDelay;
  extrudeInterval = (unsigned long)prevExtrudeDelay;
  drawText(tempExtrudeDelay, currentDirection);
}

void loop() {

  //check direction switch
  currentDirection = digitalRead(SW_PIN); //read initial direction switch
  dir = currentDirection != prevDirection; //update display flag
  if(dir){
    digitalWrite(DIR_PIN, currentDirection);  // Set Dir high, forward
    digitalWrite(EN_PIN_2, !currentDirection);
    prevDirection = currentDirection;
    //Serial.println("switch direction");
  }

  //check speed potentiometer
  checkMode();
  md = prev_mode != current_mode;
  sp = prevExtrudeDelay != settingExtruderDelay; //update display flag
  extrudeInterval = (unsigned long) (settingExtruderDelay);

  //drive the extrusion motor
  unsigned long currentMillis = millis();

  if (currentMillis - previousExtrudeMillis >= extrudeInterval) {
    // save the last time you blinked the LED
    previousExtrudeMillis = currentMillis;
    // if the LED is off turn it on and vice-versa:
    if (extrudeState == LOW) {
      extrudeState = HIGH;
    } else {
      extrudeState = LOW;
    }
    digitalWrite(STEP_PIN, extrudeState);
  }

  if (currentMillis - previousBobbinMillis >= bobbinInterval) {
    // save the last time you blinked the LED
    previousBobbinMillis = currentMillis;
    // if the LED is off turn it on and vice-versa:
    if (bobbinState == LOW) {
      bobbinState = HIGH;
    } else {
      bobbinState = LOW;
    }
    digitalWrite(STEP_PIN_2, bobbinState);
  }


  //update display
  if (md|| sp || dir) {
    tempExtrudeDelay = settingExtruderDelay;
    if(current_mode == MODE_RESET){
      digitalWrite(DIR_PIN_2,LOW); //HIGH: collect, LOW: release
    }else{
      digitalWrite(DIR_PIN_2,HIGH); //HIGH: collect, LOW: release
    }
    
    drawText(tempExtrudeDelay, currentDirection);
    if (sp) {
      prevExtrudeDelay = settingExtruderDelay;
    }
    if(md){
      prev_mode = current_mode;
    }
  }

}

// int speedUp() {
//   int potReading = analogRead(A0); // Reads the potentiometer
//   int newCustomSpeed = map(potReading, 0, 1023, 1,EXTRUDER_DELAY); // Convrests the read values of the potentiometer from 0 to 1023 into desireded delay values (300 to 4000)
//   if(potReading < 4){
//     newCustomSpeed = 1;
//   } //reduce floating value
  
//   if (potReading > 1015){
//     newCustomSpeed = EXTRUDER_DELAY;
//   } //reduce floating value
//   return newCustomSpeed;
// }

void drawText(int spd, int dir) {
  display.clearDisplay();

  display.setCursor(0,0);             // Start at top-left corner
  switch (current_mode) {
    case MODE_RESET: 
      display.println(F("RESET"));
      display.print(F("Speed:"));
      display.println(spd);
      break;
    case MODE_REG:
      display.println(F("REGULAR"));
      display.print(F("Speed:"));
      display.println(spd);
      break;
    case MODE_THERMO:
      display.println(F("THERMO"));
      display.print(F("Speed:"));
      display.println(spd);
      break;
    case MODE_CHARCOAL:
      display.println(F("CHARCOAL"));
      display.print(F("Speed:"));
      display.println(spd);
      break;
    default:
      break;
  }

  display.println("Direction:");

  if (dir == 1) {
    display.println("Forward"); // forward
  } else {
    display.println("Backward"); // backward
  }

  display.display();
}

void checkMode(){

  int potReading = analogRead(A0); // Reads the potentiometer
  //Serial.println(potReading);
  if(potReading < 140){
    current_mode = MODE_CHARCOAL;
    settingExtruderDelay = EXTRUDER_DELAY_3;
    bobbinInterval = BOBBIN_DELAY_3;
  }else if (potReading < 650){
    current_mode = MODE_THERMO;
    settingExtruderDelay = EXTRUDER_DELAY_2;
    bobbinInterval = BOBBIN_DELAY_2;

  }else if (potReading < 950){
    current_mode = MODE_REG;
    settingExtruderDelay = EXTRUDER_DELAY_1;
    bobbinInterval = BOBBIN_DELAY_1;

  }else{
    current_mode = MODE_RESET;
    settingExtruderDelay = EXTRUDER_DELAY_0;
    bobbinInterval = BOBBIN_DELAY_0;
  }
}
