#include <stdio.h>
#include <unistd.h>

int delay(int ms) {
	usleep(ms * 1000);
}

int play_song(FILE *stream)
{
	char lyrics[1000];
	int bpm = 120;
	int num_beats = 4;
	int num_notes = 4;
	int duration = 186000; // 3 minutes to milliseconds

	while (fgets(lyrics, 1000, stream) != NULL)
	{
		printf("%s", lyrics);
		delay(50);
	}
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

	play_song(stream);

	fclose(stream);

	return 0;
}
