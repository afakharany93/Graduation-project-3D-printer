#include <EEPROM.h>
#include "Stepper_3D.h"
#include "heatbed_3D.h"

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
//heatbed commands
#define CMD_SET_HEATBED         0x70    //to set heatbed temperature
#define CMD_HEATBED_STATUS      0x71
//Dealing with more than one bye of data in a message
#define MOST_SIGNIFICANT_BYTE_EQ_ZERO_STATUS   0x45
#define LEAST_SIGNIFICANT_BYTE_EQ_ZERO_STATUS  0x38
#define DATA_BYTE_EQ_ZERO_SUBSTITUTE           0xFF

//respond to recieving data
#define REPOND_WITH_RECIEVED  0x2F


String cmdBuf = "";
int cmdEndStrLen = strlen(COMMAND_END_STRING);

stepper_3d motor;
heatbed bed;

unsigned char heatbed_temp = 0;

void(* resetDevice) (void) = 0;
void setup() 
{
  	motor.black = 7;
	motor.blue = 6;
	motor.red = 5;
	motor.green = 4;

	motor.first  = motor.green;
	motor.second = motor.red;
	motor.third  = motor.black;
	motor.forth  = motor.blue;

	motor.brake = 0;	//no braking
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

ISR(PCINT1_vect)
{
  motor.inside_endstop_ISR ();
}


void loop() 
{
  bed.heatbed_control(heatbed_temp);
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

    case CMD_HEATBED_STATUS:
      cmd_heatbed_status(cmd);
      break;

    case CMD_SET_HEATBED:
      cmd_set_heatbed(cmd);
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
