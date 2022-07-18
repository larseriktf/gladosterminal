#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "utility.h"

void delay(int ms) {
	usleep(ms * 1000);
}

int play_song(FILE *stream)
{
	int n = 0;
	char **lines = get_lines(&n, stream);

	// Print each line
	for (int i = 0; i < n; i++)
		printf("%s\n", lines[i]);

	// Free space
	for (int i = 0; i < n; i++)
		free(lines[i]);	
	free(lines);
}

int main()
{
	FILE *stream = NULL;
	stream = fopen("lyrics.csv", "r");

	if (!stream)
	{
		printf("error opening file\n");
		return 1;
	}

	play_song(stream);
	fclose(stream);
	return 0;
}
