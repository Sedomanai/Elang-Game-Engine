/*****************************************************************//**
 * @file   textfile.h
 * @brief  A placeholder to store auto-imported textfiles
 * 
 * @author Sedomanai
 * @date   August 2022
 *********************************************************************/
#pragma once
#include "tool_declarer.h"
#include "registry.h"

namespace el
{
	struct TextMeta
	{
		entt::entity self;
		int ss;
	};

	struct AssetSync;

	struct Text
	{
		friend struct AssetSync;
		string text;

	private:
		void importFile(const fio::path& filePath, TextMeta&) {
			el_file::load(filePath.generic_u8string().data(), text);
		}

		void exportFile(const fio::path& filePath, TextMeta&) {
			el_file::save(filePath.generic_u8string().data(), text);
		}

		void unload(TextMeta&) { text.clear(); }
	};

	struct Thisisatest
	{

	};
}
