\#pragma once

namespace el {
	template<typename T>
	struct singleton
	{
		static T& get(bool reset = false) {
			static T* instance = nullptr;
			if (reset)
				instance = nullptr;
			else if (!instance)
				instance = new T();
			return (*instance);
		}
	};
}
