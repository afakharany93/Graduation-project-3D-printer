/*
 * This is part of a graduation project solution aims to build a 3d printer,
 * made by students at the AASTMT Sheraton, Mechatronics department, 2016
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
#include "Thermistor_3D.h"
#include "heatbed_3D.h"
#include "ext_heat_3D.h"
#include "ext_Stepper_3D.h"

#define LCD_DEBUGGING 0   //if set to one, the messages received by the arduino will be printed on the LCD, if set to zero then it won't



#define BAUD_RATE  115200

#define EEPROM_SIZE  1024

#define ID_PREFIX  "UUGear-Arduino-"

#define COMMAND_START_CHAR  'U'     //char used to identify the start of a command message received from master MCU
#define COMMAND_END_STRING  "\r\n"  //char used to identify the end of a command message received from master MCU

#define RESPONSE_START_CHAR  '\t'   //char used to define the start of response from Arduino to master
#define RESPONSE_END_STRING  ":)"   //string used to identify the end of response form Arduino to master

//commands
//when these commands are received within the command message from master in the command slot within the message, corresponding actions will be taken
//stepper commands
#define CMD_STEPPER_MOVE        0x60    //to move the stepper motor number of steps received within the same message 
#define CMD_STEPPER_D_TIME      0x61    //for defining time between steps
#define CMD_STEPPER_GO_HOME     0x62    //to tell the stepper motor to take the carriage to the home position
#define CMD_STEPPER_STOP        0x63    //to force the motor to stop
#define CMD_STEPPER_RESUME      0x64    //to resume the motor after stop
#define CMD_STEPPER_STATUS      0x65    //to send the status of the stepper motor

//temperature commands
#define CMD_TEMPERATURE_STATUS    0x66    //to send the statuses of the temperature sensors
//heatbed commands
#define CMD_SET_HEATBED         0x70    //to set heatbed temperature
#define CMD_HEATBED_STATUS      0x71
//extruder heat commands
#define CMD_SET_EXT_HEAT         0x75    //to set heatbed temperature
#define CMD_EXT_HEAT_STATUS      0x76
//stepper commands
#define CMD_EXT_STEPPER_MOVE        0x40
#define CMD_EXT_STEPPER_D_TIME      0x41    //for delay time between steps
#define CMD_EXT_STEPPER_STOP        0x42
#define CMD_EXT_STEPPER_RESUME      0x43
#define CMD_EXT_STEPPER_STATUS      0x44

//Dealing with more than one bye of data in a message
#define MOST_SIGNIFICANT_BYTE_EQ_ZERO_STATUS   0x45
#define LEAST_SIGNIFICANT_BYTE_EQ_ZERO_STATUS  0x38
#define DATA_BYTE_EQ_ZERO_SUBSTITUTE           0xFF

//respond to recieving data
#define REPOND_WITH_RECIEVED  0x2F

#if LCD_DEBUGGING
// include the LCD library code:
#include <Wire.h>                 //to enable I2C to communicate with  the LCD I2C module
#include <LiquidCrystal_I2C.h>    //to use LCD I2C module related functions



// Set the LCD address to 0x27 (Could be 0x20 if solder jumpers are bridged!)
// Set the pins on the I2C chip used for LCD connections:
//                    addr,en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C LCD(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  //create an LCD object, Set the LCD I2C address

#endif




String cmdBuf = "";
int cmdEndStrLen = strlen(COMMAND_END_STRING);

stepper_3d motor;
Thermistor_3d thermistor(A3);
heatbed bed;
ext_heat extHeat;
#if EXTRUDER
#if defined(__AVR_ATmega2560__)|| defined(__AVR_ATmega1280__) //if arduino mega is used
ext_stepper_3d extStp;
#endif
#endif
unsigned char heatbed_temp = 0;
unsigned int ext_heat_temp = 0;
// declare reset function
void(* resetDevice) (void) = 0;

void setup() {
  #if LCD_DEBUGGING
  LCD.begin(16, 2);  // 16 lines by 2 rows
  LCD.clear();        //clear LCD screen
  LCD.backlight();    //activate LCD backlight
  LCD.setCursor(0,0); //set cursor to home position
  LCD.print("Hello");       //print a message to make sure lcd is working
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
  motor.inside_ISR();
}
#if EXTRUDER
#if defined(__AVR_ATmega2560__)|| defined(__AVR_ATmega1280__) //if arduino mega is used
ISR(TIMER5_COMPA_vect)          // timer compare interrupt service routine
{
  extStp.inside_ISR();
}
#endif
#endif
ISR(PCINT1_vect)
{
  motor.inside_endstop_ISR ();
}

void loop() {

  bed.heatbed_control(heatbed_temp);
  extHeat.ext_heat_control(ext_heat_temp);
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
        //LCD.clear();        //clear LCD screen
        LCD.setCursor(0, 0); // 
        LCD.print(cmd);       //print command on screen
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
    case CMD_STEPPER_MOVE:
      cmd_stepper_move(cmd);
      break;
    case CMD_STEPPER_D_TIME:
      cmd_stepper_d_time(cmd);
      break;
    case CMD_STEPPER_GO_HOME:
      cmd_stepper_go_home(cmd);
      break;
    case CMD_STEPPER_STOP:
      cmd_stepper_stop(cmd);
      break;
      
    case CMD_STEPPER_RESUME:
      cmd_stepper_resume(cmd);
      break;

    case CMD_STEPPER_STATUS:
      cmd_stepper_status(cmd);
      break;

    case CMD_TEMPERATURE_STATUS:
      cmd_temperature_status(cmd);
      break;

    case CMD_HEATBED_STATUS:
      cmd_heatbed_status(cmd);
      break;

    case CMD_SET_HEATBED:
      cmd_set_heatbed(cmd);
      break;

    case CMD_EXT_HEAT_STATUS:
      cmd_ext_heat_status(cmd);
      break;

    case CMD_SET_EXT_HEAT:
      cmd_set_ext_heat(cmd);
      break;
#if EXTRUDER
#if defined(__AVR_ATmega2560__)|| defined(__AVR_ATmega1280__) //if arduino mega is used
    case CMD_EXT_STEPPER_MOVE:
      cmd_ext_stepper_move(cmd);
      break;
    case CMD_EXT_STEPPER_D_TIME:
      cmd_ext_stepper_d_time(cmd);
      break;

    case CMD_EXT_STEPPER_STOP:
      cmd_ext_stepper_stop(cmd);
      break;
      
    case CMD_EXT_STEPPER_RESUME:
      cmd_ext_stepper_resume(cmd);
      break;

    case CMD_EXT_STEPPER_STATUS:
      cmd_ext_stepper_status(cmd);
      break;
#endif
#endif
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
    LCD.setCursor(0, 1); //
    LCD.print(steps);   //print number of steps received
    #endif
    motor.permission = 1;
    motor.stepper_move(steps, motor.time_bet_steps_us);

    //notify master with the recieve
    Serial.write(RESPONSE_START_CHAR);
    Serial.write(clientId);
    Serial.print(REPOND_WITH_RECIEVED);
    Serial.print(RESPONSE_END_STRING);
  }
}

void cmd_stepper_d_time(String cmd) {
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
    motor.time_bet_steps_us = ((((unsigned int) most_significant_byte) << 8 ) | 0x00FF) & (((unsigned int) least_significant_byte) | 0xFF00);
    #if LCD_DEBUGGING
    LCD.setCursor(0, 1); // 
    LCD.print(motor.time_bet_steps_us); //print the received time between steps
    #endif

    //notify master with the recieve
    Serial.write(RESPONSE_START_CHAR);
    Serial.write(clientId);
    Serial.print(REPOND_WITH_RECIEVED);
    Serial.print(RESPONSE_END_STRING);
  }
}

void cmd_stepper_go_home(String cmd)
{
  if (cmd.length() > 4) 
  {
    byte clientId = cmd.charAt(2);
    motor.permission = 1;
    motor.stepper_flow (motor.backward);
    //notify master with the recieve
    Serial.write(RESPONSE_START_CHAR);
    Serial.write(clientId);
    Serial.print(REPOND_WITH_RECIEVED);
    Serial.print(RESPONSE_END_STRING);
  }
}

void cmd_stepper_stop(String cmd)
{
  if (cmd.length() > 4) 
  {
    byte clientId = cmd.charAt(2);
    motor.permission = 1;
    motor.stepper_stop ();
    //notify master with the recieve
    Serial.write(RESPONSE_START_CHAR);
    Serial.write(clientId);
    Serial.print(REPOND_WITH_RECIEVED);
    Serial.print(RESPONSE_END_STRING);
  }
}

void cmd_stepper_resume(String cmd)
{
  if (cmd.length() > 4) 
  {
    byte clientId = cmd.charAt(2);
    motor.permission = 1;
    motor.stepper_resume ();
    //notify master with the recieve
    Serial.write(RESPONSE_START_CHAR);
    Serial.write(clientId);
    Serial.print(REPOND_WITH_RECIEVED);
    Serial.print(RESPONSE_END_STRING);
  }
}

void cmd_stepper_status(String cmd)
{
  if (cmd.length() > 4) 
  {
    byte clientId = cmd.charAt(2);
    //send the status
    Serial.write(RESPONSE_START_CHAR);
    Serial.write(clientId);
    Serial.print(motor.stepper_status());
    Serial.print(RESPONSE_END_STRING);
  }
}

void cmd_temperature_status(String cmd)
{
  if (cmd.length() > 4) 
  {
    byte clientId = cmd.charAt(2);
    float temp = thermistor.temperature_measurment();
    char str_temp[6];
    /* 4 is mininum width, 2 is precision; float value is copied onto str_temp*/
    dtostrf(temp, 5, 2, str_temp);
    #if LCD_DEBUGGING
        LCD.print(temp);       //print command on screen
        #endif
    int x = 0;  //to hold the return value of sprintf
    char buff[20];
    x = sprintf(buff, "bed temp %s c", str_temp);
    #if LCD_DEBUGGING
        LCD.setCursor(0, 1); //
        LCD.print(buff);       //print command on screen
        #endif
    //send the status
    Serial.write(RESPONSE_START_CHAR);
    Serial.write(clientId);
    Serial.print(buff);
    Serial.print(RESPONSE_END_STRING);
  }
}

void cmd_heatbed_status(String cmd)
{
  if (cmd.length() > 4) 
  {
    byte clientId = cmd.charAt(2);
    //send the status
    Serial.write(RESPONSE_START_CHAR);
    Serial.write(clientId);
    Serial.print(bed.heatbed_status());
    Serial.print(RESPONSE_END_STRING);
  }
}


void cmd_set_heatbed(String cmd) {
  if (cmd.length() > 5) {
    if(cmd.charAt(2) > 2)
    {
      bed.heatbed_permission();
    }
    heatbed_temp = cmd.charAt(2);
    byte clientId = cmd.charAt(3);
    //notify master with the recieve
    Serial.write(RESPONSE_START_CHAR);
    Serial.write(clientId);
    Serial.print(REPOND_WITH_RECIEVED);
    Serial.print(RESPONSE_END_STRING);
  }
}

void cmd_ext_heat_status(String cmd)
{
  if (cmd.length() > 4) 
  {
    byte clientId = cmd.charAt(2);
    //send the status
    Serial.write(RESPONSE_START_CHAR);
    Serial.write(clientId);
    Serial.print(extHeat.ext_heat_status());
    Serial.print(RESPONSE_END_STRING);
  }
}


void cmd_set_ext_heat(String cmd) {
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
    unsigned int data = ((((unsigned int) most_significant_byte) << 8 ) | 0x00FF) & (((unsigned int) least_significant_byte) | 0xFF00);
    if(data > 2)
    {
      extHeat.ext_heat_permission();
    }
    ext_heat_temp = data;
    //notify master with the recieve
    Serial.write(RESPONSE_START_CHAR);
    Serial.write(clientId);
    Serial.print(REPOND_WITH_RECIEVED);
    Serial.print(RESPONSE_END_STRING);
  }
}
#if EXTRUDER
#if defined(__AVR_ATmega2560__)|| defined(__AVR_ATmega1280__) //if arduino mega is used
void cmd_ext_stepper_move(String cmd) {
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
    LCD.setCursor(0, 1); //
    LCD.print(steps);   //print number of steps received
    #endif
    extStp.permission = 1;
    extStp.stepper_move(steps, extStp.time_bet_steps_us);

    //notify master with the recieve
    Serial.write(RESPONSE_START_CHAR);
    Serial.write(clientId);
    Serial.print(REPOND_WITH_RECIEVED);
    Serial.print(RESPONSE_END_STRING);
  }
}

void cmd_ext_stepper_d_time(String cmd) {
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
    extStp.time_bet_steps_us = ((((unsigned int) most_significant_byte) << 8 ) | 0x00FF) & (((unsigned int) least_significant_byte) | 0xFF00);
    extStp.time_bet_steps_us = extStp.time_bet_steps_us * 95;
    #if LCD_DEBUGGING
    LCD.setCursor(0, 1); // 
    LCD.print(extStp.time_bet_steps_us); //print the received time between steps
    #endif

    //notify master with the recieve
    Serial.write(RESPONSE_START_CHAR);
    Serial.write(clientId);
    Serial.print(REPOND_WITH_RECIEVED);
    Serial.print(RESPONSE_END_STRING);
  }
}

void cmd_ext_stepper_stop(String cmd)
{
  if (cmd.length() > 4) 
  {
    byte clientId = cmd.charAt(2);
    extStp.permission = 1;
    extStp.stepper_stop ();
    //notify master with the recieve
    Serial.write(RESPONSE_START_CHAR);
    Serial.write(clientId);
    Serial.print(REPOND_WITH_RECIEVED);
    Serial.print(RESPONSE_END_STRING);
  }
}

void cmd_ext_stepper_resume(String cmd)
{
  if (cmd.length() > 4) 
  {
    byte clientId = cmd.charAt(2);
    extStp.permission = 1;
    extStp.stepper_resume ();
    //notify master with the recieve
    Serial.write(RESPONSE_START_CHAR);
    Serial.write(clientId);
    Serial.print(REPOND_WITH_RECIEVED);
    Serial.print(RESPONSE_END_STRING);
  }
}

void cmd_ext_stepper_status(String cmd)
{
  if (cmd.length() > 4) 
  {
    byte clientId = cmd.charAt(2);
    //send the status
    Serial.write(RESPONSE_START_CHAR);
    Serial.write(clientId);
    Serial.print(extStp.stepper_status());
    Serial.print(RESPONSE_END_STRING);
  }
}
#endif
#endif
