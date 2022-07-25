#define BUFFER_SIZE 4096

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

void print_line_animated(int start, int end, char *line, int ms)
{
		// Print lines gradually with delay to animate line
		for (int i = start; i < end; i++)
		{
			delay(ms);
			printf("\r");
			// Print beginning unanimated
			printf("%*.*s", start, start, line);
			// Print the rest animated
			for (int j = start; j <= i; j++)
				printf("%c", line[j]);
			fflush(stdout);
		}
}
