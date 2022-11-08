
# TextDialog

TextDialog (or textdlg) is a library for creating dialogs like the ones seen in RPGs.
This library is only availailable for POSIX-compatible systems (that includes linux).

## Features

- Showing dialogs at different speeds, X and Y positions and with a border
- Showing dialogs with a choice (up to 9 options)
- Showing dialogs where the user can input a string

## Building

If you are using a build system, just add textdlg.h into the includes directory and add textdlg.c into the source directory.

If you want to see the demo, run:
`gcc demo.c textdlg.c -o demo && ./demo`
