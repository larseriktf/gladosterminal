// Headers

#include <sys/ioctl.h>
#include <termios.h>
#include <signal.h>

// Constants

#define COLOR_NRM "\033[0m"
#define COLOR_FG  "\033[38;2;210;155;20m"
#define COLOR_BG  "\033[48;2;0;0;0m"

// Macros

#define clear_screen() puts("\033[2J")
#define move_cursor(X, Y) printf("\033[%d;%dH", Y, X)
#define enter_screen() puts("\033[?1049h\033[H")
#define exit_screen() puts("\033[?1049l")

// Function Declarations

void echo_off();
void echo_on();
void signal_resize();
void get_rows_cols(int *rows, int *cols);
void draw();

// Functions Implementations

void signal_resize()
{
	draw();
}

void draw()
{
	int rows = 0;
	int cols = 0;

	get_rows_cols(&rows, &cols);

	clear_screen();
	printf("%s%s", COLOR_FG, COLOR_BG);
	move_cursor(8, 10);
	//printf("%s%sSome text! ... \n%s", COLOR_FG, COLOR_BG, COLOR_NRM);
	printf("Some text! :D %d, %d\n", rows, cols);
	printf("%s", COLOR_NRM);
}

void get_rows_cols(int *rows, int *cols)
{
	struct winsize size;
	ioctl(1, TIOCGWINSZ, &size);
	*rows = size.ws_row;
	*cols = size.ws_col;
}

void echo_off()
{
	struct termios term;
	tcgetattr(1, &term);
	term.c_lflag &= -ECHO;
	tcsetattr(1, TCSANOW, &term);
}

void echo_on()
{
	struct termios term;
	tcgetattr(1, &term);
	term.c_lflag |= ECHO;
	tcsetattr(1, TCSANOW, &term);
}
