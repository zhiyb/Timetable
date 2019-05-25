#include <stdio.h>
#include "struct.h"

#define TSTART 9
#define TSTOP 18

// Print head time line
void printTime(void)
{
	int i;
	printf("Time\t");
	for (i = TSTART; i < TSTOP; i++)
		printf("| %2d:00 - %2d:00 ", i, i + 1);
	putchar('\n');
}

void printCut(int c)
{
	int i;
	printf("\e[%dm", c == 8 ? 37 : 90 + c);
	for (i = 0; i < 8 + 16 * (TSTOP - TSTART); i++)
		putchar((i + 8) % 16 ? '-' : '+');
	putchar('\n');
}

// Return the nth node after current node, or NULL
NODE *next(NODE *n, int num)
{
	while (num-- > 0)
		if (n->next != NULL)
			n = n->next;
		else
			return NULL;
	return n;
}

// Return true if there isn't anything to print in this level
int empty(NODE *n, const int level)
{
	while (n != NULL) {
		NODE *s = next(n, level);
		if (s == NULL)
			return 1;
		if (s->time[0] == n->time[0])
			return 0;
		n = n->next;
	}
	return 1;
}

#define FORMAT "| %13.13s "
enum TYPE {MODULE, ACTIVITY, STAFF, LOCATION, WEEK};

// Print 1 course line
void printLine(NODE *n, const int level, const int type)
{
	int i = TSTART;
	putchar('\t');
	while (n != NULL) {
		NODE *s = next(n, level);
		if (s != NULL && s->time[0] == n->time[0])
			while (s->time[2] > i) {
				while (s->time[0] > i++)
					printf(FORMAT, "");
				switch (type) {
				case MODULE:
					printf(FORMAT, s->module);
					break;
				case ACTIVITY:
					printf(FORMAT, s->activity);
					break;
				case STAFF:
					printf(FORMAT, s->staff);
					break;
				case LOCATION:
					printf(FORMAT, s->location);
					break;
				case WEEK:
					printf(FORMAT, s->week);
					break;
				}
			}
		n = n->next;
	}
	while (i++ < TSTOP)
		printf(FORMAT, "");
	putchar('\n');
}

// Print a chain of nodes
void printNode(NODE *n, int i)
{
	char *day[7] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
	int level = 0;
	while (!empty(n, level)) {
		printCut(level > 0 ? i + 1 : 8);
		printf("\e[9%dm%s", i + 1, day[i]);
		printLine(n, level, MODULE);
		printLine(n, level, ACTIVITY);
		printLine(n, level, STAFF);
		printLine(n, level, LOCATION);
		printLine(n, level, WEEK);
		level++;
	}
}

// Main output function
void output(TIMETABLE *t)
{
	int i;
	printf("\e[90m");
	printTime();
	for (i = 0; i < 7; i++)
		if (t->day[i] != NULL)
			printNode(t->day[i], i);
	printCut(8);
	printf("\e[90m");
	printTime();
	printf("\e[0m");
}

