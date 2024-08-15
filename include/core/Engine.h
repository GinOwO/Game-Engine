#pragma once

#include <core/Window.h>
#include <core/Input.h>

class Engine {
    private:
	static bool created;

	double FRAME_CAP = 60.0;
	Window window;
	bool running;

	void run();

    public:
	Engine();
	~Engine();
	void start();
	void stop();
	void cleanup();

	Window get_window() const noexcept;
};