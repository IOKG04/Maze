/************************************************\
|               Configuration data               |
|                                                |
| Copyright (c) 2024, https://github.com/IOKG04  |
| Licensed under MIT-Festival-Light, available   |
| at https://github.com/IOKG04/Licenses/blob/master/MIT-FL/MIT-Festival-Light |
|                                                |
| This file is exempt from the second condition  |
| described in the License. As a result one is   |
| allowed to change this file without specifying |
| they did so.                                   |
| This is the case, so these constants cannot    |
| limit how a program works, if the creator      |
| does not want to specify they changed MaDiEn.  |
\************************************************/

#ifndef MDE_CONFIG_H__
#define MDE_CONFIG_H__

#include <stdint.h>

/************************************************\
| Whether to show an automated licensing message |
| when setup_screen() is called, and if so for   |
| how long and whether to indicate that the      |
| Software was modified.                         |
|                                                |
| If you wish to disable this, please make sure  |
| to still put the required license notice in    |
| your software.                                 |
\************************************************/

#define MDE_LICENSING_MESSAGE 1            // 0: Dont show license notice; 1: Show license notice
#if MDE_LICENSING_MESSAGE
    #define MDE_LICENSING_MESSAGE_CHANGE 0 // 0: The Software was not modified; 1: The Software was modified
    #define MDE_LICENSING_MESSAGE_CLEAR  1 // 0: Dont clear screen after license notice; 1: Clear screen after license notice
    #define MDE_LICENSING_MESSAGE_DELAY  1 // Number of seconds the license notice is shown for
#endif

// whether to check that indexes are within buffer bounds
// 0: No checks at all
// 1: Check if complete index is within bounds (recommended)
// 2: Check if x and y index are within bounds
#define MDE_BUFFER_INDEX_CHECK 1
// whether to check buffers can be initialized / freed
#define MDE_BUFFER_ALLOC_CHECK 1

// whether to draw SE_NULL as SE_SPACE
#define MDE_PRINT_NULL_AS_SPACE 1

// whether to use true color
#define MDE_TRUE_COLOR 1

// type to use for flags in various functions, might need to be expanded at some point to accomidate more possible flags
typedef uint8_t mde_flags_t;

#endif
