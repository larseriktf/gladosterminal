/* Headers */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

#include "utility.h"
#include "tc.h"

/* Constants and Macros */

#define LYRICS_X0 3
#define LYRICS_Y0 2
#define LYRICS_X1 51
#define LYRICS_Y1 38

#define CREDITS_X0 55
#define CREDITS_Y0 3
#define CREDITS_X1 103
#define CREDITS_Y1 18

#define ASCII_X0 60
#define ASCII_Y0 21
#define ASCII_X1 100
#define ASCII_Y1 41

/* Function Declaration */

void plot_char(int x, int y, char c);
void *play_credits(void *arg);
void *draw_ascii(void *arg);
void *play_lyrics(void *arg);
void draw_border(FILE *stream);
int main();

/* Function Implementation */

int main()
{
	pthread_t p1, p2, p3;

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
	pthread_create(&p1, NULL, play_lyrics, lyrics_stream);
	pthread_create(&p2, NULL, draw_ascii, ascii_stream);
	//pthread_create(&p3, NULL, play_credits, credits_stream);

	pthread_join(p1, NULL);
	pthread_join(p2, NULL);
	//pthread_join(p3, NULL);

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

	delay(1300);

	// Draw border from file
	while((c = fgetc(stream)) != EOF)
		printf("%c", c);
}

void *play_lyrics(void *arg)
{
	FILE *stream = (FILE*) arg;
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
								//plot_char(x, y, c);
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
			plot_char(x, y, c);
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
	return NULL;
}

void *draw_ascii(void *arg)
{
	FILE *stream = (FILE*) arg;
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

			plot_char(x, y, c);
			x++;
		}
	}
	return NULL;
}

void *play_credits(void *arg)
{
	FILE *stream = (FILE*) arg;
	char buffer[9000], c, c1;
	int ms = 68;
	int x = CREDITS_X0, y = CREDITS_Y1, i = 0, cp = 0, count = 0;
	int range = CREDITS_Y1 - CREDITS_Y0 - 1;
	bool set_cp;


	// Fill buffer
	while ((buffer[i] = fgetc(stream)) != EOF) i++;
	buffer[i] = '\0';

	delay(9025);

	// Iterate buffer
	i = 0;
	while (buffer[i] != '\0')
	{
		c = buffer[i];

		if (c == '\n')
		{
			// Reset x & y
			x = CREDITS_X0;
			y = CREDITS_Y0;

			if (count < range) y += range - count;

			clear(CREDITS_X0, CREDITS_Y0, CREDITS_X1, CREDITS_Y1);
			count++;
			set_cp = true;

			// Print previous
			for (int j = cp; j < i; j++)
			{
				c1 = buffer[j];
				if (c1 == '\n')
				{
					if (count > range && set_cp)
					{
						cp = j + 1;
						set_cp = false;
					}
					x = CREDITS_X0;
					y++;
				}
				else
				{
					if (c1 == '#') c1 = ' ';
					plot_char(x, y, c1);
					x++;
				}
			}

			// Reset x & y
			x = CREDITS_X0;
			y = CREDITS_Y1;
		}
		else // Print current animated
		{
			//delay(ms);
			if (c == '#') c = ' ';
			usleep(68650);
			plot_char(x, y, c);
			x++;
		}
		i++;
	}
	return NULL;
}

void plot_char(int x, int y, char c)
{
	move_cursor(x, y);
	printf("%c", c);
	fflush(stdout);
}
