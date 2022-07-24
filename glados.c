#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "utility.h"
#include "tc.h"

void print_line_animated(int length, char *line, int ms)
{
		// Print lines gradually with delay to create an animated line
		for (int i = 0; i < length; i++)
		{
			delay(ms);
			printf("\r");
			for (int j = 0; j <= i; j++)
				printf("%c", line[j]);
			fflush(stdout);
		}
}

void play_song(FILE *stream)
{
	int n = 0;
	int letters = 0;
	char **lines = get_lines(&n, stream);

	// Print each line
	for (int i = 0; i < 10; i++)
	{
		letters = strlen(lines[i]);
		print_line_animated(letters, lines[i], 50);
		printf("\n");
		delay(50);
	}

	// Free lines
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
