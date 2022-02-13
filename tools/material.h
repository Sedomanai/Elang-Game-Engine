#pragma once

#include "../common/container.h"
#include "../common/string.h"
#include "../common/math.h"
#include "../common/enums.h"
#include "texture.h"

namespace el
{
	struct Uniform
	{
		eDataType type;
		uint32 vertex;
		void* data;
		string name;

		Uniform(eDataType type_, uint32 vertex_, void* data_, const string& name_) 
			: type(type_), vertex(vertex_), data(data_), name(name_) {};
		Uniform() : type(eDataType::FLOAT), vertex(1), data(0), name("uSomething__NULL_") {};

		template<typename Arc>
		void save(Arc& archive) const {
			archive(type, vertex, name);
			switch (type) {
				case eDataType::FLOAT: archive(*(reinterpret_cast<float*>(data))); break;
				case eDataType::VEC2: archive(*(reinterpret_cast<vec2*>(data))); break;
				case eDataType::VEC3: archive(*(reinterpret_cast<vec3*>(data))); break;
				case eDataType::VEC4: archive(*(reinterpret_cast<vec4*>(data))); break;
				case eDataType::MATRIX4: archive(*(reinterpret_cast<matrix4x4*>(data))); break;
			};
		}

		template<typename Arc>
		void load(Arc& archive) {
			archive(type, vertex, name);
			switch (type) {
				case eDataType::FLOAT: data = malloc(sizeof(float)); archive(*(reinterpret_cast<float*>(data))); break;
				case eDataType::VEC2: data = malloc(sizeof(vec2)); archive(*(reinterpret_cast<vec2*>(data))); break;
				case eDataType::VEC3: data = malloc(sizeof(vec3)); archive(*(reinterpret_cast<vec3*>(data))); break;
				case eDataType::VEC4: data = malloc(sizeof(vec4)); archive(*(reinterpret_cast<vec4*>(data))); break;
				case eDataType::MATRIX4: data = malloc(sizeof(matrix4x4)); archive(*(reinterpret_cast<matrix4x4*>(data))); break;
			}
		}
	};


	template<int N, int TextureIndex>
	struct MaterialImpl
	{
		vector<Uniform> uniforms;
		vector<asset<TextureImpl<TextureIndex>>> textures;

		void addUniform(eDataType type_, uint32 vertex_, void* data_, const string& name_) {
			uniforms.emplace_back(type_, vertex_, data_, name_);
		}

		void setTexture(asset<TextureImpl<TextureIndex>> texture, sizet index = 0) {
			while (textures.size() <= index) {
				textures.emplace_back();
			} textures[index] = texture;
		}

		bool hasTexture() {
			return ((textures.size() > 0) && textures[0]);
		}

		template<typename Arc>
		void serialize(Arc& archive) {
			archive(textures, uniforms);
		}

		void reset() {
			textures.clear();
			uniforms.clear();
		}
	};

	using Material = MaterialImpl<0, 0>;
	using EditorMaterial = MaterialImpl<1, 1>;
	using EditorProjectMaterial = MaterialImpl<1, 0>;
}

