///
/// RenderComponent.hpp
/// starlight
///
/// Created by reworks on 12/01/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_RENDERCOMPONENT_HPP_
#define STARLIGHT_RENDERCOMPONENT_HPP_

#include <vector>

#include "sol2/sol_forward.hpp"

namespace sl
{
	class RenderComponent final
	{
	public:
		///
		/// Constructor.
		///
		/// \param table sol::table containing data.
		/// 
		RenderComponent(const sol::table& table);

		///
		/// Destructor.
		///
		~RenderComponent() = default;

		///
		/// Default move assignment overload.
		///
		RenderComponent& operator=(const RenderComponent&);

	private:
		///
		/// Default constructor.
		/// Deleted.
		///
		RenderComponent() = delete;

	public:
		std::vector<unsigned int> m_renderTypes;
	};
}

#endif