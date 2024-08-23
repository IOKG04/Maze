/************************************************\
|           Terminal utility functions           |
|                                                |
| Copyright (c) 2024, https://github.com/IOKG04  |
| Licensed under MIT-Festival-Light, available   |
| at https://github.com/IOKG04/Licenses/blob/master/MIT-FL/MIT-Festival-Light |
\************************************************/

#ifndef MDE_DISPLAY_H__
#define MDE_DISPLAY_H__

#include "config.h"

// sets font as bold
#define MDE_DBOLD      0b00000001
// does NOT clear screen
#define MDE_DNCLEAR    0b00000010
// shows cursor
#define MDE_DCURSOR    0b00000100
// does NOT set cursor position to {0, 0}
#define MDE_DNHOME     0b00001000
// uses true color (if enabled in config.h)
#define MDE_DTRUECOLOR 0b00010000

// recommended default flags for TUI applications
#define MDE_DTUI (0)
// recommended default flags for ascii graphics
#define MDE_DGRAPHICS (MDE_DBOLD | MDE_DTRUECOLOR)

// initializes terminal with given flags
void setup_screen(mde_flags_t flags);
// sets display flags
void set_flags_screen(mde_flags_t flags);
// returns current display flags
mde_flags_t get_flags_screen();
// resets screen to current display flags
void reset_screen();
#if MDE_TRUE_COLOR
    // resets foreground color
    void reset_color();
#endif
// undos setup_screen()
void revert_screen();

#endif
