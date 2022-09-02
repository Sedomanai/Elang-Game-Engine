/*****************************************************************//**
 * @file   asset_loader.h
 * @brief  Basically an asset sync manager for all assets
 *		   This includes Native GUI assets (Assets that are used in-engine and not included in build)
 *		   Requires a base project directory address.
 * 
 * @author Sedomanai
 * @date   September 2022
 *********************************************************************/

#pragma once
#include "asset_sync.h"

namespace el
{
	struct GUIAsset { fio::path filePath; GUIAsset(const fio::path& key_) : filePath(key_) {}; };
	struct EditorAsset {};
	struct ELANG_DLL AssetLoader
	{
		AssetLoader();

		/**
		 * @brief * Elang native GUI assets are synced only once per start of the program. Used by Elang GUI only. 
		 * If you made any changes to the GUI, you will have restarted the program to implement it anyway. 
		 * 
		 * @brief * Native GUI assets are enlisted in the names registry of the AssetDatabase but not the id registry.
		 * They also have EditorAsset attached and no AssetData attached as well as any other AssetSync related elements.
		 * In cases where custom Editor assets are needed that should not included in the build, 
		 * manually attach EditorAsset element to an asset entity instead. They will be destroyed in the build.
		 * 
		 * @brief * In the GUI, this is the very first method you must call before you do anything.
		 * Possibly even before the creation of GUI application.
		 * The source (file) location for all native GUI related loader method code may change in the future.
		 */
		void initNativeGUI();

		/**
		 * @brief This is for GUI assets that are created in the editor.
		 * They usually do not have a meta and have no external file to be synced with.
		 * Since they do not have an external file, the created key is a mock-key. 
		 * If the mock-key clashes with one of the external files or previously created gui asset, the asset will not be created.
		 */
		template<typename T, typename ...Args>
		asset<T> createNativeGUIAsset(const string& name, AssetDatabase& db, Args... args) {
			fio::path key = "__el_editor_/" + name;
			if (!db.names.contains(key)) {
				auto data = gProject.make<T>(args...).add<EditorAsset>().add<GUIAsset>(key);
				db.names.emplace(key, data);
				return data;
			} else return NullEntity;
		}

		/**
		 * @brief This is for importing all oft-used external GUI assets and creating internal GUI assets.
		 * This must be called the very first thing OpenGL (or any graphic library) is first initialized. This will not fire twice.
		 * This may or may not be enough for the GUI program. You may have to create or import various others along the way.
		 */
		void importAllNativeGUIAssets();

		// removed - only two lines
		//asset<Material> createSingleTextureNativeGUIMaterial(const string& name, asset<Texture> tex) {
		//	auto mat = createNativeGUIAsset<Material>(name, gMaterials);
		//	mat->setTexture(tex, 0);
		//}

		/**
		 * Initiate a project. This will only sync files not import them, for memory reasons. 
		 * Import must be done manually, depending on the wanted behavior in editor.
		 * 
		 * @param rootAssetDirectoryPath : Asset root directory
		 */
		void initProject(fio::path rootAssetDirectoryPath);

		/**
		 * @brief Cleans up all assets from memory, registry, and database except GUI assets. 
		 * @brief This also includes custom project-only editor assets.
		 */
		void cleanProject();
		/**
		 * @brief Syncs all external files with their asset.
		 * @brief For more info on sync behavior, refer to AssetSync in "../tools/asset_sync.h".
		 */
		void syncAllAssets();


		/**
		* @brief * Sometimes, external files can be internally modified in the editor (GUI or otherwise).
		* Such assets must be manually attached an AssetModified module. Please do not forget this. 
		* @brief --
		* @brief * This method will search for all AssetModified modules and save them to their corresponding external file.
		* This is good when you're about to close the application and check whether you need to save all such modified files.
		* @brief --
		* @brief * Be warned that this also includes AssetConflict attached assets. 
		* AssetConflict happens when the original file was also modified externally or the original file was erased.
		* Since all AssetConflict assets will have an AssetModified attached to them, this method will also save them as well.
		* This can mean that erased files will be recreated, or that all external changes in the original file will be lost.
		* AssetConflict should probably be resolved on the fly whenever the GUI Application receives focus, but still deserves caution.
		* @brief --
		* @brief * This should also work for in-game modifications, such as saving a game.
		*/
		void saveAllAssets();

		// This is exposed to deal with individual files and to import/export/save them
		AssetSync sync;
	private:
		fio::path mGUIAssetPath;
		bool mGUIImported;

		template<typename T, typename M>
		void importNativeGUIAssets() {
			for (asset<GUIAsset> data : gProject.view<T>()) {
				if (data) {
					fio::path path = mGUIAssetPath / data->filePath;
					data.get<T>().importFile(path, data.get<M>());
				}
			}
		}

		template<typename T, typename M>
		void initNativeGUIAsset(fio::path path, AssetDatabase& base) {
			auto key = fio::relative(path, mGUIAssetPath);
			auto editorAsset = gProject.make<T>().add<M>().add<EditorAsset>().add<GUIAsset>(key);
			base.names[key] = editorAsset;
		}

		void cleanAllAssets();
	};
}
