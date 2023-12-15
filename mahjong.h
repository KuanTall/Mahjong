#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <termios.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>

#define RED "\e[;38;5;196;1m"
#define BLU_BOLD "\x1b[;34;1m"
#define YEL_BOLD "\x1b[;33;1m"
#define GRN "\x1b[;38;5;2;1m"
#define WHT "\x1b[;37;1m"
#define CYAN_BOLD_ITALIC "\x1b[;36;1;3m"
#define RESET "\x1b[0;m"

#define COl 15