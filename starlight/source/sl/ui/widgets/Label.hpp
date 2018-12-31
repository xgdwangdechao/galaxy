///
/// Label.hpp
/// starlight
///
/// Created by reworks on 17/12/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_LABEL_HPP_
#define STARLIGHT_LABEL_HPP_

#include "sl/ui/Widget.hpp"

namespace sl
{
	///
	/// Label for UI objects. Does not support tooltips.
	///
	class Label final : public Widget
	{
	public:
		///
		/// Text Constructor.
		///
		/// \param x x pos relative to UI panel.
		/// \param y y pos relative to UI panel.
		/// \param text Label text.
		/// \param theme Theme to use with widget.
		///
		Label(const int x, const int y, const std::string& text, UITheme* theme);
		
		///
		/// Lua Constructor.
		///
		/// \param table sol::table to create widget from.
		/// \param theme Theme to use with widget.
		///
		Label(const sol::table& table, UITheme* theme);

		///
		/// Destructor.
		///
		~Label() noexcept;

		///
		/// Update the widget.
		///
		/// \param dt Delta Time.
		///
		void update(const double dt) override;

		///
		/// Render the widget.
		///
		/// Does not render the tooltip.
		///
		void render() override;

		///
		/// Changes current text.
		///
		/// \param text New text to set.
		///
		void setText(const std::string& text);

	private:
		///
		/// Default constructor.
		/// Deleted.
		///
		Label() = delete;

	private:
		///
		/// Text to display.
		///
		std::string m_text;
	};
}

#endif