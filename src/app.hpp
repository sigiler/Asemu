
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
	    if (!Init()) {
	        return EXIT_FAILURE;
	    }
	    while(Running) {
	        HandleInput();
	        HandleMain();
	        HandleRender();
	    }
	    DeInit();
	    return EXIT_SUCCESS;
	}
	*/

};
