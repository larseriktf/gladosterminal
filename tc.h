// Headers

#include <sys/ioctl.h>
#include <termios.h>
#include <signal.h>

// Constants

#define COLOR_NRM "\033[0m"
#define COLOR_FG  "\033[38;2;210;155;20m"
#define COLOR_BG  "\033[48;2;0;0;0m"
#define FX_BLINK  "\033[5m"

// Macros

#define clear_screen() puts("\033[2J")
#define move_cursor(X, Y) printf("\033[%d;%dH", Y+1, X+1)
#define enter_screen() puts("\033[?1049h\033[H")
#define exit_screen() puts("\033[?1049l")
#define hide_cursor() puts("\033[?25l")
#define show_cursor() puts("\033[?25h")

// Function Declarations

void echo_on();
void echo_off();
void get_rows_cols(int *rows, int *cols);

// Functions Implementations

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
