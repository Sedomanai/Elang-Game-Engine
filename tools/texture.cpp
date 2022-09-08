#include <elpch.h>
#include "../common/algorithm.h"
#include "hashmap.h"
#include "texture.h"
#include "cell.h"
#include "atlas.h"
#include "asset.h"

#pragma warning( push )
#pragma warning( disable : 26495 )
#pragma warning( push )
#pragma warning( disable : 4267 )
#pragma warning( push )
#pragma warning( disable : 4334 )
#include <encoder/fpng.cpp>
#include <lodepng/lodepng.cpp>
#pragma warning( pop )
#pragma warning( pop )
#pragma warning( pop )

namespace el
{
	void Texture::importFile(const fio::path& path, TextureMeta& meta) {
		uint32 w = mWidth, h = mHeight, ent = 4;
		vector<uint8> out;

		auto ext = path.extension();

		if (ext == ".png") {
			if (fpng::fpng_decode_file(path.generic_u8string().c_str(), out, w, h, ent, 4) == 0) {
				mWidth = (uint16)w;
				mHeight = (uint16)h;
				make(&out[0]);
			} else { // decode with image with lodepng first and then immediately encode the same file with fpng so that it can be loaded faster later
				vector<uint8> image; //the raw pixels
				uint width, height;
				uint error = lodepng::decode(image, width, height, path.generic_u8string());

				//if there's an error, display it
				if (error) {
					cout << "Failed to load texture at " << path << " for errorcode " << error << " : "
						<< lodepng_error_text(error) << endl;
				} else {
					mWidth = (uint16)width;
					mHeight = (uint16)height;
					make(&image[0]);
					fpng::fpng_encode_image_to_file(path.generic_u8string().c_str(), &image[0], mWidth, mHeight, 4, 4);
				}
			}
		}
	}

	void Texture::exportFile(const fio::path& path, TextureMeta& meta) {

		auto ext = path.extension();
		if (ext != ".png") {
			std::cout << "Failed to export texture to " << path << ". Must have a valid extension (png)" << std::endl;
			return;
		}

		auto size = mWidth * mHeight * 4;
		unsigned char* pixels = (unsigned char*)malloc(size);
		glBindTexture(GL_TEXTURE_2D, mID);
		glGetTextureImage(mID, 0, GL_RGBA, GL_UNSIGNED_BYTE, size, pixels);
#ifdef _DEBUG
		if (fpng::fpng_encode_image_to_file(path.generic_u8string().c_str(), pixels, mWidth, mHeight, 4, 0) == 0)
			std::cout << "Failed to export texture to " << path << std::endl;
#else
		fpng::fpng_encode_image_to_file(path.generic_u8string().c_str(), pixels, mWidth, mHeight, 4, 0);
#endif
	}

	void Texture::unload(TextureMeta&) {
		glDeleteTextures(1, &mID);
		mID = -1;
	}


	void Texture::decode(vector<uint8>& in) {
		uint32 w = mWidth, h = mHeight, ent = 4;
		vector<uint8> out;

		if (fpng::fpng_decode_memory(&in[0], (uint32)in.size(), out, w, h, ent, 4) == 0) {
			make(&out[0]);
		}
#ifdef _DEBUG
		else
			std::cout << "Failed to load texture from memory" << std::endl;
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


	void Texture::autoGenerateAtlas(asset<Atlas> new_atlas, float alphaCut) {
		if (new_atlas->cells.size() > 0 || !new_atlas.has<AssetData>() || !new_atlas.has<AtlasMeta>()) {
			return;
		}

		string str = new_atlas.get<AssetData>().filePath.stem().generic_u8string();

		auto& atlasmeta = new_atlas.get<AtlasMeta>();
		atlasmeta.self = new_atlas;
		atlasmeta.width = (int32)mWidth;
		atlasmeta.height = (int32)mHeight;
		sizet w = (sizet)mWidth;
		sizet h = (sizet)mHeight;
		sizet pixcount = w * h;

		hashmap<sizet, vector<sizet>> result;
		autogenAlgorithm(result, alphaCut);

		sizet index = 0;
		auto intmax = std::numeric_limits<int64>::max();
		for (auto& vec : result) {
			int64 il = intmax;
			int64 ib = intmax;
			int64 ir = -intmax;
			int64 it = -intmax;
			for (sizet s : vec.second) {
				il = min(il, (int64)(s % w));
				ib = min(ib, (int64)(s / w));
				ir = max(ir, (int64)(s % w) + 1);
				it = max(it, (int64)(s / w) + 1);
			}
			auto name = str + "_" + std::to_string(index);
			auto meta = gProject.make<SubAssetData>(index, name, new_atlas)
				.add<CellMeta>(il, ib, ir-il, it-ib, 0, 0);
			new_atlas->addCell(meta, atlasmeta);
			index++;
		}
	}


	void Texture::removeAtlas(asset<Texture> self) {
		//if (!atlas)
		//	return;

		//asset<AtlasMeta> meta = atlas;
		//if (!meta)
		//	return;

		//auto& users = meta->users;
		//for (auto it = users.begin(); it != users.end(); it++) {
		//	if (*it == self.get<AssetData>().filePath) {
		//		

		//		atlas->textures.erase(it);
		//		if (atlas->textures.size() == 0) {
		//			atlas->destroy();
		//			gProject->atlases.erase(atlas);
		//		} break;
		//	}
		//} 
		
		atlas = NullEntity;
	}


	void Texture::autogenAlgorithm(hashmap<sizet, vector<sizet>>& result, float alphaCut) {
		sizet name = 0;
		sizet w = (sizet)mWidth;
		sizet h = (sizet)mHeight;
		sizet pixcount = w * h;

		auto pixels = (unsigned char*)malloc(pixcount);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mID);
		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glGetTexImage(GL_TEXTURE_2D, 0, GL_ALPHA, GL_UNSIGNED_BYTE, pixels);

		sizet* buffer = (sizet*)malloc((pixcount) * sizeof(sizet));
		for (sizet i = 0; i < pixcount; i++) {
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
						} else {
							// only topleft
							buffer[i] = buffer[i - w - 1];
							result[buffer[i]].push_back(i);
						}
					} else {
						// top (topleft should it exist would already be the same as top)
						buffer[i] = buffer[i - w];
						result[buffer[i]].push_back(i);
					}
				} else {
					if (notop) {
						// left (topleft should it exist would already be the same as left)
						buffer[i] = buffer[i - 1];
						result[buffer[i]].push_back(i);
					} else {
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
						} else {
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
					} else if (curr != topright) {
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