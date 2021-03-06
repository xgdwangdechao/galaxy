///
/// Widget.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_WIDGET_HPP_
#define GALAXY_WIDGET_HPP_

#include <protostar/async/ProtectedArithmetic.hpp>

#include "galaxy/ui/Tooltip.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// \brief Interactable UI object i.e. a button.
	///
	/// ANY FUNCTIONS YOU BIND TO AN EVENT ARE NOT CALLED ON THE MAIN THREAD.
	/// DO NOT CALL GL CODE IN EVENT FUNCTIONS.
	///
	class Widget
	{
		friend class GUI;
	public:
		///
		/// Virtual destructor.
		///
		virtual ~Widget() noexcept = default;

		///
		/// Create the tooltip for the widget.
		///
		/// \param theme Pointer to theme object.
		///
		/// \return Pointer to the tooltip.
		///
		Tooltip* addTooltip(Theme* theme) noexcept;

		///
		/// Get ID of this widget.
		///
		/// \return const unsigned int ID value.
		///
		virtual const unsigned int id() const noexcept final;

	protected:
		///
		/// Argument constructor.
		///
		Widget(galaxy::Theme* theme) noexcept;

		///
		/// Copy constructor.
		///
		Widget(const Widget&) noexcept = default;

		///
		/// Move constructor.
		///
		Widget(Widget&&) noexcept = default;

		///
		/// Activate widget.
		///
		virtual void activate() noexcept = 0;

		///
		/// Deactivate widget.
		///
		virtual void deactivate() noexcept = 0;

		///
		/// \brief Update the widget.
		///
		/// YOU MUST NOT CALL ANY GL CODE FROM THIS FUNCTION. THIS FUNCTION IS CALLED FROM A SEPERATE THREAD.
		///
		/// \param dt Delta Time.
		///
		virtual void update(pr::ProtectedDouble* dt) noexcept = 0;

		///
		/// \brief Perform any GL functions on the main thread in prep for rendering.
		///
		/// THIS FUNCTION IS CALLED ON THE MAIN THREAD. PUT YOUR GL CODE HERE.
		///
		/// \param camera Camera projection to apply to GUI.
		///
		virtual void render(qs::Camera& camera) noexcept = 0;

	private:
		///
		/// Deleted default constructor.
		///
		Widget() noexcept = delete;

	protected:
		///
		/// Widget ID.
		///
		unsigned int m_id;

		///
		/// Pointer to theme object.
		///
		Theme* m_theme;

		///
		/// Pointer to the tooltip.
		///
		std::unique_ptr<Tooltip> m_tooltip;
	};

	///
	/// Shorthand.
	///
	using WidgetPtr = std::unique_ptr<galaxy::Widget>;
}

#endif