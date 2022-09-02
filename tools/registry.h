/*****************************************************************//**
 * @file   registry.h
 * @brief  The heart of Elang Engine
 * 
 * @author Sedomanai
 * @date   August 2022
 *********************************************************************/
#pragma once
#include <entt/entt.hpp>

namespace el
{
	using Registry = entt::registry;
	using Entity = entt::entity;
	using RegistrySaver = entt::snapshot;
	using RegistryLoader = entt::snapshot_loader;

	#define NullEntity entt::null
}