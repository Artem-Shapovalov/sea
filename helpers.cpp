#include "helpers.hpp"
#include <regex>
#include <chrono>
#include <filesystem>
#include <utility>
#include <fstream>

std::vector<std::string> re_match(std::string line, std::string re)
{
	std::vector<std::string> res;

	std::smatch m;
	if (std::regex_search(line, m, std::regex(re)))
	{
		for (std::string match : m)
		{
			res.push_back(match);
		}
	}

	return res;
}

std::string sed(std::string line, std::string re, std::string pattern)
{
	return std::regex_replace(line, std::regex(re), pattern,
			std::regex_constants::format_sed);
}

int exec(std::string cmd)
{
	return std::system(cmd.c_str());
}

bool mkdir(std::string path)
{
	std::list<std::string> dirnames;
	std::string current_dirname;

	for (char c : path)
	{
		if (c == '/')
		{
			dirnames.push_back(current_dirname);
			current_dirname.clear();
			continue;
		}

		current_dirname.push_back(c);
	}

	if (!current_dirname.empty())
	{
		dirnames.push_back(current_dirname);
	}

	std::string current_path = ".";
	for (std::string dir : dirnames)
	{
		current_path.append("/").append(dir);
		try
		{
			if (path_exists(current_path) &&
				is_dir(current_path))
			{
				continue;
			}

			if (!std::filesystem::create_directory(current_path))
			{
				return false;
			}
		}
		catch (...)
		{
			return false;
		}
	}

	return true;
}

bool touch(std::string path)
{
	file f(path);
	f.read();
	return f.write();
}

bool rm(std::string path)
{
	try
	{
		std::filesystem::remove_all(path);
	}
	catch (...)
	{
		return false;
	}

	return true;
}

bool cp(std::string src, std::string dst)
{
	std::filesystem::copy(src, dst);
	return true;
}

bool mv(std::string src, std::string dst)
{
	if (!cp(src, dst))
	{
		return false;
	}

	if (!rm(src))
	{
		return false;
	}

	return true;
}

std::list<std::string> ls(std::string path)
{
	std::list<std::string> res;

	for (const std::filesystem::directory_entry& entry :
			std::filesystem::directory_iterator(path))
	{
		res.push_back(entry.path().string());
	}

	return res;
}

std::list<std::string> find(std::string path, std::string re)
{
	std::list<std::string> res;

	for (std::string item : ls(path))
	{
		if (is_dir(item))
		{
			std::list<std::string> nested = find(item, re);

			for (std::string i : nested)
			{
				res.push_back(i);
			}
		}

		if (is_file(item) && !re_match(item, re).empty())
		{
			res.push_back(item);
		}
	}

	return res;
}

bool path_exists(std::string path)
{
	return std::filesystem::exists(path);
}

std::string abspath(std::string path)
{
	return std::filesystem::absolute(path).string();
}

std::string curdir()
{
	return std::filesystem::current_path().string();
}

std::string dirname(std::string path)
{
	if (path.empty())
	{
		return "";
	}

	if (!path_exists(path))
	{
		return "";
	}

	std::string res = path;

	if (is_file(path))
	{
		res = sed(path, "/.*$", "");
	}

	if (!is_dir(res))
	{
		return "";
	}

	res = sed(res, ".*/([^/]+)$", "\\1");

	return res;
}

bool is_file(std::string path)
{
	return std::filesystem::is_regular_file(path);
}

bool is_dir(std::string path)
{
	return std::filesystem::is_directory(path);
}

std::time_t last_modified(std::string path)
{
	std::filesystem::file_time_type t =
		std::filesystem::last_write_time(path);
	auto tp = std::chrono::time_point_cast
		<std::chrono::system_clock::duration>(
			t -
			std::filesystem::file_time_type::clock::now() +
			std::chrono::system_clock::now());
	return std::chrono::system_clock::to_time_t(tp);
}

static std::map<std::string, std::string> ctx_values;

std::string ctx(std::string key)
{
	if (ctx_values.empty())
	{
		return "";
	}

	if (ctx_values.find(key) == ctx_values.end())
	{
		return "";
	}

	return ctx_values[key];
}

bool ctx(std::string key, std::string value)
{
	ctx_values[key] = value;
	return true;
}

bool ctx_has_key(std::string key)
{
	if (ctx_values.find(key) == ctx_values.end())
	{
		return false;
	}

	return true;
}

std::list<std::string> tokenize(std::string line)
{
	std::list<std::string> res;

	std::string current_item;
	bool in_quotes = false;
	bool escaped = false;

	for (char c : line)
	{
		if (c != '\\' && escaped)
		{
			escaped = false;
		}

		if (c == '\\')
		{
			escaped = true;
		}

		if (c == '"' && !in_quotes)
		{
			in_quotes = true;
		}

		if (c == '"' && in_quotes)
		{
			in_quotes = false;
		}

		if (c == ' ' && !escaped && !in_quotes)
		{
			if (!current_item.empty())
			{
				res.push_back(current_item);
			}

			current_item.clear();
			continue;
		}

		current_item.push_back(c);
	}

	if (!current_item.empty() && !escaped && !in_quotes)
	{
		res.push_back(current_item);
	}

	return res;
}

std::string detokenize(std::list<std::string> tokens)
{
	std::string res;

	for (std::string token : tokens)
	{
		if (res.length() != 0)
		{
			res.append(" ");
		}

		res.append(token);
	}

	return res;
}

file::file(std::string path)
	: current_path(path)
{
	read();
}

bool file::write()
{
	std::ofstream f(current_path);
	if (!f.is_open())
	{
		return false;
	}

	for (std::string& line : lines)
	{
		f << line << '\n';
	}

	return true;
}

bool file::read()
{
	std::ifstream f(current_path);
	if (!f.is_open())
	{
		return false;
	}

	lines.clear();

	std::string current_line;
	while (std::getline(f, current_line))
	{
		lines.push_back(current_line);
	}

	return true;
}

static std::string empty;

std::string& file::operator[](size_t lineno)
{
	empty.clear();

	if (lineno > lines.size())
	{
		return empty;
	}

	if (!lines.size())
	{
		return empty;
	}

	std::list<std::string>::iterator iter = lines.begin();
	for (size_t i = 0; i < lines.size() && i < lineno; i++)
	{
		iter++;
	}

	return *iter;
}

void file::append(std::string line)
{
	lines.push_back(line);
}

void file::insert_at(std::string line, size_t lineno)
{
	if (lineno >= lines.size())
	{
		append(line);
	}

	std::list<std::string>::iterator iter = lines.begin();
	for (size_t i = 0; i < lines.size() && i < lineno; i++)
	{
		iter++;
	}

	lines.insert(iter, line);
}

std::list<std::string> file::get_lines()
{
	return lines;
}

config::config(std::string path)
{
	file f(path);
	std::string current_section;

	for (std::string line : f.get_lines())
	{
		std::vector<std::string> match;

		match = re_match(line, "^\\[[ ]*([^ ]*)[ ]*\\]$");
		if (match.size() > 0)
		{
			current_section = match[1];
			continue;
		}

		match = re_match(line, "^([^ =]+)[ ]*=[ ]*(.*)$");
		if (match.size() > 0)
		{
			std::string key;
			if (current_section.length())
			{
				key.append(current_section)
					.append(".").append(match[1]);
			}
			else
			{
				key = match[1];
			}

			std::string val = match[2];

			if (values.find(key) == values.end())
			{
				values[key] = std::vector<std::string>();
			}

			values[key].push_back(val);

			continue;
		}
	}
}

std::vector<std::string> config::operator[](std::string key)
{
	if (values.empty())
	{
		return std::vector<std::string>();
	}

	if (values.find(key) == values.end())
	{
		return std::vector<std::string>();
	}

	return values[key];
}

static bool is_eval_escape(std::string& line, size_t pos)
{
	if (pos >= line.size())
	{
		return false;
	}

	if (line[pos] != '\\')
	{
		return false;
	}

	if (pos + 1 >= line.size())
	{
		return false;
	}

	if (line[pos + 1] != '\\' && line[pos + 1] != '$')
	{
		return false;
	}

	return true;
}

static bool is_eval_open(std::string& line, size_t pos)
{
	if (pos >= line.size())
	{
		return false;
	}

	if (line[pos] != '$')
	{
		return false;
	}

	if (pos + 1 >= line.size())
	{
		return false;
	}

	if (line[pos + 1] != '(')
	{
		return false;
	}

	return true;
}

static bool is_eval_close(std::string line, size_t pos, size_t& count)
{
	if (pos >= line.size())
	{
		return false;
	}

	if (is_eval_open(line, pos))
	{
		count++;
	}

	if (line[pos] != ')')
	{
		return false;
	}

	if (count != 0)
	{
		return false;
	}

	return true;
}

std::string eval(std::string expr, bool is_subex)
{
	if (is_subex)
	{
		return ctx(expr);
	}

	std::string res;

	for (size_t i = 0; i < expr.size(); i++)
	{
		if (is_eval_escape(expr, i))

		{
			i++;
			res.push_back(expr[i]);
		}

		if (is_eval_open(expr, i))
		{
			i += 2;

			std::string subex;
			size_t count = 0;

			while (!is_eval_close(expr, i, count) &&
					i < expr.size())
			{
				subex.push_back(expr[i]);
				i++;
			}
			i++;

			for (char s : eval(subex, true))
			{
				res.push_back(s);
			}
		}

		res.push_back(expr[i]);
	}

	return res;
}
