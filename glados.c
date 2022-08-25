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

char **get_lines(int *n, FILE *stream);
void str_replace_index(char *str, const char character, int start, int end);
char *str_copy_index(char *dest, const char *src, int start, int end, int size);
void delay(int ms);
void print_line_animated(int length, char *line);
void play_song(FILE *stream, int bpm);
void draw_column(int max_w, int max_h, int l_margin, int v_paddng);
void draw();
int main();

/* Function Implementation */

int main()
{
	// Set terminal settings
	signal(SIGWINCH, &draw);
	enter_screen();
	echo_off();
	hide_cursor();

	// Load lyrics file
	FILE *stream = NULL;
	stream = fopen("lyrics.txt", "r");

	if (!stream)
	{
		printf("error opening file\n");
		return 1;
	}

	draw();

	// play_song(stream, 120);

	// Keep application running
	char c = getchar();

	// Reset terminal settings
	fclose(stream);
	exit_screen();
	echo_on();
	show_cursor();

	return 0;
}

void draw()
{
	// Basic settings
	int rows = 0, cols = 0, i = 0, k = 0;
	get_rows_cols(&rows, &cols);
	clear_screen();
	printf("%s%s", COLOR_FG, COLOR_BG);

	int col1_max_w = 71, col1_max_h = 30;
	int col1_min_w = 30, col1_min_h = 30;
	int col2_max_w = 72, col2_max_h = 20;
	int col2_min_w = 30, col2_min_h = 30;

	// Actual drawing
	// Fill background with black
	for (i = 0; i < rows; i++)
	{
		for (k = 0; k < cols; k++) printf(" ");
		printf("\n");
	}

	// First column
	/*
	for (i = 0; i < col1_max_w; i++)
	{
		if (i % 2 == 0)
		{
			move_cursor(i, 0);
			printf("_");
			move_cursor(i, col1_max_h);
			printf("_");
		}
	}
	for (i = 2; i < col1_max_h; i++)
	{
		if (i % 2 == 0)
		{
			move_cursor(0, i);
			printf("|");
			move_cursor(col1_max_w, i);
			printf("|");
		}
	}
	*/

	draw_column(col1_max_w, col1_max_h, 0, 0);
	printf("\n");
	draw_column(col2_max_w, col2_max_h, col1_max_w + 2, 2);

	// Second column
	/*
	for (i = 0; i < col2_max_w; i++)
	{
		if (i % 2 == 0)
		{
			move_cursor(i + (col1_max_w + 2), 0);
			printf("_");
			move_cursor(i + (col1_max_w + 2), col2_max_h);
			printf("_");
		}
	}
	for (i = 2; i < col2_max_h; i++)
	{
		if (i % 2 == 0)
		{
			move_cursor(0 + (col1_max_w + 2), i);
			printf("|");
			move_cursor(col2_max_w + (col1_max_w + 2), i);
			printf("|");
		}
	}
	*/

	// Reset effects
	printf("%s", COLOR_NRM);
}

void draw_column(int max_w, int max_h, int l_margin, int v_paddng)
{
	for (int i = 0; i < max_w; i++)
	{
		if (i % 2 == 0)
		{
			move_cursor(i + l_margin, 0);
			printf("_");
			move_cursor(i + l_margin, max_h);
			printf("_");
		}
	}
	for (int i = 2; i < max_h; i++)
	{
		if (i % 2 == 0)
		{
			move_cursor(l_margin, i);
			printf("|");
			move_cursor(l_margin + max_w, i);
			printf("|");
		}
	}
}

void play_song(FILE *stream, int bpm)
{
	int n = 0;
	int letters = 0;
	char **lines = get_lines(&n, stream);

	printf("\033[38;2;255;0;0m");
	printf("\033[48;2;0;0;0m");

	// Print each line
	for (int i = 0; i < n; i++)
	{
		char *line = lines[i];

		letters = strlen(line);
		print_line_animated(letters, line);
	}

	printf("\033[0m\n");

	// Free lines
	for (int i = 0; i < n; i++)
		free(lines[i]);	
	free(lines);
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

void print_line_animated(int length, char *line)
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
			// If buffer is empty, prepare for printing
			if (buffer[0] == '\0')
			{
				str_copy_index(buffer, line, start + 1, end - 1, 4);
				str_replace_index(line, '~', start, end);
				ms = strtol(buffer, NULL, 10);
			}

			// Gradually print message with delay
			delay(ms);
			printf("\r");
			for (int j = 0; j <= i; j++)
			{
				if (line[j] == '~') continue;
				if (line[j] == '\\') printf("");
				else printf("%c", line[j]);
			}
			fflush(stdout);
		}
	}
	printf("\n");
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
