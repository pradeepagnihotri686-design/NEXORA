#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

/* Reads a non-empty line safely (no buffer overflow). */
void read_string(const char *prompt, char *buf, int size)
{
    while (1) {
        printf("%s", prompt);
        if (!fgets(buf, size, stdin)) {      /* EOF: quit cleanly */
            printf("\n");
            exit(0);
        }
        if (strchr(buf, '\n') == NULL) {     /* line too long: discard the rest */
            int c;
            while ((c = getchar()) != '\n' && c != EOF) { }
        }
        buf[strcspn(buf, "\n")] = '\0';
        if (buf[0] != '\0')
            return;
        printf(C_RED "Input cannot be empty. Try again.\n" C_RESET);
    }
}

int read_int(const char *prompt, int min, int max)
{
    char line[64], *end;
    while (1) {
        printf("%s", prompt);
        if (!fgets(line, sizeof line, stdin)) {
            printf("\n");
            exit(0);
        }
        long v = strtol(line, &end, 10);
        if (end != line && (*end == '\n' || *end == '\0') && v >= min && v <= max)
            return (int)v;
        printf(C_RED "Invalid input. Enter a whole number between %d and %d.\n" C_RESET, min, max);
    }
}

float read_float(const char *prompt, float min, float max)
{
    char line[64], *end;
    while (1) {
        printf("%s", prompt);
        if (!fgets(line, sizeof line, stdin)) {
            printf("\n");
            exit(0);
        }
        double v = strtod(line, &end);
        if (end != line && (*end == '\n' || *end == '\0') && v >= min && v <= max)
            return (float)v;
        printf(C_RED "Invalid input. Enter a number between %.0f and %.0f.\n" C_RESET, min, max);
    }
}

void pause_screen(void)
{
    char tmp[8];
    printf("\nPress Enter to continue...");
    if (!fgets(tmp, sizeof tmp, stdin))
        exit(0);
}

void print_header(const char *title)
{
    printf("\n" C_CYAN "==================================================\n");
    printf("  %s\n", title);
    printf("==================================================" C_RESET "\n");
}
