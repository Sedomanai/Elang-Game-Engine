/*****************************************************************//**
 * @file   material.h
 * @brief  Graphic materials. 
 *		   Elang Engine materials do not hold shader data. That is reserved for Painters. 
 *		   However, materials do hold Uniform/TextureSampler data and can be used across multiple Painters with different shaders
 *		   This is possible because unmatching Uniforms/TextureSamplers are only discarded and not thrown
 * 
 * @author Sedomanai
 * @date   August 2022
 *********************************************************************/
#pragma once
#include "tool_declarer.h"
#include "../common/define.h"
#include "../common/enums.h"
#include "../elang_builder.h"

namespace el
{
	struct Texture;
	struct NullMeta;
	/**
	 * @brief Shader Uniform data. Ideally works for any kind of graphic interface. Currently only GLSL is proven to work
	 */
	struct Uniform
	{
		// Type of data
		eDataType type;
		// 0 for Vertex Shader, 1 for Fragment Shader
		uint32 vertex;
		// Name of data, must match name of variable in shader, otherwise discarded
		string name;
		void* data;

		Uniform(eDataType type_, uint32 vertex_, void* data_, const string& name_)
			: type(type_), vertex(vertex_), data(data_), name(name_) {
		};
		Uniform() : type(eDataType::Float), vertex(1), data(0), name("uSomething__NULL_") {};

		template<typename T>
		void save(T& archive) const {
			archive(type, vertex, name);
			switch (type) {
				case eDataType::Float: archive(*(reinterpret_cast<float*>(data))); break;
				case eDataType::Vec2: archive(*(reinterpret_cast<vec2*>(data))); break;
				case eDataType::Vec3: archive(*(reinterpret_cast<vec3*>(data))); break;
				case eDataType::Vec4: archive(*(reinterpret_cast<vec4*>(data))); break;
				case eDataType::Matrix4: archive(*(reinterpret_cast<matrix4x4*>(data))); break;
			};
		}

		template<typename T>
		void load(T& archive) {
			archive(type, vertex, name);
			switch (type) {
				case eDataType::Float: data = malloc(sizeof(float)); archive(*(reinterpret_cast<float*>(data))); break;
				case eDataType::Vec2: data = malloc(sizeof(vec2)); archive(*(reinterpret_cast<vec2*>(data))); break;
				case eDataType::Vec3: data = malloc(sizeof(vec3)); archive(*(reinterpret_cast<vec3*>(data))); break;
				case eDataType::Vec4: data = malloc(sizeof(vec4)); archive(*(reinterpret_cast<vec4*>(data))); break;
				case eDataType::Matrix4: data = malloc(sizeof(matrix4x4)); archive(*(reinterpret_cast<matrix4x4*>(data))); break;
			}
		}
	};

	/**
	 * @brief Holds uniform and texture/sampler2d data. Does not hold shader data, that's reserved for painters.
	 */
	struct ELANG_DLL Material
	{
		vector<Uniform> uniforms;
		vector<asset<Texture>> textures;

		void addUniform(eDataType type_, uint32 vertex_, void* data_, const string& name_);
		// Check if it has a valid texture (in index 0)
		void setTexture(asset<Texture> texture, sizet index = 0);
		// Check if it has a valid texture (in index 0)
		bool hasTexture(int i = 0);
		void importFile(fio::path path, NullMeta& meta);
		void exportFile(fio::path path, NullMeta& meta);
		void unload(NullMeta& meta);

		// Used by Ceraal
		template<typename Arc>
		void serialize(Arc& archive) {
			archive(textures, uniforms);
		}
	};
}

