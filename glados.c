/* Headers */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "tc.h"
#include "utility.h"

/* Constants and Macros */

#define LYRICS_X0 2
#define LYRICS_Y0 1
#define LYRICS_X1 50
#define LYRICS_Y1 36

// For future credits implementation
// #define CREDITS_X0 55
// #define CREDITS_Y0 1
// #define CREDITS_X1 102
// #define CREDITS_Y1 18

/* Function Declaration */

void print_line_animated(int letters, char *line, bool *wrap, int x, int y);
void play_song(FILE *stream);
void draw_border(FILE *file);
char **get_lines(int *n, FILE *stream);
int main();

/* Function Implementation */

int main()
{
	// Set terminal settings
	enter_screen();
	echo_off();
	hide_cursor();
	printf("%s%s", COLOR_FG, COLOR_BG);

	FILE *border_stream = fopen("border.txt", "r");
	FILE *lyrics_stream = fopen("lyrics.txt", "r");
	FILE *images_stream = fopen("ascii_art.txt", "r");

	if (!border_stream) return error("error opening border.txt\n");
	if (!lyrics_stream) return error("error opening lyrics.txt\n");
	if (!images_stream) return error("error opening ascii_art.txt\n");

	// Basic settings
	//int n = 0;
	//char **lines = get_lines(&n, stream);

	draw_border(border_stream);
	play_song(lyrics_stream);

	// Keep application running until keypress
	char c = getchar();

	// Reset terminal settings

	fclose(border_stream);
	fclose(lyrics_stream);
	fclose(images_stream);

	exit_screen();
	echo_on();
	show_cursor();
	printf("%s", COLOR_NRM);

	return 0;
}

void draw_border(FILE *file)
{
	char c;
	int rows = 0, cols = 0;
	get_rows_cols(&rows, &cols);

	clear(0, 0, cols, rows);
	move_cursor(0, 0);

	// Draw border from file
	while((c = fgetc(file)) != EOF)
		printf("%c", c);
}


void play_song(FILE *stream)
{
	char c;
	int ms = 0;
	int x = LYRICS_X0, y = LYRICS_Y0;
	bool wrap = false, print = false;	

	while((c = fgetc(stream)) != EOF)
	{
		switch (c)
		{
			case '[':
								ms = 0;
								print = false;
								continue; break;
			case ']':
								print = true;
								continue; break;
			case '\n':
								y++;
								x = LYRICS_X0;
								continue; break;
			case '\\':
								delay(ms);
								printf("");
								continue; break;
			case '#': wrap = true;
								continue; break;
		}

		if (y > LYRICS_Y1) wrap = true;

		if (wrap)
		{
			clear(LYRICS_X0, LYRICS_Y0, LYRICS_X1, LYRICS_Y1);
			y = LYRICS_Y0;
			wrap = false;
		}

		if (print)
		{
			delay(ms);
			move_cursor(x, y);
			printf("%c", c);
			x++;
			fflush(stdout);
		}
		else
		{
			int ic = c - '0';
			ms = ms * 10;
			ms = ms + ic;
		}
	}
}
