/* Headers */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "utility.h"
#include "tc.h"

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

#define ASCII_X0 60
#define ASCII_Y0 20
#define ASCII_X1 100
#define ASCII_Y1 40

/* Function Declaration */

void draw_ascii_art(FILE *stream);
void play_song(FILE *stream);
void draw_border(FILE *file);
int main();

/* Function Implementation */

int main()
{
	// Set terminal settings
	enter_screen();
	echo_off();
	set_cursor_style(3);
	printf("%s%s", COLOR_FG, COLOR_BG);

	FILE *border_stream = fopen("border.txt", "r");
	FILE *lyrics_stream = fopen("lyrics.txt", "r");
	FILE *ascii_stream = fopen("ascii_art.txt", "r");

	if (!border_stream) return error("error opening border.txt\n");
	if (!lyrics_stream) return error("error opening lyrics.txt\n");
	if (!ascii_stream) return error("error opening ascii_art.txt\n");

	draw_border(border_stream);
	//play_song(lyrics_stream);
	draw_ascii_art(ascii_stream);

	// Keep application running until keypress
	char c = getchar();

	// Reset terminal settings

	fclose(border_stream);
	fclose(lyrics_stream);
	fclose(ascii_stream);

	exit_screen();
	echo_on();
	show_cursor();
	printf("%s", COLOR_NRM);
	set_cursor_style(0);

	return 0;
}

void draw_border(FILE *stream)
{
	char c;
	int rows = 0, cols = 0;
	get_rows_cols(&rows, &cols);

	clear(0, 0, cols, rows);
	move_cursor(0, 0);

	// Draw border from file
	while((c = fgetc(stream)) != EOF)
		printf("%c", c);
}

void play_song(FILE *stream)
{
	char c;
	int ms = 0;
	int x = LYRICS_X0, y = LYRICS_Y0;
	bool print = false;	

	while((c = fgetc(stream)) != EOF)
	{
		switch (c)
		{
			case '[': ms = 0; print = false; continue; break;
			case ']': print = true; continue; break;
			case '\n': y++; x = LYRICS_X0; continue; break;
			case '\\':
								move_cursor(x, y);
								printf("");
								fflush(stdout);
								delay(ms);
								continue; break;
			case '#':
								clear(LYRICS_X0, LYRICS_Y0, LYRICS_X1, LYRICS_Y1);
								y = LYRICS_Y0;
								continue; break;
		}

		if (print)
		{
			move_cursor(x, y);
			x++;
			printf("%c", c);
			fflush(stdout);
			delay(ms);
		}
		else
		{ // Cast c to int & add to ms
			// E.g 3 + '2' = 32
			int ic = c - '0';
			ms = ms * 10;
			ms = ms + ic;
		}
	}
}

void draw_ascii_art(FILE *stream)
{
	char c;
	int ms = 0, count = 20;
	int x = ASCII_X0, y = ASCII_Y0;

	while((c = fgetc(stream)) != EOF)
	{
		if (count == 20)
		{
			if (c == '\n')
			{
				delay(ms);
				y = ASCII_Y0;
				ms = 0;
				count = 0;
				clear(ASCII_X0, ASCII_Y0, ASCII_X1, ASCII_Y1);
				continue;
			}

			int ic = c - '0';
			ms = ms * 10;
			ms = ms + ic;
		}
		else
		{
			if (c == '\n')
			{
				y++;
				x = ASCII_X0;
				count++;
				continue;
			}

			//delay(ms);
			move_cursor(x, y);
			printf("%c", c);
			fflush(stdout);
			x++;
		}
	}
}
