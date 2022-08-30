/*****************************************************************//**
 * @file   registry.h
 * @brief  The heart of Elang Engine
 * 
 * @author Sedomanai
 * @date   August 2022
 *********************************************************************/

#pragma once

#include <common/fileio.h>
#include <entt/entt.hpp>
#include "../elang_library_builder.h"
#include "serialization.h"

namespace el
{
	using Registry = entt::registry;
	using Entity = entt::entity;
	using RegistrySaver = entt::snapshot;
	using RegistryLoader = entt::snapshot_loader;

	#define NullEntity entt::null
	template<typename T>
	struct asset;
	template<typename T>
	struct obj;


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

    struct Label {
        int index;
        string name;
    };

	ELANG_DLL extern Project* gProject;
	ELANG_DLL extern asset<Stage> gStage;

#define __EL_DEFINE_REGISTRY_NODES(classname, regname) \
    template<typename T> \
    struct classname \
    { \
        template<typename U> \
        friend struct classname; \
\
        classname() : e(NullEntity) {};\
        classname(uint32_t raw_index) : e(static_cast<Entity>(raw_index)) {};\
        classname(const Entity& e_) : e(e_) {};\
        classname(Entity&& e_) : e(e_) {};\
        classname(entt::null_t e_) : e(e_) {};\
\
        operator Entity() const { return e; }\
        operator uint32() const { return (uint32)e; }\
\
        template<typename U>\
        classname(const classname<U>& src) : e(src.e) {};\
        template<typename U>\
        classname(classname<U>&& src) : e(src.e) {};\
        template<typename U>\
        void operator=(const classname<U>& src) { e = src.e; }\
        template<typename U>\
        void operator=(classname<U>&& src) { e = src.e; }\
\
        void operator=(entt::null_t src) { e = src; }\
        void operator=(const Entity& src) { e = src; }\
        void operator=(Entity&& src) { e = src; }\
\
        template<typename U>\
        bool operator==(const classname<U>& right) { return (e == right.e); }\
        template<typename U>\
        bool operator!=(const classname<U>& right) { return (e != right.e); }\
        bool operator==(const Entity& right) { return (e == right); }\
        bool operator!=(const Entity& right) { return (e != right); }\
\
        operator bool() const { return (regname->valid(e) && regname->all_of<T>(e)); }\
        T* operator->() { return &(regname->get<T>(e)); }\
        T& operator *() { return regname->get<T>(e); }\
\
        template<typename U, typename ...Arg>\
        classname<U> add(Arg... args) { regname->emplace<U>(e, args...); return e; }\
        bool valid() const { return regname->valid(e); }\
        void destroy() { regname->destroy(e); }\
\
        bool has() const { return regname->all_of<T>(e); }\
        template<typename U>\
        bool has() const { return regname->all_of<U>(e); }\
        void remove() { regname->remove<T>(e); }\
        template<typename U>\
        void remove() { regname->remove<U>(e); }\
        T& get() { return regname->get<T>(e); }\
        template<typename U>\
        U& get() { return regname->get<U>(e); }\
\
        T& update() { return regname->patch<T>(e); }\
        template<typename U>\
        U& update() { return regname->patch<U>(e); }\
        template<typename ...Arg>\
        void update(Arg... args) { regname->replace<T>(e, args...); }\
        template<typename U, typename ...Arg>\
        void update(Arg... args) { regname->replace<U>(e, args...); }\
\
        Entity entity() { return e; }\
        entt::id_type index() { return static_cast<entt::id_type>(e); }\
\
        template<typename T>\
        void serialize(T& archive) {\
            archive(e);\
        }\
    private:\
        Entity e;\
    };

    __EL_DEFINE_REGISTRY_NODES(asset, gProject)
    __EL_DEFINE_REGISTRY_NODES(obj, gStage)
}