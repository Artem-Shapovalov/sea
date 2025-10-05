#include "loader.hpp"
#include <curl/curl.h>
#include <thread>
#include <atomic>
#include <map>
#include <fstream>

#include <cstdio>

static class initializer
{
	public:
		initializer() { curl_global_init(CURL_GLOBAL_DEFAULT); }
		~initializer() { curl_global_cleanup(); }
} initializer;

struct item
{
	std::atomic<float> progress;
	std::thread thr;
	~item()
	{
		thr.join();
	}
};

static std::map<std::string, item> pool;

static size_t writer(void* ptr, size_t size, size_t num, void* stream)
{
	std::ofstream* out = (std::ofstream*)stream;
	out->write((const char*)ptr, size * num);
	return size * num;
}

static int progress(void* data,
	curl_off_t download_total, curl_off_t download_now,
	curl_off_t upload_total, curl_off_t upload_now)
{
	std::atomic<float>* prog = (std::atomic<float>*)data;
	if (download_total > 0)
	{
		*prog = (float)download_now / download_total;
	}
	else
	{
		*prog = 0;
	}

	return 0;
}

bool download_start(std::string url, std::string dir)
{
	if (pool.find(url) != pool.end() && pool[url].progress != 1)
	{
		return false;
	}

	std::string filename;
	for (size_t i = 0; i < url.length(); i++)
	{
		if (url[url.length() - i - 1] == '/')
		{
			break;
		}
		filename.insert(0, 1, url[url.length() - i - 1]);
	}

	pool[url].progress = 0;
	pool[url].thr = std::thread(
	[](std::string url, std::string out_file) -> void
	{
		CURL* curl = curl_easy_init();
		if (curl == nullptr)
		{
			pool[url].progress = -1;
			return;
		}

		std::ofstream ofile(out_file, std::ios::binary);
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ofile);
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
		curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, progress);
		curl_easy_setopt(curl, CURLOPT_XFERINFODATA,
				&pool[url].progress);

		CURLcode res = curl_easy_perform(curl);
		if (res != CURLE_OK)
		{
			pool[url].progress = -1;
		}
		else
		{
			pool[url].progress = 1;
		}

		ofile.close();
		curl_easy_cleanup(curl);
	}, url, dir + "/" + filename);

	return true;
}

float download_progress(std::string url)
{
	if (pool.find(url) == pool.end())
	{
		return -1;
	}

	float retval = (float)pool[url].progress;
	if (retval == 1 || retval == -1)
	{
		pool.erase(url);
	}
	return retval;
}
