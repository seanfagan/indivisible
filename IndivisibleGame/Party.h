#pragma once
#include <string>

struct Party {
	// Constructors
	Party(std::string const& name, std::string const& name_plural, char ascii)
		: m_name(name), m_name_plural(name_plural), m_ascii(ascii) {};

	// Vars
	std::string m_name;
	std::string m_name_plural;
	char m_ascii;
};