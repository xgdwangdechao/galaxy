///
/// Level.cpp
/// starlight
///
/// Created by reworks on 21/11/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "Level.hpp"

namespace sl
{
	Level::Level(const Rect<float, int>& bounds)
	{
	}

	const Rect<float, int>& Level::getBounds() const
	{
		return m_bounds;
	}
}