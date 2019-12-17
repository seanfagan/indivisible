#pragma once
#include <string>
#include <vector>

struct Party {
	Party(const std::string& name, const std::string& name_plural, char ascii)
		: m_name(name), m_name_plural(name_plural), m_ascii(ascii) {};

	std::string m_name;
	std::string m_name_plural;
	char m_ascii;
};