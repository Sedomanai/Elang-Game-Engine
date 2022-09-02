/*****************************************************************//**
 * @file   texture.h
 * @brief  Texture 2D interface. Currently only supports OpenGL and decodes/encodes png.
 *         decode/encode
 *
 * @author Sedomanai
 * @date   August 2022
 *********************************************************************/
#pragma once
#include "tool_declarer.h"
#include "project.h"
#include "../common/define.h"
#include "../elang_builder.h"

namespace el
{
	struct Atlas;
	struct TextureMeta
	{
		sizet meta;
	};

	/**
	 * 2D Texture interface. 3D textures not planned. Currently only supports OpenGL.
	 */
	struct ELANG_DLL Texture
	{
		Texture() : mID(-1), mWidth(10), mHeight(10) {}
		uint32 id() { return mID; }
		uint32 width() { return mWidth; }
		uint32 height() { return mHeight; }

		/**
		 * Load external image. Extensions: a valid .bmp, .jpg, .jpeg, .png, .targa, etc.
		 * (if not in fpng format, decode with lodepng then encode with fpng)
		 * @param filePath
		 */
		void importFile(const fio::path& path, TextureMeta&);

		/**
		 * Save current image to external filePath. Currently only supports png. (encode with fpng)
		 *
		 * @param filePath
		 */
		void exportFile(const fio::path& path, TextureMeta&);


		/**
		 * destroy texture.
		 */
		void unload(TextureMeta&);

		// detach atlas from texture - warning: if no other atlas users exist, the atlas is destroyed
		void removeAtlas(asset<Texture> self);

		/**
		 * Used by Cereal.
		 *
		 * @param archive - OutArchive
		 */
		template<typename T>
		void save(T& archive) const {
			vector<uint8> encoded;
			encode(encoded);
			archive(mWidth, mHeight, atlas, encoded);
		}
		/**
		 * Used by Cereal.
		 *
		 * @param archive - InArchive
		 */
		template<typename T>
		void load(T& archive) {
			vector<uint8> decoded;
			decode(decoded);
			archive(mWidth, mHeight, atlas, decoded);
		}

		/**
		 * @brief Auto generate an atlas. The genereated atlas' cells is not sorted in any manner at the moment.
		 * The atlas must also be empty but this may change in the near future.
		 *
		 * @param atlas - The given empty atlas should already have been created using AssetSync.
		 * Its users are not populated, this must be done manually.
		 * @param alphacut
		 */
		void autoGenerateAtlas(asset<Atlas> atlas, float alphaCut);

		asset<Atlas> atlas;
	private:
		// make texture from raw pixels
		void make(unsigned char* pixels);
		void autogenAlgorithm(hashmap<sizet, vector<sizet>>&, float alphaCut);
		void encode(vector<uint8>& out) const;
		void decode(vector<uint8>& in);
		uint32 mID;
		uint16 mWidth, mHeight;
	};
}