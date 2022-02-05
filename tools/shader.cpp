#include "shader.h"

namespace el {

	ShaderLoader gGlsl;

	void ShaderLoader::compileDefaultShaders(const char* assetdir) {
		static bool done = false;
		if (!done) {
			string dir = string(assetdir);
			compileShaderProgram<Position2DVertexData>(GL_VERTEX_SHADER, dir + "_2d_pos.vert");
			compileShaderProgram<SpriteVertexData>(GL_VERTEX_SHADER, dir + "_basic_sprite.vert");
			compileShaderProgram<Primitive2DVertexData>(GL_VERTEX_SHADER, dir + "_debug2d.vert");
			compileShaderProgram<PrimitiveVertexData>(GL_VERTEX_SHADER, dir + "_primitive3d.vert");
			compileShaderProgram<>(GL_FRAGMENT_SHADER, dir + "grid.frag");
			compileShaderProgram<>(GL_FRAGMENT_SHADER, dir + "raw_pass.frag");
			compileShaderProgram<>(GL_FRAGMENT_SHADER, dir + "texture_uv.frag");
			done = true;
		}
	}

	void ShaderLoader::errorCheck(sizet check, const uint32 shader, const char* filePath) {
		if (check == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

			std::cout << "shader compile error " << filePath << std::endl;
			std::printf("%s\n", &(errorLog[0]));

			glDeleteShader(shader);
		}
	}

	GlslProgram ShaderLoader::find(const string& key) {
		try {
			return mPrograms.at(key);
		} catch (std::exception& e) {
			cout << e.what() << " at line ShaderLoader::find with key " << key;
			return GlslProgram(0, 0, 0);
		}
	}
}
