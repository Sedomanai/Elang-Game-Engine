#pragma once

#include <rapidxml/rapidxml_iterators.hpp>
#include <rapidxml/rapidxml_print.hpp>
#include <rapidxml/rapidxml_utils.hpp>
#include <rapidxml/rapidxml.hpp>

#include "fileio.h"

namespace el
{

	using xmlntype = rapidxml::node_type;
	using xmldoc = rapidxml::xml_document<char>;
	using xmlnode = rapidxml::xml_node<char>;
	using xmlattrib = rapidxml::xml_attribute<char>;
	using xmlbase = rapidxml::xml_base<char>;

	inline void loadXML(const char* filePath, xmldoc& doc, string& mem) {
		loadFile(filePath, mem);
		doc.parse<0>(&mem[0]);
	}
	inline void saveXML(const char* filePath, xmldoc& doc) {
		string out;
		rapidxml::print(std::back_inserter(out), doc, 0);
		saveFile(filePath, out);
	}
}