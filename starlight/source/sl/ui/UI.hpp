///
/// UI.hpp
/// starlight
///
/// Created by reworks on 14/09/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_UI_HPP_
#define STARLIGHT_UI_HPP_

#include <vector>

#include "sl/ui/Panel.hpp"
#include "sl/libs/sol2/sol_forward.hpp"

namespace sl
{
	///
	/// Manages UI for a dynamic array of Panels.
	/// Stores panels as unique_ptrs.
	///
	class UI final
	{
	public:
		///
		/// \brief Constructor.
		///
		/// Automatically registers in-engine widgets for you.
		///
		UI();

		///
		/// Destructor.
		///
		~UI() noexcept;

		///
		/// Add a Panel to the UI.
		///
		/// \param args Arguments for panel to construct.
		///
		/// \return Returns pointer to newly created panel.
		///
		template<typename... Args>
		Panel* addPanel(Args&&... args);

		///
		/// \brief Registers widget types into the ui system.
		///                                                                                                      
		/// You will need to register your custom widgets here.
		/// Automatically registers in-engine widgets for you.
		///
		/// \param widget Name of widget class you are registering.
		///
		template<typename WidgetType>
		void registerWidget(const std::string& widget);

		///
		/// \brief Create a full UI from a lua script.
		///
		/// You need to have a constructor that takes a const sol::table& parameter in your widget class.
		/// Don't forget to register your widget first!
		///
		/// \param luaScript The script needed to create the ui. The script folder is automatically appended!
		/// 
		/// \return Returns an unordered_map with the widgets created, and the key is the widget name. You will need to cast the pointers to the correct type.
		/// 		Each widget name has the structure: <Class>_<Name>. For example in your lua script, you would have Button_MenuQuit. This will create a button with the widget name of MenuQuit.
		///			So in this hashmap you only get the <Name> as the key.
		///
		std::unordered_map<std::string, Widget*> createFromScript(const std::string& luaScript);

		///
		/// Update the UI.
		///
		/// \param dt Delta Time.
		///
		void update(const double dt);

		///
		/// Render UI to screen.
		///
		void render();

	private:
		///
		/// Lua state for the UI.
		///
		sol::state m_lua;

		///
		/// Collection of panels in the ui.
		///
		std::vector<std::unique_ptr<Panel>> m_panels;

		///
		/// Used to allow for widget creation without having to know the widget type.
		///
		std::unordered_map<std::string, std::function<Widget*(Panel* panel, const sol::table&)>> m_widgetFactory;
	};

	template<typename... Args>
	inline Panel* UI::addPanel(Args&&... args)
	{
		// Forward arguments to std::vector's construct in place method.
		Panel* ref = m_panels.emplace_back(std::make_unique<Panel>(std::forward<Args>(args)...)).get();
		if (!ref)
		{
			LOG_S(FATAL) << "Failed to create or emplace panel.";
		}

		// Then return a pointer to object placed.
		return ref;
	}

	template<typename WidgetType>
	inline void UI::registerWidget(const std::string& widget)
	{
		// Make sure there are no duplicate widget types before registering.
		if (m_widgetFactory.find(widget) != m_widgetFactory.end())
		{
			LOG_S(WARNING) << "Attempted to register duplicate widget: " << widget;
		}
		else
		{
			m_widgetFactory.emplace(widget, [this](Panel* panel, const sol::table& table)
			{
				return dynamic_cast<Widget*>(panel->add<WidgetType>(table));
			});
		}
	}
}

#endif