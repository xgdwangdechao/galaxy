///
/// UI.cpp
/// starlight
///
/// Created by reworks on 14/09/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "sl/core/World.hpp"
#include "sl/fs/VirtualFS.hpp"
#include "sl/core/ServiceLocator.hpp"

#include "sl/ui/widgets/Button.hpp"
#include "sl/ui/widgets/Image.hpp"
#include "sl/ui/widgets/Label.hpp"
#include "sl/ui/widgets/ProgressBar.hpp"
#include "sl/ui/widgets/Slider.hpp"
#include "sl/ui/widgets/Textbox.hpp"
#include "sl/ui/widgets/TextInput.hpp"
#include "sl/ui/widgets/ToggleButton.hpp"

#include "UI.hpp"

namespace sl
{
	UI::UI()
	{
		// Register library widgets.
		registerWidget<Button>("Button");
		registerWidget<Image>("Image");
		registerWidget<Label>("Label");
		registerWidget<ProgressBar>("ProgressBar");
		registerWidget<Slider>("Slider");
		registerWidget<Textbox>("Textbox");
		registerWidget<TextInput>("TextInput");
		registerWidget<ToggleButton>("ToggleButton");
	}

	UI::~UI()
	{
		// Ensure unique_ptr's are deconstructed and detroyed.
		m_panels.clear();
	}
	
	void UI::createFromScript(const std::string& luaScript, std::unordered_map<std::string, Widget*>* widgetsMap, std::unordered_map<std::string, UITheme>* themesMap)
	{
		// Load script into lua.
		std::string fullPath = Locator::world->m_scriptFolderPath + luaScript;
		Locator::lua->script(Locator::virtualFS->openAsString(fullPath));

		// Panel currently being created.
		sl::Panel* panel = nullptr;

		// Get master table and check for validity.
		sol::table ui = Locator::lua->get<sol::table>("ui");
		if (!ui.valid() || ui.empty())
		{
			LOG_S(ERROR) << "UI script table \"ui\" is invalid or empty for script: " << luaScript;
		}
		else
		{
			// Create theme(s).
			sol::table themes = ui.get<sol::table>("themes");
			if (!themes.valid() || themes.empty())
			{
				LOG_S(ERROR) << "UI script table \"themes\" is invalid or empty for script: " << luaScript;
			}
			else
			{
				// Load each theme.
				themes.for_each([&](std::pair<sol::object, sol::object> pair)
				{
					sol::table themeTable = pair.second.as<sol::table>();
					if (!themeTable.valid() || themeTable.empty())
					{
						LOG_S(ERROR) << "Themes script table \"" << pair.first.as<std::string>() << "\" is invalid or empty for script: " << luaScript;
					}
					else
					{
						// Retrieve colour.
						ALLEGRO_COLOR themeCol;
						sol::table colTable = themeTable.get<sol::table>("colour");
						if (!colTable.valid() || colTable.empty())
						{
							LOG_S(ERROR) << "Theme table \"colour\" is invalid or empty.";
						}
						else
						{
							themeCol = al_map_rgba
							(
								colTable.get<unsigned char>("r"),
								colTable.get<unsigned char>("g"),
								colTable.get<unsigned char>("b"),
								colTable.get<unsigned char>("a")
							);
						}
						
						// Have to work around std::pair's constructor issues.
						themesMap->emplace(std::piecewise_construct,
							std::forward_as_tuple(pair.first.as<std::string>()),
							std::forward_as_tuple(themeTable.get<std::string>("font"), themeCol, themeTable.get<std::string>("masterTexture")));
					}
				});
			}

			sol::table panelsTable = ui.get<sol::table>("panels");
			if (!panelsTable.valid() || panelsTable.empty())
			{
				LOG_S(ERROR) << "UI script table \"panels\" is invalid or empty for script: " << luaScript;
			}
			else
			{
				// Iterate over each panel.
				panelsTable.for_each([&](std::pair<sol::object, sol::object> pair)
				{
					// Retrieve table, checking validity to ensure loading can proceed.
					sol::table panelTable = pair.second.as<sol::table>();
					if (!panelTable.valid() || panelTable.empty())
					{
						LOG_S(ERROR) << "Panels script table \"" << pair.first.as<std::string>() << "\" is invalid or empty for script: " << luaScript;
					}
					else
					{
						// Now retrieve bounds and check for validity.
						sol::table bounds = panelTable.get<sol::table>("bounds");
						if (!bounds.valid() || bounds.empty())
						{
							LOG_S(ERROR) << "Panel table \"bounds\" is invalid or empty for script: " << luaScript;
						}
						else
						{
							int x = bounds.get<int>("x");
							int y = bounds.get<int>("y");
							int w = bounds.get<int>("w");
							int h = bounds.get<int>("h");

							// Check to see if colour / image exists.
							auto colourCheck = panelTable["colour"];
							auto imageCheck = panelTable["image"];
							if (colourCheck.valid())
							{
								// Make sure colour is not empty.
								sol::table colour = panelTable.get<sol::table>("colour");
								if (!colour.valid() || colour.empty())
								{
									LOG_S(ERROR) << "Panel table \"colour\" is invalid or empty for script: " << luaScript;
								}
								else
								{
									unsigned char r = colour.get<unsigned char>("r");
									unsigned char g = colour.get<unsigned char>("g");
									unsigned char b = colour.get<unsigned char>("b");
									unsigned char a = colour.get<unsigned char>("a");

									panel = addPanel(sl::Rect<int>{x, y, w, h}, al_map_rgba(r, g, b, a));
								}
							}
							else if (imageCheck.valid()) // Else if image is valid.
							{
								std::string image = panelTable.get<std::string>("image");
								if (!image.empty())
								{
									panel = addPanel(sl::Rect<int>{x, y, w, h}, image);
								}
								else
								{
									LOG_S(ERROR) << "Panel table image string is empty for script: " << luaScript;
								}
							}
							else
							{
								LOG_S(ERROR) << "Found no valid image or colour table for UI in script: " << luaScript;
							}
						}

						// Now to load the widgets.
						sol::table widgets = panelTable.get<sol::table>("widgets");
						if (!widgets.valid() || widgets.empty())
						{
							LOG_S(ERROR) << "Panel table \"widgets\" is invalid or empty for script: " << luaScript;
						}
						else
						{
							// Iterate over every widget.
							widgets.for_each([&](std::pair<sol::object, sol::object> pair)
							{
								// Here we are splitting up the full name string into the type and name of the string.
								// So we know what type of widget to create and have a unique identifier for that widget.
								std::string fullName = pair.first.as<std::string>();
								std::size_t markerPos = fullName.find("_");
								std::string widgetName = fullName.substr(markerPos + 1);
								std::string type = fullName.substr(0, markerPos - 1);

								// The table and theme
								sol::table wfTable = pair.second.as<sol::table>();
								UITheme* wfTheme = &(themesMap->at(wfTable.get<std::string>("theme")));

								// Use the assign function to create components for entities without having to know the type.
								Widget* createdWidget = m_widgetFactory[type](panel, wfTable, wfTheme);
								if (!createdWidget)
								{
									LOG_S(ERROR) << "Widget failed to be created: " << fullName;
								}

								widgetsMap->emplace(widgetName, createdWidget);
							});
						}
					}
				});
			}
		}
	}

	void UI::update(const double dt)
	{
		// Iterate over panels, update, and call each widgets update function.
		for (auto& panel : m_panels)
		{
			panel->update(dt);
		}
	}

	void UI::render()
	{
		// Iterate over panels, render, and call each widgets render function.
		for (auto& panel : m_panels)
		{
			panel->render();
		}
	}
}