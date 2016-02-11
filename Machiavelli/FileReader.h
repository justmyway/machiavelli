#pragma once

#include <vector>

class FileReader
{
public:
	const static std::string _EOF;

	FileReader();
	std::vector<std::string> Read(const std::string &inputPath);
	std::string ColumnValue(const std::string &propertyLine, int column);
};

