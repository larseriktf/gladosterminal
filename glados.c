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

#define CREDITS_X0 55
#define CREDITS_Y0 1
#define CREDITS_X1 102
#define CREDITS_Y1 18

#define ASCII_X0 60
#define ASCII_Y0 20
#define ASCII_X1 100
#define ASCII_Y1 40

/* Function Declaration */

void point_char(int x, int y, char c);
void draw_credits(FILE *stream);
void draw_ascii_art(FILE *stream);
void draw_lyrics(FILE *stream);
void draw_border(FILE *stream);
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
	FILE *credits_stream = fopen("credits.txt", "r");

	if (!border_stream) return error("error opening border.txt\n");
	if (!lyrics_stream) return error("error opening lyrics.txt\n");
	if (!ascii_stream) return error("error opening ascii_art.txt\n");
	if (!credits_stream) return error("error opening credits.txt\n");

	draw_border(border_stream);
	//draw_lyrics(lyrics_stream);
	//draw_ascii_art(ascii_stream);
	draw_credits(credits_stream);

	// Keep application running until keypress
	char c = getchar();

	// Reset terminal settings

	fclose(border_stream);
	fclose(lyrics_stream);
	fclose(ascii_stream);
	fclose(credits_stream);

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

void draw_lyrics(FILE *stream)
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
								//point_char(x, y, c);
								delay(ms);
								continue; break;
			case '#':
								clear(LYRICS_X0, LYRICS_Y0, LYRICS_X1, LYRICS_Y1);
								y = LYRICS_Y0;
								continue; break;
		}

		if (print)
		{
			x++;
			point_char(x, y, c);
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

			point_char(x, y, c);
			x++;
		}
	}
}

void draw_credits(FILE *stream)
{
	char buffer[9000], c;
	int ms = 50;
	int x = CREDITS_X0, y = CREDITS_Y0,
			start = 0, end = CREDITS_Y1,
			i = 0, count = 0, cp = 0;
	bool setcp = false;

	// Fill buffer
	while ((buffer[i] = fgetc(stream)) != EOF) i++;
	buffer[i] = '\0';

	// Iterate buffer
	i = 0;
	while (buffer[i] != '\0')
	{
		c = buffer[i];

		// set checkpoint
		if (setcp)
		{
			cp = i;
			setcp = false;
		}

		if (c == '\n')
		{
			y++; x = CREDITS_X0; count++;
			setcp = true;
			//if (count == start + 1) setcp = true;
		}
		else
		{
			if (count == end)
			{
				start++; end++;
				count = start;
				y = CREDITS_Y0; i = cp;
				clear(CREDITS_X0, CREDITS_Y0, CREDITS_X1, CREDITS_Y1);
			}

			// Change delay for range
			if (end > CREDITS_Y1 && count < end - 1) delay(0);
			else delay(ms);

			point_char(x, y, c);
			x++;
		}
		i++;
	}
}

void point_char(int x, int y, char c)
{
	move_cursor(x, y);
	printf("%c", c);
	fflush(stdout);
}
