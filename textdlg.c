#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <termios.h>
#include "textdlg.h"

static void msleep(long msec)
{
    struct timespec ts;
    int res;

    if (msec < 0)
    {
        errno = EINVAL;
        return;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do
    {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);
}

int textdlg_show_interval = TEXTDLG_DEFAULT_SHOW_INTERVAL;
bool textdlg_play_sound = false;

static struct termios t_old, t_current;

static char getch(void)
{
    tcgetattr(0, &t_old);
    t_current = t_old;
    t_current.c_lflag &= ~ICANON;
    t_current.c_lflag &= ~ECHO;
    tcsetattr(0, TCSANOW, &t_current);
    char ch = getchar();
    tcsetattr(0, TCSANOW, &t_old);
}

static void show_border(int x, int y, int w, int h, char corner_char, char sides_char, char planes_char)
{
    // Print top and bottom border
    for (int i = 0; i < w + 2; ++i)
    {
        printf("\033[%d;%dH", y - 1, x - 1 + i);
        if (i == 0 || i == w + 1)
            putchar(corner_char);
        else
            putchar(planes_char);

        printf("\033[%d;%dH", y + h, x - 1 + i);
        if (i == 0 || i == w + 1)
            putchar(corner_char);
        else
            putchar(planes_char);
    }

    // Print left and right border
    for (int i = 0; i < h + 1; ++i)
    {
        printf("\033[%d;%dH", y + i, x - 1);
        if (i == h)
            putchar(corner_char);
        else
            putchar(sides_char);

        printf("\033[%d;%dH", y + i, x + w);
        if (i == h)
            putchar(corner_char);
        else
            putchar(sides_char);
    }
}

static void v_show_choice(int x, int y, const char *text, int num_choices, ChoiceDialogHandler handler, va_list args)
{
    textdlg_show_nokey_noclear(x, y, text);

    // Show choices
    for (int i = 0; i < num_choices; ++i)
    {
        char *str = va_arg(args, char *);
        printf("\033[%d;%dH%d. %s", y + 2 + i, x, i + 1, str);
    }

    struct termios old, current;

    // idk what this does
    tcgetattr(0, &old);
    current = old;
    current.c_lflag &= ~ICANON;
    current.c_lflag &= ~ECHO;
    tcsetattr(0, TCSANOW, &current);

    while (true)
    {
        char ch = getchar();       // Get the character
        if (ch > '0' && ch <= '9') // Check if it is within bounds
        {
            int choice = ch - '0';
            if (choice <= num_choices)
            {
                // Call the handler
                tcsetattr(0, TCSANOW, &old);
                handler(choice);
                break;
            }
        }
    }
}

void textdlg_show_nokey_noclear(int x, int y, const char *text)
{
    // Set cursor position
    printf("\033[%d;%dH", y, x);

    int text_len = strlen(text);
    for (int i = 0; i < text_len; ++i)
    {
        if (text[i] == '\n') // When new line, reset X and increment Y
            printf("\033[%d;%dH", ++y, x);
        else
            putchar(text[i]);
        if (textdlg_play_sound)
            putchar('\a');
        fflush(stdout); // Show the character
        msleep(textdlg_show_interval);
    }
}

void textdlg_show(int x, int y, const char *text)
{
    printf("\033[2J"); // Clear screen

    textdlg_show_nokey_noclear(x, y, text);

    // Wait for key press
    getch();
    tcflush(0, TCIFLUSH);
}

void textdlg_show_border(int x, int y, const char *text, char corner_char, char sides_char, char planes_char)
{
    printf("\033[2J"); // Clear screen

    textdlg_show_border_nokey_noclear(x, y, text, corner_char, sides_char, planes_char);

    // Wait for key press
    getch();
    tcflush(0, TCIFLUSH);
}

void textdlg_show_border_nokey_noclear(int x, int y, const char *text, char corner_char, char sides_char, char planes_char)
{
    int num_newlines = 0;
    for (int i = 0; i < strlen(text); ++i)
        if (text[i] == '\n')
            ++num_newlines;
    show_border(x, y, strlen(text), num_newlines + 1, corner_char, sides_char, planes_char);

    textdlg_show_nokey_noclear(x, y, text);
}

void textdlg_show_border_nokey(int x, int y, const char *text, char corner_char, char sides_char, char planes_char)
{
    printf("\033[2J"); // Clear screen

    textdlg_show_border_nokey_noclear(x, y, text, corner_char, sides_char, planes_char);
}

void textdlg_show_border_noclear(int x, int y, const char *text, char corner_char, char sides_char, char planes_char)
{
    textdlg_show_border_nokey_noclear(x, y, text, corner_char, sides_char, planes_char);

    // Wait for key press
    getch();
    tcflush(0, TCIFLUSH);
}

void textdlg_show_nokey(int x, int y, const char *text)
{
    printf("\033[2J"); // Clear screen
    textdlg_show_nokey_noclear(x, y, text);
}

void textdlg_show_noclear(int x, int y, const char *text)
{
    textdlg_show_nokey_noclear(x, y, text);

    // Wait for key press
    getch();
    tcflush(0, TCIFLUSH);
}

void textdlg_show_choice(int x, int y, const char *text, int num_choices, ChoiceDialogHandler handler, ...)
{
    // Check handler
    if (!handler)
    {
        printf("\nError in show_choice_dialog: handler is null\n\n");
        exit(1);
    }

    // Check if number of choices is valid
    if (num_choices < 1 || num_choices > 9)
    {
        printf("\nError in show_choice_dialog: invalid num_choices (%d)\n\n", num_choices);
        exit(1);
    }

    printf("\033[2J");

    va_list args;
    va_start(args, handler);
    v_show_choice(x, y, text, num_choices, handler, args);
    va_end(args);
}

void textdlg_show_choice_border(int x, int y, const char *text, char corner_char, char sides_char, char planes_char, int num_choices, ChoiceDialogHandler handler, ...)
{
    // Check handler
    if (!handler)
    {
        printf("\nError in show_choice_dialog: handler is null\n\n");
        exit(1);
    }

    // Check if number of choices is valid
    if (num_choices < 1 || num_choices > 9)
    {
        printf("\nError in show_choice_dialog: invalid num_choices (%d)\n\n", num_choices);
        exit(1);
    }

    printf("\033[2J");

    int num_newlines = 0;
    for (int i = 0; i < strlen(text); ++i)
        if (text[i] == '\n')
            ++num_newlines;
    show_border(x, y, strlen(text), num_newlines + 2 + num_choices, corner_char, sides_char, planes_char);

    va_list args;
    va_start(args, handler);
    v_show_choice(x, y, text, num_choices, handler, args);
    va_end(args);
}

void textdlg_show_input(int x, int y, const char *text, unsigned int max_chars, InputDialogHandler handler)
{
    // Check handler
    if (!handler)
    {
        printf("\nError in show_input_dialog: handler is null\n\n");
        exit(1);
    }

    textdlg_show_nokey(x, y, text);

    // Input the string
    printf("\033[%d;%dH> ", y + 2, x);
    char *str = (char *)malloc(max_chars * sizeof(char));
    scanf("%[^\n]", str);

    // Call handler
    handler(str);
    free(str);
}

void textdlg_show_input_border(int x, int y, const char *text, unsigned int max_chars, char corner_char, char sides_char, char planes_char, InputDialogHandler handler)
{
    // Check handler
    if (!handler)
    {
        printf("\nError in show_input_dialog: handler is null\n\n");
        exit(1);
    }

    printf("\033[2J");

    int num_newlines = 0;
    for (int i = 0; i < strlen(text); ++i)
        if (text[i] == '\n')
            ++num_newlines;
    show_border(x, y, (strlen(text) > max_chars) ? strlen(text) : max_chars, num_newlines + 3, corner_char, sides_char, planes_char);
    textdlg_show_nokey_noclear(x, y, text);

    // Input the string
    printf("\033[%d;%dH> ", y + 2, x);
    char *str = (char *)malloc(100 * sizeof(char));
    scanf("%[^\n]", str);

    // Call handler
    handler(str);
    free(str);
}
