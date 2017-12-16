
#pragma once

#include <SDL2/SDL.h>

#include "../emu.hpp"

#include "../app.hpp"

#include "../frontend/sdl_input.hpp"
#include "../frontend/sdl_output.hpp"


class App_SDL : App {

public:
	bool Running;

	SDL_Window* Window;  // window management

	SDL_Renderer* WindowRenderer;  // graphics buffer shown in screen
	SDL_Texture* WindowTexture;    // graphics buffer hardware accelerated copied to screen
	u32* pixels;
	//u32 pitch;

	SDL_AudioSpec want, have;  // audio sample specification
	SDL_AudioDeviceID audiodev;     // audio device
	u16* samples;              // holds ring buffer
	u32 samplesLen;            // size of samples ring buffer
	u32 sampleNext;            // ring buffer index

	Emu* e;               // emulator core
	u32 granurality;      // amount of advance of emulation, bigger = more efficient, smaller = more accurate
	u32 cyclesPerSecond;  // emulation speed
	u32 frames;           // emulated frames?
	u32 timeFrameStart;
	u32 timeFrameEnd;
	u32 fps;              // current fps

public:
	App_SDL();
	~App_SDL();

	bool Init();
	void DeInit();

	void HandleInput();
	void HandleMain();
	void HandleRender();

	void OnEvent(SDL_Event* Event);
	void Sync();

	int Run();

};
