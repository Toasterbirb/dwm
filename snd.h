#pragma once

void snd_init();
void* snd_load(char* file_path);
void snd_play(void* sound);
void snd_destroy(void* sound);
