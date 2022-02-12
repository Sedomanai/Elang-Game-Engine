#pragma once

#include "../default_glsl.h"

#include "../tools/texture.h"
#include "../tools/atlas.h"
#include "../tools/bmpfont.h"
#include "../tools/painter.h"
#include "../tools/camera.h"

#include "../elements/basic.h"
#include "../elements/canvas.h"
#include "../elements/sprite.h"
#include "../elements/button.h"
#include "../elements/platformer.h"
#include "../elements/textbox.h"


namespace el
{	
	enum class eFileExtension
	{
		IMG,
		JPG,
		JPEG,
		PNG,
		TGA,
		TARGA,

		ATLS,
		BFNT
	};

	enum class eFileState
	{
		UNCHANGED,
		CREATED,
		MODIFIED,
		ERASED
	};

	struct FileData
	{
		FileData() {}
		FileData(const string& key_, long long time_, eFileExtension ext_, eFileState state_)
			: key(key_), updateTime(time_), extension(ext_), state(state_) 
		{}

		template<typename T>
		void serialize(T& archive) {
			archive(key, extension, updateTime, state);
		}

		string key; // relative path
		long long updateTime;
		eFileExtension extension;
		eFileState state;
	};

	inline long long __s_gLoader_AssetCurrTime;
	inline string __s_gLoader_AssetCurrKey;


	extern void ELANG_DLL __s_registerAllAssets();
	extern void ELANG_DLL __s_registerAsset(bihashmap<string, Entity>& map, eFileExtension);
	extern void ELANG_DLL reloadAll(bool reloadUnchanged = false);
	extern void ELANG_DLL compileDefaultShaders();
	extern void ELANG_DLL connectAssetDstr();
	extern void ELANG_DLL initElang();

	extern asset<Material> ELANG_DLL createSingleTextureMaterial(const string& label);
	extern void ELANG_DLL saveElangProject(const char* filePath);
	extern void ELANG_DLL loadElangProject(const char* filePath, bool loadAllExternals = false);

	extern void ELANG_DLL saveElangStage(const char* filePath);
	extern void ELANG_DLL loadElangStage(const char* filePath);

	extern void ELANG_DLL loadTexture(const string& key);
};