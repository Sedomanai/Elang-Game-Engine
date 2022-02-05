#pragma once

#include "../common/container.h"
#include "../common/string.h"
#include "../common/math.h"
#include "texture.h"

namespace el
{
	struct Uniform
	{
		enum
		{
			FLOAT,
			VEC2,
			VEC3,
			VEC4,
			MATRIX // TODO
		};
		uint32 type;
		uint32 vertex;
		void* data;
		string name;

		Uniform(uint32 type_, uint32 vertex_, void* data_, const string& name_) 
			: type(type_), vertex(vertex_), data(data_), name(name_) {};
		Uniform() : type(FLOAT), vertex(1), data(0), name("uSomething__NULL_") {};

		template<typename T>
		void save(T& archive) const {
			archive(type, vertex, name);
			switch (type) {
				case FLOAT: archive(*(reinterpret_cast<float*>(data))); break;
				case VEC2: archive(*(reinterpret_cast<vec2*>(data))); break;
				case VEC3: archive(*(reinterpret_cast<vec3*>(data))); break;
				case VEC4: archive(*(reinterpret_cast<vec4*>(data))); break;
			};
		}

		template<typename T>
		void load(T& archive) {
			archive(type, vertex, name);
			switch (type) {
				case FLOAT: data = malloc(sizeof(float)); archive(*(reinterpret_cast<float*>(data))); break;
				case VEC2: data = malloc(sizeof(vec2)); archive(*(reinterpret_cast<vec2*>(data))); break;
				case VEC3: data = malloc(sizeof(vec3)); archive(*(reinterpret_cast<vec3*>(data))); break;
				case VEC4: data = malloc(sizeof(vec4)); archive(*(reinterpret_cast<vec4*>(data))); break;
			}
		}
	};

	struct Material
	{
		vector<Uniform> uniforms;
		vector<asset<Texture>> textures;

		void addUniform(uint32 type_, uint32 vertex_, void* data_, const string& name_) {
			uniforms.emplace_back(type_, vertex_, data_, name_);
		}

		bool hasTexture() {
			return ((textures.size() > 0) && textures[0]);
		}

		template<typename T>
		void serialize(T& archive) {
			archive(textures, uniforms);
		}

		void reset() {
			textures.clear();
			uniforms.clear();
		}
	};
}

