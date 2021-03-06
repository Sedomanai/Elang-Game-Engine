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

	void FragmentShader::compileShader(string& glsl, GLenum type)
	{
		const char* link = glsl.c_str();
		const uint blob = glCreateShader(type);

		if (blob) {
			glShaderSource(blob, 1, &link, NULL);
			glCompileShader(blob);

			int compiled = GL_FALSE;
			glGetShaderiv(blob, GL_COMPILE_STATUS, &compiled);
			if (compiled == GL_FALSE)
				glslErrorCheck(blob, glsl.c_str());
			else {
				mShader = glCreateProgram();
				if (!mShader) {
					std::cout << "program link error" << std::endl;
					mShader = -1;
					return;
				}

				glProgramParameteri(mShader, GL_PROGRAM_SEPARABLE, GL_TRUE);
				glAttachShader(mShader, blob);
				glslVertexProgram(glsl);
				glLinkProgram(mShader);
				glDetachShader(mShader, blob);
				glDeleteShader(blob);
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
			glDisableVertexAttribArray(uint(i));
	}

	void VertexShader::vertexAttribPointer() {
		sizet offset = 0;
		for (sizet i = 0; i < mData.size(); i++) {
			switch (mData[i]) {
			case eDataType::VEC2:
				glVertexAttribPointer((uint)i, 2, GL_FLOAT, GL_FALSE, (int)mSize, (void*)offset);
				offset += sizeof(vec2);
				break;
			case eDataType::VEC3:
				glVertexAttribPointer((uint)i, 3, GL_FLOAT, GL_FALSE, (int)mSize, (void*)offset);
				offset += sizeof(vec3);
				break;
			case eDataType::VEC4:
				glVertexAttribPointer((uint)i, 4, GL_FLOAT, GL_FALSE, (int)mSize, (void*)offset);
				offset += sizeof(vec4);
				break;
			case eDataType::COLOR8:
				glVertexAttribPointer((uint)i, 4, GL_UNSIGNED_BYTE, GL_TRUE, (int)mSize, (void*)offset);
				offset += sizeof(color8);
				break;
			}
		}
	}

	void VertexShader::glslVertexProgram(string& file) {
		int state = 0, nameshift = 0;
		iterate(file, '\n', [&](strview line, sizet) -> bool {
			int read = 0;
			bool bvec4 = false;
			if (state == 2)
				return true;

			iterate(line, ' ', [&](strview str, sizet) -> bool {
				switch (read) {
				default:
					if (str == "in") {
						read++;
						if (state == 0)
							state = 1;
					} else if (state == 1) {
						state = 2;
						return true;
					}
					break;
				case 1:
					if (str == "vec2")
						addData(eDataType::VEC2);
					else if (str == "vec3")
						addData(eDataType::VEC3);
					else if (str == "vec4") {
						bvec4 = true;
					} read++;
					break;
				case 2:
					if (bvec4) {
						if (str[0] == 'v' && str[1] == '8')
							addData(eDataType::COLOR8);
						else addData(eDataType::VEC4);
					}

					string name = string(str);
					if (name.size() > 0 && name.back() == ';') {
						name.pop_back();
					}

					glBindAttribLocation(mShader, nameshift, name.c_str());
					nameshift++;
					break;
				} return false;
			}); return false;
		});
	}
}
