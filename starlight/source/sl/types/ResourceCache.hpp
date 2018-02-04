///
/// ResourceCache.hpp
/// starlight
///
/// Created by reworks on 17/12/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_RESOURCECACHE_HPP_
#define STARLIGHT_RESOURCECACHE_HPP_

#include <type_traits>
#include <unordered_map>

#include "sl/utils/Utils.hpp"
#include "entt/core/hashed_string.hpp"

namespace sl
{
	template<typename Resource>
	class ResourceCache
	{
	public:
		///
		/// Destructor.
		///
		virtual ~ResourceCache() = default;

		///
		/// Retrieve a resource.
		///
		/// \param name The name of the resource to retrieve.
		///
		/// \return Returns a pointer to the resource.
		///
		inline typename utils::ReturnReferenceIfFalse<std::is_pointer<Resource>::value, Resource>::type get(entt::HashedString id)
		{
			return m_resourceMap[id];
		}

		///
		/// Clean up resources.
		///
		virtual void clean() = 0;

	protected:
		///
		/// Default constructor.
		///
		ResourceCache() = default;

		///
		/// Copy Constructor.
		/// Deleted.
		///
		ResourceCache(const ResourceCache&) = delete;

		///
		/// Move Constructor.
		/// Deleted.
		///
		ResourceCache(ResourceCache&&) = delete;

	protected:
		std::unordered_map<entt::HashedString::hash_type, Resource> m_resourceMap;
	};
}

#endif