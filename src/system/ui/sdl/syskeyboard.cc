/* 
 *	PearPC
 *	keyboard.cc - keyboardaccess functions for POSIX
 *
 *	Copyright (C) 1999-2004 Stefan Weyergraf (stefan@weyergraf.de)
 *	Copyright (C) 1999-2004 Sebastian Biallas (sb@biallas.net)
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License version 2 as
 *	published by the Free Software Foundation.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program; if not, write to the Free Software
 *	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <cstdlib>

#include <SDL/SDL.h>
#include <SDL/SDL_keysym.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

#include "system/display.h"
#include "system/keyboard.h"
#include "system/systhread.h"

#include "tools/snprintf.h"

#include "sdlcommon.h"

//#define DPRINTF(a...)
#define DPRINTF(a...) ht_printf("[Display/SDL]: "a)

//fix this to use SDLKeys instead of scancodes
#ifdef __WIN32__

static uint8 sdl_key_to_adb_key[512] = {
	0xff,0x35,0x12,0x13,0x14,0x15,0x17,0x16,0x1a,0x1c,0x19,0x1d,0x1b,0x18,0x33,0x30,
	0x0c,0x0d,0x0e,0x0f,0x11,0x10,0x20,0x22,0x1f,0x23,0x21,0x1e,0x24,0x36,0x00,0x01,
	0x02,0x03,0x05,0x04,0x26,0x28,0x25,0x29,0x27,0xff,0x38,0x2a,0x06,0x07,0x08,0x09,
	0x0b,0x2d,0x2e,0x2b,0x2f,0x2c,0x38,0x43,0x37,0x31,0x39,0x7a,0x78,0x63,0x76,0x60,
	0x61,0x62,0x64,0x65,0x6d,0xff,0xff,0x59,0x5b,0x5c,0x4e,0x56,0x57,0x58,0x45,0x53,
	0x54,0x55,0x52,0x41,0xff,0xff,0xff,0x67,0x6f,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x6f,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x4c,0x36,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0x4b,0xff,0xff,0x3a,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0x47,0xff,0x73,0x3e,0x74,0xff,0x3b,0xff,0x3c,0xff,0x77,
	0x3d,0x79,0x72,0x75,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
};

#else

static uint8 sdl_key_to_adb_key[256] = {
        // 0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15
        0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x35,0x12,0x13,0x14,0x15,0x17,0x16,
        0x1a,0x1c,0x19,0x1d,0x1b,0x18,0x33,0x30,0x0c,0x0d,0x0e,0x0f,0x11,0x10,0x20,0x22,
        0x1f,0x23,0x21,0x1e,0x24,0x36,0x00,0x01,0x02,0x03,0x05,0x04,0x26,0x28,0x25,0x29,
        0x27,0x32,0x38,0x2a,0x06,0x07,0x08,0x09,0x0b,0x2d,0x2e,0x2b,0x2f,0x2c,0x38,0x43,
        0x3a,0x31,0xff,0x7a,0x78,0x63,0x76,0x60,0x61,0x62,0x64,0x65,0x6d,0x47,0xff,0x59,
        0x5b,0x5c,0x4e,0x56,0x57,0x58,0x45,0x53,0x54,0x55,0x52,0x41,0xff,0xff,0xff,0x67,
        0x6f,0x73,0x3e,0x74,0x3b,0xff,0x3c,0x77,0x3d,0x79,0x72,0x75,0x4c,0x36,0xff,0xff,
        0x4b,0x37,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
        0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
        0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
        0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
        0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
        0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
        0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
        0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
        0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
};

#endif

class SDLSystemKeyboard: public SystemKeyboard {
protected:

public:
int getKeybLEDs()
{
	int r = 0;
	SDLMod keyMods = SDL_GetModState();
	if (keyMods & KMOD_NUM)
		r |= KEYB_LED_NUM;
	if (keyMods & KMOD_CAPS)
		r |= KEYB_LED_CAPS;
	/*
	if (keyMods & SDLK_SCROLLOCK)
		r |= KEYB_LED_SCROLL;*/
	return r;
}

void setKeybLEDs(int leds)
{
	int r = getKeybLEDs() ^ leds;
	SDLMod keyMods = SDL_GetModState();

	if (r & KEYB_LED_NUM && leds & KEYB_LED_NUM)
		(int)keyMods |= KMOD_NUM;
	else
		(int)keyMods &= KMOD_NUM;

	if (r & KEYB_LED_CAPS && leds & KEYB_LED_CAPS)
		(int)keyMods |= KMOD_CAPS;
	else
		(int)keyMods &= KMOD_CAPS;
	/*
	if (r & KEYB_LED_SCROLL && leds & KEYB_LED_SCROLL)
		keyMods |= SDLK_SCROLLOCK;
	else
		keyMods &= SDLK_SCROLLOCK;
	*/
	SDL_SetModState(keyMods);
}

bool getEvent(SystemEvent &ev, bool mayBlock)
{
	while (1) {
		SDL_Event event;
		if (mayBlock) {
			SDL_WaitEvent(&event);
		} else {
			if (!SDL_PollEvent(&event)) break;
		}
		switch (event.type) {
		case SDL_KEYUP:
			ev.key.keycode = sdl_key_to_adb_key[event.key.keysym.scancode];
			if ((ev.key.keycode & 0xff) == 0xff) break;
			ev.type = sysevKey;
			ev.key.pressed = false;
			ev.key.chr = event.key.keysym.unicode;
			return true;
		case SDL_KEYDOWN:
			ev.key.keycode = sdl_key_to_adb_key[event.key.keysym.scancode];
			if (event.key.keysym.mod & KMOD_SHIFT){
				if (event.key.keysym.sym == SDLK_F9) {
					if(SDL_WM_GrabInput(SDL_GRAB_QUERY) == SDL_GRAB_OFF) {
						SDL_WM_GrabInput(SDL_GRAB_ON);
						DPRINTF("Grab on!\n");
						break;;
					}
					SDL_WM_GrabInput(SDL_GRAB_OFF);
					DPRINTF("Grab off!\n");
					break;
				}
//				if (event.key.keysym.sym == SDLK_F12) 
//					ToggleFullScreen();
			}
//			if(event.key.keysym.sym == SDLK_F1) { changeCDFunc(); }                                
			if ((ev.key.keycode & 0xff) == 0xff) break;
			ev.type = sysevKey;
			ev.key.pressed = true;
			ev.key.keycode = sdl_key_to_adb_key[event.key.keysym.scancode];
			ev.key.chr = event.key.keysym.unicode;
			return true;
		default:
			handleSDLEvent(event);
		}
	}
	return false;
}

};

SystemKeyboard *allocSystemKeyboard()
{
	if (gKeyboard) return gKeyboard;
	if (!gDisplay) {
		fprintf(stderr, "You must call allocSystemDisplay() first\n");
	}
	return new SDLSystemKeyboard();
}