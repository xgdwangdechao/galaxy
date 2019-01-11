///
/// Button.cpp
/// starlight
///
/// Created by reworks on 19/09/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "sl/graphics/Window.hpp"
#include "sl/resources/FontBook.hpp"
#include "sl/core/ServiceLocator.hpp"
#include "sl/libs/entt/signal/dispatcher.hpp"

#include "Button.hpp"

namespace sl
{
	Button::Button(const int x, const int y, const std::array<std::string, 3>& textures, UITheme* theme)
		:Widget({x, y, 0, 0}, theme), m_callback(nullptr)
	{
		// Load each bitmap from the array and check for errors.
		for (auto i = 0; i < 3; ++i)
		{
			m_textures[i] = m_theme->widgetTexture(textures[i]);
			if (!m_textures[i])
			{
				LOG_S(FATAL) << "Failed to create sub bitmap: " << textures[i] << " Errno: " << al_get_errno();
			}
		}

		// Set dimensions.
		m_bounds.m_width = al_get_bitmap_width(m_textures[0]);
		m_bounds.m_height = al_get_bitmap_height(m_textures[0]);

		// Register events.
		sl::Locator::dispatcher->sink<sl::MousePressedEvent>().connect<Button, &Button::receivePress>(this);
		sl::Locator::dispatcher->sink<sl::MouseReleasedEvent>().connect<Button, &Button::receiveRelease>(this);
		sl::Locator::dispatcher->sink<sl::MouseMovedEvent>().connect<Button, &Button::recieveMoved>(this);
	}

	Button::Button(const int x, const int y, const std::string& text, const std::array<ALLEGRO_COLOR, 3>& colors, UITheme* theme)
		:Widget({ x, y, 0, 0 }, theme), m_callback(nullptr)
	{	
		// Find correct button size.
		m_bounds.m_width = al_get_text_width(m_theme->font(), text.c_str());
		m_bounds.m_height = al_get_font_line_height(m_theme->font());

		for (auto i = 0; i < 3; ++i)
		{
			// Create bitmap, draw text to it, then store it. It does not need to be freed because it is freed in the destructor.
			ALLEGRO_BITMAP* bitmap = al_create_bitmap(m_bounds.m_width, m_bounds.m_height);
			al_set_target_bitmap(bitmap);
			al_clear_to_color(al_map_rgba(0, 0, 0, 0));

			al_draw_text(m_theme->font(), colors[i], 0, 0, ALLEGRO_ALIGN_LEFT, text.c_str());

			al_flip_display();
			m_textures[i] = bitmap;
		}

		al_set_target_backbuffer(Locator::window->getDisplay());

		// Register events.
		sl::Locator::dispatcher->sink<sl::MousePressedEvent>().connect<Button, &Button::receivePress>(this);
		sl::Locator::dispatcher->sink<sl::MouseReleasedEvent>().connect<Button, &Button::receiveRelease>(this);
		sl::Locator::dispatcher->sink<sl::MouseMovedEvent>().connect<Button, &Button::recieveMoved>(this);
	}

	Button::Button(const sol::table& table, UITheme* theme)
		:Widget({ 0, 0, 0, 0 }, theme), m_callback(nullptr)
	{
		// Get position data.
		m_bounds.m_x = table.get<int>("x");
		m_bounds.m_y = table.get<int>("y");

		// Get texture data.
		std::array<std::string, 3> textures =
		{
			table.get<std::string>("defaultTexture"),
			table.get<std::string>("pressedTexture"),
			table.get<std::string>("hoverTexture")
		};

		// Load each bitmap from the array and check for errors.
		for (auto i = 0; i < 3; ++i)
		{
			m_textures[i] = m_theme->widgetTexture(textures[i]);
			if (!m_textures[i])
			{
				LOG_S(FATAL) << "Failed to create sub bitmap: " << textures[i] << " Errno: " << al_get_errno();
			}
		}

		// Set dimensions.
		m_bounds.m_width = al_get_bitmap_width(m_textures[0]);
		m_bounds.m_height = al_get_bitmap_height(m_textures[0]);

		// Register events.
		sl::Locator::dispatcher->sink<sl::MousePressedEvent>().connect<Button, &Button::receivePress>(this);
		sl::Locator::dispatcher->sink<sl::MouseReleasedEvent>().connect<Button, &Button::receiveRelease>(this);
		sl::Locator::dispatcher->sink<sl::MouseMovedEvent>().connect<Button, &Button::recieveMoved>(this);
	}

	Button::~Button()
	{
		for (ALLEGRO_BITMAP* texture : m_textures)
		{
			if (texture)
			{
				al_destroy_bitmap(texture);
			}
		}
	}

	void Button::update(const double dt)
	{
		if (m_isVisible)
		{
			if (m_state == Button::State::PRESSED)
			{
				m_callback();
			}
		}
	}

	void Button::render()
	{
		if (m_isVisible)
		{
			// Simply render depending on button state.
			switch (m_state)
			{
			case Button::State::DEFAULT:
				al_draw_bitmap(m_textures[0], m_bounds.m_x, m_bounds.m_y, 0);
				break;

			case Button::State::PRESSED:
				al_draw_bitmap(m_textures[1], m_bounds.m_x, m_bounds.m_y, 0);
				break;

			case Button::State::HOVER:
				al_draw_bitmap(m_textures[2], m_bounds.m_x, m_bounds.m_y, 0);

				if (m_tooltip)
				{
					m_tooltip->draw();
				}
				break;
			}
		}
	}

	void Button::receivePress(const sl::MousePressedEvent& e)
	{
		if (m_isVisible)
		{
			if (contains(e.m_x, e.m_y) && e.m_button == 1)
			{
				m_state = Button::State::PRESSED;
			}
		}
	}

	void Button::receiveRelease(const sl::MouseReleasedEvent& e)
	{
		if (m_isVisible)
		{
			if (contains(e.m_x, e.m_y))
			{
				m_state = Button::State::HOVER;
			}
			else
			{
				m_state = Button::State::DEFAULT;
			}
		}
	}

	void Button::recieveMoved(const sl::MouseMovedEvent& e)
	{
		if (m_isVisible)
		{
			if (contains(e.m_x, e.m_y))
			{
				m_state = Button::State::HOVER;
			}
			else
			{
				m_state = Button::State::DEFAULT;
			}
		}
	}

	void Button::registerCallback(const std::function<void()>& callback)
	{
		m_callback = callback;
	}
}