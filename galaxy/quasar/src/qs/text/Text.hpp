///
/// Text.hpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_TEXT_HPP_
#define QUASAR_TEXT_HPP_

#include "qs/text/Font.hpp"
#include "qs/core/Colours.hpp"
#include "qs/graphics/Sprite.hpp"
#include "qs/core/RenderTexture.hpp"

///
/// Core namespace.
///
namespace qs
{
	///
	/// Forward dec(s).
	///
	class Window;
	class Shader;
	class Renderer;

	///
	/// Creates text for use with OpenGL.
	///
	class Text final
	{
	public:
		///
		/// Constructor.
		///
		Text() noexcept;

		///
		/// Argument constructor.
		///
		/// \param text Text to draw.
		/// \param font Font to apply to text. Holds a reference, does not store.
		/// \param col Colour of the text.
		///
		explicit Text(const std::string& text, qs::Font* font, protostar::Colour& col) noexcept;

		///
		/// Default destructor.
		///
		~Text() noexcept = default;

		///
		/// Load resources used.
		///
		/// \param text Text to draw.
		/// \param font Font to apply to text. Holds a reference, does not store.
		/// \param col Colour of the text.
		///
		void load(const std::string& text, qs::Font* font, const protostar::Colour& col) noexcept;

		///
		/// \brief Create text from inputs.
		///
		/// This draws text to a single image for performance, so this function is best done during loading.
		///
		/// \param window The window to restore framebuffer to.
		/// \param renderer Renderer to use when drawing text.
		/// \param shader Shader to use when drawing text to texture.
		///
		void create(qs::Window& window, qs::Renderer& renderer, qs::Shader& shader) noexcept;

		///
		/// \brief Update text.
		///
		/// Do not call load() or create(). Use this.
		/// Not thread safe. Calls GL code.
		/// If you need to have a string you want to update i.e. health: 20.
		/// Its better to split into 2 strings so health is static and the 20 is dynamic.
		/// Much faster.
		///
		/// \param text The new text.
		/// \param window The window to restore framebuffer to.
		/// \param renderer Renderer to use when drawing text.
		/// \param shader Shader to use when drawing text to texture.
		///
		void updateText(const std::string& text, qs::Window& window, qs::Renderer& renderer, qs::Shader& shader) noexcept;

		///
		/// Get text as a sprite.
		///
		/// \return Reference to a qs::Sprite.
		///
		qs::Sprite& asSprite() noexcept;

		///
		/// Get internal render texture.
		///
		/// \param Reference to qs::RenderTexture.
		///
		qs::RenderTexture& getTexture() noexcept;

	private:
		///
		/// Text.
		///
		std::string m_text;

		///
		/// Font.
		///
		qs::Font* m_font;

		///
		/// Colour.
		///
		protostar::Colour m_colour;

		///
		/// RenderTexture to draw to.
		///
		qs::RenderTexture m_texture;

		///
		/// Sprite variant of text.
		///
		qs::Sprite m_sprite;
	};
}

#endif