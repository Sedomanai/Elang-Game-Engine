#pragma once

#include "../elang_library_builder.h"
#include "../common/fileio.h"
#include "vertex.h"
#include "hash.h"

namespace el {

	struct FragmentShader
	{
		FragmentShader() : mShader(-1) {};
		void compileShader(string& data, GLenum type);
		void compileShaderFromGlslFile(const string& filePath);
		uint shader() { return mShader; }

	protected:
		virtual void glslVertexProgram(string& file) {};
		void glslErrorCheck(const uint32 shader, const char* filePath);
		uint mShader;
	};

	struct VertexShader : FragmentShader
	{
		VertexShader() : FragmentShader(), mSize(0) {}

		void addData(eDataType data);
		void enableVertexAttribArray();
		void disableVertexAttribArray();
		void vertexAttribPointer();
		
		sizet size() { return mSize; }
	private:
		void glslVertexProgram(string& file) override;

		vector<eDataType> mData;
		sizet mSize;
	};

	extern ELANG_DLL hashmap<string, FragmentShader> gFragmentShaders;
	extern ELANG_DLL hashmap<string, VertexShader> gVertexShaders;
}