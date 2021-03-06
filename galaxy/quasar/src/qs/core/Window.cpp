///
/// Window.cpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#include <iostream>
#include <filesystem>

#include <glad/glad.h>
#include <pulsar/Log.hpp>
#include <stb_image.h>
#include <stb_image_write.h>

#include "qs/core/WindowSettings.hpp"

#include "Window.hpp"

///
/// Core namespace.
///
namespace qs
{
	Window::Window() noexcept
	    : m_window {nullptr}, m_cursor {nullptr}, m_width {0}, m_height {0}, m_colour {1.0f, 1.0f, 1.0f, 1.0f}
	{
	}

	Window::Window(std::string_view title, const pr::positive_integer auto w, const pr::positive_integer auto h)
	    : m_window {nullptr}, m_cursor {nullptr}, m_width {0}, m_height {0}, m_colour {1.0f, 1.0f, 1.0f, 1.0f}
	{
		if (!create(title, w, h))
		{
			PL_LOG(PL_FATAL, "GLFW window creation failed.");
		}
	}

	Window::~Window() noexcept
	{
		// Call again to ensure everything is cleaned up.
		// Has checks to ensure no null data is destroyed.
		destroy();
	}

	bool Window::create(std::string_view title, const pr::positive_integer auto w, const pr::positive_integer auto h)
	{
		// Function result.
		bool result = true;

		// Window w/h.
		m_width  = w;
		m_height = h;

		// Error callbacks.
		glfwSetErrorCallback([](int error, const char* description) {
			PL_LOG(PL_ERROR, "[GLFW] Code: {0}. Desc: {1}.", error, description);
		});

		// Init glfw.
		if (!glfwInit())
		{
			PL_LOG(PL_FATAL, "Failed to initialize glfw!");
		}
		else
		{
			// Set the OpenGL flags.
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);

// Set debug when compiling for debug mode.
#ifdef _DEBUG
			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

			// Window related hints.
			glfwWindowHint(GLFW_RESIZABLE, true);
			glfwWindowHint(GLFW_VISIBLE, true);
			glfwWindowHint(GLFW_DECORATED, true);
			glfwWindowHint(GLFW_FOCUSED, true);

			// Framebuffer stuff
			glfwWindowHint(GLFW_RED_BITS, 8);
			glfwWindowHint(GLFW_GREEN_BITS, 8);
			glfwWindowHint(GLFW_BLUE_BITS, 8);
			glfwWindowHint(GLFW_ALPHA_BITS, 8);
			glfwWindowHint(GLFW_DEPTH_BITS, 24);
			glfwWindowHint(GLFW_STENCIL_BITS, 8);
			glfwWindowHint(GLFW_DOUBLEBUFFER, true);
			glfwWindowHint(GLFW_SRGB_CAPABLE, true);

			// MSAA
			glfwWindowHint(GLFW_SAMPLES, qs::WindowSettings::s_anti_aliasing);

			// Create the window from input, ensuring it is centered in the screen.
			m_window = glfwCreateWindow(m_width, m_height, static_cast<std::string>(title).c_str(), nullptr, nullptr);

			// Then if the window failed to create:
			if (!m_window)
			{
				PL_LOG(PL_FATAL, "Failed to create window.");
				result = false;
			}
			else
			{
				// Set window context and aspect ratio.
				glfwMakeContextCurrent(m_window);

				if (!(qs::WindowSettings::s_aspect_ratio_x == -1 || qs::WindowSettings::s_aspect_ratio_y == -1))
				{
					glfwSetWindowAspectRatio(m_window, qs::WindowSettings::s_aspect_ratio_x, qs::WindowSettings::s_aspect_ratio_y);
				}

				// Set up glad.
				if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
				{
					PL_LOG(PL_FATAL, "Failed to init glad.");
					result = false;
				}
				else
				{
					// Set internal pointer references.
					glfwSetWindowUserPointer(m_window, reinterpret_cast<void*>(this));

					// Set resize callback.
					glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int w, int h) {
						static_cast<qs::Window*>(glfwGetWindowUserPointer(window))->resize(w, h);
					});

					// Set vsync.
					glfwSwapInterval(qs::WindowSettings::s_vsync);

					// Raw mouse input.
					if (glfwRawMouseMotionSupported())
					{
						glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, qs::WindowSettings::s_raw_mouse_input);
					}

// Error handling.
#ifdef _DEBUG
					glEnable(GL_DEBUG_OUTPUT);
					// Callback.

					glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) -> void {
						PL_LOG(PL_WARNING, "[OpenGL] {0}.", message);
					},
							       nullptr);
#endif

					// Enable MSAA.
					glEnable(GL_MULTISAMPLE);

					// Depth testing.
					//glEnable(GL_DEPTH_TEST);

					// Set up blending.
					glEnable(GL_BLEND);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

					// Allow for changing vertex point size.
					glEnable(GL_PROGRAM_POINT_SIZE);

					// sRGB colours for rendering.
					if (qs::WindowSettings::s_srgb)
					{
						glEnable(GL_FRAMEBUFFER_SRGB);
					}

					// Print OpenGL version.
					PL_LOG(PL_INFO, "OpenGL v{0}.", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
				}
			}
		}

		return result;
	}

	void Window::set_window_background(pr::Colour& col)
	{
		m_colour = col.get_normalized();
	}

	void Window::set_icon(std::string_view icon)
	{
		stbi_set_flip_vertically_on_load(true);

		// Always convert to proper path before loading.
		auto path = std::filesystem::path {icon};

		// Fill glfw-compatible struct.
		GLFWimage img;
		img.pixels = stbi_load(path.string().c_str(), &img.width, &img.height, nullptr, STBI_rgb_alpha);
		if (!img.pixels)
		{
			PL_LOG(PL_ERROR, "Failed to load image: {0}.", path.string());
		}
		else
		{
			// Copies data so safe to destroy.
			glfwSetWindowIcon(m_window, 1, &img);
		}

		stbi_image_free(img.pixels);
	}

	void Window::set_icon(std::span<unsigned char> buffer)
	{
		stbi_set_flip_vertically_on_load(true);

		// Fill glfw-compatible struct.
		GLFWimage img;
		img.pixels = stbi_load_from_memory(buffer.data(), buffer.size_bytes(), &img.width, &img.height, nullptr, STBI_rgb_alpha);

		if (!img.pixels)
		{
			PL_LOG(PL_ERROR, "Failed to load image for window icon from memory.");
		}
		else
		{
			// Copies data so safe to destroy.
			glfwSetWindowIcon(m_window, 1, &img);
		}

		stbi_image_free(img.pixels);
	}

	void Window::set_cursor_visibility(const bool visible) noexcept
	{
		visible == true ? glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL)
				: glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}

	void Window::remove_cursor() noexcept
	{
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	void Window::set_cursor_icon(std::string_view icon)
	{
		stbi_set_flip_vertically_on_load(true);

		// Always convert to proper path before loading.
		auto path = std::filesystem::path {icon};

		// Fill glfw-compatible struct.
		GLFWimage img;
		img.pixels = stbi_load(path.string().c_str(), &img.width, &img.height, nullptr, STBI_rgb_alpha);

		if (!img.pixels)
		{
			PL_LOG(PL_ERROR, "Failed to load image: {0}.", path.string());
		}
		else
		{
			// Copies data so safe to destroy.
			m_cursor = glfwCreateCursor(&img, 0, 0);
			glfwSetCursor(m_window, m_cursor);
		}

		stbi_image_free(img.pixels);
	}

	void Window::set_cursor_icon(std::span<unsigned char> buffer)
	{
		stbi_set_flip_vertically_on_load(true);

		// Fill glfw-compatible struct.
		GLFWimage img;
		img.pixels = stbi_load_from_memory(buffer.data(), buffer.size_bytes(), &img.width, &img.height, nullptr, STBI_rgb_alpha);

		if (!img.pixels)
		{
			PL_LOG(PL_ERROR, "Failed to load image for cursor icon from memory.");
		}
		else
		{
			// Copies data so safe to destroy.
			m_cursor = glfwCreateCursor(&img, 0, 0);
			glfwSetCursor(m_window, m_cursor);
		}

		stbi_image_free(img.pixels);
	}

	void Window::set_scrolling_callback(GLFWscrollfun func) noexcept
	{
		glfwSetScrollCallback(m_window, func);
	}

	void Window::destroy() noexcept
	{
		// Clean up window data, checking to make sure its not already been destroyed.
		if (m_window != nullptr)
		{
			glfwDestroyWindow(m_window);
			m_window = nullptr;
		}

		// Cursor
		if (m_cursor != nullptr)
		{
			glfwDestroyCursor(m_cursor);
		}

		glfwTerminate();
	}

	bool Window::is_open() const noexcept
	{
		return (!glfwWindowShouldClose(m_window));
	}

	void Window::close() noexcept
	{
		glfwSetWindowShouldClose(m_window, true);
	}

	void Window::resize(const pr::positive_integer auto w, const pr::positive_integer auto h) noexcept
	{
		m_width  = w;
		m_height = h;

		glfwSetWindowSize(m_window, w, h);
	}

	void Window::request_attention() noexcept
	{
		glfwRequestWindowAttention(m_window);
	}

	void Window::begin() noexcept
	{
		glViewport(0, 0, m_width, m_height);
		glClearColor(m_colour[0], m_colour[1], m_colour[2], m_colour[3]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Window::end() noexcept
	{
		glfwSwapBuffers(m_window);
	}

	void Window::poll_events() noexcept
	{
		glfwPollEvents();
	}

	GLFWwindow* Window::gl_window() noexcept
	{
		return m_window;
	}

	const int Window::get_width() const noexcept
	{
		return m_width;
	}

	const int Window::get_height() const noexcept
	{
		return m_height;
	}
} // namespace qs