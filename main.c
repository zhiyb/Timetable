#include <stdio.h>
#include <malloc.h>
#include "struct.h"
#include "read.h"
#include "output.h"

void help(char *path)
{
	printf("Output a more readable table for school timetable\n" \
		"Suggested terminal size: at least 152x45\n" \
		"\nUsage: %s <csv file>\n", path);
}

int main(int argc, char *argv[])
{
	TIMETABLE *t;
	switch (argc) {
	case 2:
		t = read(argv[1]);
		output(t);
		clean(t);
		return 0;
	}
	help(argv[0]);
	return 1;
}

