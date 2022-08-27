/* Headers */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "tc.h"

/* Constants and Macros */

#define BUFFER_SIZE 4096

/* Function Declaration */

void str_replace_index(char *str, const char character, int start, int end);
char *str_copy_index(char *dest, const char *src, int start, int end, int size);
void delay(int ms);
void print_line_animated(int length, char *line, int x, int y);
void play_song(char **lines, int n);
void draw_column(int max_w, int max_h, int l_margin, int v_padding);
void clear(int x1, int y1, int x2, int y2);
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
	{
		printf("error opening file\n");
		return 1;
	}

	// Basic settings
	int n = 0;
	char **lines = get_lines(&n, stream);

	draw();
	play_song(lines, n);

	// Keep application running
	char c = getchar();

	// Reset terminal settings
	fclose(stream);
	exit_screen();
	echo_on();
	show_cursor();
	printf("%s", COLOR_NRM);

	return 0;
}

char **get_lines(int *n, FILE *stream)
{
	char **lines;
	char buffer[BUFFER_SIZE];
	int length = 0;

	// Read and assign n from first line in stream
	fgets(buffer, BUFFER_SIZE, stream);	
	*n = atoi(buffer);

	lines = malloc(*n * sizeof(char *));

	// Fill lines array and eliminate \n character
	for (int i = 0; i < *n; i++)
	{
		fgets(buffer, BUFFER_SIZE, stream);
		length = strlen(buffer);
		buffer[length - 1] = '\0';
		lines[i] = malloc(length * sizeof(char));
		strcpy(lines[i], buffer);
	}

	return lines;
}

void draw()
{
	int rows = 0, cols = 0;
	get_rows_cols(&rows, &cols);

	int col1_min_w = 31, col1_max_w = 98;
	int col1_min_h = 20, col1_max_h = 64;
	int col2_min_w = 31, col2_max_w = 71;
	int col2_min_h = 10, col2_max_h = 20;

	int col1_w = col1_max_w, col1_h = col1_max_h;
	int col2_w = col2_max_w, col2_h = col2_max_h;

	int spacing = 2;

	// Actual drawing
	clear(0, 0, cols, rows);
	// Draw column 1 and 2
	draw_column(col1_w, col1_h, 0, 0);
	//draw_column(col2_w, col2_h, col1_w + spacing, 2);
}

void clear(int x1, int y1, int x2, int y2)
{
	for (int i = y1; i < y2; i++)
	{
		for (int k = x1; k < x2; k++)
		{
			move_cursor(x1 + k, y1 + i);
			printf(" ");
		}
	}
}

void draw_column(int max_w, int max_h, int l_margin, int v_padding)
{
	int i = 0;
	for (i = 0 + v_padding; i < max_w - v_padding; i++)
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
	int rows = 0, cols = 0;
	get_rows_cols(&rows, &cols);

	// Print each line
	for (int i = 0; i < n; i++)
	{
		char *line = lines[i];
		letters = strlen(line);

		print_line_animated(letters, line, 2, iterator + 1);

		// @TODO: optimize this
		// Wrap around
		if (iterator >= rows - 1)
		{
			clear(1, 1, 100, rows - 1);
			iterator = 0;
		}
		iterator++;
	}

	// Free lines
	for (int i = 0; i < n; i++)
		free(lines[i]);	
	free(lines);
}


void print_line_animated(int length, char *line, int x, int y)
{
	bool print = false;	
	char buffer[4] = {'\0'};
	int start = 0, end = 0;
	long int ms;

	// Iterate character by character
	for (int i = 0; i < length; i++)
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
			if (line[i] == '\\') printf("");
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

void delay(int ms) { usleep(ms * 1000); }

char *str_copy_index(char *dest, const char *src, int start, int end, int size)
{
	if (dest == NULL) return NULL;
	if (start > end) return NULL;
	if (end - start > size) return NULL;

	char *ptr = dest;

	// Copy portion of string with pointer arithmetic
	// Inspired by Portfolio Courses: https://portfoliocourses.com/
	while (*src != '\0')
	{
		// Count down start and end to define portion of the string
		if (start == 0)
		{
			*dest = *src;
			dest++;
		}
		else start--;

		if (end == 0) break;
		else end--;

		src++;
	}
	*dest = '\0';
	
	return ptr;
}

void str_replace_index(char *str, const char character, int start, int end)
{
	if (str == NULL) return;
	if (start > end) return;

	while (*str != '\0')
	{
		// Count down start and end to define portion of the string
		if (start == 0)
			*str = character;
		else start--;

		if (end == 0) break;
		else end--;

		str++;
	}
}
