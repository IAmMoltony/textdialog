/**
 * @file textdlg.h
 * @author Moltony (serlysus@hotmail.com)
 * @brief Library for creating dialogs like in RPGs in the console
 * @version 1.0
 * @date 2022-11-07
 *
 * @copyright Copyright (c) moltony 2022
 *
 */

#ifdef WIN32
#error textdlg is not supported on windows
#endif

#ifdef TEXTDLG_DEFAULT_SHOW_INTERVAL
#undef TEXTDLG_DEFAULT_SHOW_INTERVAL
#endif

// Default time between showing characters (in ms)
#define TEXTDLG_DEFAULT_SHOW_INTERVAL 70

#include <stdbool.h>

#ifndef TEXTDLG_H_
#define TEXTDLG_H_

/**
 * @brief Handler for textdlg_show_choice
 *
 */
typedef void (*ChoiceDialogHandler)(int);

/**
 * @brief Handler for textdlg_show_input
 *
 */
typedef void (*InputDialogHandler)(char *);

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Interval between showing characters in dialog (in ms)
     *
     */
    extern int textdlg_show_interval;

    /**
     * @brief If set to true, terminal bell will be played every time a character is displayed.
     *
     */
    extern bool textdlg_play_sound;

    /**
     * @brief Show a dialog
     *
     * @param x Text X position
     * @param y Text Y position
     * @param text Text to show
     */
    void textdlg_show(int x, int y, const char *text);

    /**
     * @brief Show a dialog but with a border
     *
     * @param x Text X position
     * @param y Text Y position
     * @param text Text to show
     * @param corner_char Character used for corners
     * @param sides_char Character used for left and right of border
     * @param planes_char Character used for top and bottom of border
     */
    void textdlg_show_border(int x, int y, const char *text, char corner_char, char sides_char, char planes_char);

    /**
     * @brief Show a dialog but with a border, without clearing the screen and without waiting for a key to be pressed
     *
     * @param x Text X position
     * @param y Text Y position
     * @param text Text to show
     * @param corner_char Character used for corners
     * @param sides_char Character used for left and right of border
     * @param planes_char Character used for top and bottom of border
     */
    void textdlg_show_border_nokey_noclear(int x, int y, const char *text, char corner_char, char sides_char, char planes_char);

    /**
     * @brief Show a dialog but with a border, without waiting for a key to be pressed
     *
     * @param x Text X position
     * @param y Text Y position
     * @param text Text to show
     * @param corner_char Character used for corners
     * @param sides_char Character used for left and right of border
     * @param planes_char Character used for top and bottom of border
     */
    void textdlg_show_border_nokey(int x, int y, const char *text, char corner_char, char sides_char, char planes_char);

    /**
     * @brief Show a dialog but with a border, without clearing the screen
     *
     * @param x Text X position
     * @param y Text Y position
     * @param text Text to show
     * @param corner_char Character used for corners of border
     * @param sides_char Character used for left and right of border
     * @param planes_char Character used for top and bottom of border
     */
    void textdlg_show_border_noclear(int x, int y, const char *text, char corner_char, char sides_char, char planes_char);

    /**
     * @brief Show a dialog without clearing the screen and without waiting for a key to be pressed
     *
     * @param x Text X position
     * @param y Text Y position
     * @param text Text to show
     */
    void textdlg_show_nokey_noclear(int x, int y, const char *text);

    /**
     * @brief Show a dialog without waiting for a key to be pressed
     *
     * @param x Text X position
     * @param y Text Y position
     * @param text Text to show
     */
    void textdlg_show_nokey(int x, int y, const char *text);

    /**
     * @brief Show a dialog without clearing the screen
     *
     * @param x Text X position
     * @param y Text Y position
     * @param text Text to show
     */
    void textdlg_show_noclear(int x, int y, const char *text);

    /**
     * @brief Show a dialog where you can choose an option
     *
     * @param x Text X position
     * @param y Text Y position
     * @param text Prompt text
     * @param num_choices Number of choices (up to 9)
     * @param handler Function to call when user makes a choice
     * @param ... Choices
     */
    void textdlg_show_choice(int x, int y, const char *text, int num_choices, ChoiceDialogHandler handler, ...);

    /**
     * @brief Show a dialog where you can choose and option but with a border
     *
     * @param x Text X position
     * @param y Text Y position
     * @param text Prompt text
     * @param corner_char Character used for corners of border
     * @param sides_char Character used for left and right of border
     * @param planes_char Character used for top and bottom of border
     * @param num_choices Number of choices (up to 9)
     * @param handler Function to call when user makes a choice
     * @param ... Choices
     */
    void textdlg_show_choice_border(int x, int y, const char *text, char corner_char, char sides_char, char planes_char, int num_choices, ChoiceDialogHandler handler, ...);

    /**
     * @brief Show a dialog asking the user to input a string
     *
     * @param x Text X position
     * @param y Text Y position
     * @param text Prompt text
     * @param max_chars Maximum characters in input
     * @param handler Function to call when user inputs the string
     */
    void textdlg_show_input(int x, int y, const char *text, unsigned int max_chars, InputDialogHandler handler);

    /**
     * @brief Show a dialog asking the user to input a string but with a border
     *
     * @param x Text X position
     * @param y Text Y position
     * @param text Prompt text
     * @param max_chars Maximum characters in input
     * @param corner_char Character used for corners of border
     * @param sides_char Character used for left and right of border
     * @param planes_char Character used for top and bottom of border
     * @param handler Funuction to call when user inputs the string
     */
    void textdlg_show_input_border(int x, int y, const char *text, unsigned int max_chars, char corner_char, char sides_char, char planes_char, InputDialogHandler handler);

#ifdef __cplusplus
}
#endif

#endif
