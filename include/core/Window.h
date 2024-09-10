#pragma once

#include <misc/glad.h>
#include <GLFW/glfw3.h>

#include <core/Input.h>

#include <string>

class Window {
    public:
	Window(const Window &) = delete;
	Window &operator=(const Window &) = delete;

	static Window &get_instance();

    private:
	GLFWwindow *window;
	Window();

    public:
	~Window();
	bool set_window_context();
	bool gl_create_window();
	void terminate_window();
	bool should_close();
	void swap_buffers();

	void set_key_callback(void (*key_callback)(GLFWwindow *, int, int, int,
						   int));
	void set_mouse_callback(
		void (*mouse_motion_callback)(GLFWwindow *, double, double),
		void (*mouse_button_callback)(GLFWwindow *, int, int, int),
		void (*mouse_scroll_callback)(GLFWwindow *, double, double));

	void set_focus_callback(void (*handle_window_focus)(GLFWwindow *, int));

	int get_window_height() const noexcept;
	int get_window_width() const noexcept;

	void set_mouse_position(double x, double y);
};