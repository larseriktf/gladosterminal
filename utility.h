#include <stdbool.h>

#define BUFFER_SIZE 4096
#define BEATS_BUFFER_SIZE 4

void delay(int ms) { usleep(ms * 1000); }

char **get_lines(int *n, FILE *stream)
{
	char **lines;
	char buffer[BUFFER_SIZE];
	int length = 0;

	// Read and assign n from first line in stream
	fgets(buffer, BUFFER_SIZE, stream);	
	*n = atoi(buffer);

	lines = malloc(*n * sizeof(char *));

	// Fill lines array and eliminate \n character
	for (int i = 0; i < *n; i++)
	{
		fgets(buffer, BUFFER_SIZE, stream);
		length = strlen(buffer);
		buffer[length - 1] = '\0';
		lines[i] = malloc(length * sizeof(char));
		strcpy(lines[i], buffer);
	}

	return lines;
}

char *str_copy_index(char *dest, const char *src, int start, int end, int size)
{
	if (dest == NULL) return NULL;
	if (start > end) return NULL;
	if (end - start > size) return NULL;

	char *ptr = dest;

	// Copy portion of string with pointer arithmetic
	// Inspired by Portfolio Courses: https://portfoliocourses.com/
	while (*src != '\0')
	{
		// Count down start and end vars to define portion of the string
		if (start == 0)
		{
			*dest = *src;
			dest++;
		}
		else start--;

		if (end == 0) break;
		else end--;

		src++;
	}
	*dest = '\0';
	
	return ptr;
}

void print_line_animated(int length, char *line)
{
	bool read = false;	
	char beats[4] = {'\0'};
	int start = 0, end = 0;
	long int ms;

	// Print lines gradually with delay to animate line
	// Speed is based on beats read from the file
	// Beats are converted into MS
	for (int i = 0; i < length; i++)
	{
		if (line[i] == '\\')
		{
			read = !read;
			if (start <= end) start = i + 1;
			else end = i - 1;
			if (read) beats[0] = '\0';
		}
		else if (read == false)
		{
			if (beats[0] == '\0')
			{
				str_copy_index(beats, line, start, end, 4);
				ms = strtol(beats, NULL, 10);
			}

			// Gradually print message
			delay(ms);
			printf("\r");
			for (int j = 0; j <= i; j++)
			{
				printf("%c", line[j]);
			}
			fflush(stdout);
		}
	}
}
