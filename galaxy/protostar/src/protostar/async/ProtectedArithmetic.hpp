///
/// ProtectedArithmetic.hpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef PROTOSTAR_PROTECTEDARITHMETIC_HPP_
#define PROTOSTAR_PROTECTEDARITHMETIC_HPP_

#include <mutex>

#include "protostar/system/Concepts.hpp"

///
/// Core namespace.
///
namespace pr
{
	///
	/// Protected arithmetic type that has its read/write protected by a lock_guard.
	///
	template<is_arithmetic Type>
	class ProtectedAirthmetic final
	{
	public:
		///
		/// Default constructor.
		///
		ProtectedAirthmetic() noexcept;

		///
		/// Argument constructor.
		///
		/// \param value New value for var.
		///
		explicit ProtectedAirthmetic(const Type value) noexcept;

		///
		/// Deleted move constructor.
		///
		ProtectedAirthmetic(ProtectedAirthmetic&&) = delete;

		///
		/// Deleted copy constructor.
		///
		ProtectedAirthmetic(const ProtectedAirthmetic&) = delete;

		///
		/// Deleted move assignment operator.
		///
		ProtectedAirthmetic& operator=(ProtectedAirthmetic&&) = delete;

		///
		/// Deleted copy assignment operator.
		///
		ProtectedAirthmetic& operator=(const ProtectedAirthmetic&) = delete;

		///
		/// Default destructor.
		///
		~ProtectedAirthmetic() noexcept = default;

		///
		/// Set/update the value.
		///
		/// \param value New value for var.
		///
		void set(const Type value) noexcept;

		///
		/// Get value stored.
		///
		/// \return Mutex protected value.
		///
		[[nodiscard]] const Type get() noexcept;

	private:
		///
		/// The variable.
		///
		Type m_var;

		///
		/// Mutex protecting the variable.
		///
		std::mutex m_mutex;
	};

	template<is_arithmetic Type>
	inline ProtectedAirthmetic<Type>::ProtectedAirthmetic() noexcept
	    : m_var {0}
	{
	}

	template<is_arithmetic Type>
	inline ProtectedAirthmetic<Type>::ProtectedAirthmetic(const Type value) noexcept
	{
		set(value);
	}

	template<is_arithmetic Type>
	inline void ProtectedAirthmetic<Type>::set(const Type value) noexcept
	{
		m_mutex.lock();

		m_var = value;

		m_mutex.unlock();
	}

	template<is_arithmetic Type>
	inline const Type ProtectedAirthmetic<Type>::get() noexcept
	{
		std::lock_guard<std::mutex> lock {m_mutex};
		return m_var;
	}

	///
	/// Predefined type for float.
	///
	using ProtectedFloat = ProtectedAirthmetic<float>;

	///
	/// Predefined type for double.
	///
	using ProtectedDouble = ProtectedAirthmetic<double>;

	///
	/// Predefined type for int.
	///
	using ProtectedInt = ProtectedAirthmetic<int>;

	///
	/// Predefined type for unsigned int.
	///
	using ProtectedUInt = ProtectedAirthmetic<unsigned int>;

	///
	/// Predefined type for bool.
	///
	using ProtectedBool = ProtectedAirthmetic<bool>;
} // namespace pr

#endif
