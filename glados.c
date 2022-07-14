#include <stdio.h>


int main()
{
	FILE *fp;
	char lyrics[1000];
	
	fp = fopen("lyrics.txt", "r");

	if (!fp)
	{
		printf("error opening file\n");
		return 1;
	}

	while (fgets(lyrics, 1000, fp) != NULL)
	{
		printf("%s", lyrics);
	}

	fclose(fp);

	return 0;
}
