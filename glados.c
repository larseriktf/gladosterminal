#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 4096

int delay(int ms) {
	usleep(ms * 1000);
}

int play_song(FILE *stream)
{
	char **lines;
	char buffer[BUFFER_SIZE];
	int n = 0;

	// Get number of strings from first line
	fgets(buffer, BUFFER_SIZE, stream);	
	n = atoi(buffer);

	lines = malloc(n * sizeof(char *));
	int length = 0;

	for (int i = 0; i < n; i++)
	{
		fgets(buffer, BUFFER_SIZE, stream);
		length = strlen(buffer);

		// Eliminate \n character at the end of each string
		buffer[length - 1] = '\0';

		// Fill lines array
		lines[i] = malloc(length * sizeof(char));
		strcpy(lines[i], buffer);
	}

	// Print each line
	for (int i = 0; i < n; i++)
		printf("%s\n", lines[i]);

	// Free space
	for (int i = 0; i < n-1; i++)
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
