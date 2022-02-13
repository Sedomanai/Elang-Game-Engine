#pragma once

#include <GL/glew.h>

#include "../elang_library_builder.h"
#include "../common/container.h"
#include "../common/fileio.h"
#include "../common/math.h"

#include "registry.h"
#include "atlas.h"


namespace el
{

	template<int N>
	struct ELANG_DLL TextureImpl
	{
		TextureImpl() : mID(-1), mWidth(0), mHeight(0) {}

		uint32 id() { return mID; }
		uint32 width() { return mWidth; }
		uint32 height() { return mHeight; }

		// a valid .bmp, .jpg, .jpeg, .png, .targa, etc.
		void makeFromStandardImageFile(const string& filePath);

		// exports elang texture file (encode with fpng)
		void exportFile(const string& key);
		// imports elang texture file (decode with fpng)
		void importFile(const string& key);

		// make texture from raw pixels
		void make(unsigned char* pixels);
		// destroy texture
		void destroy();

		// auto generate an atlas - not perfect, use the gui instead of a raw call
		void autoGenerateAtlas(asset<TextureImpl<N>> self, float alphaCut);

		// detach atlas from texture - warning: if no other atlas users exist, the atlas is destroyed
		void removeAtlas(asset<TextureImpl<N>> self);

		template<typename T>
		void save(T& archive) const {
			archive(mWidth, mHeight, atlas);
			if (gProject->internalBinary) {
				vector<uint8> encoded;
				encode(encoded);
				archive(encoded);
			}
		}
		template<typename T>
		void load(T& archive) {
			archive(mWidth, mHeight, atlas);
			if (gProject->internalBinary) {
				vector<uint8> decoded;
				archive(decoded);
				decode(decoded);
			}
		}

		asset<AtlasImpl<N>> atlas;
	private:
		void autogenAlgorithm(hashmap<int64, vector<int64>>&, float alphaCut);
		void encode(vector<uint8>& out) const;
		void decode(vector<uint8>& in);
		uint32 mID;
		uint16 mWidth, mHeight;
	};

	using Texture = TextureImpl<0>;
}