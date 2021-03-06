///
/// ShaderBook.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SHADERBOOK_HPP_
#define GALAXY_SHADERBOOK_HPP_

#include <protostar/res/ResourceCache.hpp>
#include <qs/core/Shader.hpp>

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// Resource manager for shaders.
	///
	class ShaderBook final : public pr::ResourceCache<qs::Shader>
	{
	public:
		///
		/// Default constructor.
		///
		ShaderBook() noexcept = default;

		///
		/// JSON constructor.
		///
		/// \param json JSON file to load.
		///
		explicit ShaderBook(std::string_view json);

		///
		/// Destructor.
		///
		~ShaderBook();

		///
		/// Create ShaderBook from JSON.
		///
		/// \param json JSON file to load.
		///
		void create_from_json(std::string_view json);

		///
		/// Clean up.
		///
		void clear() override;
	};
} // namespace galaxy

#endif