#ifndef PKGLIST_HPP
#define PKGLIST_HPP

#include <string>
#include <list>

struct package_desc
{
	std::string name;
	std::list<std::string> deps;
	std::string cksum;
};

std::list<package_desc> pkglist_read(std::string file);

#endif // PKGLIST_HPP
