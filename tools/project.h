/*****************************************************************//**
 * @file   project.h
 * @brief  Registry that deals with asset management
 *
 * @author Sedomanai
 * @date   August 2022
 *********************************************************************/
#pragma once
#include "registry.h"
#include <filesystem>
#include "../common/define.h"
#include "../elang_builder.h"

namespace el
{
	namespace fio = std::filesystem;
	template<typename T>
	struct asset;

	struct ELANG_DLL Project : Registry
	{
		using Registry::Registry;
		fio::path directory;

		template<typename T, typename... Args>
		asset<T> make(Args... args) {
			auto e = Registry::create();
			Registry::emplace<T>(e, args...);
			return e;
		}
	};

	ELANG_DLL extern Project gProject;

	template<typename T>
	struct asset
	{
		template<typename U>
		friend struct asset;

		asset() : e(NullEntity) {};
		asset(uint32_t raw_index) : e(static_cast<Entity>(raw_index)) {};
		asset(const Entity& e_) : e(e_) {};
		asset(Entity&& e_) : e(e_) {};
		asset(entt::null_t e_) : e(e_) {};

		operator Entity() const { return e; }
		operator uint32() const { return (uint32)e; }

		template<typename U>
		asset(const asset<U>& src) : e(src.e) {};
		template<typename U>
		asset(asset<U>&& src) : e(src.e) {};
		template<typename U>
		void operator=(const asset<U>& src) { e = src.e; }
		template<typename U>
		void operator=(asset<U>&& src) { e = src.e; }

		void operator=(entt::null_t src) { e = src; }
		void operator=(const Entity& src) { e = src; }
		void operator=(Entity&& src) { e = src; }

		template<typename U>
		bool operator==(const asset<U>& right) { return (e == right.e); }
		template<typename U>
		bool operator!=(const asset<U>& right) { return (e != right.e); }
		bool operator==(const Entity& right) { return (e == right); }
		bool operator!=(const Entity& right) { return (e != right); }

		operator bool() const { return (gProject.valid(e) && gProject.all_of<T>(e)); }
		T* operator->() { return &(gProject.get<T>(e)); }
		T& operator *() { return gProject.get<T>(e); }

		template<typename U, typename ...Arg>
		asset<U> add(Arg... args) { gProject.emplace<U>(e, args...); return e; }
		bool valid() const { return gProject.valid(e); }
		void destroy() { gProject.destroy(e); }

		bool has() const { return gProject.all_of<T>(e); }
		template<typename U>
		bool has() const { return gProject.all_of<U>(e); }
		void remove() { gProject.remove<T>(e); }
		template<typename U>
		void remove() { gProject.remove<U>(e); }

		T& get() { return gProject.get<T>(e); }
		template<typename ...Args>
		decltype(auto) addif(Args... args) { return gProject.get_or_emplace<T>(e, args...); }
		template<typename ...Args>
		T& replace(Args... args) { return gProject.emplace_or_replace<T>(e, args...); }
		template<typename U>
		U& get() { return gProject.get<U>(e); }
		template<typename U, typename ...Args>
		decltype(auto) addif(Args... args) { return gProject.get_or_emplace<U>(e, args...); }
		template<typename U, typename ...Args>
		U& replace(Args... args) { return gProject.emplace_or_replace<U>(e, args...); }

		T& update() { return gProject.patch<T>(e); }
		template<typename U>
		U& update() { return gProject.patch<U>(e); }
		template<typename ...Arg>
		void update(Arg... args) { gProject.replace<T>(e, args...); }
		template<typename U, typename ...Arg>
		void update(Arg... args) { gProject.replace<U>(e, args...); }

		Entity entity() { return e; }
		entt::id_type index() { return static_cast<entt::id_type>(e); }

		template<typename T>
		void serialize(T& archive) {
			archive(e);
		}
	private:
		Entity e;
	};
}