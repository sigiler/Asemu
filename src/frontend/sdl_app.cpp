
#include "sdl_app.hpp"

App_SDL::App_SDL() {
    Running = true;  // app, not the emulator

    Window = nullptr;

    WindowRenderer = nullptr;
    WindowTexture = nullptr;
    u32 screenBufferLength = 128 * 128;
    pixels = new u32[screenBufferLength];

    //want = 0;
    //have = 0;
    audiodev = 0;
    samplesLen = 48000 * 2 * 2 / 60;  // holds enough for 2/60 seconds
    samples = new u16[samplesLen];
    sampleNext = 0;

	granurality = 0;
	cyclesPerSecond = 0;
	frames = 0;
	timeFrameStart = 0;
	timeFrameEnd = 0;
	fps = 0;

    e = new Emu;

#ifdef MULTI_CORE
    p1 = new cpu;
    p2 = new cpu;
    threadP1 = nullptr;
    threadP2 = nullptr;
#endif
}

App_SDL::~App_SDL() {
    Window = nullptr;
    WindowRenderer = nullptr;
    WindowTexture = nullptr;
    Running = false;

    delete[] pixels;
    delete[] samples;
    delete e;

#ifdef MULTI_CORE
    delete p1;
	delete p2;
    threadP1 = nullptr;
    threadP2 = nullptr;
#endif
}


int ThreadCore1(void* data) {
	return 0;
}
int ThreadCore2(void* data) {
	return 0;
}

bool App_SDL::OnInit() {

    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        return false;
    }

    Window = SDL_CreateWindow("Emuplat", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 640, SDL_WINDOW_SHOWN );
    if (Window == nullptr) {
        return false;
    }

    // create graphics hw accelerated, blank, RGBA format, won't use alpha channel
    WindowRenderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(WindowRenderer, 0x00, 0x00, 0x00, 0x00);
	WindowTexture = SDL_CreateTexture(WindowRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 128, 128);

	// create sound system
	SDL_memset(&have, 0, sizeof(have));
	SDL_memset(&want, 0, sizeof(want));
	want.freq = 48000;
	want.format = AUDIO_S16;
	want.channels = 2;
	want.samples = 4096;
	want.callback = NULL;      // queue instead of callback
	want.userdata = &samples;
	audiodev = SDL_OpenAudioDevice(NULL, 0, &want, &have, 0);  // possibly disallow changes and make SDL convert for us
    SDL_PauseAudioDevice(audiodev, 0);  // start play audio

	e->Init();

#ifdef MULTI_CORE
	p1 = new cpu;
	threadP1 = SDL_CreateThread(ThreadCore1, "Extra Core 1", p1);
	p2 = new cpu;
	threadP2 = SDL_CreateThread(ThreadCore2, "Extra Core 2", p2);
#endif

    return true;
}

int App_SDL::Run() {
    if (OnInit() == false) {
        return -1;
    }

    SDL_Event Event;

    while(Running) {
        timeFrameStart = SDL_GetTicks();
        while(SDL_PollEvent(&Event)) {
            OnEvent(&Event);
        }
        OnLoop();
        OnRender();
        timeFrameEnd = SDL_GetTicks();
        Sync();
    }

    OnDeInit();

    return 0;
}

void App_SDL::OnDeInit() {

	SDL_DestroyTexture(WindowTexture);
	WindowTexture = nullptr;

	SDL_DestroyRenderer(WindowRenderer);
	WindowRenderer = nullptr;

	SDL_DestroyWindow(Window);
	Window = nullptr;

    SDL_CloseAudioDevice(audiodev);
    audiodev = 0;

    SDL_Quit();
}



void App_SDL::OnEvent(SDL_Event* Event) {
    if (Event->type == SDL_QUIT) {
        Running = false;
    }
    if (Event->type == SDL_KEYDOWN) {

    }
    if (Event->type == SDL_KEYUP) {

    }
}


void App_SDL::OnLoop() {

	e->Advance(e->c->clock_rate / 60);
	updateKeyboard(e->k);
	updateScreen(WindowTexture, pixels, e->s);
	updateSpeaker(audiodev, samples, &samplesLen, &sampleNext, e->a);

}

void App_SDL::OnRender() {

	// clear screen
	//SDL_RenderClear(WindowRenderer);

	// draw
	SDL_RenderCopy(WindowRenderer, WindowTexture, NULL, NULL);

	// update screen
	SDL_RenderPresent(WindowRenderer);

	// queue sound samples
	SDL_QueueAudio(audiodev, samples, samplesLen);

}

void App_SDL::Sync() {
	// slow down simulation by delaying next
	// also generally good idea to give some time back to OS
	int delay_amount = (1000 / 60) - (timeFrameEnd - timeFrameStart);
	if (delay_amount < 1)
		delay_amount = 1;
	printf("delay = %i, used = %i\n", delay_amount, timeFrameEnd - timeFrameStart);
	SDL_Delay(delay_amount);
}
