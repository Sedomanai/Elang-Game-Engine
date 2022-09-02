/*****************************************************************//**
 * @file   stage.h
 * @brief  Registry that deals with in-game objects, per base
 *
 * @author Sedomanai
 * @date   August 2022
 *********************************************************************/
#pragma once
#include "project.h"

namespace el
{
	template<typename T>
	struct obj;

	struct ELANG_DLL Stage : Registry
	{
		using Registry::Registry;

		template<typename T, typename... Args>
		obj<T> make(Args... args) {
			auto e = Registry::create();
			Registry::emplace<T>(e, args...);
			return e;
		}
	};

	ELANG_DLL extern asset<Stage> gStage;

	template<typename T>
	struct obj
	{
		template<typename U>
		friend struct obj;

		obj() : e(NullEntity) {};
		obj(uint32_t raw_index) : e(static_cast<Entity>(raw_index)) {};
		obj(const Entity& e_) : e(e_) {};
		obj(Entity&& e_) : e(e_) {};
		obj(entt::null_t e_) : e(e_) {};

		operator Entity() const { return e; }
		operator uint32() const { return (uint32)e; }

		template<typename U>
		obj(const obj<U>& src) : e(src.e) {};
		template<typename U>
		obj(obj<U>&& src) : e(src.e) {};
		template<typename U>
		void operator=(const obj<U>& src) { e = src.e; }
		template<typename U>
		void operator=(obj<U>&& src) { e = src.e; }

		void operator=(entt::null_t src) { e = src; }
		void operator=(const Entity& src) { e = src; }
		void operator=(Entity&& src) { e = src; }

		template<typename U>
		bool operator==(const obj<U>& right) { return (e == right.e); }
		template<typename U>
		bool operator!=(const obj<U>& right) { return (e != right.e); }
		bool operator==(const Entity& right) { return (e == right); }
		bool operator!=(const Entity& right) { return (e != right); }

		operator bool() const { return (gStage->valid(e) && gStage->all_of<T>(e)); }
		T* operator->() { return &(gStage->get<T>(e)); }
		T& operator *() { return gStage->get<T>(e); }

		template<typename U, typename ...Args>
		obj<U> add(Args... args) { gStage->emplace<U>(e, args...); return e; }
		bool valid() const { return gStage->valid(e); }
		void destroy() { gStage->destroy(e); }

		bool has() const { return gStage->all_of<T>(e); }
		template<typename U>
		bool has() const { return gStage->all_of<U>(e); }
		void remove() { gStage->remove<T>(e); }
		template<typename U>
		void remove() { gStage->remove<U>(e); }
		
		T& get() { return gStage->get<T>(e); }
		template<typename ...Args>
		T& addif(Args... args) { return gStage->get_or_emplace<T>(e, args...); }
		template<typename ...Args>
		T& replace(Args... args) { return gStage->emplace_or_replace<T>(e, args...); }
		template<typename U>
		U& get() { return gStage->get<U>(e); }
		template<typename U, typename ...Args>
		U& addif(Args... args) { return gStage->get_or_emplace<U>(e, args...); }
		template<typename U, typename ...Args>
		U& replace(Args... args) { return gStage->emplace_or_replace<U>(e, args...); }

		T& update() { return gStage->patch<T>(e); }
		template<typename U>
		U& update() { return gStage->patch<U>(e); }
		template<typename ...Arg>
		void update(Arg... args) { gStage->replace<T>(e, args...); }
		template<typename U, typename ...Arg>
		void update(Arg... args) { gStage->replace<U>(e, args...); }

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