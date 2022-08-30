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

	struct MaterialMeta { sizet meta; };
	struct Material
	{
		vector<Uniform> uniforms;
		vector<asset<Texture>> textures;

		void addUniform(eDataType type_, uint32 vertex_, void* data_, const string& name_) {
			uniforms.emplace_back(type_, vertex_, data_, name_);
		}

		void setTexture(asset<Texture> texture, sizet index = 0) {
			while (textures.size() <= index) {
				textures.emplace_back();
			} textures[index] = texture;
		}

		bool hasTexture() {
			return ((textures.size() > 0) && textures[0]);
		}

		void importFile(fio::path path, MaterialMeta& meta) {
			InArchive archive(path.generic_u8string());

			int32 size;
			archive(size);
			for (auto i = 0; i < (sizet)size; i++) {
				string tex;
				archive(tex);
				if (gTextures.names.contains(tex)) {
					textures.emplace_back(gTextures.names[tex]);
				} else
					textures.emplace_back(NullEntity);
			} archive(uniforms);
		}

		void exportFile(fio::path path, MaterialMeta& meta) {
			OutArchive archive(path.generic_u8string());

			auto size = (int32)textures.size();
			archive(size);
			for (auto tex : textures) {
				if (tex) {
					archive(tex.get<AssetData>().filePath.generic_u8string());
				} else {
					archive("");
				}
			} archive(uniforms);
		}

		template<typename Arc>
		void serialize(Arc& archive) {
			archive(textures, uniforms);
		}

		void unload(MaterialMeta& meta) {
			textures.clear();
			uniforms.clear();
		}
	};
}

