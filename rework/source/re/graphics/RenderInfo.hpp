///
///  RenderInfo.hpp
///  rework
///
///  Created by reworks on 13/12/2017.
///  Copyright (c) 2017 reworks.
///  Refer to LICENSE.txt for more details.
///

#ifndef REWORK_RENDERINFO_HPP_
#define REWORK_RENDERINFO_HPP_

namespace re
{
	///
	/// A struct to determine rendertype. We dont use enums because we want users to inherit from it.
	///
	/// OCCUPIED VALUES ARE: 0 - 4.
	///
	struct RenderType
	{
		static const int SPRITE = 0;
		static const int TEXT = 1;
		static const int TILEMAP = 2;
		static const int TILEMAP_ANIMATED = 3;
		static const int TILEMAP_IMAGE = 4;
	};

	///
	/// Object containing rendering information
	///
	template<typename Renderable>
	struct RenderInfo
	{
		std::string textureAtlasID;
		int renderType;
		Renderable* renderable;
	};
}

#endif