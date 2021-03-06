///
/// ResourceCache.hpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef PROTOSTAR_RESOURCECACHE_HPP_
#define PROTOSTAR_RESOURCECACHE_HPP_

#include <unordered_map>

#include <pulsar/Log.hpp>

#include "protostar/system/Concepts.hpp"

///
/// Core namespace.
///
namespace pr
{
	///
	/// Container structure for storing resourceCache objects in containers.
	///
	class ResCacheBase
	{
	public:
		///
		/// Default constructor.
		///
		ResCacheBase() noexcept = default;

		///
		/// Virtual default destructor.
		///
		virtual ~ResCacheBase() noexcept = default;
	};

	///
	/// Stores a cache of resources in order to make effective use of memory.
	/// resources can be a texture, sound, script, shader, etc...
	///
	template<not_pointer_or_ref Resource>
	class ResourceCache : public pr::ResCacheBase
	{
	public:
		///
		/// Virtual destructor.
		///
		virtual ~ResourceCache();

		///
		/// Create a resource.
		///
		/// \param name Should be name of resource without path or extension.
		/// \param args Constructor arguments.
		///
		/// \return Pointer to newly created resource.
		///
		template<typename... Args>
		[[nodiscard]] Resource* create(std::string_view name, Args&&... args);

		///
		/// Move a resource into cache.
		///
		/// \param name Should be name of resource without path or extension.
		/// \param resource Move-compatible reference to resource.
		///
		/// \return Pointer to newly created resource.
		///
		[[maybe_unused]] Resource* move(std::string_view name, Resource& resource);

		///
		/// Retrieve a resource.
		///
		/// \param handle The name of the file without path or extension.
		///
		/// \return Returns a pointer to the resource.
		///
		[[nodiscard]] Resource* get(std::string_view handle);

		///
		/// Clean up.
		///
		virtual void clear() = 0;

	protected:
		///
		/// Default constructor.
		///
		ResourceCache() noexcept;

		///
		/// Copy Constructor.
		///
		ResourceCache(const ResourceCache&) noexcept = default;

		///
		/// Move Constructor.
		///
		ResourceCache(ResourceCache&&) noexcept = default;

		///
		/// Contiguous resource array.
		///
		std::unordered_map<std::string, Resource> m_resources;
	};

	template<not_pointer_or_ref Resource>
	inline ResourceCache<Resource>::~ResourceCache()
	{
		clear();
	}

	template<not_pointer_or_ref Resource>
	template<typename... Args>
	inline Resource* ResourceCache<Resource>::create(std::string_view name, Args&&... args)
	{
		const auto str = static_cast<std::string>(name);
		m_resources.emplace(
		    std::piecewise_construct, std::make_tuple(str), std::make_tuple(std::forward<Args>(args)...));

		return &m_resources[str];
	}

	template<not_pointer_or_ref Resource>
	inline Resource* ResourceCache<Resource>::move(std::string_view name, Resource& resource)
	{
		const auto str   = static_cast<std::string>(name);
		m_resources[str] = std::move(resource);

		return &m_resources[str];
	}

	template<not_pointer_or_ref Resource>
	inline Resource* ResourceCache<Resource>::get(std::string_view name)
	{
		const auto str = static_cast<std::string>(name);
		if (m_resources.contains(str))
		{
			return &m_resources[str];
		}
		else
		{
			PL_LOG(PL_WARNING, "Failed to find resource with name: " + str);
			return nullptr;
		}
	}

	template<not_pointer_or_ref Resource>
	inline ResourceCache<Resource>::ResourceCache() noexcept
	{
	}
} // namespace pr

#endif