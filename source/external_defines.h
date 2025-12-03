/* date = February 21st 2025 6:07 pm */

#include <stdio.h>
#include <stdbool.h>
#include <assert.h>


#include "SDL/SDL.h"
#ifdef __WIN32__

// needed so the compiler MSVC does not thinkg that we are using the SDL_main as an entry point.
#undef main
#include "windows.h"
#endif // __WIN32__

