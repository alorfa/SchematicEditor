#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>

namespace al
{
	class File
	{
		FILE* fptr;
	public:
		bool open(const std::string& filename, const std::string& flags);
		void read(std::string& content);
		void write(std::string& data);
		void close();
		~File();
	};
}