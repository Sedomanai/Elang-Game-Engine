#include <elpch.h>
#include "asset_loader.h"
#include "../tools/painter.h"
#include "../tools/camera.h"
#include "../tools/shader.h"
#include "../tools/texture.h"
#include "../tools/atlas.h"
#include "../tools/textfile.h"

namespace el
{
	AssetLoader::AssetLoader() : mGUIAssetPath("../___gui/asset"), mGUIImported(false) {}

	/**
		* Elang native GUI assets are synced only once per start of the program. Used by Elang GUI only.
		* If you made any changes to the GUI, you will have restarted the program to implement it anyway.
		* Native GUI assets are enlisted in the names registry of the AssetDatabase but not the id registry.
		* They also have EditorAsset attached and no AssetData attached as well as any other AssetSync related elements.
		* In cases where custom Editor assets are needed that should not included in the build,
		* manually attach EditorAsset element to an asset entity instead. They will be destroyed in the build.
		*
		* In the GUI, this is the very first method you must call before you do anything.
		* Possibly even before the creation of GUI application.
		* The source (file) location for all native GUI related loader method code may change in the future.
		*/
	void AssetLoader::initNativeGUI() {
		for (auto e : fio::recursive_directory_iterator(mGUIAssetPath)) {
			if (!e.is_directory()) {
				auto path = e.path();
				auto ext = path.extension();

				if (ext == ".vert") {
					initNativeGUIAsset<VertexShader, ShaderMeta>(path, gShaders);
				} else if (ext == ".frag") {
					initNativeGUIAsset<FragmentShader, ShaderMeta>(path, gShaders);
				} else if (ext == ".png") {
					initNativeGUIAsset<Texture, TextureMeta>(path, gTextures);
				}
			}
		};
	}


	/**
		* This is for importing all oft-used external GUI assets and creating internal GUI assets.
		* This must be called the very first thing OpenGL (or any ) is first initialized. This will not fire twice.
		* This may or may not be enough for the GUI program. You may have to create or import various others along the way.
		*/
	void AssetLoader::importAllNativeGUIAssets() {
		if (!mGUIImported) {
			Painter::sCreateNullTexture();

			importNativeGUIAssets<VertexShader, ShaderMeta>();
			importNativeGUIAssets<FragmentShader, ShaderMeta>();
			importNativeGUIAssets<Texture, TextureMeta>();

			auto main_editor_cam = createNativeGUIAsset<Camera>("main_cam", gCameras);
			//createNativeGUIAsset<Painter>(
			//	"sprite_painter", gPainters, 
			//	"__el_editor_/shader/basic_sprite.vert", 
			//	"__el_editor_/shader/raw_pass.frag", 
			//	36, main_editor_cam, 
			//	Projection::eOrtho, ePainterFlags::DEPTH_SORT | ePainterFlags::MULTI_MATERIAL | ePainterFlags::Z_READ_ONLY
			//)->init();

			//auto debug = createNativeGUIAsset<Painter>(
			//	"debug_painter", gPainters,
			//	"__el_editor_/shader/debug.vert",
			//	"__el_editor_/shader/debug.frag",
			//	36, main_editor_cam,
			//	Projection::eOrtho, ePainterFlags::DEPTH_SORT | ePainterFlags::MULTI_MATERIAL | ePainterFlags::Z_READ_ONLY
			//	);
			//debug->drawtype = GL_LINES;
			//debug->init();

			mGUIImported = true;
		}
	}

	// removed - only two lines
	//asset<Material> createSingleTextureNativeGUIMaterial(const string& name, asset<Texture> tex) {
	//	auto mat = createNativeGUIAsset<Material>(name, gMaterials);
	//	mat->setTexture(tex, 0);
	//}

	/**
		* Initiate a project. This will only sync files not import them, for memory reasons.
		* Import must be done manually, depending on the wanted behavior in editor.
		*/
	void AssetLoader::initProject(fio::path rootAssetDirectoryPath) {
		cleanProject();
		gProject.directory = rootAssetDirectoryPath;
		syncAllAssets();
	}

	/**
		* Cleans up all assets from memory, registry, and database except GUI assets.
		* This also includes custom project-only editor assets.
		*/
	void AssetLoader::cleanProject() {
		auto assetView = gProject.view<AssetData>();
		gProject.destroy(assetView.begin(), assetView.end());

		for (asset<SubAssetData> sub : gProject.view<SubAssetData>()) {
			if (!gProject.valid(sub->parent)) {
				sub.add<AssetErased>();
			}
		}
		auto subView = gProject.view<AssetErased>();
		gProject.destroy(subView.begin(), subView.end());

		//auto labelView = gProject.view<Label>();
		//gProject.destroy(labelView.begin(), labelView.end());

		sync.eraseUnused(gShaders);
		sync.eraseUnused(gTexts);
		sync.eraseUnused(gMaterials);
		sync.eraseUnused(gPainters);
		sync.eraseUnused(gTextures);
		sync.eraseUnused(gAtlases);
	}

	/**
		* Syncs all external files with their asset.
		* For more info on sync behavior, refer to AssetSync in "../tools/asset_sync.h".
		*/
	void AssetLoader::syncAllAssets() {
		for (auto e : fio::recursive_directory_iterator(gProject.directory)) {
			if (!e.is_directory()) {
				auto path = e.path();
				auto ext = path.extension();

				if (ext == ".txt") {
					sync.syncAsset<Text, TextMeta>(path, gTexts).add<AssetSynced>();
				} else if (ext == ".png") {
					sync.syncAsset<Texture, TextureMeta>(path, gTextures).add<AssetSynced>();
				} else if (ext == ".atls") {
					sync.syncAsset<Atlas, AtlasMeta>(path, gAtlases).add<AssetSynced>();
				}
			}
		}

		cleanAllAssets();
	}


	/**
	* Sometimes, external files can be internally modified in the editor (GUI or otherwise).
	* Such assets must be manually attached an AssetModified module. Please do not forget this.
	* This method will search for all AssetModified modules and save them to their corresponding external file.
	* This is good when you're about to close the application and check whether you need to save all such modified files.
	*
	* Be warned that this also includes AssetConflict attached assets.
	* AssetConflict happens when the original file was also modified externally or the original file was erased.
	* Since all AssetConflict assets will have an AssetModified attached to them, this method will also save them as well.
	* This means that erased files will be recreated and all external changes in the original file will be lost.
	* AssetConflict should probably be resolved on the fly whenever the GUI Application receives focus, but still deserves caution.
	*
	* This is probably not meant for in-game modifications.
	* But the same empty module can be used to create custom behavior.
	*/
	void AssetLoader::saveAllAssets() {
		syncAllAssets();

		// save here
		auto modified = gProject.view<AssetModified>();
		for (asset<AssetData> data : modified) {
			auto path = gProject.directory / data->filePath;

			if (auto txt = asset<Text>(data)) {
				sync.unsafeSave<Text, TextMeta>(path, txt);
			} else if (auto atlas = asset<Atlas>(data)) {
				sync.unsafeSave<Atlas, AtlasMeta>(path, atlas);
			}

		} gProject.clear<AssetModified>();
	}

	void AssetLoader::cleanAllAssets() {
		auto view = gProject.view<AssetData>();
		for (asset<AssetData> data : view) {
			if (!data.has<AssetSynced>()) {
				if (data.has<AssetLoaded>()) {
					gProject.emplace_or_replace<AssetConflict>(data, true);
					data->inode = -1;
				} else
					data.add<AssetErased>();
			}
		} gProject.clear<AssetSynced>();

		auto erased = gProject.view<AssetErased>();
		gProject.destroy(erased.begin(), erased.end());

		for (asset<SubAssetData> data : gProject.view<SubAssetData>()) {
			if (gProject.valid(data->parent)) {
				data.add<AssetErased>();
			}
		}
		auto erasedSub = gProject.view<AssetErased>();
		gProject.destroy(erasedSub.begin(), erasedSub.end());

		for (asset<AssetConflict> conf : gProject.view<AssetConflict>()) {
			if (!conf.has<AssetModified>())
				conf.add<AssetModified>();
		}
		sync.eraseUnused(gTexts);
		sync.eraseUnused(gAtlases);
	}
};

