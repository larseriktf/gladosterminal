/* Headers */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "tc.h"
#include "utility.h"

/* Constants and Macros */

#define COL1_W 64
#define COL1_H 40
#define COL2_W 64
#define COL2_H 40

/* Function Declaration */

void print_line_animated(int letters, char *line, bool *wrap, int x, int y);
void play_song(char **lines, int n);
void draw_column(int max_w, int max_h, int l_margin);
void draw();
char **get_lines(int *n, FILE *stream);
int main();

/* Function Implementation */

int main()
{
	// Set terminal settings
	signal(SIGWINCH, &draw);
	enter_screen();
	echo_off();
	hide_cursor();
	printf("%s%s", COLOR_FG, COLOR_BG);

	// Load lyrics file
	FILE *stream = NULL;
	stream = fopen("lyrics.txt", "r");
	if (!stream)
		return error("error opening file\n");

	// Basic settings
	int n = 0;
	char **lines = get_lines(&n, stream);

	draw();
	play_song(lines, n);

	// Keep application running until keypress
	char c = getchar();

	// Reset terminal settings
	fclose(stream);
	exit_screen();
	echo_on();
	show_cursor();
	printf("%s", COLOR_NRM);

	return 0;
}

void draw()
{
	int rows = 0, cols = 0;
	get_rows_cols(&rows, &cols);

	int spacing = 2;

	// Actual drawing
	clear(0, 0, cols, rows);
	draw_column(COL1_W, COL1_H, 0 );
	//draw_column(COL2_W, COL2_H, COL1_W + spacing);
}

void draw_column(int max_w, int max_h, int l_margin)
{
	int i = 0;
	for (i = 0; i < max_w; i++)
	{
		if (i % 2 == 0)
		{
			move_cursor(i + l_margin, 0);
			printf("_");
			move_cursor(i + l_margin, max_h-1);
			printf("_");
		}
	}
	for (i = 1; i < max_h-1; i++)
	{
		if (i % 2 == 0)
		{
			move_cursor(l_margin, i);
			printf("|");
			move_cursor(l_margin + max_w - 1, i);
			printf("|");
		}
	}
}

void play_song(char **lines, int n)
{
	int letters = 0, iterator = 0;
	bool wrap = false;

	// Print each line
	for (int i = 0; i < n; i++)
	{
		char *line = lines[i];
		letters = strlen(line);

		print_line_animated(letters, line, &wrap, 2,iterator + 1);

		// Wrap around
		if (iterator == COL1_H - 3) wrap = true;

		if (wrap)
		{
			clear(1, 1, COL1_W - 1, COL1_H - 1);
			iterator = 0;
			wrap = false;
		}
		else iterator++;
	}

	// Free lines
	for (int i = 0; i < n; i++)
		free(lines[i]);	
	free(lines);
}


void print_line_animated(int letters, char *line, bool *wrap, int x, int y)
{
	bool print = false;	
	char buffer[4] = {'\0'};
	int start = 0, end = 0;
	long int ms;

	// Iterate character by character
	for (int i = 0; i < letters; i++)
	{
		if (line[i] == '[')
		{
			print = false;
			start = i;
			buffer[0] = '\0';
		}
		else if (line[i] == ']')
		{
			print = true;
			end = i;
		}
		else if (print)
		{
			// If buffer is empty, read number and prepare for printing
			if (buffer[0] == '\0')
			{
				str_copy_index(buffer, line, start + 1, end - 1, 4);
				str_replace_index(line, '~', start, end);
				ms = strtol(buffer, NULL, 10);
			}

			// Gradually print message with delay
			if (line[i] == '~') continue;
			else if (line[i] == '#') *wrap = true;
			else if (line[i] == '\\') printf("");
			else
			{
				move_cursor(x, y);
				printf("%c", line[i]);
				x++;
			}
			delay(ms);
			fflush(stdout);
		}
	}
}

