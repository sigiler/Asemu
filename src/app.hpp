
#pragma once

struct App {

	App() {};
	virtual ~App() {};

	virtual bool Init() = 0;
	virtual void DeInit() = 0;

	virtual void HandleInput() = 0;
	virtual void HandleMain() = 0;
	virtual void HandleRender() = 0;

	/*
	bool Running;
	int RunMain() {
	    if (Init() == false) {
	        return -1;
	    }

	    while(Running) {
	        HandleInput();
	        HandleMain();
	        HandleRender();
	    }

	    DeInit();

	    return 0;
	}
	*/

};
