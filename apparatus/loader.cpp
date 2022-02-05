#include "loader.h"

namespace el
{
	asset<Material> createSingleTextureMaterial(const string& tex_label) {
		asset<Material> outMaterial = gProject->make<Material>(gProject->materials, tex_label);
		if (outMaterial->textures.size() == 0)
			outMaterial->textures.emplace_back(gProject->textures[tex_label]);
		return outMaterial;
	}

	void connectAssetDstr() {
		gProject->on_destroy<Painter>().connect<entt::invoke<&Painter::destroy>>();
		gProject->on_destroy<Texture>().connect<entt::invoke<&Texture::destroy>>();
		gProject->on_destroy<Atlas>().connect<entt::invoke<&Atlas::destroy>>();
		gProject->on_destroy<FontFace>().connect<entt::invoke<&FontFace::destroy>>();
		//TODO: Fonts and everything
	}

	void __s_registerAsset(bihashmap<string, Entity>& map, eFileExtension ext) {
		if (map.contains(__s_gLoader_AssetCurrKey)) {
			asset<FileData> file = map[__s_gLoader_AssetCurrKey];

			if (file->updateTime == __s_gLoader_AssetCurrTime)
				file->state = eFileState::UNCHANGED;
			else {
				file->state = eFileState::MODIFIED;
				file->updateTime = __s_gLoader_AssetCurrTime;
			}
		} else {
			auto file = gProject->makeSub<FileData>(
				__s_gLoader_AssetCurrKey, __s_gLoader_AssetCurrTime,
				ext, eFileState::CREATED
				);
			map.emplace(__s_gLoader_AssetCurrKey, file);
		}
	}
	void __s_registerAllAssets() {
		string& dir = gProject->assetDir;
		iterateFolderRecursive(dir, [=](const fentry& e) {
			if (!e.is_directory()) {
				auto& path = e.path();
				if (path.has_extension()) {
					auto ext = path.extension().string();
					auto filePath = path.string();
					auto fileName = path.filename().string();

					auto wtime = lastWriteTime(e);
					__s_gLoader_AssetCurrTime = wtime.time_since_epoch().count();

					string keyext = filePath.substr(
						filePath.find(dir) + dir.size(),
						filePath.size() - dir.size()
					); replace(keyext, "\\", "/");

					tokenize(keyext, '.', [&](strview head, strview) {
						__s_gLoader_AssetCurrKey = head;
						if (ext == ".png") {
							__s_registerAsset(gProject->textures, eFileExtension::PNG);
						} else if (ext == ".atls") {
							__s_registerAsset(gProject->atlases, eFileExtension::ATLS);
						} else if (ext == ".fnt") {
							__s_registerAsset(gProject->fonts, eFileExtension::BFNT);
						}
					});
				}
			}
		});
	}

	void reloadAll(bool reloadUnchanged) {
		__s_registerAllAssets();

		for (asset<FileData> file : gProject->view<FileData>()) {
			switch (file->state) {
				break;
				case eFileState::ERASED:
					switch (file->extension) {
					case eFileExtension::PNG:
					case eFileExtension::JPEG:
					case eFileExtension::JPG:
						gProject->textures.erase(file);
						break;
					case eFileExtension::ATLS:
						gProject->atlases.erase(file);
						break;
					case eFileExtension::BFNT:
						gProject->fonts.erase(file);
						break;
					} file.destroy();
				break;
				case eFileState::CREATED:
					switch (file->extension) {
					case eFileExtension::PNG:
					case eFileExtension::JPEG:
					case eFileExtension::JPG:
						file.add<Texture>()->makeFromStandardImageFile(gProject->assetDir + file->key + ".png");
					break;
					case eFileExtension::ATLS:
						file.add<Atlas>()->makeFromAtlsFile(gProject->assetDir + file->key + ".atls");
					break;
					case eFileExtension::BFNT:
						file.add<FontFace>()->makeFromBfntFile(gProject->assetDir + file->key + ".fnt");
					break;
					}
				break;
				case eFileState::MODIFIED:
					switch (file->extension) {
					case eFileExtension::PNG:
					case eFileExtension::JPEG:
					case eFileExtension::JPG:
						file.get<Texture>().destroy();
						file.get<Texture>().makeFromStandardImageFile(gProject->assetDir + file->key + ".png");
					break;
					case eFileExtension::ATLS:
						file.get<Atlas>().destroy();
						file.get<Atlas>().makeFromAtlsFile(gProject->assetDir + file->key + ".atls");
					break;
					case eFileExtension::BFNT:
						file.get<FontFace>().destroy();
						file.get<FontFace>().makeFromBfntFile(gProject->assetDir + file->key + ".fnt");
					break;
					}
				break;
				case eFileState::UNCHANGED:
					if (reloadUnchanged) {
						switch (file->extension) {
						case eFileExtension::PNG:
						case eFileExtension::JPEG:
						case eFileExtension::JPG:
							file.get<Texture>().destroy();
							file.get<Texture>().makeFromStandardImageFile(gProject->assetDir + file->key + ".png");
							break;
						}
					}
				break;
			}
			if (file.valid()) file->state = eFileState::ERASED;
		}

		//gProject->sort<AssetData>([](const AssetData& lhs, const AssetData& rhs) { return lhs.state > rhs.state; });
		for (asset<Atlas> atlas : gProject->view<Atlas>()) {
			for (auto tex : atlas->textures) {
				if (tex) tex->atlas = atlas;
			}
		}

		for (asset<FontFace> font : gProject->view<FontFace>()) {
			const auto& key = font.get<FileData>().key;
			if (gProject->textures.contains(key)) { 
				font->material = gProject->materials.contains(key) ?
					asset<Material>(gProject->materials[key]) : createSingleTextureMaterial(key);
			} else {
				cout << "Bitmap Font " << key << " could not link to non-existing texture " << endl;
			}
		}

	}

	void saveElangProject(const char* project_path) {
		OutArchive ar(project_path);
		if (ar.valid()) {
			gProject->save<FileData, Texture, Atlas, Cell, Clipframe, Clip,
				FontFace, FontGlyph, Material, Painter, Camera>(ar);
			cout << "Saving Elang Project at Project Path " << project_path << "  ..." << endl;
		} else {
			cout << "Could not save Elang Project at project path " + string(project_path) << endl;
		}
	}

	void loadElangProject(const char* project_path, bool loadAllExternals) {
		InArchive ar(project_path);
		if (ar.valid()) {
			cout << "Loading Elang Project at Project Path " << project_path << "  ..." << endl;
			gProject->load<FileData, Texture, Atlas, Cell, Clipframe, Clip,
				FontFace, FontGlyph, Material, Painter, Camera>(ar);
			cout << "Project Name:	   " << gProject->name << endl;
			cout << "Asset  Directory: " << gProject->assetDir << endl;
			cout << "Data   Directory: " << gProject->datDir << endl;
			cout << "Source Directory: " << gProject->srcDir << endl;
			initElang();
			if (loadAllExternals && !gProject->internalBinary)
				reloadAll(true);
		} else {
			cout << "Could not load Elang Project at project path " + string(project_path) << endl;
		}
	}

	void saveElangStage(const char* filePath) {
		OutArchive ar(filePath);
		if (ar.valid()) {
			gStage->project = gProject->name;
			gStage->serializeProject(ar);
			gStage->save<Position, Planar, Box, Radius, Canvas, Sprite,
				Button, Platform, Platformer, Textfield, Textbox>(ar);
#ifdef _DEBUG
			cout << "Saving Elang Stage at Project Path " << filePath << "  ..." << endl;
#endif
		}
#ifdef _DEBUG
		else {
			cout << "Could not save Elang Stage at project path " + string(filePath) << endl;
		}
#endif
	}


	void loadElangStage(const char* filePath) {
		InArchive ar(filePath);
		if (ar.valid()) {
			auto temp = gStage->project;
			gStage->serializeProject(ar);
			if (gProject->name == gStage->project) {
				gStage->load<Position, Planar, Box, Radius, Canvas, Sprite,
					Button, Platform, Platformer, Textfield, Textbox>(ar);
#ifdef _DEBUG
				cout << "Loading Elang Stage at Project Path " << filePath << "  ..." << endl;
#endif
			} else {
				gStage->project = temp;
#ifdef _DEBUG
				cout << "Elang Stage at Project Path " << filePath << " does not belong to current project" << endl;
#endif
			}
			
		}
#ifdef _DEBUG
		else {
			cout << "Could not load Elang Stage at project path " + string(filePath) << endl;
		}
#endif
	}

	void loadTexture(const string& key) {
		asset<FileData> data(gProject->textures[key]);
		switch (data->extension) {
		case eFileExtension::PNG:
			asset<Texture>(data)->makeFromStandardImageFile(gProject->assetDir + key + ".png");
			break;
		}
	}

	void compileDefaultShaders() {
		static bool uinit = true;

		if (uinit) {
			auto& v1 = gVertexShaders["position2d"];
			v1.compileShader(gGlslVertexPosition2d, GL_VERTEX_SHADER);
			auto& v2 = gVertexShaders["debug2d"];
			v2.compileShader(gGlslVertexDebug2d, GL_VERTEX_SHADER);
			auto& v3 = gVertexShaders["debug3d"];
			v3.compileShader(gGlslVertexDebug3d, GL_VERTEX_SHADER);
			auto& v4 = gVertexShaders["basic_sprite"];
			v4.compileShader(gGlslVertexBasicSprite, GL_VERTEX_SHADER);
			auto& v5 = gVertexShaders["color_sprite"];
			v5.compileShader(gGlslVertexColorSprite, GL_VERTEX_SHADER);

			auto& f1 = gFragmentShaders["debug"];
			f1.compileShader(gGlslFragmentDebug, GL_FRAGMENT_SHADER);
			auto& f2 = gFragmentShaders["texture_uv"];
			f2.compileShader(gGlslFragmentTextureUV, GL_FRAGMENT_SHADER);
			auto& f3 = gFragmentShaders["grid"];
			f3.compileShader(gGlslFragmentGrid, GL_FRAGMENT_SHADER);

			uinit = false;
		}
	}
}

