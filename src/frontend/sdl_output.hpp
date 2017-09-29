
#pragma once

#include <SDL2/SDL.h>
#include "../core/screen.hpp"
#include "../core/speaker.hpp"

void updateScreen(SDL_Texture* texture, u32* pixels, screen* s);
void updateSpeaker(SDL_AudioDeviceID audiodev, u16* samples, u32* samplesLen, u32* nextSample, speaker* s);
