#include "loader.hpp"
#include <cstdio>
#include <thread>
#include <chrono>

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		std::printf("Wrong arguments number.\n");
		return -1;
	}

	if (!download_start(argv[1], argv[2]))
	{
		std::printf("Can't start download.\n");
		return -1;
	}

	float progress = 0;
	while ((progress = download_progress(argv[1])) != 1)
	{
		std::printf("%f\n", progress);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	std::printf("done\n");

	return 0;
}
