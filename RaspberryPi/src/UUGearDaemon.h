/*
 * UUGear Solution: extend your Raspberry Pi with Arduino
 *
 * Author: Shawn (shawn@uugear.com)
 *
 * Copyright (c) 2014 UUGear s.r.o.
 *
 ***********************************************************************
 *  This file is part of UUGear Solution: 
 *  http://www.uugear.com/uugear-rpi-arduino-solution/
 *  
 *  UUGear Solution is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published
 *  by the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *  
 *  UUGear Solution is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *  
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with UUGear Solution.  If not, see <http://www.gnu.org/licenses/>.
 *
 ***********************************************************************
 */
#ifndef UUGEAR_DAEMON_H_
#define UUGEAR_DAEMON_H_

#define DAEMON_NAME "uugear_daemon"

/* The daemon may need root privileges to access Arduino and message queue */
#define RUN_AS_USER "root"

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

#define REQUEST_QUEUE_NAME  	"/uugear_request_queue"
#define RESPONSE_QUEUE_PREFIX	"/uugear_response_queue_"
#define MAX_QUEUE_NAME_LENGTH	32
#define MAX_MSG_PARTS		8
#define MAX_MSG_SIZE    	2048
#define MSG_PART_SEPARATOR	"]=["

#define COMMAND_START_CHAR		'U'
#define COMMAND_END_CHAR1		'\r'
#define COMMAND_END_CHAR2		'\n'
#define UUGEAR_DEVICE_PREFIX1		"ttyUSB"
#define UUGEAR_DEVICE_PREFIX2		"ttyACM"
#define UUGEAR_DEVICE_BAUD_RATE		115200
#define UUGEAR_ID_PREFIX			"UUGear-"
#define UUGEAR_ID_MAX_LENGTH		1024
#define UUGEAR_RESPONSE_START_CHAR	'\t'
#define UUGEAR_RESPONSE_END_STRING	":)"

/* Messages between client and daemon */
#define MSG_EXIT    		0
#define MSG_GET_DEVICE  	1
#define MSG_CLOSE_DEVICE  	2

#define MSG_RESET_DEVICE		217

#define MSG_STEPPER_MOVE        50

/* Commands between daemon and UUGear device */
#define CMD_GET_DEVICE_ID  		0x30

#define CMD_RESET_DEVICE		0xFF

#define CMD_STEPPER_MOVE        0x60

#define ASSERT_TRUE(x) \
    do { \
        if (!(x)) { \
            fprintf(stderr, "%s:%d: ", __func__, __LINE__); \
            perror(#x); \
            exit(EXIT_FAILURE); \
        } \
    } while (0) \

struct DeviceOpen
{
	int fd;			/* file describer for serial device */
	char *devName;	/* serial device name */
	char *id;		/* UUGear device id */
};

typedef struct DeviceOpen DeviceOpen;

#endif /* #ifndef UUGEAR_DAEMON_H_ */