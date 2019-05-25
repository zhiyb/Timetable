#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include "struct.h"

#define debug(...) fprintf(stderr, __VA_ARGS__)
#define FATAL(m) { \
	fputs(m "\n", stderr); \
	exit(1); \
}

int readDay(FILE *fp)
{
	char ch, str[10];
	int i = 0;
	if ((ch = fgetc(fp)) != '"') {
		if (ch == '\n' || feof(fp))
			return 0;
		else
			FATAL("File format is not correct! (Start of line)");
	}
	while ((ch = fgetc(fp)) != '"' && i < 9)
		str[i++] = ch;
	str[i] = '\0';
	if (ch != '"')
		FATAL("File format is not correct! (Day)");
	if (strcmp(str, "Monday") == 0)
		return 1;
	else if (strcmp(str, "Tuesday") == 0)
		return 2;
	else if (strcmp(str, "Wednesday") == 0)
		return 3;
	else if (strcmp(str, "Thursday") == 0)
		return 4;
	else if (strcmp(str, "Friday") == 0)
		return 5;
	else if (strcmp(str, "Saturday") == 0)
		return 6;
	else if (strcmp(str, "Sunday") == 0)
		return 7;
	return 0;
}

char *readItem(FILE *fp)
{
	char c, *s = malloc(1 * sizeof(char));
	if (fgetc(fp) != ',' || fgetc(fp) != '"')
		FATAL("File format is not correct! (Start of item)");
	*s = '\0';
	while ((c = fgetc(fp)) != '"') {
		if (feof(fp))
			FATAL("File format is not correct! (Item)");
		s = realloc(s, strlen(s) + 2);
		*(s + strlen(s)) = c;
		*(s + strlen(s) + 1) = '\0';
	}
	return s;
}

void readLine(FILE *fp, NODE **n)
{
	char *s;
	while (*n != NULL)
		n = &(*n)->next;
	*n = malloc(sizeof(NODE));
	(*n)->next = NULL;
	s = readItem(fp);			// Item: start time
	(*n)->time[0] = (*s - '0') * 10 + *(s + 1) - '0';
	(*n)->time[1] = (*(s + 2) - '0') * 10 + *(s + 3) - '0';
	free(s);
	s = readItem(fp);			// Item: stop time
	(*n)->time[2] = (*s - '0') * 10 + *(s + 1) - '0';
	(*n)->time[3] = (*(s + 2) - '0') * 10 + *(s + 3) - '0';
	free(s);
	s = readItem(fp);			// Item: module & activity
	(*n)->module = s;
	while (*(s++) != ' ')
		if (s - (*n)->module > 10)
			FATAL("File format is not correct! (Module)");
	*(s - 1) = '\0';
	(*n)->activity = s;
	(*n)->staff = readItem(fp);		// Item: staff
	(*n)->location = readItem(fp);		// Item: location
	(*n)->week = readItem(fp);		// Item: week number
	if (fgetc(fp) != '\n' && !feof(fp))
		FATAL("File format is not correct! (End of line)");
}

TIMETABLE *read(char *path)
{
	int i;
	FILE *fp = fopen(path, "r");
	TIMETABLE *t = malloc(sizeof(TIMETABLE));
	if (fp == NULL)
		FATAL("Error openning file!");
	rewind(fp);
	for (i = 0; i < 7; i++)
		t->day[i] = NULL;
	while (!feof(fp)) {
		int l = readDay(fp);
		if (l <= 0 || l > 7)
			continue;
		readLine(fp, &t->day[l - 1]);
	}
	fclose(fp);
	return t;
}

void clean(TIMETABLE *t)
{
	int i;
	for (i = 0; i < 7; i++)
		while (t->day[i] != NULL) {
			NODE **n;
			n = &t->day[i];
			while ((*n)->next != NULL)
				n = &(*n)->next;
			free((*n)->module);
			free((*n)->staff);
			free((*n)->location);
			free((*n)->week);
			*n = NULL;
		}
	free(t);
}

