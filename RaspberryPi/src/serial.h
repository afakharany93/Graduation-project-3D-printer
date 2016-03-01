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
extern int   serialOpen (char *device, int baud);
extern void  serialClose (int fd);
extern void  serialFlush (int fd);
extern void  serialWritechar (int fd, unsigned char c);
extern void  serialWriteString (int fd, char *s);
extern void  serialWriteData (int fd, char *buf, int len);
extern int   serialReadChar (int fd);
