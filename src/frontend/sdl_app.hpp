
#pragma once

#include <SDL2/SDL.h>

#include "../app.hpp"
#include "../emu.hpp"
#include "sdl_input.hpp"
#include "sdl_output.hpp"

// move this to proper place
//#define MULTI_CORE

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

#ifdef MULTI_CORE
	cpu* p1;
	SDL_Thread* threadP1;
	cpu* p2;
	SDL_Thread* threadP2;
#endif

public:
	App_SDL();
	~App_SDL();

	bool OnInit();
	void OnDeInit();

	void OnEvent(SDL_Event* Event);
	void OnLoop();
	void OnRender();
	void Sync();

	int Run();

};
