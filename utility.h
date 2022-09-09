// Constants

#define BUFFER_SIZE 4096

// Function declarations

void str_replace_index(char *str, const char character, int start, int end);
char *str_copy_index(char *dest, const char *src, int start, int end, int size);
void clear(int x0, int y0, int x1, int y1);
void delay(int ms);
char **get_lines(int *n, FILE *stream);
int error(char *msg);

// Function implementations

int error(char *msg)
{
	printf("ERROR: %s", msg);
	return 1;
}

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

void delay(int ms) { usleep(ms * 1000); }

void clear(int x0, int y0, int x1, int y1)
{
	for (int y = y0; y < y1; y++)
	{
		for (int x = x0; x < x1; x++)
		{
			move_cursor(x, y);
			printf(" ");
		}
	}
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
		// Count down start and end to define portion of the string
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

void str_replace_index(char *str, const char character, int start, int end)
{
	if (str == NULL) return;
	if (start > end) return;

	while (*str != '\0')
	{
		// Count down start and end to define portion of the string
		if (start == 0)
			*str = character;
		else start--;

		if (end == 0) break;
		else end--;

		str++;
	}
}
