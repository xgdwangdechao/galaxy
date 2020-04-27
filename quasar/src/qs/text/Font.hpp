///
/// Font.hpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_FONT_HPP_
#define QUASAR_FONT_HPP_

#include <string>
#include <unordered_map>

#include "qs/text/FreeType.hpp"
#include "qs/text/Character.hpp"

///
/// Core namespace.
///
namespace qs
{
	///
	/// A font.
	///
	class Font final
	{
	public:
		///
		/// Constructor.
		///
		Font() noexcept;

		///
		/// \brief Argument constructor.
		///
		/// Calls create().
		///
		/// \param file Font file to load.
		/// \param size Size to set the font at.
		///
		explicit Font(const std::string& file, const int size);

		///
		/// Destructor.
		///
		~Font() noexcept;

		///
		/// Create the font.
		///
		/// \param file Font file to load.
		/// \param size Size to set the font at.
		///
		void create(const std::string& file, const int size);

		///
		/// Retrieve width of a string of text.
		///
		/// \param text Text to get width of.
		///
		const int getTextWidth(const std::string& text);
		
		///
		/// Retrieve height of a string of text.
		///
		/// \param text Text to get height of.
		///
		const int getTextHeight(const std::string& text);
		
		///
		/// Get characters.
		///
		/// \return Returns reference to an unordered_map. Char is key, value is qs::Character.
		///
		std::unordered_map<char, qs::Character>& getChars() noexcept;

	private:
		///
		/// Stores mapped characters.
		///
		std::unordered_map<char, qs::Character> m_characterMap;
	};
}

#endif