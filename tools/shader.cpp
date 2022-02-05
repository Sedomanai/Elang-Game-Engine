#include "shader.h"

namespace el {
	hashmap<string, FragmentShader> gFragmentShaders;
	hashmap<string, VertexShader> gVertexShaders;

	void FragmentShader::compileShaderFromGlslFile(const string& filePath) {
		tokenize(filePath, '.', [&](strview, strview tail) {
			string file;
			if (loadFile(filePath.c_str(), file))
				compileShader(file, ((tail == ".vert") ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER));
			});
	}

	void FragmentShader::compileShader(string& file, GLenum type)
	{
		const char* link = file.c_str();
		const uint32 shader = glCreateShader(type);

		if (shader) {
			glShaderSource(shader, 1, &link, NULL);
			glCompileShader(shader);

			int compiled = GL_FALSE;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
			if (compiled == GL_FALSE)
				glslErrorCheck(shader, file.c_str());
			else {
				auto id = glCreateProgram();
				if (!id) {
					std::cout << "program link error" << std::endl;
					return;
				}

				mShader = id;
				glProgramParameteri(mShader, GL_PROGRAM_SEPARABLE, GL_TRUE);
				glAttachShader(mShader, shader);
				glslVertexProgram(shader, file);
				glLinkProgram(mShader);
				glDetachShader(mShader, shader);
				glDeleteShader(shader);
			}
		}
	}

	void FragmentShader::glslErrorCheck(const uint32 shader, const char* filePath) {
		GLint maxLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

		cout << "shader compile error " << endl;
		cout << filePath << endl;
		cout << endl;
		std::printf("%s\n", &(errorLog[0]));

		glDeleteShader(shader);
	}

	void VertexShader::addData(eDataType data) {
		switch (data) {
		case eDataType::VEC2: mSize += sizeof(vec2); break;
		case eDataType::VEC3: mSize += sizeof(vec3); break;
		case eDataType::VEC4: mSize += sizeof(vec4); break;
		case eDataType::COLOR8: mSize += sizeof(color8); break;
		} mData.emplace_back(data);
	}

	void VertexShader::enableVertexAttribArray() {
		for (sizet i = 0; i < mData.size(); i++)
			glEnableVertexAttribArray(uint(i));
	}

	void VertexShader::disableVertexAttribArray() {
		for (sizet i = 0; i < mData.size(); i++)
			glEnableVertexAttribArray(uint(i));
	}

	void VertexShader::vertexAttribPointer() {
		sizet offset = 0;
		for (sizet i = 0; i < mData.size(); i++) {
			switch (mData[i]) {
			case eDataType::VEC2:
				glVertexAttribPointer((uint)i, 2, GL_FLOAT, GL_FALSE, (int)mSize, &offset);
				offset += sizeof(vec2);
				break;
			case eDataType::VEC3:
				glVertexAttribPointer((uint)i, 3, GL_FLOAT, GL_FALSE, (int)mSize, &offset);
				offset += sizeof(vec3);
				break;
			case eDataType::VEC4:
				glVertexAttribPointer((uint)i, 4, GL_FLOAT, GL_FALSE, (int)mSize, &offset);
				offset += sizeof(vec4);
				break;
			case eDataType::COLOR8:
				glVertexAttribPointer((uint)i, 4, GL_UNSIGNED_BYTE, GL_TRUE, (int)mSize, &offset);
				offset += sizeof(color8);
				break;
			}
		} cout << offset << endl;
	}

	void VertexShader::glslVertexProgram(uint32 shader, string& file) {
		iterate(file, '\n', [&](strview line, sizet) {
			int read = 0;
			uint nameshift = 0;
			bool bvec4 = false;
			iterate(line, ' ', [&](strview str, sizet) {
				if (read == 1) {
					if (str == "vec2")
						addData(eDataType::VEC2);
					else if (str == "vec3")
						addData(eDataType::VEC3);
					else if (str == "vec4") {
						bvec4 = true;
					} read++;
				} else if (read == 2) {
					if (bvec4) {
						if (str[0] == 'v' && str[1] == '8')
							addData(eDataType::COLOR8);
						else addData(eDataType::VEC4);
						bvec4 = false;
					} 

					string name = string(str);
					name.pop_back();
					glBindAttribLocation(shader, nameshift, name.c_str());
					read = 0;
				}

				if (str == "in") {
					read++;
				}
			});
		});
	}
}
