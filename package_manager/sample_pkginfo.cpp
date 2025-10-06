#include "pkginfo.hpp"
#include <cstdio>

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		std::printf("Wrong arguments num");
		return -1;
	}

	package_info res = pkginfo_read(argv[1]);

	std::printf("%s / %s\n", res.name.c_str(), res.category.c_str());
	std::printf("%s\n", res.desc.c_str());
	
	std::printf("defines:\n");
	for (std::pair<std::string, std::string>& define : res.defines)
	{
		std::printf("\t%s\t\t%s\n",
				define.first.c_str(),
				define.second.c_str());
	}

	std::printf("includes:\n");
	for (std::string& include : res.includes)
	{
		std::printf("\t%s\n", include.c_str());
	}

	std::printf("static libraries:\n");
	for (std::string& static_lib : res.static_libs)
	{
		std::printf("\t%s\n", static_lib.c_str());
	}

	std::printf("dynamic libraries:\n");
	for (std::string& dynamic_lib : res.dynamic_libs)
	{
		std::printf("\t%s\n", dynamic_lib.c_str());
	}

	std::printf("library paths:\n");
	for (std::string& library_path : res.library_paths)
	{
		std::printf("\t%s\n", library_path.c_str());
	}

	return 0;
}
