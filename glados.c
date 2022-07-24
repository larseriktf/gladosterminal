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
	/*
	for (int i = 0; i < n; i++)
	{
		char *line = lines[i];
		char *str_piece;
		int beats = 0;
		int start = 0;
		int end = 0;

		if (i % 2 == 0)
		{
			// Even: Print line
			str_piece = strtok(line, ";");
			end += strlen(str_piece);
			while (str_piece != NULL)
			{
				print_line_animated(start, end, line, 50);
				start += strlen(str_piece);
				str_piece = strtok(NULL, ";");
				end += strlen(str_piece);
			}
		}
		else
		{
			// Odd: Read beats
			str_piece = strtok(line, ";");
			while (str_piece != NULL)
			{
				letters = strlen(str_piece);
				str_piece = strtok(NULL, ";");
			}
		}

		printf("\n");
	}
	*/
	print_line_animated(6, 17, lines[1], 50);

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

	play_song(stream, 120);
	fclose(stream);
	return 0;
}
