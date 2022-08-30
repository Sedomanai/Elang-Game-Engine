/*****************************************************************//**
 * @file   fileio.h
 * @brief  Basic wrapper for c++ std::filesystem and related algorithms
 * 
 * @author Sedomanai
 * @date   August 2022
 *********************************************************************/

#pragma once

#include <iostream>
#include <filesystem>

#include "stream.h"
#include "string.h"
#include "define.h"

#ifdef _WIN32
#include "windows.h"
#endif

namespace el 
{

	namespace fio = std::filesystem;

	inline long long fileIdentifier(fio::path path) {
#ifdef _WIN32
		HANDLE fHandle;
		DWORD  dwBytesRead = 0;
		char   ReadBuffer[] = { 0 };
		LPOVERLAPPED ol = { 0 };

		string p = path.generic_u8string();

		fHandle = CreateFileA(p.c_str(),  // file to open
				GENERIC_READ,          // open for reading
				FILE_SHARE_READ,       // share for reading
				NULL,                  // default security
				OPEN_EXISTING,         // existing file only
				FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, // normal file
				NULL);

		BY_HANDLE_FILE_INFORMATION finfo;
		GetFileInformationByHandle(fHandle, &finfo);
		
		ULARGE_INTEGER ul;
		ul.LowPart = finfo.nFileIndexLow;
		ul.HighPart = finfo.nFileIndexHigh;

		CloseHandle(fHandle);

		return ul.QuadPart;
#endif

		cout << "This OS does not support file inode or a similar way to detect a file. Please create another one." << endl;
		return -1;
	}


	/**
	 * Most basic file output function.
	 * 
	 * @param filePath- Directory path address
	 * @param output- Content
	 * 
	 * @return 0 if success, 1 if fail (address failure)
	 */
	inline int saveFile(strview filePath, strview output) {
		std::ofstream file;
		file.open(filePath, std::ios::out | std::ios::binary);
		if (file.fail()) {
			std::cout << "File save failed, check for valid filepath: " << filePath << std::endl;
			file.close();
			return 1;

		}
		file << output;
		file.close();
		return 0;
	}

	/**
	 * Most basic file input function.
	 *
	 * @param filePath- Directory path address
	 * @param input- Completely replaces with content
	 * 
	 * @return 0 if success, 1 if fail
	 */
	inline int loadFile(const char* filePath, string& input) {
		std::ifstream file(filePath, std::ios::in | std::ios::binary);
	
		if (file.fail()) {;
			std::cout << "File load failed: " << filePath << std::endl;
			file.close();
			return 1;
		}

		file.seekg(0, std::ios::end);
		auto size = file.tellg();
		file.seekg(0, std::ios::beg);
		size -= file.tellg();

		input.resize(sizet(size));
		file.read(&input[0], size);
		file.close();
	
		return 0;
	}

#if _MSC_VER && !__INTEL_COMPILER

	/**
	 * Set local environment variable, not global.
	 * 
	 * @param name- Name of variable
	 * @param value- Value of variable
	 * @param overwrite- check for overwrite errors if true
	 * 
	 * @return Errorcode, 0 if success
	 */
	inline int setenv(const char* name, const char* value, int overwrite) {
		int errcode = 0;
		if (!overwrite) {
			size_t envsize = 0;
			errcode = getenv_s(&envsize, NULL, 0, name);
			if (errcode || envsize) return errcode;
		}
		return _putenv_s(name, value);
	}

	/**
	 * Get local environment variable, not global.
	 * 
	 * @param name- Name of variable
	 * @return- Value of variable in string. Returns empty string if environment doesn't exist
	 */
	inline string getenv(const char* name) {
		char* pValue2 = 0;
		size_t len = 0;
		_dupenv_s(&pValue2, &len, name);
		string str;
		if (pValue2)
			str.copy(pValue2, len);
		//free(pValue2);
		return str;
	}
#endif
}