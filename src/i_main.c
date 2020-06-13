//
// Copyright(C) 1993-1996 Id Software, Inc.
// Copyright(C) 2005-2014 Simon Howard
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// DESCRIPTION:
//	Main program, simply calls D_DoomMain high level loop.
//

#include "config.h"

#include <stdio.h>
#include <stdlib.h>

#include "SDL.h"

#include "doomtype.h"
#include "i_system.h"
#include "m_argv.h"

#ifdef __PSP__
#include <pspuser.h>
#include <pspdebug.h>

PSP_MODULE_INFO("DOOM", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER | PSP_THREAD_ATTR_VFPU);
PSP_HEAP_SIZE_MAX();

int psp_exit_callback(int arg1, int arg2, void *common)
{
	exit(0);
	return 0;
}

int psp_callback_thread(SceSize args, void *argp)
{
	int callbackId = sceKernelCreateCallback("Exit callback", psp_exit_callback, NULL);
	sceKernelRegisterExitCallback(callbackId);
	sceKernelSleepThreadCB();
	return 0;
}

int psp_setup_callbacks(void)
{
	int threadId = sceKernelCreateThread("Callback exit thread", psp_callback_thread, 0x11, 0xFA0, THREAD_ATTR_USER, 0);
	if(threadId >= 0)
    {
		sceKernelStartThread(threadId, 0, 0);
    }
	return threadId;
}
#endif

//
// D_DoomMain()
// Not a globally visible function, just included for source reference,
// calls all startup code, parses command line options.
//

void D_DoomMain (void);

int main(int argc, char **argv)
{
#ifdef __PSP__
    pspDebugScreenInit();
    psp_setup_callbacks();
    atexit(sceKernelExitGame);
#endif

    // save arguments

    myargc = argc;
    myargv = argv;

    //!
    // Print the program version and exit.
    //
    if (M_ParmExists("-version") || M_ParmExists("--version")) {
        printf("%s\n", PACKAGE_STRING);
        exit(0);
    }

#if defined(_WIN32)
    // compose a proper command line from loose file paths passed as arguments
    // to allow for loading WADs and DEHACKED patches by drag-and-drop
    M_AddLooseFiles();
#endif

    M_FindResponseFile();

    #ifdef SDL_HINT_NO_SIGNAL_HANDLERS
    SDL_SetHint(SDL_HINT_NO_SIGNAL_HANDLERS, "1");
    #endif

    // start doom

    D_DoomMain ();

    return 0;
}

