#include <elpch.h>
#include "material.h"
#include "project.h"
#include "serialization.h"
#include "asset_database.h"
#include "../common/fileio.h"
#include "../common/vec2.h"
#include "../common/vec3.h"
#include "../common/vec4.h"
#include "../common/matrix4x4.h"
#include "../tools/texture.h"

namespace el 
{
	void Material::addUniform(eDataType type_, uint32 vertex_, void* data_, const string& name_) {
		uniforms.emplace_back(type_, vertex_, data_, name_);
	}

	// Check if it has a valid texture (in index 0)
	void Material::setTexture(asset<Texture> texture, sizet index) {
		while (textures.size() <= index) {
			textures.emplace_back();
		} textures[index] = texture;
	}

	// Check if it has a valid texture (in index 0)
	bool Material::hasTexture(int i) {
		return ((textures.size() > 0) && textures[i] && textures[i].has<AssetLoaded>());
	}

	void Material::importFile(fio::path path, NullMeta& meta) {
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

	void Material::exportFile(fio::path path, NullMeta& meta) {
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

	void Material::unload(NullMeta& meta) {
		textures.clear();
		uniforms.clear();
	}
}
