/*****************************************************************//**
 * @file   signal.h
 * @brief  Extremely simple signal-slot system, like event/delegates
 *
 * @author Sedomanai
 * @date   August 2022
 *********************************************************************/
#pragma once
#include <functional>

namespace el
{
	/**
	 * @brief Signal
	 * @brief Able to connect to slots of type function pointer, std::function, and/or a pointer to an object and its method
	 */
	template<typename ...Arg>
	struct signal
	{
		void invoke(Arg... arg) {
			for (auto& s : mSlots) {
				s(arg...);
			}
		}
		void connect(std::function<void(Arg...)> function) {
			mSlots.push_back(function);
		}
		void connect(void(*function)(Arg... arg)) {
			mSlots.push_back(function);
		}
		template<typename T>
		void connect(T* instance, void(T::* method)(Arg... arg)) {
			mSlots.push_back([instance, method](Arg... arg) { ((*instance).*method)(arg...); });
		}
		void clear() { mSlots.clear(); }
	private:
		std::vector<std::function<void(Arg...)>> mSlots;
	};

	/**
	 * @brief Signal that can only connect to one Slot
	 * @brief Able to connect to slots of type function pointer, std::function, and/or a pointer to an object and its method
	 */
	template<typename ...Arg>
	struct single_signal
	{
		void invoke(Arg... arg) {
			if (mSlot) mSlot(arg...);
		}
		void connect(std::function<void(Arg...)> function) {
			mSlot = function;
		}
		void connect(void(*function)(Arg... arg)) {
			mSlot = function;
		}
		template<typename T>
		void connect(T& instance, void(T::* method)(Arg... arg)) {
			mSlot = [&instance, method](Arg... arg) { (instance.*method)(arg...); };
		}
		void disconnect() { mSlot.swap(); }
	private:
		std::function<void(Arg...)> mSlot;
	};
}