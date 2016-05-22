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


extern void sendMessageWithParameter(mqd_t in, int msgType, int clientId, int fd, int val);

extern void sendMessageWith2Parameter(mqd_t in, int msgType, int clientId, int fd, int val, int parameter);

extern void send_message_with_3_data_bytes(mqd_t in, int msgType, int clientId, int fd, int data_byte_1, int data_byte_2 , int data_byte_3);

extern int waitForInteger(UUGearDevice *dev, int * errorCode);

extern char * waitForString(UUGearDevice *dev, int * errorCode);

extern float waitForFloat(UUGearDevice *dev, int * errorCode);

extern int stepper_move(UUGearDevice *dev, short steps);

extern int stepper_time_bet_steps(UUGearDevice *dev, unsigned short time_us ) ;

extern int stepper_go_home (UUGearDevice *dev);

extern int stepper_stop (UUGearDevice *dev);

extern int stepper_resume (UUGearDevice *dev);

extern char * stepper_status(UUGearDevice *dev);

extern char * temperature_status(UUGearDevice *dev);

extern char * heatbed_status(UUGearDevice *dev);

extern int heatbed_set_temp (UUGearDevice *dev, int temp);

extern char * ext_heat_status(UUGearDevice *dev);

extern int ext_heat_set_temp (UUGearDevice *dev, unsigned short temp);
