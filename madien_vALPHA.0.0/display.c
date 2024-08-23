/************************************************\
|          Implementations for display.h         |
|                                                |
| Copyright (c) 2024, https://github.com/IOKG04  |
| Licensed under MIT-Festival-Light, available   |
| at https://github.com/IOKG04/Licenses/blob/master/MIT-FL/MIT-Festival-Light |
\************************************************/

#include "display.h"

#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include "config.h"

// data for old termios
struct termios old_termios;
// flags
mde_flags_t display_flags;


/******************\
| PUBLIC FUNCTIONS |
\******************/

// initializes terminal
void setup_screen(mde_flags_t flags){
    // set termios
    tcgetattr(fileno(stdin), &old_termios);
    struct termios new_termios = old_termios;
    new_termios.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(fileno(stdin), TCSANOW, &new_termios);

    // set flags
    set_flags_screen(flags);

    // apply flags
    reset_screen();

    // show licensing message
    #if MDE_LICENSING_MESSAGE
        // display MaDiEn licensing information
        #if MDE_LICENSING_MESSAGE_CHANGE
            printf("\x1b[2J\x1b[HThis software uses a modified version of MaDiEn Copyright (c) 2024, https://github.com/IOKG04, licensed under the MIT-Festival-Light License (https://github.com/IOKG04/MaDiEn/blob/master/LICENSE).\n");
        #else
            printf("\x1b[2J\x1b[HThis software uses MaDiEn Copyright (c) 2024, https://github.com/IOKG04, licensed under the MIT-Festival-Light License (https://github.com/IOKG04/MaDiEn/blob/master/LICENSE).\n");
        #endif
        sleep(MDE_LICENSING_MESSAGE_DELAY);
        #if MDE_LICENSING_MESSAGE_CLEAR
            printf("\x1b[2J");
        #endif
    #endif
}
// sets display flags
void set_flags_screen(mde_flags_t flags){
    display_flags = flags;
}
// returns current display flags
mde_flags_t get_flags_screen(){
    return display_flags;
}
// resets screen to the definitions given to setup_screen()
void reset_screen(){
    printf("\x1b[22m");
    // hide cursor
    if(!(display_flags & MDE_DCURSOR)) printf("\x1b[?25l");
    // clear screen
    if(!(display_flags & MDE_DNCLEAR)) printf("\x1b[2J");
    // set / unset bold
    if(display_flags & MDE_DBOLD)      printf("\x1b[1m");
    // set cursor position to {0, 0}
    if(!(display_flags & MDE_DNHOME))  printf("\x1b[H");
}
#if MDE_TRUE_COLOR
    // resets foreground color
    void reset_color(){
        printf("\x1b[39m");
    }
#endif
// undos setup_screen()
void revert_screen(){
    // reset termios
    tcsetattr(fileno(stdin), TCSANOW, &old_termios);

    // reset screen
    printf("\x1b[?25h\x1b[0m");
}
