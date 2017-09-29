
#pragma once

struct App {

	App();
	~App();

	bool OnInit();
	void OnDeInit();

	void OnEvent();
	void OnLoop();
	void OnRender();

	int RunMain();

};
