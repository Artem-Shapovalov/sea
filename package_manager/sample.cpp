#include "extractor.hpp"
#include <cstdio>

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		std::printf("Wrong arguments\n");
		return -1;
	}

	if (!extract(argv[1], argv[2]))
	{
		std::printf("Can't extract archive\n");
		return -1;
	}

	return 0;
}
