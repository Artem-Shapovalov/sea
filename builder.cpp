#include "builder.hpp"
#include "helpers.hpp"
#include <string>
#include <cstdio>
#include <list>

std::string search_dependency(std::string path)
{
	std::list<std::string> includes =
		tokenize(ctx("project_includes"));
	includes.push_back(".");
	for (std::string include : includes)
	{
		std::string full_path = include;
		full_path.append("/");
		full_path.append(path);

		if (path_exists(full_path))
		{
			return full_path;
		}
	}

	return "";
}

bool is_header(std::string path)
{
	if (re_match(path, ".*\\.(h|hpp|hxx|pch)$").size())
	{
		return true;
	}

	return false;
}

bool is_c_source(std::string path)
{
	if (re_match(path, ".*\\.(c|cc)$").size())
	{
		return true;
	}
	
	return false;
}

bool is_cpp_source(std::string path)
{
	if (re_match(path, ".*\\.(cpp|cxx)$").size())
	{
		return true;
	}

	return false;
}

std::list<std::string> scan_deps(std::string path)
{
	std::list<std::string> res;

	file source(path);
	for (std::string line : source.get_lines())
	{
		std::vector<std::string> match =
			re_match(line,
				"^[ \\t]*#include.*[\"<]([^\"<>]+)[\">]");
		if (match.size() && is_header(match[1]))
		{
			res.push_back(search_dependency(match[1]));

			std::list<std::string> inner_deps =
				scan_deps(match[1]);

			res.insert(res.end(),
					inner_deps.begin(), inner_deps.end());
		}
	}

	return res;
}

struct build_item
{
	std::string path;
	bool resolved = false;
	enum
	{
		UNKNOWN,
		CPP_SOURCE,
		C_SOURCE,
		HEADER,
		OBJECT,
		STATIC_LIB,
		DYNAMIC_LIB,
		EXECUTABLE
	} type = UNKNOWN;
	std::list<build_item> deps;
};

build_item make_root_object()
{
	build_item res;
	res.type = 
		ctx("project_type") == "executable" ?
			build_item::EXECUTABLE :
		ctx("project_type") == "dynamic_library" ?
			build_item::DYNAMIC_LIB :
		ctx("project_type") == "static_library" ?
			build_item::STATIC_LIB :
		build_item::UNKNOWN;
	res.path = "./";
	res.path.append(ctx("project_name"));

	if (ctx("project_type") == "executable")
	{
	}
	else if (ctx("project_type") == "static_library")
	{
		res.path.append(".a");
	}
	else if (ctx("project_type") == "dynamic_library")
	{
		res.path.append(".so");
	}
	return res;
}

build_item make_build_tree()
{
	build_item res = make_root_object();

	std::list<std::string> sources = find(".", ".*\\.(c|cc|cpp|cxx)$");

	for (std::string source : sources)
	{
		build_item src;
		src.type =
			is_c_source(source) ? build_item::C_SOURCE :
			is_cpp_source(source) ? build_item::CPP_SOURCE :
			build_item::UNKNOWN;
		src.path = source;	

		build_item obj;
		obj.type = build_item::OBJECT;
		obj.path = ".sea/obj/";
		obj.path.append(sed(source, "\\.(c|cc|cpp|cxx)$", "\\.o"));
		obj.deps.push_back(src);

		std::list<std::string> deps = scan_deps(source);
		for (std::string dep : deps)
		{
			if (dep.empty())
			{
				continue;
			}

			build_item _dep;
			_dep.type = is_header(dep) ?
				build_item::HEADER : build_item::UNKNOWN;
			_dep.path = dep;

			obj.deps.push_back(_dep);
		}

		res.deps.push_back(obj);
	}

	return res;
}

std::string item_type(build_item& item)
{
	std::string type;

	switch (item.type)
	{
		case build_item::CPP_SOURCE: type = "CPP_SOURCE"; break;
		case build_item::C_SOURCE: type = "C_SOURCE"; break;
		case build_item::HEADER: type = "HEADER"; break;
		case build_item::OBJECT: type = "OBJECT"; break;
		case build_item::STATIC_LIB: type = "STATIC_LIB"; break;
		case build_item::DYNAMIC_LIB: type = "DYNAMIC_LIB"; break;
		case build_item::EXECUTABLE: type = "EXECUTABLE"; break;
		default: type = "UNKNOWN"; break;
	}

	return type;
}

std::string list_dep_objects(build_item item)
{
	std::string list;

	for (build_item obj : item.deps)
	{
		if (obj.type != build_item::OBJECT)
		{
			continue;
		}

		if (!list.empty())
		{
			list.append(" ");
		}

		list.append(obj.path);
	}

	return list;
}

std::string list_defines()
{
	std::string res;

	for (std::string def : tokenize(eval(ctx("project_defines"))))
	{
		if (!res.empty())
		{
			res.append(" ");
		}

		res
			.append("-D")
			.append(def);
	}

	return res;
}

std::string list_includes()
{
	std::string res;

	for (std::string inc : tokenize(eval(ctx("project_includes"))))
	{
		if (!res.empty())
		{
			res.append(" ");
		}

		res
			.append("-I")
			.append(inc);
	}

	return res;
}

std::string build_cmd(build_item item)
{
	std::string cmd;
	std::string opath = ".sea/obj/";

	switch (item.type)
	{
		case build_item::C_SOURCE:
			opath.append(sed(item.path, "\\.*(c|cc)$", "\\.o"));
			cmd
				.append("gcc -c ")
				.append(ctx("project_type")
					== "dynamic_library" ? "-fPIC " : "")
				.append(item.path)
				.append(" -o ")
				.append(opath)
				.append(" ")
				.append(list_defines())
				.append(" ")
				.append(list_includes());
			break;

		case build_item::CPP_SOURCE:
			opath.append(sed(item.path, "\\.(cpp|cxx)$", "\\.o"));
			cmd
				.append("g++ -c ")
				.append(ctx("project_type")
					== "dynamic_library" ? "-fPIC " : "")
				.append(item.path)
				.append(" -o ")
				.append(opath)
				.append(" ")
				.append(list_defines())
				.append(" ")
				.append(list_includes());
			break;

		case build_item::EXECUTABLE:
			cmd
				.append("g++ ")
				.append(list_dep_objects(item))
				.append(" -o ")
				.append(item.path);
			break;

		case build_item::STATIC_LIB:
			cmd
				.append("ar rcs ")
				.append(item.path)
				.append(" ")
				.append(list_dep_objects(item));
			break;

		case build_item::DYNAMIC_LIB:
			cmd
				.append("g++ -shared ")
				.append(list_dep_objects(item))
				.append(" -o ")
				.append(item.path);
			break;

		default: break;
	}

	return cmd;
}

bool process(build_item& item)
{
	for (build_item& i : item.deps)
	{
		if (process(i) == false)
		{
			return false;
		}
	}

	std::string cmd = build_cmd(item);
	if (cmd.empty())
	{
		return true;
	}

	if (item.type == build_item::C_SOURCE ||
		item.type == build_item::CPP_SOURCE)
	{
		std::string opath = ".sea/obj/";
		opath.append(sed(item.path, "[^/]+\\.(c|cc|cpp|cxx)$", ""));
		mkdir(opath);
	}

	std::printf("%s\n", cmd.c_str());
	int res = exec(cmd);
	if (res)
	{
		return false;
	}

	return true;
}

bool build()
{
	build_item root = make_build_tree();
	process(root);

	return true;
}

bool clean()
{
	rm(".sea/obj");
	rm(make_root_object().path);
	return false;
}
