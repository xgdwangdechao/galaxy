///
/// MusicPlayer.cpp
/// galaxy
///
/// Created by reworks on 12/10/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/World.hpp"
#include "galaxy/fs/VirtualFS.hpp"
#include "galaxy/core/ServiceLocator.hpp"

#include "MusicPlayer.hpp"

namespace galaxy
{
	MusicPlayer::MusicPlayer(const std::string& script)
		:m_nullMusic("")
	{
		std::string fullPath = Locator::world->m_scriptFolderPath + script;
		Locator::lua->script(Locator::virtualFS->openAsString(fullPath));
		sol::table music = Locator::lua->get<sol::table>("music");

		if (!music.empty())
		{
			// Iterate over each sound effect in the lua table and construct it in place.
			music.for_each([this](std::pair<sol::object, sol::object> pair)
			{
				m_resourceMap.emplace(entt::HashedString(pair.first.as<const char*>()), pair.second.as<sol::table>());
			});
		}
		else
		{
			LOG_S(WARNING) << "Loaded empty music table for script: " << script;
		}
	}

	MusicPlayer::~MusicPlayer()
	{
		// See clean().
		clean();
	}

	void MusicPlayer::changeVolume(const float volume)
	{
		// Correct volume range.
		float corrected = std::max(0.0f, volume);

		// Adjust volume of the music.
		for (auto& it : m_resourceMap)
		{
			it.second.setVolume(corrected);
		}
	}

	void MusicPlayer::play(const std::string& music)
	{
		get(music.c_str()).play();
	}

	void MusicPlayer::stop(const std::string& music)
	{
		get(music.c_str()).stop();
	}

	void MusicPlayer::resume(const std::string& music)
	{
		get(music.c_str()).resume();
	}

	void MusicPlayer::clean()
	{
		// Music objects are wrapped in classes so destructors handle cleaning of object data.
		m_resourceMap.clear();
	}
}