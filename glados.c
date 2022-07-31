#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "utility.h"
#include "tc.h"

void play_song(FILE *stream, int bpm)
{
	int n = 0;
	int letters = 0;
	char **lines = get_lines(&n, stream);

	// Print each line
	for (int i = 0; i < n; i++)
	{
		char *line = lines[i];

		letters = strlen(line);
		print_line_animated(letters, line);
	}

	// Free lines
	for (int i = 0; i < n; i++)
		free(lines[i]);	
	free(lines);
}

int main()
{
	FILE *stream = NULL;
	stream = fopen("lyrics.txt", "r");

	if (!stream)
	{
		printf("error opening file\n");
		return 1;
	}

	play_song(stream, 120);
	fclose(stream);
	return 0;
}
