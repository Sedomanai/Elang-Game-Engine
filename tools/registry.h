// The heart of Elang Engine

#pragma once

#include <entt/entt.hpp>

#include "../elang_library_builder.h"
#include "serialization.h"

namespace el
{
    using Registry = entt::registry;
    using RegistrySaver = entt::snapshot;
    using RegistryLoader = entt::snapshot_loader;
    using Entity = entt::entity;
    using Observer = entt::observer;
#define NullEntity entt::null

    template<typename T>
    struct asset;
    template<typename T>
    struct obj;

    struct ELANG_DLL Project : Registry
    {
        using Registry::Registry;

        string name, assetDir, datDir, srcDir, entryStage;
        bihashmap<string, Entity> textures, atlases, models, fonts,
            sfx, bgm, events, texts, materials, painters, cameras, misc;
        bool internalBinary;

        template<typename T, typename... Args>
        asset<T> makeSub(Args... args) {
            auto e = Registry::create();
            Registry::emplace<T>(e, args...);
            return e;
        }

        template<typename T, typename... Args>
        asset<T> make(bihashmap<string, Entity>& labels, const string& key, Args... args) {
            if (!labels.contains(key)) {
                auto e = Registry::create();
                Registry::emplace<T>(e, args...);
                labels.emplace(key, e);
                return e;
            } return labels[key];
        }

        template<typename ...Arg>
        void save(OutArchive& archive) const {
            RegistrySaver shot(*this);
            archive(internalBinary);
            shot.entities(archive).component<Arg...>(archive);
            archive(name, assetDir, datDir, srcDir, entryStage,
                textures, atlases, models, fonts, sfx, bgm, events, texts, materials, painters, cameras, misc);
        }
        template<typename ...Arg>
        void load(InArchive& archive) {
            clear();
            RegistryLoader shot(*this);
            archive(internalBinary);
            shot.entities(archive).component<Arg...>(archive);
            archive(name, assetDir, datDir, srcDir, entryStage,
                textures, atlases, models, fonts, sfx, bgm, events, texts, materials, painters, cameras, misc);
        }
        void reset() {
            clear();
            name = assetDir = datDir = srcDir = entryStage = "";
            textures.clear();
            atlases.clear();
            models.clear();
            fonts.clear();
            sfx.clear();
            bgm.clear();
            events.clear();
            texts.clear();
            materials.clear();
            painters.clear();
            cameras.clear();
            misc.clear();
        }
    };

    struct ELANG_DLL Stage : Registry
    {
        using Registry::Registry;

        string name, project;
        bihashmap<string, Entity> named;
        vector<Entity> atelier;
        //bihashmap<string, vector<Entity>> namedGroups; // TODO: can't because of vector hash function, find another way

        template<typename T, typename... Args>
        obj<T> make(Args... args) {
            auto e = create();
            Registry::emplace<T>(e, args...);
            return e;
        }

        template<typename T, typename... Args>
        obj<T> makeNamed(const string& key, Args... args) {
            if (!named.contains(key)) {
                auto e = Registry::create();
                Registry::emplace<T>(e, args...);
                named.emplace(key, e);
                return e;
            } return named[key];
        }

        template<typename T>
        void serializeProject(T& archive) { archive(project); }

        template<typename ...Arg>
        void save(OutArchive& archive) const {
            RegistrySaver shot(*this);
            shot.entities(archive).component<Arg...>(archive);
            archive(named);
        }
        template<typename ...Arg>
        void load(InArchive& archive) {
            clear();
            RegistryLoader shot(*this);
            shot.entities(archive).component<Arg...>(archive);
            archive(named);
        }
        //template<typename T, typename... Args>
        //obj<T> makeForGroup(const string& key, Args... args) {
        //    auto e = Registry::create();
        //    Registry::emplace<T>(e, args...);
        //    namedGroups[key].emplace_back(e);
        //    return e;
        //}
    };

    ELANG_DLL extern Project* gProject;
    ELANG_DLL extern Stage* gStage;
    inline void bind(Project& project) { gProject = &project; }
    inline void bind(Stage& stage) { gStage = &stage; }

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

    template<typename T, typename ...Arg>
    inline void connectObserver(Observer& observer) {
        observer.connect(*gStage, entt::collector.update<T>().where<Arg...>());
    }
}