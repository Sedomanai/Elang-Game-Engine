#pragma once

#define RAPIDJSON_HAS_STDSTRING 1
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/filewritestream.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include "fileio.h"

namespace el
{
	using jsondoc = rapidjson::Document;
	using jsonval = rapidjson::Value;
	using jsonstream = rapidjson::FileWriteStream;
	using jsonalloc = rapidjson::MemoryPoolAllocator<>;
	using jsontype = rapidjson::Type;
	inline auto jsonstr(const char* str) {
		return rapidjson::StringRef(str);
	}
	inline auto jsonstr(const char* str, sizet size) {
		return rapidjson::StringRef(str, size);
	}
	inline auto jsonstr(string& str) {
		return rapidjson::StringRef(str.c_str(), str.size());
	}

	inline sizet loadJson(const char* filePath, jsondoc& doc) {
		string str;
		sizet errortype;
		if (errortype = loadFile(filePath, str)) {
			doc.Parse(str.c_str());
		} return errortype;
	}

	inline void saveJson(const char* filePath, jsondoc& doc) {
		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		doc.Accept(writer);
		saveFile(filePath, buffer.GetString());
	}

	inline void findJson(const char* name, jsonval& parent, jsonval& child) {
		auto it = parent.FindMember(name);
		if (it != parent.MemberEnd())
			child = it->value;
	}
}