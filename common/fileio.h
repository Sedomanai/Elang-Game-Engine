#pragma once

#include <iostream>
#include <filesystem>

#include "stream.h"
#include "string.h"
#include "define.h"


namespace el 
{

	typedef std::filesystem::file_time_type ftimetype;
	typedef std::filesystem::directory_entry fentry;

	inline ftimetype lastWriteTime(const fentry& e) {
		return std::filesystem::last_write_time(e);
	}
/** Iterates through all the files **/
// output parameter : ()(const fentry& e)
template <typename Func>
inline void iterateFolder(const char* filePath, Func output) {
	for (const fentry& e : std::filesystem::directory_iterator(filePath)) {
		output(e);
	};
}


/** Iterates through all the files **/
// output parameter : ()(const fentry& e)
template <typename Func>
inline void iterateFolderRecursive(strview filePath, Func output) {
	for (const fentry& e : std::filesystem::recursive_directory_iterator(filePath)) {
		output(e);
	};
}

/** Create directories no matter how deep (deprected with std::filesystem) **/
inline void createDirectory(strview dir) {
	traverse(dir, '/', [](strview str) {
	if (!std::filesystem::is_directory(str))
		std::filesystem::create_directory(str);
	});
}

/** Create directories for the intended file path, no matter how deep **/
inline void createDirectoryForFilePath(strview filePath) {
	tokenize(filePath, '/', [](strview dir, strview) {
		/*createDirectory(dir);*/ //deprecated
		std::filesystem::create_directories(dir);
	});
};

/** Basic save file **/
inline void saveFile(strview filePath, strview output) {
	std::ofstream file;
	file.open(filePath, std::ios::out | std::ios::binary);
	file << output;
	file.close();
}

/** Basic load file **/
inline int loadFile(const char* filePath, string& input) {
	std::ifstream file(filePath, std::ios::in | std::ios::binary);
	
	if (file.fail()) {;
		std::cout << "File load failed: " << filePath << std::endl;
		file.close();
		return 0;
	}

	file.seekg(0, std::ios::end);
	auto size = file.tellg();
	file.seekg(0, std::ios::beg);
	size -= file.tellg();

	input.resize(sizet(size));
	file.read(&input[0], size);
	
	return 1;
}

#if _MSC_VER && !__INTEL_COMPILER
	inline int setenv(const char* name, const char* value, int overwrite) {
		int errcode = 0;
		if (!overwrite) {
			size_t envsize = 0;
			errcode = getenv_s(&envsize, NULL, 0, name);
			if (errcode || envsize) return errcode;
		}
		return _putenv_s(name, value);
	}

	inline string getenv(const char* name) {
		char* pValue2 = 0;
		size_t len = 0;
		_dupenv_s(&pValue2, &len, name);
		string str;
		str.copy(pValue2, len);
		//free(pValue2);
		return str;
	}
#endif
}