
#include "sdl_app.hpp"
#include "../emu.hpp"

App_SDL::App_SDL() {
    Running = true;  // app, not the emulator

    Window = nullptr;

    e = new Emu;

    WindowRenderer = nullptr;
    WindowTexture = nullptr;
    u32 screenBufferLength = e->s->width * e->s->height;  // in 32 bit depth for SDL
    pixels = new u32[screenBufferLength];

    //want = 0;
    //have = 0;
    audiodev = 0;
    samplesLen = 48000 * 2 * 2 / 24;  // holds enough for 48khz sample rate, 2 channels, 16 bit depth and 1 fps
    samples = new u16[samplesLen];
    sampleNext = 0;

	granurality = 0;
	cyclesPerSecond = 0;
	frames = 0;
	timeFrameStart = 0;
	timeFrameEnd = 0;
	fps = 0;

}

App_SDL::~App_SDL() {
    Window = nullptr;
    WindowRenderer = nullptr;
    WindowTexture = nullptr;
    Running = false;

    delete[] pixels;
    delete[] samples;
    delete e;

}


int ThreadCore1(void* data) {
	UNUSED(data);
	return 0;
}
int ThreadCore2(void* data) {
	UNUSED(data);
	return 0;
}

bool App_SDL::Init() {

    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        return false;
    }
    
    int window_w = e->s->width*2;
    int window_h = e->s->height*2;

    Window = SDL_CreateWindow("Asemu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_w, window_h, SDL_WINDOW_SHOWN );
    if (Window == nullptr) {
        return false;
    }

    // create graphics hw accelerated, blank, RGBA format, won't use alpha channel
    WindowRenderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(WindowRenderer, 0x00, 0x00, 0x00, 0x00);
	WindowTexture = SDL_CreateTexture(WindowRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, e->s->width, e->s->height);

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

    return true;
}

int App_SDL::Run() {
    if (Init() == false) {
        return -1;
    }

    while(Running) {
        timeFrameStart = SDL_GetTicks();
        HandleInput();
        HandleMain();
        HandleRender();
        timeFrameEnd = SDL_GetTicks();
        Sync();
    }

    DeInit();

    return 0;
}

void App_SDL::DeInit() {

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

void App_SDL::HandleInput() {
    SDL_Event Event;
    while(SDL_PollEvent(&Event)) {
        OnEvent(&Event);
    }

}


void App_SDL::HandleMain() {
	e->Advance(e->c->clock_rate / e->s->framerate);
	updateKeyboard(e->k);
	updateScreen(WindowTexture, pixels, e->s);
	updateSpeaker(audiodev, samples, &samplesLen, &sampleNext, e->a);

}

void App_SDL::HandleRender() {

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
    int timeFrame = timeFrameEnd - timeFrameStart;
	int delay_amount = (1000 / e->s->framerate) - timeFrame;
	if (delay_amount < 1)
		delay_amount = 1;
	printf("framerate = %i, delay = %i, used = %i\n", e->s->framerate, delay_amount, timeFrame);
	SDL_Delay(delay_amount);
}
