#pragma once

#include "../common/fileio.h"
#include <entt/entt.hpp>

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
			loadFile(filePath.generic_u8string().data(), text);
		}

		void exportFile(const fio::path& filePath, TextMeta&) {
			saveFile(filePath.generic_u8string().data(), text);
		}

		void unload(TextMeta&) { text.clear(); }
	};

	struct Thisisatest
	{

	};
}
