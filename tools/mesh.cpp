#include "mesh.h"

namespace el {
	//void load(Meshes& meshes, const string& filePath, const string& key) {
		//if (meshes.has(key))
		//	return;

		//string input;
		//loadFile(filePath.c_str(), input);

		//MeshAction action = MeshAction::NONE;
		//float x, y, z;
		//iterate(input, '\n', [&](char* file, sizet file_size, sizet) {
		//	iterate(file, file_size, ' ', [&](char* str, sizet str_size, sizet iter_num) {
		//		if (iter_num == 0) {
		//			action = MeshAction::NONE;
		//			if (str_size == 1) {
		//				if (!strcmp(str, "o")) {
		//					action = MeshAction::OBJECT_NAME;
		//				} else if (!strcmp(str, "v")) {
		//					action = MeshAction::VERTEX;
		//				} else if (!strcmp(str, "f")) {
		//					action = MeshAction::FACE;
		//				}
		//			} else if (str_size == 2) {
		//				if (!strcmp(str, "vt")) {
		//					action = MeshAction::UV;
		//				}
		//			}
		//		} else {
		//			switch (action) {
		//			case MeshAction::OBJECT_NAME:
		//				meshes.emplace(key);
		//				break;
		//			case MeshAction::VERTEX:
		//				switch (iter_num) {
		//				case 1: x = toFloat(str); break;
		//				case 2: y = toFloat(str); break;
		//				case 3: z = toFloat(str); meshes.back().vertices.emplace_back(vec3{ x, y, z }); break;
		//				}
		//				break;
		//			case MeshAction::UV:
		//				switch (iter_num) {
		//				case 1: x = toFloat(str); break;
		//				case 2: y = toFloat(str); meshes.back().uv.emplace_back(vec2{ x, y }); break;
		//				}
		//				break;
		//			case MeshAction::FACE:
		//				/*switch (iter_num) {
		//				case 1:*/
		//					iterate(str, str_size, '/', [&](char* i, sizet, sizet iter_num) {
		//						switch (iter_num) {
		//						case 0:
		//							meshes.back().indices.push_back(toUint(i) - 1);
		//							break;
		//						}
		//					});
		//					break;
		//				//}
		//				//break;
		//			}
		//		}
		//	});
		//});
	//}
}