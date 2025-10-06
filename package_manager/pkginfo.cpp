#include "pkginfo.hpp"
#include <fstream>

static std::pair<std::string, std::string> split(std::string val)
{
	std::pair<std::string, std::string> res;

	bool first = true;

	for (char c : val)
	{
		if (c == '=')
		{
			first = false;
			continue;
		}

		if (first)
		{
			res.first.push_back(c);
		}
		else
		{
			res.second.push_back(c);
		}
	}

	return res;
}

package_info pkginfo_read(std::string file)
{
	package_info res;

	std::ifstream ifile(file);

	if (!ifile.is_open())
	{
		return res;
	}

	char ch;
	std::string current;
	enum class stage
	{
		key,
		value
	} stage = stage::key;

	std::string key;

	while (ifile.get(ch))
	{
		if (ch == ':' && stage == stage::key)
		{
			key = current;
			current.clear();
			stage = stage::value;
			continue;
		}
		else if (ch == '\n' && stage == stage::value
				&& key == "name")
		{
			res.name = current;
			current.clear();
			stage = stage::key;
			continue;
		}
		else if (ch == '\n' && stage == stage::value
				&& key == "desc")
		{
			res.desc = current;
			current.clear();
			stage = stage::key;
			continue;
		}
		else if (ch == '\n' && stage == stage::value
				&& key == "category")
		{
			res.category = current;
			current.clear();
			stage = stage::key;
			continue;
		}
		else if (ch == '\n' && stage == stage::value
				&& key == "defines")
		{
			res.defines.push_back(split(current));
			current.clear();
			stage = stage::key;
			continue;
		}
		else if (ch == '\n' && stage == stage::value
				&& key == "includes")
		{
			res.includes.push_back(current);
			current.clear();
			stage = stage::key;
			continue;
		}
		else if (ch == '\n' && stage == stage::value
				&& key == "static_libs")
		{
			res.static_libs.push_back(current);
			current.clear();
			stage = stage::key;
			continue;
		}
		else if (ch == '\n' && stage == stage::value
				&& key == "dynamic_libs")
		{
			res.dynamic_libs.push_back(current);
			current.clear();
			stage = stage::key;
			continue;
		}
		else if (ch == '\n' && stage == stage::value
				&& key == "library_paths")
		{
			res.library_paths.push_back(current);
			current.clear();
			stage = stage::key;
			continue;
		}
		else if (ch == ',' && stage == stage::value
				&& key == "defines")
		{
			res.defines.push_back(split(current));
			current.clear();
			continue;
		}
		else if (ch == ',' && stage == stage::value
				&& key == "includes")
		{
			res.includes.push_back(current);
			current.clear();
			continue;
		}
		else if (ch == ',' && stage == stage::value
				&& key == "static_libs")
		{
			res.static_libs.push_back(current);
			current.clear();
			continue;
		}
		else if (ch == ',' && stage == stage::value
				&& key == "dynamic_libs")
		{
			res.dynamic_libs.push_back(current);
			current.clear();
			continue;
		}
		else if (ch == ',' && stage == stage::value
				&& key == "library_paths")
		{
			res.library_paths.push_back(current);
			current.clear();
			continue;
		}

		current.push_back(ch);
	}

	ifile.close();
	return res;
}
