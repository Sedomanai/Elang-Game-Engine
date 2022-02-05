#include "texture.h"

#include <encoder/fpng.h>
#include <encoder/fpng.cpp>

#include <lodepng/lodepng.h>
#include <lodepng/lodepng.cpp>

namespace el {
	void Texture::makeFromStandardImageFile(const string& filePath) {
		uint32 w = mWidth, h = mHeight, ent = 4;
		vector<uint8> out;
		if (fpng::fpng_decode_file(filePath.c_str(), out, w, h, ent, 4) == 0) {
			mWidth = (uint16)w;
			mHeight = (uint16)h;
			make(&out[0]);
		} else { // load with image with spng first and then encode with fpng so that it can be faster later
			vector<unsigned char> image; //the raw pixels
			unsigned width, height;
			unsigned error = lodepng::decode(image, width, height, filePath);

			//if there's an error, display it
			if (error) {
				cout << "Failed to load texture at " << filePath << " for errorcode " << error << " : "
					<< lodepng_error_text(error) << endl;
			}
			else {
				mWidth = (uint16)width;
				mHeight = (uint16)height;
				make(&image[0]);
				fpng::fpng_encode_image_to_file(filePath.c_str(), &image[0], mWidth, mHeight, 4, 4);
			}
		}
	}

	void Texture::importFile(const string& key) {
		string filePath = gProject->datDir + key;
		uint32 w = mWidth, h = mHeight, ent = 4;
		vector<uint8> out;
		if (fpng::fpng_decode_file(filePath.c_str(), out, w, h, ent, 4) == 0) {
			make(&out[0]);
		}
#ifdef _DEBUG
		else {
			cout << "Failed to decode Erang texture file at " << filePath << std::endl;
		}
#endif
	}

	void Texture::exportFile(const string& key) {
		string filePath = gProject->datDir + key;
		auto size = mWidth * mHeight * 4;
		unsigned char* pixels = (unsigned char*)malloc(size);
		glBindTexture(GL_TEXTURE_2D, mID);
		glGetTextureImage(mID, 0, GL_RGBA, GL_UNSIGNED_BYTE, size, pixels);
#ifdef _DEBUG
		if (fpng::fpng_encode_image_to_file(filePath.c_str(), pixels, mWidth, mHeight, 4, 0) == 0)
			std::cout << "Failed to export texture to " << filePath << std::endl;
#else
		fpng::fpng_encode_image_to_file(filePath.c_str(), pixels, mWidth, mHeight, 4, 0);
#endif
	}

	void Texture::destroy() {
		glDeleteTextures(1, &mID);
		mID = -1;
	}

	void Texture::decode(vector<uint8>& in) {
		uint32 w = mWidth, h = mHeight, ent = 4;
		vector<uint8> out;

		if (fpng::fpng_decode_memory(&in[0], (uint32)in.size(), out, w, h, ent, 4) == 0)
			make(&out[0]);
#ifdef _DEBUG
		else
			std::cout << "Failed to load texture from memory"  << std::endl;
#endif
	}

	void Texture::encode(vector<uint8>& out) const {
		auto size = mWidth * mHeight * 4;
		unsigned char* pixels = (unsigned char*)malloc(size);
		glBindTexture(GL_TEXTURE_2D, mID);
		glGetTextureImage(mID, 0, GL_RGBA, GL_UNSIGNED_BYTE, size, pixels);
		if (!fpng::fpng_encode_image_to_memory(pixels, mWidth, mHeight, 4, out, 0)) {
#ifdef _DEBUG
			cout << "failed to encode texture" << endl;
#endif
		}
	}

	void Texture::make(unsigned char* pixels) {
		glGenTextures(1, &mID);
		glBindTexture(GL_TEXTURE_2D, mID);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (int)mWidth, (int)mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		//glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

}