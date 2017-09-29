
#include "sdl_output.hpp"


void updateScreen(SDL_Texture* texture, u32* pixels, screen* s) {

	u32 screenBufferLength = 128 * 128;

	int pitch = 128 * 4;

	u8* screen = s->getPixelBuffer();

	SDL_LockTexture(texture, NULL, (void**)&pixels, &pitch);
	u32 i;
	for (i = 0; i < screenBufferLength; i++) {
		pixels[i] = screen[3*i] << 24 | screen[3*i+1] << 16 | screen[3*i+2] << 8 | 0x00;
	}
	SDL_UnlockTexture(texture);

}

void updateSpeaker(SDL_AudioDeviceID audiodev, u16* samples, u32* samplesLen, u32* nextSample, speaker* s) {
	bool sound_enabled = false;
	if (sound_enabled) {
		u8* samples_u24 = s->getSamplesBuffer();
		u32 max = *samplesLen;
		u32 i;
		for (i = 0; i < max; i+=2) {
			samples[i] = samples_u24[i*3];
			// TODO replace this with proper white/brown noise for not hurting my ears and brain while testing
			if (rand() < RAND_MAX / 2)
				samples[i+1] = samples[i+1] + rand();
			else
				samples[i+1] = samples[i+1] - rand();
		}
		*samplesLen = max;
		*nextSample = 0;
	}
}
