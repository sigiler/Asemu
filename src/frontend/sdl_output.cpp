
#include "sdl_output.hpp"
#include <random>

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


std::default_random_engine generator;
std::uniform_real_distribution<float> distribution(-128.0, 128.0);

void whiteNoise(float* out, u32 length) {
    for (u32 i = 0; i < length; i++) {
        out[i] = distribution(generator);
    }
}

// m = number of interpolated samples
void redNoise(float* out, u32 length, u32 m) {
	whiteNoise(out, length);
    for (u32 i=0; i<length/m-1; i++) {
        u32 k = i*m;
        u32 l = ((i+1)*m <= length-1) ? (i+1)*m : length-1;
        double d = (out[l] - out[k])/((float)(m));
        for (u32 j=0; j<m; j++) {
            out[k+j] = out[k] + d*j;
        }
    }
}

void updateSpeaker(SDL_AudioDeviceID audiodev, u16* samples, u32* samplesLen, u32* nextSample, speaker* s) {
	UNUSED(audiodev);
	bool sound_enabled = false;
	bool noise_testing = true;    // right speaker gets noise for testing SDL sound purpose

	if (sound_enabled) {
		u8* samples_u24 = s->getSamplesBuffer();
		u32 max = *samplesLen;
		u32 i;
		u32 sampleL, sampleR;

		float* noise = new float[max];
		redNoise(noise, max, 10);

		for (i = 0; i < max; i+=2) {
			// convert s24 to s16
			sampleL = samples_u24[i*3] | samples_u24[i*3+1] << 8 | samples_u24[i*3+2] << 16;
			sampleL = sampleL / 256;
			sampleR = samples_u24[i*3+3] | samples_u24[i*3+4] << 8 | samples_u24[i*3+5] << 16;
			sampleR = sampleR / 256;
			// left speaker
			samples[i+0] = sampleL;
			// right speaker
			samples[i+1] = sampleR;
			if (noise_testing) {
				samples[i+1] = (u16)noise[i+1];
			}
		}
		*samplesLen = max;
		*nextSample = 0;
	}
}
