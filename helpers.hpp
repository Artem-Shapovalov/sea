#ifndef HELPERS_HPP
#define HELPERS_HPP

#include <string>
#include <list>
#include <vector>
#include <map>
#include <ctime>

std::vector<std::string> re_match(std::string line, std::string re);
std::string sed(std::string line, std::string re, std::string pattern);
int exec(std::string cmd);
bool mkdir(std::string path);
bool touch(std::string path);
bool rm(std::string path);
bool cp(std::string src, std::string dst);
bool mv(std::string src, std::string dst);
std::list<std::string> ls(std::string path);
std::list<std::string> find(std::string path, std::string re);
bool path_exists(std::string path);
std::string abspath(std::string path);
std::string curdir();
std::string dirname(std::string path);
bool is_file(std::string path);
bool is_dir(std::string path);
std::time_t last_modified(std::string path);
std::string ctx(std::string key);
bool ctx(std::string key, std::string value);
bool ctx_has_key(std::string key);
std::string ctx_keys();
std::list<std::string> tokenize(std::string line);
std::string detokenize(std::list<std::string> tokens);
std::string eval(std::string expr, bool is_subex = false);

class file
{
	public:
	file(std::string path);
	
	bool write();
	bool read();
	std::string& operator[](size_t lineno);
	void append(std::string line);
	void insert_at(std::string line, size_t lineno);
	std::list<std::string> get_lines();

	private:
	std::string current_path;
	std::list<std::string> lines;
};

class config
{
	public:
	config(std::string path);
	std::vector<std::string> operator[](std::string key);

	private:
	std::map<std::string, std::vector<std::string>> values;
};

#endif // HELPERS_HPP
