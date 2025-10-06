#include "pkglist.hpp"
#include <fstream>

std::list<package_desc> pkglist_read(std::string file)
{
	std::list<package_desc> res;

	std::ifstream ifile(file);

	if (!ifile.is_open())
	{
		return res;
	}

	char ch;
	std::string current;
	enum class stage
	{
		name,
		deps,
		cksum
	} stage = stage::name;
	package_desc candidate;

	while (ifile.get(ch))
	{
		if (ch == ':' && stage == stage::name)
		{
			candidate.name = current;
			current.clear();
			stage = stage::deps;
			continue;
		}
		else if (ch == ':' && stage == stage::deps)
		{
			if (!current.empty())
			{
				candidate.deps.push_back(current);
			}
			current.clear();
			stage = stage::cksum;
			continue;
		}
		else if (ch == ',' && stage == stage::deps)
		{
			if (!current.empty())
			{
				candidate.deps.push_back(current);
			}
			current.clear();
			continue;
		}
		else if (ch == '\n' && stage == stage::cksum)
		{
			candidate.cksum = current;
			res.push_back(candidate);
			current.clear();
			candidate.name.clear();
			candidate.deps.clear();
			candidate.cksum.clear();
			stage = stage::name;
			continue;
		}

		current.push_back(ch);
	}

	ifile.close();
	return res;
}
