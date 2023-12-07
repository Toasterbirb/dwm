#include "snd.h"
#include <SDL2/SDL_mixer.h>
#include <stdio.h>

void snd_init()
{
	int flags = MIX_INIT_OPUS;
	int initted = Mix_Init(MIX_INIT_OPUS);
	if ((initted&flags) != flags)
	{
		fprintf(stderr, "Failed to initialize SDL_mixer\n");
		return;
	}

	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) != 0)
	{
		fprintf(stderr, "Failed to initialize SDL_mixer\n");
		return;
	}

	Mix_Volume(-1, MIX_MAX_VOLUME * 0.75);
}

void* snd_load(char* file_path)
{
	return Mix_LoadWAV(file_path);
}

void snd_play(void* sound)
{
	Mix_PlayChannel(-1, sound, 0);
}

void snd_destroy(void* sound)
{
	Mix_FreeChunk(sound);
}
