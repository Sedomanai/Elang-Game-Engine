#include "texture.h"

#include <encoder/fpng.h>
#include <encoder/fpng.cpp>

#include <lodepng/lodepng.h>
#include <lodepng/lodepng.cpp>

namespace el {

	template struct TextureImpl<0>;

	template<int N>
	void TextureImpl<N>::makeFromStandardImageFile(const string& filePath) {
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

	template<int N>
	void TextureImpl<N>::importFile(const string& key) {
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

	template<int N>
	void TextureImpl<N>::exportFile(const string& key) {
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

	template<int N>
	void TextureImpl<N>::destroy() {
		glDeleteTextures(1, &mID);
		mID = -1;
	}

	template<int N>
	void TextureImpl<N>::decode(vector<uint8>& in) {
		uint32 w = mWidth, h = mHeight, ent = 4;
		vector<uint8> out;

		if (fpng::fpng_decode_memory(&in[0], (uint32)in.size(), out, w, h, ent, 4) == 0) {
			make(&out[0]);
		}
#ifdef _DEBUG
		else
			std::cout << "Failed to load texture from memory"  << std::endl;
#endif
	}

	template<int N>
	void TextureImpl<N>::encode(vector<uint8>& out) const {
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

	template<int N>
	void TextureImpl<N>::make(unsigned char* pixels) {
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

	template<int N>
	void TextureImpl<N>::autoGenerateAtlas(asset<TextureImpl<N>> self, float alphaCut) {
		if (!gProject->textures.contains(self))
			return;

		auto key = gProject->textures[self];

		if (atlas) {
			atlas->destroy();
		} else {
			atlas = gProject->make<AtlasImpl<N>>(gProject->atlases, key);
		}

		int64 w = (sizet)mWidth;
		int64 h = (sizet)mHeight;
		int64 pixcount = w * h;

		hashmap<int64, vector<int64>> result;
		autogenAlgorithm(result, alphaCut);

		int64 name = 0;
		auto intmax = std::numeric_limits<int64>::max();
		for (auto& vec : result) {
			int64 il = intmax;
			int64 ib = intmax;
			int64 ir = -intmax;
			int64 it = -intmax;
			for (int s : vec.second) {
				il = min(il, (s % w));
				ib = min(ib, (s / w));
				ir = max(ir, (s % w) + 1);
				it = max(it, (s / w) + 1);
			}

			auto cell = gProject->makeSub<Cell>((int)il, (int)ib, (int)(ir - il), (int)(it - ib), 0, 0, (int)w, (int)h, -1);
			atlas->cells.emplace(key + "_" + std::to_string(name++), cell);
		}
	}

	template<int N>
	void TextureImpl<N>::removeAtlas(asset<TextureImpl<N>> self) {
		if (atlas) {
			auto& tex = atlas->textures;
			for (auto it = tex.begin(); it != tex.end(); it++) {
				if (*it == self) {
					atlas->textures.erase(it);
					if (atlas->textures.size() == 0) {
						atlas->destroy();
						gProject->atlases.erase(atlas);
					} break;
				}
			} atlas = NullEntity;
		}
	}

	template<int N>
	void TextureImpl<N>::autogenAlgorithm(hashmap<int64, vector<int64>>& result, float alphaCut) {
		int64 name = 0;
		int64 w = (int64)mWidth;
		int64 h = (int64)mHeight;
		int64 pixcount = w * h;

		auto pixels = (unsigned char*)malloc(pixcount);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mID);
		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glGetTexImage(GL_TEXTURE_2D, 0, GL_ALPHA, GL_UNSIGNED_BYTE, pixels);

		int64* buffer = (int64*)malloc((pixcount) * sizeof(int64));
		for (int64 i = 0; i < pixcount; i++) {
			bool notop = ((i < w) || (buffer[i - w] == 0));
			bool noleft = ((i % w == 0) || (buffer[i - 1] == 0));
			bool notopleft = ((i < w) || (i % w == 0) || (buffer[i - w - 1] == 0));
			bool notopright = ((i < w) || (i % w == w - 1) || (buffer[i - w + 1] == 0));

			buffer[i] = 0;

			if (pixels[i] > alphaCut) {
				if (noleft) {
					if (notop) {
						if (notopleft) {
							buffer[i] = ++name;
							result[name].reserve(256);
							result[name].push_back(i);
						}
						else {
							// only topleft
							buffer[i] = buffer[i - w - 1];
							result[buffer[i]].push_back(i);
						}
					}
					else {
						// top (topleft should it exist would already be the same as top)
						buffer[i] = buffer[i - w];
						result[buffer[i]].push_back(i);
					}
				}
				else {
					if (notop) {
						// left (topleft should it exist would already be the same as left)
						buffer[i] = buffer[i - 1];
						result[buffer[i]].push_back(i);
					}
					else {
						if (notopleft) {
							// only left and above
							auto top = buffer[i - w];
							auto left = buffer[i - 1];

							if (top != left) {
								auto& tops = result[top];
								auto& lefts = result[left];
								for (uint i = 0; i < tops.size(); i++) {
									buffer[tops[i]] = left;
								}
								lefts.reserve(lefts.size() + tops.size());
								lefts.insert(lefts.end(), tops.begin(), tops.end());
								result.erase(top);
							}

							buffer[i] = left;
							result[left].push_back(i);
						}
						else {
							// all three topleft pixels are the same, could use any
							buffer[i] = buffer[i - 1];
							result[buffer[i]].push_back(i);
						}
					}
				}

				if (notop && !notopright) {
					// special case where topright exists but top doesn't
					auto topright = buffer[i - w + 1];
					auto curr = buffer[i];
					if (curr == 0) {
						buffer[i] = topright;
						result[topright].push_back(i);
					}
					else if (curr != topright) {
						auto& toprights = result[topright];
						auto& currs = result[curr];
						for (sizet i = 0; i < currs.size(); i++) {
							buffer[currs[i]] = topright;
						}
						toprights.reserve(toprights.size() + currs.size());
						toprights.insert(toprights.end(), currs.begin(), currs.end());
						result.erase(curr);
					}
				}
			}
		}

		free(pixels);
		free(buffer);
	}
}