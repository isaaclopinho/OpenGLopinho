#pragma once

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "theoraplay.h"
#ifdef __APPLE__
#include <SDL2/SDL.h>
#include <sys/uio.h>
#else
#include <SDL.h>
#include <io.h>
#endif

using namespace std;

typedef struct AudioQueue {
	const THEORAPLAY_AudioPacket *audio;
	int offset;
	struct AudioQueue *next;
} AudioQueue;

class Movie
{
public:

	Movie();
	~Movie();
	static void ResetVars();
	static void SDLCALL audio_callback(void * userdata, Uint8 * stream, int len);
	static void queue_audio(const THEORAPLAY_AudioPacket * audio);
	static void playfile(const char * fname, SDL_Window * screen, SDL_Renderer * renderer);
};

