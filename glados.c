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

#define CREDITS_X0 55
#define CREDITS_Y0 1
#define CREDITS_X1 102
#define CREDITS_Y1 18

/* Function Declaration */

void print_line_animated(int letters, char *line, bool *wrap, int x, int y);
void play_song(char **lines, int n);
void draw_border(FILE *file);
char **get_lines(int *n, FILE *stream);
int main();

/* Function Implementation */

int main()
{
	// Set terminal settings
	//signal(SIGWINCH, &draw_border);
	enter_screen();
	echo_off();
	hide_cursor();
	printf("%s%s", COLOR_FG, COLOR_BG);

	// Load files
	FILE *stream = fopen("lyrics.txt", "r");
	FILE *border = fopen("border.txt", "r");
	FILE *ascii_art = fopen("ascii_art.txt", "r");

	// Error check
	if (!stream) return error("error opening lyrics.txt\n");
	if (!border) return error("error opening border.txt\n");
	if (!stream) return error("error opening ascii_art.txt\n");

	// Basic settings
	int n = 0;
	char **lines = get_lines(&n, stream);

	draw_border(border);
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


void play_song(char **lines, int n)
{
	int letters = 0, iterator = 0;
	bool wrap = false;

	// Print each line
	for (int i = 0; i < n; i++)
	{
		char *line = lines[i];
		letters = strlen(line);

		print_line_animated(letters, line, &wrap, 2, iterator + 1);

		// Wrap around
		if (iterator == LYRICS_Y1 - 3) wrap = true;

		if (wrap)
		{
			clear(LYRICS_X0, LYRICS_Y0, LYRICS_X1, LYRICS_Y1);
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

