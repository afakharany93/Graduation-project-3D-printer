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
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <mqueue.h>
#include <sys/wait.h>
#include <errno.h>
#include <time.h>
#include <stdarg.h>

#include "UUGear.h"


int showLogs = 1;

void setShowLogs (int show)
{
	showLogs = show;
}

void printLog (char* fmt, ...)
{
	if (showLogs)
	{
	    va_list args;
	    va_start(args,fmt);
	    vprintf(fmt,args);
	    va_end(args);
	}
}

void setupUUGear ()
{
	mqd_t mq = mq_open (REQUEST_QUEUE_NAME, O_WRONLY);
	if ((mqd_t)-1 == mq)
	{
		/* start UUGear daemon */
		pid_t pid;
		int status;
		fflush (NULL);
		pid = fork ();
		switch (pid) {
			case -1:
				printLog ("Error to fork().\n");
			break;
			case 0:
				execl ("./UUGearDaemon", "UUGearDaemon", (char *) 0);
			break;
			default:
				fflush (NULL);
				waitpid (pid, &status, 0);
				while ((mqd_t)-1 == mq_open (REQUEST_QUEUE_NAME, O_WRONLY))
				{
					usleep (100000);
				}
			break;
		}
	}
	else
	{
		printLog ("UUGear daemon is running already.\n");
		mq_close (mq);
	}
}


UUGearDevice internalAttachDevice (char *id, int printError)
{
	UUGearDevice dev;
	strcpy (dev.id, id);
	dev.clientId = -1;
	dev.fd = -1;
	
	/* input */
	mqd_t in = mq_open (REQUEST_QUEUE_NAME, O_WRONLY);
    ASSERT_TRUE ((mqd_t)-1 != in);
    dev.in = in;
    
    /* output (prefix + clientId) */
    struct mq_attr attr;
		attr.mq_flags = 0;
	    attr.mq_maxmsg = 10;
	    attr.mq_msgsize = MAX_MSG_SIZE;
	    attr.mq_curmsgs = 0;
    int clientId;
    char queueName[strlen (RESPONSE_QUEUE_PREFIX) + 4];
    for (clientId = 1; clientId < 256; clientId ++)
    {
	    sprintf (queueName, "%s%d", RESPONSE_QUEUE_PREFIX, clientId);
	    
	    mqd_t out = mq_open (queueName, O_CREAT | O_EXCL | O_RDONLY, 0644, &attr);
	    if ((mqd_t)-1 != out)
	    {
	    	printLog ("Client queue name=%s\n", queueName);
	    	dev.clientId = clientId;
	    	dev.out = out;
	    	break;
	    }
	}
	
	/* if queue is not created, exit */
	ASSERT_TRUE (dev.clientId != -1);
	
	/* construct and send message */
	char buffer[MAX_MSG_SIZE + 1];
	sprintf (buffer,"%d%s%d%s%d%s%s", MSG_GET_DEVICE, MSG_PART_SEPARATOR, clientId, MSG_PART_SEPARATOR, -1, MSG_PART_SEPARATOR, id);
	
	ASSERT_TRUE (0 <= mq_send (in, buffer, strlen (buffer), 0));
	
	/* wait for the response */
	int bytes = 0;
	struct timespec ts;
    ts.tv_sec = time(0) + 2;
    ts.tv_nsec = 0;
    
	if ((bytes = mq_timedreceive (dev.out, buffer, MAX_MSG_SIZE, NULL, &ts)) == -1)
    {
    	if (printError)
    	{
	        if (errno == ETIMEDOUT)
	        {
	        	printLog ("UUGear device not found before timeout.\n");
	        }
	        else
	        {
	        	printLog ("UUGear device not found. Error=%d.\n", errno);
	        }
    	}
        /* device not found, release the queue name */
		mq_unlink (queueName);
    }
    else
    {
    	buffer[bytes] = 0;
    	dev.fd = atoi (buffer);
		printLog ("UUGear device found. fd=%d\n", dev.fd);
    }
	return dev;
}

UUGearDevice attachUUGearDevice (char *id)
{
	UUGearDevice dev = internalAttachDevice (id, 0);
	if (dev.fd == -1)
	{
		/* newly connected device may need a second shoot */
		sleep (1);
		dev = internalAttachDevice (id, 1);
	}
	return dev;
}

void sendMessageWithoutParameter(mqd_t in, int msgType, int clientId, int fd)
{
	char buffer[MAX_MSG_SIZE + 1];
	sprintf (buffer,"%d%s%d%s%d", msgType, MSG_PART_SEPARATOR, clientId, MSG_PART_SEPARATOR, fd);
	ASSERT_TRUE (0 <= mq_send (in, buffer, strlen (buffer), 0));
}

void sendMessageWithParameter(mqd_t in, int msgType, int clientId, int fd, int val)
{
	char buffer[MAX_MSG_SIZE + 1];
	sprintf (buffer,"%d%s%d%s%d%s%d", msgType, MSG_PART_SEPARATOR, clientId, MSG_PART_SEPARATOR, fd, MSG_PART_SEPARATOR, val);
	ASSERT_TRUE (0 <= mq_send (in, buffer, strlen (buffer), 0));
}

void sendMessageWith2Parameter(mqd_t in, int msgType, int clientId, int fd, int val, int parameter)
{
	char buffer[MAX_MSG_SIZE + 1];
	sprintf (buffer,"%d%s%d%s%d%s%d%s%d", msgType, MSG_PART_SEPARATOR, clientId, MSG_PART_SEPARATOR, fd, MSG_PART_SEPARATOR, val, MSG_PART_SEPARATOR, parameter);
	ASSERT_TRUE (0 <= mq_send (in, buffer, strlen (buffer), 0));
}

void send_message_with_3_data_bytes(mqd_t in, int msgType, int clientId, int fd, int data_byte_1, int data_byte_2 , int data_byte_3)
{
	char buffer[MAX_MSG_SIZE + 1];
	sprintf (buffer,"%d%s%d%s%d%s%d%s%d%s%d", msgType, MSG_PART_SEPARATOR, clientId, MSG_PART_SEPARATOR, fd, MSG_PART_SEPARATOR, data_byte_1, MSG_PART_SEPARATOR, data_byte_2, MSG_PART_SEPARATOR, data_byte_3);
	ASSERT_TRUE (0 <= mq_send (in, buffer, strlen (buffer), 0));
}


char * waitForString(UUGearDevice *dev, int * errorCode)
{
	int errCode = 0;
	char * buffer = malloc (MAX_MSG_SIZE + 1);
	int bytes = 0;
	struct timespec ts;
    ts.tv_sec = time(0) + 2;
    ts.tv_nsec = 0;
	if ((bytes = mq_timedreceive (dev->out, buffer, MAX_MSG_SIZE, NULL, &ts)) == -1)
    {
        if (errno == ETIMEDOUT)
        {
        	printLog ("No data was received before timeout.\n");
        	errCode = -1;
        }
        else
        {
        	printLog ("Can not receive data. Error=%d.\n", errno);
        	errCode = -2;
        }
    }
    else
    {
    	buffer[bytes] = 0;
    }
    if (errorCode != NULL)
	{
		*errorCode = errCode;
	}
	return (char *)buffer;
}

float waitForFloat(UUGearDevice *dev, int * errorCode)
{
	char * str = waitForString (dev, errorCode);
	float result = atof (str);
	free ((void *)str);
	return result;
}

int waitForInteger(UUGearDevice *dev, int * errorCode)
{
	char * str = waitForString (dev, errorCode);
	int result = atoi (str);
	free ((void *)str);
	return result;
}



void detachUUGearDevice (UUGearDevice *dev)
{
	char buffer[MAX_MSG_SIZE + 1];
	sprintf (buffer,"%d%s%d", MSG_CLOSE_DEVICE, MSG_PART_SEPARATOR, dev->clientId);
	ASSERT_TRUE (0 <= mq_send (dev->in, buffer, strlen (buffer), 0));
	
	/* close message queues */
	ASSERT_TRUE ((mqd_t)-1 != mq_close (dev->out));
	ASSERT_TRUE ((mqd_t)-1 != mq_close (dev->in));
	
	/* relese the queue name */
	char queueName[strlen (RESPONSE_QUEUE_PREFIX) + 4];
	sprintf (queueName, "%s%d", RESPONSE_QUEUE_PREFIX, dev->clientId);
	mq_unlink (queueName);
}

void resetUUGearDevice(UUGearDevice *dev)
{
	sendMessageWithoutParameter(dev->in, MSG_RESET_DEVICE, dev->clientId, dev->fd);
}

void cleanupUUGear ()
{
	mqd_t mq = mq_open (REQUEST_QUEUE_NAME, O_WRONLY);
	if ((mqd_t)-1 == mq)
	{
		printLog ("UUGear daemon is not running.\n");
	}
	else
	{
		/* stop UUGear daemon */
		char cmd[16];
		memset (cmd, 0, sizeof cmd);
		sprintf (cmd,"%d", MSG_EXIT);
		mq_send (mq, cmd, strlen (cmd), 0);
		
		/* release all queue names */
		int clientId;
		char queueName[strlen (RESPONSE_QUEUE_PREFIX) + 4];
	    for (clientId = 1; clientId < 256; clientId ++)
	    {
		    sprintf (queueName, "%s%d", RESPONSE_QUEUE_PREFIX, clientId);
		    mq_unlink (queueName);
		}
	}
}

int stepper_move(UUGearDevice *dev, short steps ) 
{
	char least_significant_byte  = (char)(steps & 0xFF) ;
	char most_significant_byte   = (char)((steps >> 8) & 0xFF) ;
	unsigned char status_byte = STATUS_BYTE_INITIAL_VAL;
	if (most_significant_byte == 0)
	{
		status_byte = MOST_SIGNIFICANT_BYTE_EQ_ZERO_STATUS;
		most_significant_byte = DATA_BYTE_EQ_ZERO_SUBSTITUTE;
	}
	else if (least_significant_byte == 0)
	{
		status_byte = LEAST_SIGNIFICANT_BYTE_EQ_ZERO_STATUS;
		least_significant_byte = DATA_BYTE_EQ_ZERO_SUBSTITUTE;
	}
	send_message_with_3_data_bytes(dev->in, MSG_STEPPER_MOVE, dev->clientId, dev->fd, least_significant_byte, most_significant_byte , status_byte);
	//recieve akhnolodgment procedure
	int errorCode = 0;
	int recieved = waitForInteger(dev, &errorCode);
	return errorCode == 0 ? recieved : -1;
}

int stepper_time_bet_steps(UUGearDevice *dev, unsigned short time_us ) 
{
	char least_significant_byte  = (char)(time_us & 0xFF) ;
	char most_significant_byte   = (char)((time_us >> 8) & 0xFF) ;
	unsigned char status_byte = STATUS_BYTE_INITIAL_VAL;
	if (most_significant_byte == 0)
	{
		status_byte = MOST_SIGNIFICANT_BYTE_EQ_ZERO_STATUS;
		most_significant_byte = DATA_BYTE_EQ_ZERO_SUBSTITUTE;
	}
	else if (least_significant_byte == 0)
	{
		status_byte = LEAST_SIGNIFICANT_BYTE_EQ_ZERO_STATUS;
		least_significant_byte = DATA_BYTE_EQ_ZERO_SUBSTITUTE;
	}
	send_message_with_3_data_bytes(dev->in, MSG_STEPPER_D_TIME, dev->clientId, dev->fd, least_significant_byte, most_significant_byte , status_byte);
	//recieve akhnolodgment procedure
	int errorCode = 0;
	int recieved = waitForInteger(dev, &errorCode);
	return errorCode == 0 ? recieved : -1;
}

int stepper_go_home (UUGearDevice *dev)
{
	sendMessageWithoutParameter(dev->in, MSG_STEPPER_GO_HOME, dev->clientId, dev->fd);
	//recieve akhnolodgment procedure
	int errorCode = 0;
	int recieved = waitForInteger(dev, &errorCode);
	return errorCode == 0 ? recieved : -1;
}

int stepper_stop (UUGearDevice *dev)
{
	sendMessageWithoutParameter(dev->in, MSG_STEPPER_STOP, dev->clientId, dev->fd);
	//recieve akhnolodgment procedure
	int errorCode = 0;
	int recieved = waitForInteger(dev, &errorCode);
	return errorCode == 0 ? recieved : -1;
}

int stepper_resume (UUGearDevice *dev)
{
	sendMessageWithoutParameter(dev->in, MSG_STEPPER_RESUME, dev->clientId, dev->fd);
	//recieve akhnolodgment procedure
	int errorCode = 0;
	int recieved = waitForInteger(dev, &errorCode);
	return errorCode == 0 ? recieved : -1;
}

char * stepper_status(UUGearDevice *dev)
{
	sendMessageWithoutParameter(dev->in, MSG_STEPPER_STATUS, dev->clientId, dev->fd);
	//recieve staus response
	int errorCode = 0;
	char * recieved = waitForString(dev, &errorCode);
	return errorCode == 0 ? recieved : "-1";

}

char * temperature_status(UUGearDevice *dev)
{
	sendMessageWithoutParameter(dev->in, MSG_TEMPERATURE_STATUS, dev->clientId, dev->fd);
	//recieve staus response
	int errorCode = 0;
	char * recieved = waitForString(dev, &errorCode);
	return errorCode == 0 ? recieved : "-1";

}

char * heatbed_status(UUGearDevice *dev)
{
	sendMessageWithoutParameter(dev->in, MSG_HEATBED_STATUS, dev->clientId, dev->fd);
	//recieve staus response
	int errorCode = 0;
	char * recieved = waitForString(dev, &errorCode);
	return errorCode == 0 ? recieved : "-1";

}

int heatbed_set_temp (UUGearDevice *dev, int temp)
{
	sendMessageWithParameter(dev->in, MSG_SET_HEATBED, dev->clientId, dev->fd, temp);
	//recieve akhnolodgment procedure
	int errorCode = 0;
	int recieved = waitForInteger(dev, &errorCode);
	return errorCode == 0 ? recieved : -1;
}

char * ext_heat_status(UUGearDevice *dev)
{
	sendMessageWithoutParameter(dev->in, MSG_EXT_HEAT_STATUS, dev->clientId, dev->fd);
	//recieve staus response
	int errorCode = 0;
	char * recieved = waitForString(dev, &errorCode);
	return errorCode == 0 ? recieved : "-1";

}

int ext_heat_set_temp (UUGearDevice *dev, int temp)
{
	sendMessageWithParameter(dev->in, MSG_SET_EXT_HEAT, dev->clientId, dev->fd, temp);
	//recieve akhnolodgment procedure
	int errorCode = 0;
	int recieved = waitForInteger(dev, &errorCode);
	return errorCode == 0 ? recieved : -1;
}


