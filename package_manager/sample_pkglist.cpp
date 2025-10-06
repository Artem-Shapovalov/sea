#include "pkglist.hpp"
#include <cstdio>

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		std::printf("Wrong arguments num");
		return -1;
	}

	std::list<package_desc> packages = pkglist_read(argv[1]);

	for (package_desc& package : packages)
	{
		std::printf("%s / %s\n",
				package.name.c_str(),
				package.cksum.c_str());
		for (std::string& dep : package.deps)
		{
			std::printf("\t- %s\n", dep.c_str());
		}

		std::printf("\n");
	}

	return 0;
}
