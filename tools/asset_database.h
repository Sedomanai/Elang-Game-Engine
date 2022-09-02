/*****************************************************************//**
 * @file   asset_database.h
 * @brief  All actual asset databases. It was separated from asset.h to help reduce compile time for some sources
 * 
 * @author Sedomanai
 * @date   August 2022
 *********************************************************************/
#pragma once
#include "asset.h"
#include "../elang_builder.h"

namespace el
{
	ELANG_DLL extern AssetDatabase gTexts, gShaders, gCameras, gPainters, gMaterials, gTextures, gAtlases;
}
