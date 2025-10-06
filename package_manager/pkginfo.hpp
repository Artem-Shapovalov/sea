#ifndef PKGINFO_HPP
#define PKGINFO_HPP

#include <string>
#include <list>
#include <utility>

struct package_info
{
	std::string name;
	std::string desc;
	std::string category;
	std::list<std::pair<std::string, std::string>> defines;
	std::list<std::string> includes;
	std::list<std::string> static_libs;
	std::list<std::string> dynamic_libs;
	std::list<std::string> library_paths;
};

package_info pkginfo_read(std::string file);

#endif // PKGINFO_HPP
