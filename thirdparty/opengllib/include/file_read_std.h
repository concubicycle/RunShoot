#ifndef _FILE_READ_STD_H__
#define _FILE_READ_STD_H__

#include <string>
#include <iostream>
#include <fstream>

#include "spdlog/spdlog.h"

class FileReadStd
{
protected:
	std::string readFile(const char *fileName) const
	{
		std::ifstream file_stream(fileName);
		std::string file;
		std::string line;

		if (!file_stream.is_open())
		{
			spdlog::error("Failed to read file {0}", fileName);
		}

		while (file_stream.good())
		{
			getline(file_stream, line);
			file.append(line);
			file.append("\n");
		}
		file_stream.close();

		return file;
	}
};

#endif