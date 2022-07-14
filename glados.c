#include <stdio.h>
#include <unistd.h>

int micros(millis) return millis * 1000;

int main()
{
	FILE *stream;
	char lyrics[1000];
	
	stream = fopen("lyrics.txt", "r");

	if (!stream)
	{
		printf("error opening file\n");
		return 1;
	}

	while (fgets(lyrics, 1000, stream) != NULL)
	{
		printf("%s", lyrics);
		usleep(micros(1000));
	}

	fclose(stream);

	return 0;
}
