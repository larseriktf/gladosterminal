#define BUFFER_SIZE 4096
#define ASCII_ESC 27

void delay(int ms) { usleep(ms * 1000); }

void print_escape_code(char *code) { printf("%c%s", ASCII_ESC, code); }

char **get_lines(int *n, FILE *stream)
{
	char **lines;
	char buffer[BUFFER_SIZE];
	int length = 0;

	// Read and assign n lines
	fgets(buffer, BUFFER_SIZE, stream);	
	*n = atoi(buffer);

	lines = malloc(*n * sizeof(char *));

	// Fill lines and eliminate \n character
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
