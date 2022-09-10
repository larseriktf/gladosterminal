// Constants

#define BUFFER_SIZE 4096

// Function declarations

void clear(int x0, int y0, int x1, int y1);
void delay(int ms);
int error(char *msg);

// Function implementations

int error(char *msg)
{
	printf("ERROR: %s", msg);
	return 1;
}

void delay(int ms) { usleep(ms * 1000); }

void clear(int x0, int y0, int x1, int y1)
{
	for (int y = y0; y <= y1; y++)
	{
		for (int x = x0; x <= x1; x++)
		{
			move_cursor(x, y);
			printf(" ");
		}
	}
}
