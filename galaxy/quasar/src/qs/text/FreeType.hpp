///
/// FreeType.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_FREETYPE_HPP_
#define QUASAR_FREETYPE_HPP_

#include <ft2build.h>
#include FT_FREETYPE_H

///
/// Shortcut Macro.
///
#define FTLIB qs::FreeTypeLib::handle()

///
/// Core namespace.
///
namespace qs
{
	///
	/// Wraps around freetype library initialization.
	///
	class FreeTypeLib final
	{
	public:
		///
		/// Destructor.
		///
		~FreeTypeLib() noexcept = default;

		///
		/// Get handle to library.
		///
		/// \return Refernce to this static instance.
		///
		static FreeTypeLib& handle() noexcept;

		///
		/// Open library.
		///
		void open();

		///
		/// Close library.
		///
		void close() noexcept;

		///
		/// Handle to FT library.
		///
		/// \return Reference to FT_Library.
		///
		[[nodiscard]] FT_Library& lib() noexcept;

	private:
		///
		/// Constructor.
		///
		FreeTypeLib() noexcept = default;

	private:
		///
		/// FreeType library structure.
		///
		FT_Library m_freetype_lib;
	};
} // namespace qs

#endif