#pragma once

#include "../elang_library_builder.h"
#include "../common/fileio.h"
#include "vertex.h"
#include "hash.h"

namespace el {
	struct ELANG_DLL ShaderLoader
	{
		void compileDefaultShaders(const char* assetdir);

		template <typename T = VoidVertexData>
		std::enable_if_t<std::is_base_of_v<IVertexData, T>, void>
		inline compileShaderProgram(sizet type, const string& filePath) {
			string file;
			loadFile(filePath.c_str(), file);
			tokenize(filePath, '/', [&](strview, strview name) {
				GLenum type = 0;
				tokenize(name, '.', [&](strview, strview ext) {
					if (ext == "frag")
						type = GL_FRAGMENT_SHADER;
					else if (ext == "vert")
						type = GL_VERTEX_SHADER;
					});

				if (!type)
					throw "Invalid shader file";

				const char* link = file.c_str();
				const uint32 shader = glCreateShader(type);
				if (shader) {
					glShaderSource(shader, 1, &link, NULL);
					glCompileShader(shader);

					int compiled = GL_FALSE;
					glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
					errorCheck(compiled, shader, filePath.c_str());
					createProgram<T>(shader, type, name.data());
				}
			});
		}

		void clear() { mPrograms.clear(); };
		void errorCheck(sizet check, const uint32 shader, const char* filePath);
		GlslProgram find(const string&);
		const hashmap<string, GlslProgram>& all() const { return mPrograms; }

	private:
		hashmap<string, GlslProgram> mPrograms;

		template <typename T>
		inline void createProgram(uint32 shader, sizet type, const char* key) {
			auto id = glCreateProgram();
			if (!id) {
				std::cout << "program link error" << std::endl;
				return;
			}

			glProgramParameteri(id, GL_PROGRAM_SEPARABLE, GL_TRUE);
			glAttachShader(id, shader);

			IVertexData* vt = nullptr;
			if (type == GL_VERTEX_SHADER) {
				vt = new T();
				//vt = VertexDataRegistry::sRegister<T>();
				#ifdef QOPENGLVERSIONFUNCTIONS_4_5_CORE_H
					vt->initializeOpenGLFunctions();
				#endif
				vt->bindAttribLocation(id);
			}

			glLinkProgram(id);
			glDetachShader(id, shader);
			glDeleteShader(shader);

			auto it = mPrograms.find(key);
			if (it == mPrograms.end()) {
				mPrograms.try_emplace(key, id, vt, key);
			}
		}
	};

	extern ELANG_DLL ShaderLoader gGlsl;
}