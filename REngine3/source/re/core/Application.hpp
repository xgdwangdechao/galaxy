//
//  Application.hpp
//  REngine3
//
//  Created by reworks on 8/07/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef RENGINE3_APPLICATION_HPP_
#define RENGINE3_APPLICATION_HPP_

#include "re/core/World.hpp"
#include "re/services/VFS.hpp"
#include "re/services/Config.hpp"
#include "re/graphics/Window.hpp"
#include "re/managers/FontManager.hpp"
#include "re/managers/StateManager.hpp"
#include "re/managers/DebugManager.hpp"
#include "re/managers/Box2DManager.hpp"

namespace re
{
	class Application
	{
	public:
		///
		/// \brief Default constructor.
		///
		/// Sets up the engine. You need to inherit this and call it from a subclass.
		/// 
		/// \param gravity Sets up the default gravity for Box2D.
		/// 
		Application(int vMajor, int vMinor, int vPatch, double ups, bool saveLog, int width, int height, bool fullscreen, int msaa, int msaaValue, const std::string& title, const std::string& icon, float32 gravity);

		//
		/// Cleans up engine.
		/// 
		~Application();

		///
		/// Return app->run() from your main method.
		///
		/// \return Returns EXIT_SUCCESS or EXIT_FAILURE.
		/// 
		int run();

	private:
		int m_versionMajor;
		int m_versionMinor;
		int m_versionPatch;
		double m_ups;
		bool m_saveLog;
		std::string m_appTitle;

		VFS m_vfs;
		Window m_window;
		World m_world;
		StateManager m_stateManager;
		FontManager m_fontManager;
		Box2DManager m_b2dManager;
		DebugManager m_debugManager;
	};
}

#endif
