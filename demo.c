#include "textdlg.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef void (*ChoiceDialogHandler)(int);
typedef void (*InputDialogHandler)(char *);

// Handler for choice dialog
void dlg_handler(int choice)
{
    // Choices are numbered 1-9

    if (choice == 1)
        textdlg_show_nokey(10, 10, "You picked apples");
    else if (choice == 2)
        textdlg_show_nokey(10, 10, "You picked bananas");

    sleep(1);
    textdlg_show_noclear(10, 12, "Good choice!");
}

// Handler for name input dialog
void name_dlg_handler(char *name)
{
    // Print "Hello, <name>!"
    char *str = (char *)malloc(45 * sizeof(char));
    strcpy(str, "Hello, ");
    strcat(str, name);
    strcat(str, "!");
    textdlg_show_nokey(10, 10, str);
    printf("\n\n");
}

int main(void)
{
    // Basic dialogs
    textdlg_show(10, 10, "Hello world");
    textdlg_show(20, 20, "This is another dialog");

    // Dialog with a border
    // Corner character: +
    // Character for left and right side: |
    // Character for top and bottom: -
    textdlg_show_border(20, 10, "look\ni have a border", '+', '|', '-');

    // Make interval higher, so that the text appears to show up slower
    textdlg_show_interval = 200;
    textdlg_show(12, 12, "I appear slowly.............");

    // Restore interval
    textdlg_show_interval = TEXTDLG_DEFAULT_SHOW_INTERVAL;

    // Show a dialog with 2 choices: Apples and Bananas
    textdlg_show_choice(15, 15, "Which do you like more?", 2, dlg_handler, "Apples", "Bananas");

    // Show dialog with sound
    textdlg_play_sound = true;
    textdlg_show(10, 10, "hello hello hello!!");
    textdlg_play_sound = false;

    // Show a dialog asking to input your name
    textdlg_show_input(10, 10, "Enter your name", 30, name_dlg_handler);

    return 0;
}
