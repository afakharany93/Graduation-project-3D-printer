/*
 * This is part of a graduation project solution aims to build a 3d printer,
 * made by students at the AASTMT sheraton, Mechatronics department, 2016
 * https://github.com/afakharany93/Graduation-project-3D-printer
 *
 * Used UUgear for communication
 *
 * Authors:
 * Ahmed El Fakharany - afakharany93@gmail.com
 ***********************************************************************
 * 
 * Quick test: after uploading the sketch, send "U01" with both NL and CR in
 * the serial monitor, you should be able to see the 1 + {device id} + :)
 */
#include <EEPROM.h>
#include "Stepper_3D.h"

#define LCD_DEBUGGING 1

#define BAUD_RATE  115200

#define EEPROM_SIZE  1024

#define ID_PREFIX  "UUGear-Arduino-"

#define COMMAND_START_CHAR  'U'
#define COMMAND_END_STRING  "\r\n"

#define RESPONSE_START_CHAR  '\t'
#define RESPONSE_END_STRING  ":)"

//Dealing with more than one bye of data in a message
#define MOST_SIGNIFICANT_BYTE_EQ_ZERO_STATUS   0x45
#define LEAST_SIGNIFICANT_BYTE_EQ_ZERO_STATUS  0x38
#define DATA_BYTE_EQ_ZERO_SUBSTITUTE           0xFF

//respond to recieving data
#define REPOND_WITH_RECIEVED  0x2F

// include the library code:
#include <LiquidCrystal.h>

#if LCD_DEBUGGING
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(13, 12, 10, 9, 8, 7);
#endif




String cmdBuf = "";
int cmdEndStrLen = strlen(COMMAND_END_STRING);

stepper_3d extruder;


// declare reset function
void(* resetDevice) (void) = 0;

void setup() {
  #if LCD_DEBUGGING
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  #endif

  // if has no id yet, generate one
  if (getID() == "") {
    generateID();
  }

  // initialize serial port
  Serial.begin(BAUD_RATE);
  while (!Serial) {
    ; // wait for serial port to connect
  }
  

  //Serial.println(getID());
}

/* the ISR function is the one that does the moving of the stepper motor, it outputs the step at the time required */
ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{
  extruder.inside_ISR();
}

void loop() {

  // listen to incoming commands
  int len = Serial.available();
  for (int i = 0; i < len; i ++) {
    cmdBuf += (char)Serial.read();
  }
  if (cmdBuf != "") {
    // drop useless data
    if (cmdBuf.charAt(0) != COMMAND_START_CHAR) {
      int pos = cmdBuf.indexOf(COMMAND_START_CHAR);
      if (pos != -1) {
        cmdBuf = cmdBuf.substring(pos);
      } 
      else {
        cmdBuf = "";
      }
    }
    // extract complete command
    if (cmdBuf != "") {
      int pos = cmdBuf.indexOf(COMMAND_END_STRING);
      if (pos != -1) {
        String cmd = cmdBuf.substring(0, pos + cmdEndStrLen);
        cmdBuf = cmdBuf.substring(pos + cmdEndStrLen);
        #if LCD_DEBUGGING
        lcd.setCursor(0, 0); // bottom left
        lcd.print(cmd);
        #endif
        processCommand(cmd);
      }
    }
  }
}

// read ID from EEPROM
String getID() {
  String id = "";
  for (int i = 0; i < EEPROM_SIZE; i ++) {
    int value = EEPROM.read(i);
    if (value == 0xFF) {
      return id;
    }
    id += char(value);
  }
  return id;
}

// generate ID and save it into EEPROM
void generateID() {
  randomSeed(analogRead(0));
  int part1 = random(1000, 10000);
  int part2 = random(1000, 10000);
  String id = ID_PREFIX + String(part1) + "-" + String(part2);
  int len = id.length();
  for (int i = 0; i < EEPROM_SIZE; i ++) {
    EEPROM.write(i, i < len ? id.charAt(i) : 0xFF);
  }
}

// log string to serial, with hex format
void logAsHex(String str) {
  String hex = "";
  int len = str.length();
  for (int i = 0; i < len; i ++) {
    int val = str.charAt(i);
    if (val < 16) {
      hex += "0";
    }
    hex += String(val, HEX);
    hex += " ";
  }
  hex.toUpperCase();
  Serial.println(hex);
}



// process the command
void processCommand(String cmd) {
  //logAsHex(cmd);
  if (cmd.length() > 3) {
    byte cmdId = cmd.charAt(1);
    switch(cmdId) {
    case 0x30:
      cmdGetID(cmd);
      break;
    case 0x60:
      cmd_stepper_move(cmd);
      break;
    case 0xFF:
      resetDevice();
      break;
    }
  }
}

// command to retrive the ID
// example: 55 30 01 0D 0A
void cmdGetID(String cmd) {
  if (cmd.length() > 4) {
    byte clientId = cmd.charAt(2);
    Serial.write(RESPONSE_START_CHAR);
    Serial.write(clientId);
    Serial.print(getID());
    Serial.print(RESPONSE_END_STRING);
  }
}


void cmd_stepper_move(String cmd) {
  if (cmd.length() > 5) {
    int least_significant_byte = cmd.charAt(2);
    int most_significant_byte = cmd.charAt(3);
    int status_byte = cmd.charAt(4);
    byte clientId = cmd.charAt(5);
    if (status_byte == MOST_SIGNIFICANT_BYTE_EQ_ZERO_STATUS)
    {
      most_significant_byte = 0;   
    }
    else if (status_byte == LEAST_SIGNIFICANT_BYTE_EQ_ZERO_STATUS)
    {
      least_significant_byte = 0;
    }
    int steps = ((((int) most_significant_byte) << 8 ) | 0x00FF) & (((int) least_significant_byte) | 0xFF00);
    #if LCD_DEBUGGING
    lcd.setCursor(0, 1); // bottom left
    lcd.print(steps);
    #endif
    extruder.permission = 1;
    extruder.stepper_move(steps, 400);

    //notify master with the recieve
    Serial.write(RESPONSE_START_CHAR);
    Serial.write(clientId);
    Serial.print(REPOND_WITH_RECIEVED);
    Serial.print(RESPONSE_END_STRING);
  }
}
