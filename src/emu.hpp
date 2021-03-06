
#pragma once

#include <fstream>
#include <iostream>
#include <filesystem>

#include "core/cpu.hpp"
#include "core/mem.hpp"
#include "core/ports.hpp"
#include "core/keys.hpp"
#include "core/screen.hpp"
#include "core/speaker.hpp"
#include "core/timer.hpp"


struct Emu {

	u8 id;
	bool running;

	cpu* c;
	memory* m;
	screen *s;
	speaker *a;
	keys *k;
	timer *t;

	Emu();
	~Emu();

	void Init();
	void DeInit();

	void PowerOn();
	void PowerOff();
	void Reset();

	void Run();
	void Pause();
	void Resume();
	void EndRun();

	void Advance(u32 steps);

	void loadRom(std::string filename);
	void saveRom(std::string filename);

	void saveState(std::string stateFileName);
	void loadState(std::string stateFileName);

	void resetConfig();
	void saveConfig(std::string configFileName);
	void loadConfig(std::string configFileName);

};
