#include "app.hpp"
#include "helpers.hpp"
#include "builder.hpp"
#include <string>
#include <iostream>
#include <stdarg.h>

#ifndef VERSION
#define VERSION "unversioned"
#endif // VERSION

static const char* help_str =
"By default runs in interactive mode\n"
"Available options:\n"
"  --help           - this help\n"
"  --build          - build the project\n"
"  --clean          - clean the project\n"
"  --rebuild        - rebuild the project\n";

bool cli(int argc, char** argv)
{
	bool clean_req = false;
	bool build_req = false;

	for (int i = 1; i < argc; i++)
	{
		if (argv[i] == std::string("--help"))
		{
			std::printf("sea %s - C and C++ toolkit\n", VERSION);
			std::printf("%s\n", help_str);
			return true;
		}
		else if (argv[i] == std::string("--clean"))
		{
			clean_req = true;
			continue;
		}
		else if (argv[i] == std::string("--build"))
		{
			build_req = true;
			continue;
		}
		else if (argv[i] == std::string("--rebuild"))
		{
			clean_req = true;
			build_req = true;
			continue;
		}
		else
		{
			std::printf("Unknown argument: %s\n", argv[i]);
			std::printf("See help: sea --help\n");
			return true;
		}
	}

	bool job_done = false;

	if (clean_req || build_req)
	{
		init_data_model();
	}

	if (clean_req)
	{
		clean();
		job_done = true;
	}

	if (build_req)
	{
		build();
		job_done = true;
	}
	
	return job_done;
}

static std::string ask(std::string question)
{
	std::cout << question << '\n';
	std::string res;
	std::getline(std::cin, res);

	return res;
}

static int ask_num(std::string question, std::list<std::string> items)
{
	if (items.size() == 0)
	{
		std::printf("%s - no options\n", question.c_str());
		return 0;
	}

	int result = 0;
	while (true)
	{
		std::printf("%s\n", question.c_str());
		int counter = 1;
		for (std::string item : items)
		{
			std::printf("%d\t%s\n", counter, item.c_str());
			counter++;
		}

		try
		{
			std::string choice;
			std::getline(std::cin, choice);
			result = std::stoi(choice);
		}
		catch (...)
		{
			continue;
		}

		if (result < 1 || result > (int)items.size())
		{
			continue;
		}

		break;
	}

	return result;
}

static int ask_num(std::string question, size_t list_size, ...)
{
	va_list args;
	std::list<std::string> a;

	va_start(args, list_size);
	for (size_t i = 0; i < list_size; i++)
	{
		a.push_back(va_arg(args, const char*));
	}
	va_end(args);

	return ask_num(question, a);
}

bool ask_yn(std::string question)
{
	question.append(" (y/n)");
	while (true)
	{
		std::string res = ask(question);
		if (res != "y" && res != "n")
		{
			continue;
		}

		if (res == "y")
		{
			return true;
		}

		return false;
	}

	return false;
}

void print_variables()
{
	std::printf("Available variables: %s\n", ctx_keys().c_str());
}

static void set_project_name()
{
	ctx("project_name", ask("Enter the project name: "));
}

static void set_project_version()
{
	ctx("project_version", ask("Enter the project version: "));
}

static void set_project_type()
{
	switch (ask_num("Select the project type: ", 3,
				"Executable",
				"Static library",
				"Dynamic library"))
	{
		case 1: ctx("project_type", "executable"); break;
		case 2: ctx("project_type", "static_library"); break;
		case 3: ctx("project_type", "dynamic_library"); break;
		default: ctx("project_type", "executable"); break;
	}
}

void add_item(std::list<std::string>& tokens)
{
	print_variables();
	tokens.push_back(ask("Enter the new item: "));
}

void del_item(std::list<std::string>& tokens)
{
	std::list<std::string> variants = tokens;
	variants.push_back("Go back");
	size_t idx = ask_num("Select item to delete: ", variants);
	if (idx == variants.size())
	{
		return;
	}

	std::list<std::string>::iterator iter = tokens.begin();

	for (size_t i = 0; i < idx - 1; i++)
	{
		iter++;
	}

	tokens.erase(iter);
}

void edit_item(std::list<std::string>& tokens)
{
	std::list<std::string> variants = tokens;
	variants.push_back("Go back");
	size_t idx = ask_num("Select item to edit: ", variants);
	if (idx == variants.size())
	{
		return;
	}

	std::list<std::string>::iterator iter = tokens.begin();

	for (size_t i = 0; i < idx - 1; i++)
	{
		iter++;
	}

	*iter = ask("Enter the new value: ");
}

static void set_project_defines()
{
	std::list<std::string> defines = tokenize(ctx("project_defines"));
	while (true)
	{
		std::printf("Current defines: %s\n",
			ctx("project_defines").c_str());	
		switch (ask_num("Defines edit options: ", 4,
					"Add a new define",
					"Delete define",
					"Edit define",
					"Quit"))
		{
			case 1: add_item(defines); break;
			case 2: del_item(defines); break;
			case 3: edit_item(defines); break;
			case 4: ctx("project_defines", detokenize(defines));
				return;

			default: break;
		}

		ctx("project_defines", detokenize(defines));
	}
}

static void set_project_includes()
{
	std::list<std::string> includes = tokenize(ctx("project_includes"));
	while (true)
	{
		std::printf("Current includes: %s\n",
				ctx("project_includes").c_str());
		switch (ask_num("Includes edit options: ", 4,
					"Add a new include",
					"Delete include",
					"Edit include",
					"Quit"))
		{
			case 1: add_item(includes); break;
			case 2: del_item(includes); break;
			case 3: edit_item(includes); break;
			case 4: ctx("project_includes", detokenize(includes));
				return;

			default: break;
		}
		
		ctx("project_includes", detokenize(includes));
	}
}

void project_settings()
{
	while (true)
	{
		switch (ask_num("Project settings:", 6,
					std::string("Name: ")
					.append(ctx("project_name"))
					.c_str(),
					std::string("Version: ")
					.append(ctx("project_version"))
					.c_str(),
					std::string("Type: ")
					.append(ctx("project_type"))
					.c_str(),
					std::string("Defines: ")
					.append(ctx("project_defines"))
					.c_str(),
					std::string("Includes: ")
					.append(ctx("project_includes"))
					.c_str(),
					"Save and go back"))
		{
			case 1: set_project_name(); break;
			case 2: set_project_version(); break;
			case 3: set_project_type(); break;
			case 4: set_project_defines(); break;
			case 5: set_project_includes(); break;
			case 6: save_data_model(); return;
			default: break;
		}
	}
}

void project_build()
{
	std::printf("Build\n");
	build();
}

void project_clean()
{
	std::printf("Clean\n");
}

int interactive()
{
	while (true)
	{
		switch (ask_num(std::string("Project: ")
					.append(ctx("project_name")),
					4,
					"Change the project settings",
					"Build",
					"Clean",
					"Quit"))
		{
			case 1: project_settings(); break;
			case 2: project_build(); break;
			case 3: project_clean(); break;
			case 4: return 0;

			default: break;
		}
	}

	return 0;
}

void init_data_model()
{
	if (path_exists(".sea/conf"))
	{
		config conf(".sea/conf");

		if (conf["Project.name"].size())
		{
			ctx("project_name", conf["Project.name"][0]);
		}

		if (conf["Project.version"].size())
		{
			ctx("project_version", conf["Project.version"][0]);
		}

		if (conf["Project.type"].size())
		{
			ctx("project_type", conf["Project.type"][0]);
		}

		if (conf["Project.defines"].size())
		{
			ctx("project_defines", conf["Project.defines"][0]);
		}

		if (conf["Project.includes"].size())
		{
			ctx("project_includes", conf["Project.includes"][0]);
		}
	}
	else
	{
		if (ctx("project_name").empty())
		{
			ctx("project_name", dirname(curdir()));
		}

		ctx("project_version", "v0.1a");

		ctx("project_defines", "VERSION=\"\\\"$(project_version)\\\"\"");
		ctx("project_includes", ".");

		if (ctx("project_type").empty())
		{
			ctx("project_type", "executable");
		}
	}
}

void save_data_model()
{
	if (!path_exists(".sea"))
	{
		mkdir(".sea");
	}

	if (path_exists(".sea"))
	{
		rm(".sea/conf");
		file conf(".sea/conf");
		
		conf.append("[Project]");

		conf.append(std::string("name=")
				.append(ctx("project_name")));
		conf.append(std::string("version=")
				.append(ctx("project_version")));
		conf.append(std::string("type=")
				.append(ctx("project_type")));
		conf.append(std::string("defines=")
				.append(ctx("project_defines")));
		conf.append(std::string("includes=")
				.append(ctx("project_includes")));

		conf.write();
	}
}
