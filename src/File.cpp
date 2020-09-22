#include "File.hpp"

namespace al
{
	bool File::open(const std::string& filename, const std::string& flags)
	{
		if (fptr = fopen(filename.c_str(), flags.c_str()))
			return true;
		return false;
	}
	void File::read(std::string& content)
	{
		int sumbol;
		while ((sumbol = fgetc(fptr)) != EOF)
			content += sumbol;
	}
	void File::write(std::string& data)
	{
		for (size_t i = 0; i < data.size(); i++)
			fputc(data[i], fptr);
	}
	void File::close()
	{
		if (fptr)
		{
			fclose(fptr);
			fptr = nullptr;
		}
	}
	File::~File()
	{
		close();
	}
}