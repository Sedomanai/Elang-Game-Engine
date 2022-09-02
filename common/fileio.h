/*****************************************************************//**
 * @file   fileio.h
 * @brief  Basic wrapper for c++ std::filesystem and related algorithms
 *
 * @author Sedomanai
 * @date   August 2022
 *********************************************************************/
#pragma once
#include <filesystem>
#include "define.h"
#include "string.h"
#include "../elang_builder.h"

namespace el
{
	namespace fio = std::filesystem;

	namespace el_file
	{
		/**
		 * Gets the unique identifier of a path
		 * @brief Implementation OS dependent
		 * @brief ****
		 * @brief *** WARNING: Currently only supports Windows
		 *
		 * @param path : full path of filesystem
		 * @return File unique ID in long long type
		 */
		extern ELANG_DLL long long identifier(fio::path path);

		/**
		 * Most basic file output function
		 *
		 * @param filePath : Directory path address
		 * @param output : Content
		 *
		 * @return 0 if success, 1 if fail (address failure)
		 */
		extern ELANG_DLL int save(strview filePath, strview output);

		/**
		 * Most basic file input function
		 *
		 * @param filePath : Directory path address
		 * @param input : Completely replaces with content
		 *
		 * @return 0 if success, 1 if fail
		 */
		extern ELANG_DLL int load(const char* filePath, string& input);
	}

	namespace el_env
	{

#if _MSC_VER && !__INTEL_COMPILER
		/**
		 * Set local environment variable, not global
		 *
		 * @param name : Name of variable
		 * @param value : Value of variable
		 * @param overwrite : check for overwrite errors if true
		 *
		 * @return Errorcode, 0 if success
		 */
		extern ELANG_DLL int setenv(const char* name, const char* value, int overwrite);

		/**
		 * Get local environment variable, not global
		 *
		 * @param name : Name of variable
		 * @return : Value of variable in string. Returns empty string if environment doesn't exist
		 */
		extern ELANG_DLL string getenv(const char* name);
#endif
	}
}