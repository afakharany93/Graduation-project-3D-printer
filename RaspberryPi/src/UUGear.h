/*
 * This is part of a graduation project solution aims to build a 3d printer,
 * made by students at the AASTMT sheraton, Mechatronics department, 2016
 * https://github.com/afakharany93/Graduation-project-3D-printer
 *
 * Used UUgear for communication
 *
 * Authors:
 * Ahmed El Fakharany - afakharany93@gmail.com
 */
#include "UUGearDaemon.h"

struct UUGearDevice
{
	char id[UUGEAR_ID_MAX_LENGTH];
	int clientId;
	int fd;
	mqd_t in;
	mqd_t out;
};

typedef struct UUGearDevice UUGearDevice;

extern void setShowLogs (int show);

extern void setupUUGear ();

extern struct UUGearDevice attachUUGearDevice (char *id);


extern void detachUUGearDevice (UUGearDevice *dev);

extern void resetUUGearDevice(UUGearDevice *dev);


extern void cleanupUUGear();


extern void sendMessage(mqd_t in, int msgType, int clientId, int fd, int pin);

extern void sendMessageWithParameter(mqd_t in, int msgType, int clientId, int fd, int pin, int parameter);

extern void send_message_with_3_data_bytes(mqd_t in, int msgType, int clientId, int fd, int data_byte_1, int data_byte_2 , int data_byte_3);

extern int waitForInteger(UUGearDevice *dev, int * errorCode);

extern char * waitForString(UUGearDevice *dev, int * errorCode);

extern float waitForFloat(UUGearDevice *dev, int * errorCode);

extern int stepper_move(UUGearDevice *dev, short steps);