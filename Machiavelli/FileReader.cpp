#include "FileReader.h"

#include <fstream>
#include <sstream>
#include <iostream>

const std::string FileReader::_EOF = "EOF";

FileReader::FileReader()
{
}

std::vector<std::string> FileReader::Read(const std::string &inputPath)
{
	std::ifstream stream(inputPath.c_str());
	if (!stream.good()) {
		throw std::runtime_error("Could not read input file '" + inputPath + "'.");
		return std::vector<std::string>();
	}

	std::string line;
	std::vector<std::string> lines;

	while (getline(stream, line)) {
		if (line.empty())
			continue;

		lines.push_back(line);
	}

	lines.push_back(_EOF);

	return lines;
}

std::string FileReader::ColumnValue(const std::string &propertyLine, int column)
{
	std::stringstream streamLine(propertyLine);
	std::string segment;
	while (getline(streamLine, segment, ';')) {
		column--;
		if (column == 0)
			return segment;
	}

	throw std::runtime_error("Could not read segment '" + std::to_string(column) + "' from '" + propertyLine + "'.");
}
