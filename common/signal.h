/*****************************************************************//**
 * @file   signal.h
 * @brief  Signal-slot system, like event/delegates
 *		   Possibly deprecated due to entt
 * 
 * @author Sedomanai
 * @date   August 2022
 *********************************************************************/

#pragma once

#include <functional>
#include <vector>

namespace el
{
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
		void connect(T& instance, void(T::* method)(Arg... arg)) {
			mSlots.push_back([&instance, method](Arg... arg) { (instance.*method)(arg...); });
		}
		void clear() { mSlots.clear(); }
	private:
		std::vector<std::function<void(Arg...)>> mSlots;
	};
	
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