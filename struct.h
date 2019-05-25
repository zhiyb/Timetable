#ifndef _STRUCT_H
#define _STRUCT_H

typedef struct node
{
	struct node *next;
	unsigned char time[4];
	char *module, *activity, *staff, *location, *week;
} NODE;

typedef struct timetable
{
	NODE *day[7];
} TIMETABLE;

#endif

