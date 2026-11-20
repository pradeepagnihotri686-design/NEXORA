#indef UTILS_H
#define UTLIS_H

/* Set to 0 if your terminal shows strange characters insted of colours*/
#define USE_COLOR 1

#if USE_COLOR
#define C_RED  "\033[1;31m"
#define C_GREEN "\033[1;32m"
#define C_YELLOW "\033[1;33m"
#define C_CYAN   "\033[1;36m"
#define C_RESET  "\033[0m"
#else
#define C_RED      ""
#define C_GREEN    ""
#define C_YELLOW   ""
#define C_CYAN     ""
#define C_RESET    ""
#endif

void read_string(const char *promt, char *buf , int size);
int read_int(const char *prompt , int min , int max);
float read_float(const char *prompt, float min , float max);
void pause_screen(void);
void print_header(const char *title);

#endif