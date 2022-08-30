/*****************************************************************//**
 * @file   shader.h
 * @brief  General shader asset interface. Imports and compiles shader. Currently only supports GLSL.
 * 
 * @author Sedomanai
 * @date   August 2022
 *********************************************************************/

#pragma once

#include "../elang_library_builder.h"
#include "../common/fileio.h"
#include "vertex.h"
#include "hash.h"

namespace el {

	struct ShaderMeta
	{
		bool compiled;
		string file;

		ShaderMeta() : compiled(false) {}
	};

	/** 
	 * Shader for FragmentShader. Imports a fragment shader from .frag files.
	 * Currently only supports GLSL.
	 */
	struct ELANG_DLL FragmentShader
	{
		FragmentShader() : mShader(-1) {};

		/**
		 * Import and compile glsl fragment shader file, extension .frag.
		 * 
		 * @param path
		 * @param meta
		 */
		void importFile(const fio::path& path, ShaderMeta& meta);

		/**
		 * Shaders will probably never be unloaded, so not defined.
		 */
		void unload(ShaderMeta& meta) {}

		/**
		 * Opengl GLSL vertex shader program index. 
		 * Note that it's a GLSL PROGRAM index, not the shader index.
		 * This is because the shader program interface allows combinations of shader programs.
		 * In other words, a vertex shader program and a fragment shader program can fit and swapped like a puzzle piece.
		 * 
		 * @return Shader program index
		 */
		uint shader() { return mShader; }

	protected:

		/**
		 * Shaders are different from other assets that they are most probably never unloaded. 
		 * This means that if it's already compiled the meta will note it and will not recompile.
		 * 
		 * @param type
		 * @param meta
		 */
		void compileShaderVariant(GLuint type, ShaderMeta& meta);
		virtual void setupGlslVertexProgram(string& file) {};
		void glslErrorCheck(const uint32 shader, const char* filePath);
		uint mShader;
	};

	/**
	 * Shader for VertexShader. Parses and imports a vertex shader from .vert files.
	 * Currently only supports GLSL.
	 */
	struct ELANG_DLL VertexShader : FragmentShader
	{
		VertexShader() : FragmentShader(), mSize(0) {}

		 /**
		  * Import and compile glsl vertex shader file, extension .vert.
		  *
		  * @param path - absoltue path
		  * @param meta - ShaderMeta
		  */
		void importFile(const fio::path& path, ShaderMeta& meta);

		/**
		 *Wrapper for glEnableVertexAttribArray for all elements. Used by Painters. 
		 * Manual invoking of this method is fine but not recommended.
		 */
		void enableVertexAttribArray();

		/**
		 * Wrapper for glDisableVertexAttribArray. Used by Painters. 
		 * Manual invoking of this method is fine but not recommended.
		 */
		void disableVertexAttribArray();

		/**
		 * Wrapper for glVertexAttribPointer. Used by Painters. 
		 * Manual invoking of this method is fine but not recommended.
		 */
		void vertexAttribPointer();
		
		sizet size() { return mSize; }
	private:
		void addData(eDataType data);
		/**
		 * This is a very crude custom parser of glsl vertex shader.
		 * It works for now, but the algorithm may change to be more accurate and accomodating in the future.
		 * 
		 * @param file
		 */
		void setupGlslVertexProgram(string& file) override;

		vector<eDataType> mData;
		sizet mSize;
	};
}